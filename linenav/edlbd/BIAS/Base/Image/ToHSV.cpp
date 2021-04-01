/* 
   This file is part of the BIAS library (Basic ImageAlgorithmS).

   Copyright (C) 2003, 2004    (see file CONTACTS for details)
   Multimediale Systeme der Informationsverarbeitung
   Institut fuer Informatik
   Christian-Albrechts-Universitaet Kiel


   BIAS is free software; you can redistribute it and/or modify
   it under the terms of the GNU Lesser General Public License as published by
   the Free Software Foundation; either version 2.1 of the License, or
   (at your option) any later version.

   BIAS is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with BIAS; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "ImageConvert.hh"

using namespace BIAS;

namespace BIAS
{

template <class StorageType>
BIASImageBase_EXPORT 
int ImageConvert::ToHSV(const Image<StorageType>& source, 
                        Image<StorageType> &dest)
{
  int res = -1;
  
  if (dest.IsEmpty())
    dest.Init(source.GetWidth(), source.GetHeight(), source.GetChannelCount(),
              source.GetStorageType());
  
#ifdef BIAS_DEBUG
  if (!dest.SamePixelAndChannelCount(source)){
    BIASERR("ImageConvert::ToHSV() sould be called with images of"\
            <<" same size");
  }
#endif
  
  switch (source.GetColorModel()) {
  case ImageBase::CM_RGB:
    res = RGBToHSV_(source,dest);
    break;
  default:
    BIASERR("Image::ToHSV(): color model not implemented");
    break;
  }
  dest.SetColorModel(ImageBase::CM_HSV);
  return res;
}



template <class StorageType>
int ImageConvert::RGBToHSV_(const Image<StorageType>& /*source*/,
                            Image<StorageType> &/*dest*/)
{
  BIASERR("ImageConvert::RGBToHSV_ should be called from float image");
  return -1;
}


template <>
int ImageConvert::RGBToHSV_(const Image<float>& source1,
                            Image<float> &dest)

{
  
  
  
  BIASWARN("Unneccessary image copy operation. This may hurt performance.");
  Image<float> source(source1);
  
  
  register float *R = source.GetImageData();
  register float *G = NULL , *B = NULL; 
  register float *ImageDataEnd = 
      R + source.GetPixelCount() *  source.GetChannelCount();
  register float *H = dest.GetImageData();
  register float *S = NULL , *V = NULL;
  register float Min = 0, Max = 0;
  float MinVal = 0, MaxVal = 0;
  
  // scale image so RGB Values are between 0.0 and 1.0
  // if necessairy
  for (unsigned short int i = 0; i < dest.GetChannelCount(); i++){
    source.GetMinMaxPixelValue(Min, Max, i);
    if (Max > MaxVal)
      MaxVal = Max;
    if (Min < MinVal)
      MinVal = Min;
  }
  if ( (MinVal < 0.0) || (MaxVal > 1.0) )
    source.ScaleShiftBetween(0.0, 1.0);
  
  if (source.IsPlanar()){
    
    G = R + source.GetPixelCount();
    B = G + source.GetPixelCount();
    S = H + dest.GetPixelCount();
    V = S + dest.GetPixelCount();
    while (R < ImageDataEnd){
      // calc Max and Min
      if ( (*R >= *G) && (*R >= *B) ){
        Max = *R;
        if (*G <= *B)
          Min = *G;
        else
          Min = *B;
      } else if (*G >= *B){
        Max = *G;
        if (*B <= *R)
          Min = *B;
        else 
          Min = *R;
      } else {
        Max = *B;
        if (*R <= *G)
          Min = *R;
        else
          Min = *G;
      }
      
      if (Max != Min){
        // H (ue) 
        if (*R == Max)
          *H = (*G - *B)/(Max - Min);
        else if (*G == Max)
          *H = 2.0f + (*B - *R)/(Max - Min);
        else // (*B == Max)
          *H = 4.0f + (*R - *G)/(Max - Min);
        *H *= 60;
        if (*H < 0)
          *H += 360;
      } else {
        *H = 240;
      }
      
      if (Max != 0.0){
        // S (aturation) 
        *S = 1 - Min/Max;
        // V (alue) 
        //*V = float(0.299* *R + 0.587* *G + 0.114* *B);
        *V = Max;
      } else { 
        *H = 240;
        *S = 0;
        *V = 0;
      }
      
      
      // increase pointers
      H++;
      S++;
      V++;
      R++;
      G++;
      B++;
      
    }    
    dest.SetInterleaved(false);
    dest.SetColorModel(ImageBase::CM_HSV);
    
  } else { // source is interleaved
    S = H + 1;
    V = S + 1;
    while (R < ImageDataEnd){
      
      // calulate min(R,G,B) and max(R,G,B)
      Max = Min = R[0]; 
      for (register int i = 1; i < 3; i++){
        if ( Max < R[i])
          Max = R[i];
        if (Min > R[i])
          Min = R[i];
      }
      G = R+1;
      B = R+2;
      
      if (Max != Min) {
        // H (ue) 
        if (*R == Max )
          *H = (*G - *B)/(Max - Min);
        else if (*G == Max )
          *H = 2 + (*B - *R)/(Max - Min);
        else // (*B == Max )
          *H = 4 + (*R - *G)/(Max - Min);
        *H *= 60;
        if (*H < 0)
          *H += 360;
      } else {
        *H = 0;
      }
      
      
      if (Max != 0.0){
        // S (aturation) 
        *S = 1 - Min/Max;
        // V (alue) 
//        *V =  0.299f* *R + 0.587f* *G + 0.114f* *B;       
        *V = Max;
      } else {
        *S = 0;
        *V = 0;
      }      
      
      // increase pointers
      H+=3;
      S+=3;
      V+=3;
      R+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSV);
  }
  return 0;
}


// H [0,255]  0 = red, 85 = green , 171 = blue
// saturation [0,255]
// value [0,255]
// fixed for interleaved by Daniel Grest, April 2003
template <>
int ImageConvert::RGBToHSV_(const Image<unsigned char>& source,
                            Image<unsigned char> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_RGB){
    BIASERR("source not in RGB format");
    return -1;
  }
#endif
  
  BIASASSERT(!source.IsEmpty());
  BIASASSERT(!dest.IsEmpty());
  BIASASSERT(source.GetWidth() == dest.GetWidth() );
  BIASASSERT(source.GetHeight() == dest.GetHeight() );
  
  
  register const unsigned char *R = source.GetImageData();
  register const unsigned char *ImageDataEnd = 
    R + source.GetSizeByte();
  register unsigned char *H = dest.GetImageData();
  register const unsigned char *G = NULL, *B = NULL; 
  register unsigned char *S = NULL, *V = NULL;
  //register unsigned char Min = 0, Max = 0;
  unsigned char Min = 0;
  unsigned char Max = 0;

  int TmpH=0;

  BIASASSERT( R != NULL);
  BIASASSERT( H != NULL);

  if (source.IsPlanar()) {
    G = R + source.GetPixelCount();
    B = G + source.GetPixelCount();
    S = H + dest.GetPixelCount();
    V = S + dest.GetPixelCount();
    while (R < ImageDataEnd){
      // calc Max and Min
      if ( (*R >= *G) && (*R >= *B) ){
        Max = *R;
        if (*G <= *B)
          Min = *G;
        else
          Min = *B;
      } else if (*G >= *B){
        Max = *G;
        if (*B <= *R)
          Min = *B;
        else 
          Min = *R;
      } else {
        Max = *B;
        if (*R <= *G)
          Min = *R;
        else
          Min = *G;
      }
      
      if (Max != Min){
        // H (ue) 
        if (*R == Max)
          TmpH = 256 * (*G - *B)/(Max - Min);
        else if (*G == Max)
          TmpH = 512 + 256 * (*B - *R)/(Max - Min);
        else // (*B == Max)
          TmpH = 1024 + 256 * (*R - *G)/(Max - Min);
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 256);
        else
          *H = (unsigned char)TmpH;
      } else {
        *H = 171;
      }
      
      if (Max != (unsigned char)0){
        // S (aturation) [0,255]
        *S = 255 - (unsigned char)(255.0 * (float)Min/(float)Max);
        // V (alue) 
//        *V =  (unsigned char)(0.299* (float)(*R) + 
//                              0.587* (float)(*G) + 0.114* (float)(*B) + 0.5);
        *V = ((int)(*R) + (int)(*G) + (int)(*B)) / 3;
        
      } else { 
        *H = 171;
        *S = 0;
        *V = 0;
      }
      
      // increase pointers
      H++; S++; V++; R++; G++; B++;
    }    
    dest.SetInterleaved(false);
    dest.SetColorModel(ImageBase::CM_HSV);
    
  } else {// source is interleaved
    // src
    G = R + 1;
    B = G + 1;
    // dst
    S = H + 1;
    V = S + 1;
    while (R < ImageDataEnd){
      // calulate min(R,G,B) and max(R,G,B)
      Max = Min = *R;
      //jw 
      //Min = *R;
      //Max = Min;	  
      //BIASASSERT(R!=NULL);
      //BIASASSERT(G!=NULL);
      //BIASASSERT(B!=NULL);
      
      if (Max < *G) Max = *G;
      if (Max < *B) Max = *B;
      if (Min > *G) Min = *G;
      if (Min > *B) Min = *B;
      
      if (Max != Min) {
        // H (ue) 
        if (*R == Max)
          TmpH = (256 * (*G - *B))/(Max - Min);
        else if (*G == Max )
          TmpH = 512 + (256 * (*B - *R))/(Max - Min);
        else // (*B == Max )
          TmpH = 1024 + (256 * (*R - *G))/(Max - Min);
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 256);
        else
          *H = (unsigned char)TmpH;
      } else {
        *H = 171;
      }
      
      
      if (Max != (unsigned char)0){
        // S (aturation) 
        *S = (unsigned char)(255.0 * ((float)Max-(float)Min)/(float)Max);
        // V (alue) 
//        *V =  (unsigned char)(0.299* (float)(*R) + 
//                              0.587* (float)(*G) + 0.114* (float)(*B) + 0.5);
        *V = ((int)(*R) + (int)(*G) + (int)(*B)) / 3;
        
      } else {
        *H = 171;
        *S = 0;
        *V = 0;
      }      
      
      // increase pointers
      H+=3;
      S+=3;
      V+=3;
      R+=3;G+=3;B+=3;      
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSV);
  }
  return 0;
}

template <class StorageType>
int BIAS::ImageConvert::HSVToRGB_(const Image<StorageType>& source, Image<StorageType>& dest)
{
  unsigned int nrOfPel = source.GetPixelCount();
  
  const StorageType *sp = source.GetImageData();
  StorageType *dp = dest.GetImageData();
  
  if (source.IsPlanar()) {
    BIASERR("Only for interleaved images!");
    return -1;
  }
  if (source.GetChannelCount() != 3) {
    BIASERR("Only for 3 channels!");
    return -1;
  }
  unsigned int indexS = 0;
  unsigned int indexD = 0;
  StorageType h = 0, s = 0, v = 0, r = 0, g = 0, b = 0;
  for (unsigned int x = 0; x < nrOfPel; x++) {
    h = sp[indexS++];
    s = sp[indexS++];
    v = sp[indexS++];
    
    ImageConvert::HSVToRGB(h,s,v,r,g,b);
    
    dp[indexD++] = r;
    dp[indexD++] = g;
    dp[indexD++] = b;
  }
  return 0;
}


/* hue (0.0 to 360.0, is circular, 0=360)
   s and v are from 0.0 - 1.0) */
template <class StorageType>
BIASImageBase_EXPORT
int BIAS::ImageConvert::HSVToRGB(const StorageType hs, const StorageType ss, StorageType vs,
															   StorageType &r, StorageType &g, StorageType &b)
{
	float p1, p2, p3, i, f;
	float xh;
	float h = (float)hs;
	float s = (float)ss;
	float v = (float)vs;
	/* (THIS LOOKS BACKWARDS)       */
	if (h == 360.0) h = 0.0;
	xh = h / 60.0f;                  /* convert hue to be in 0,6       */
	i = (float)floor((double)xh);    /* i = greatest integer <= h    */
	f = xh - i;                      /* f = fractional part of h     */
	p1 = v * (1 - s);
	p2 = v * (1 - (s * f));
	p3 = v * (1 - (s * (1 - f)));
	switch ((int) i)
	{
	case 0:
		r = StorageType(v); g = StorageType(p3);  b = StorageType(p1); break;
	case 1:
		r = StorageType(p2); g = StorageType(v);  b = StorageType(p1); break;
	case 2:
		r = StorageType(p1); g = StorageType(v);  b = StorageType(p3); break;
	case 3:
		r = StorageType(p1); g = StorageType(p2); b = StorageType(v);  break;
	case 4:
		r = StorageType(p3); g = StorageType(p1); b = StorageType(v);  break;
	case 5:
		r = StorageType(v); g = StorageType(p1); b = StorageType(p2); break;
	}
	/* Normalize the values to 255 */
	r = (StorageType)(float)(r * 255.);
	g = (StorageType)(float)(g * 255.);
	b = (StorageType)(float)(b * 255.);
	return 0;
}

} // namespace BIAS
