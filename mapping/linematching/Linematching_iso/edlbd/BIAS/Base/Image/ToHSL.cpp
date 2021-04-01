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
//#include "ImageIO.hh"

using namespace BIAS;


namespace BIAS
{

template <class StorageType>
BIASImageBase_EXPORT
int ImageConvert::ToHSL(const Image<StorageType>& source,
                        Image<StorageType> &dest)
{
  int res = -1;
  
  if (dest.IsEmpty())
    dest.Init(source.GetWidth(), source.GetHeight(), 3,
              source.GetStorageType());
  

  

  
  switch (source.GetColorModel()) {
  case ImageBase::CM_RGB:
    res = RGBToHSL_(source,dest);
    break;
    case ImageBase::CM_BGR:
    res = BGRToHSL_(source,dest);
    break;
  case ImageBase::CM_BGRA:
    res = BGRAToHSL_(source,dest);
    break;
  case ImageBase::CM_UYVY422:{
    BIAS::Image<StorageType> tmp;
    res = ImageConvert::ToRGB(source,tmp);    	 
    res = ToHSL(tmp,dest);     
    break;
  }
  default:
    BIASERR("Image::ToHSL(): color model not implemented for "<<
            source.GetColorModel());      
    break;
  }   
  dest.SetColorModel(ImageBase::CM_HSL);
  return res;
}



template <class StorageType>
int ImageConvert::RGBToHSL_(const Image<StorageType>& source,
                            Image<StorageType> &/*dest*/)
{
  BIASERR("ImageConvert::RGBToHSL only for <float> and <unsgined char> not "<<
          source.GetStorageType());
  return -1;
}


// S and L in [0..1]
// H in [0..360] where red is 360=0 , blue is 240 and green is 120;
template <>
int ImageConvert::RGBToHSL_(const Image<float>& source1,
                            Image<float> &dest)  {
  BIASWARN("Unnecessary image copy operation. This may hurt performance.");
  Image<float> source(source1);
  
  register float *R = source.GetImageData();
  register float *G = NULL , *B = NULL;
  register float *ImageDataEnd =
      R + source.GetPixelCount() *  source.GetChannelCount();
  register float *H = dest.GetImageData();
  register float *S = NULL , *L = NULL;
  register float Min = 0, Max = 0;
  float MinVal = 0, MaxVal = 0;
  
  // scale image so RGB Values are between 0.0 and 1.0
  // if necessary
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
    L = S + dest.GetPixelCount();
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
      *L = (Max + Min) / 2.0f;
      if (Max == Min){
        *H = 240;
        *S = 0;
      } else {
        // Saturation
        if ((*L) < 0.5 ) *S=(Max-Min)/(Max+Min);
        else  *S=(Max-Min)/(2.0f-Max-Min);
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
      }
      // increase pointers
      H++;
      S++;
      L++;
      R++;
      G++;
      B++;
    }
    dest.SetInterleaved(false);
    dest.SetColorModel(ImageBase::CM_HSL);
    
  } else { // source is interleaved
    S = H + 1;
    L = H + 2;
    while (R < ImageDataEnd){
      // calculate Min(R,G,B) and Max(R,G,B)
      Max = Min = R[0];
      for (register int i = 1; i < 3; i++){
        if ( Max < R[i])
          Max = R[i];
        if (Min > R[i])
          Min = R[i];
      }
      G = R+1;
      B = R+2;
      *L = (Max + Min) / 2.0f;
      if (Max == Min){
        *H = 240;
        *S = 0;
      } else {
        // Saturation
        if ((*L) < 0.5 ) *S=(Max-Min)/(Max+Min);
        else  *S=(Max-Min)/(2.0f-Max-Min);
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
      }
      
      // increase pointers
      H+=3;
      S+=3;
      L+=3;
      R+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSL);
  }
  return 0;
}


// H [0,255]  0 = red, 85 = green , 171 = blue
// saturation [0,255]
// lighting [0,255]
// by Daniel Grest, April 2003
template <>
int ImageConvert::RGBToHSL_(const Image<unsigned char>& source,
                            Image<unsigned char> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_RGB){
    BIASERR("source not in RGB format");
    return -1;
  }
#endif
  register const unsigned char *R = source.GetImageData();
  register const unsigned char *ImageDataEnd =
      R + source.GetSizeByte();
  register unsigned char *H = dest.GetImageData();
  register const unsigned char *G = NULL, *B = NULL;
  register unsigned char *S = NULL, *L = NULL;
  register int Min = 0, Max = 0;
  register int delta,TmpH;
  
  if (source.IsPlanar()){
    G = R + source.GetPixelCount();
    B = G + source.GetPixelCount();
    S = H + dest.GetPixelCount();
    L = S + dest.GetPixelCount();
    while (R < ImageDataEnd){
      // calc Max and Min
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (unsigned char)((Max + Min) / 2);
      
      delta = Max - Min;
      if (delta==0){
        *H = 171;
        *S = 0;
      } else {
        // Saturation
        if ((*L) < 127)
          *S= (unsigned char)(int(255*delta)/ int(Max+Min));
        else  *S= (unsigned char)(int(255*delta)/int(510-Max-Min));
        
        // H (ue)
        if (*R == Max)
          TmpH = (255 * (*G - *B)) / delta;
        else if (*G == Max)
          TmpH = 510 + (255 * (*B - *R)) / delta;
        else // (*B == Max)
          TmpH = 1020 + (255 * (*R - *G)) / delta;
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 255);
        else
          *H = (unsigned char)TmpH;
      }
      
      // increase pointers
      H++; S++; L++; R++; G++; B++;
    }
    dest.SetInterleaved(false);
    dest.SetColorModel(ImageBase::CM_HSL);
  } else {// source is interleaved
    G = R + 1;
    B = G + 1;
    S = H + 1;
    L = S + 1;
    while (R < ImageDataEnd){
      // calculate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (unsigned char)((Max + Min) / 2);
      
      delta = Max - Min;
      if (delta==0){
        *H = 171;
        *S = 0;
      } else {
        // Saturation
        
        if ((*L) < 127)
          *S= (unsigned char)rint(float(255*delta)/ float(Max+Min));
        else  *S= (unsigned char)rint(float(255*delta)/float(510-Max-Min));
        
        // H (ue)
        if (*R == Max)
          TmpH = (255 * (*G - *B)) / delta;
        else if (*G == Max)
          TmpH = 510 + (255 * (*B - *R)) / delta;
        else // (*B == Max)
          TmpH = 1020 + (255 * (*R - *G)) / delta;
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 255);
        else
          *H = (unsigned char)TmpH;
      }
      
      // increase pointers
      H+=3;
      S+=3;
      L+=3;
      R+=3;G+=3;B+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSL);
  }
  return 0;
}


///////////////////////////// BGR //////////////////////////////

template <class StorageType>
int ImageConvert::BGRToHSL_(const Image<StorageType>& source,
                            Image<StorageType> &/*dest*/)
{
  BIASERR("ImageConvert::BGRToHSL only for <unsgined char> not "<<
          source.GetStorageType());
  return -1;
}


// H [0,255]  0 = red, 85 = green , 171 = blue
// saturation [0,255]
// lighting [0,255]
// by Daniel Grest, April 2003
template <>
int ImageConvert::BGRToHSL_(const Image<unsigned char>& source,
                            Image<unsigned char> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_BGR){
    BIASERR("source not in BGR format");
    return -1;
  }
#endif
  register const unsigned char *B = source.GetImageData();
  register const unsigned char *ImageDataEnd =
      B + source.GetSizeByte();
  register unsigned char *H = dest.GetImageData();
  register const unsigned char *G = NULL, *R = NULL;
  register unsigned char *S = NULL, *L = NULL;
  register unsigned char Min = 0, Max = 0;
  register int delta,TmpH;
  
  if (source.IsPlanar()){
    G = B + source.GetPixelCount();
    R = G + source.GetPixelCount();
    S = H + dest.GetPixelCount();
    L = S + dest.GetPixelCount();
    while (B < ImageDataEnd){
      // calc Max and Min
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      delta = Max - Min;
      if (delta==0){
        *H = 171;
        *S = 0;
      } else {
        // Saturation
        if ((*L) > 127)
          *S= (unsigned char)((255*delta)/ (Max+Min));
        else  *S= (unsigned char)((255*delta)/(510-delta));
        // H (ue)
        if (*R == Max)
          TmpH = (255 * (*G - *B)) / delta;
        else if (*G == Max)
          TmpH = 510 + (255 * (*B - *R)) / delta;
        else // (*B == Max)
          TmpH = 1020 + (255 * (*R - *G)) / delta;
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 255);
        else
          *H = (unsigned char)TmpH;
      }
      
      // increase pointers
      H++; S++; L++; R++; G++; B++;
    }
    dest.SetInterleaved(false);
    dest.SetColorModel(ImageBase::CM_HSL);
  } else {// source is interleaved
    G = B + 1;
    R = G + 1;
    S = H + 1;
    L = S + 1;
    while (B < ImageDataEnd){
      // calulate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      delta = Max - Min;
      if (delta==0){
        *H = 171;
        *S = 0;
      } else {
        // Saturation
        if ((*L) > 127)
          *S= (unsigned char)((255*delta)/ (Max+Min));
        else  *S= (unsigned char)((255*delta)/(510-delta));
        // H (ue)
        if (*R == Max)
          TmpH = (255 * (*G - *B)) / delta;
        else if (*G == Max)
          TmpH = 510 + (255 * (*B - *R)) / delta;
        else // (*B == Max)
          TmpH = 1020 + (255 * (*R - *G)) / delta;
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 255);
        else
          *H = (unsigned char)TmpH;
      }
      
      // increase pointers
      H+=3;
      S+=3;
      L+=3;
      R+=3;G+=3;B+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSL);
  }
  return 0;
}


///////////////////////////// BGRA //////////////////////////////

template <class StorageType>
int ImageConvert::BGRAToHSL_(const Image<StorageType>& source,
                             Image<StorageType> &/*dest*/)
{
  BIASERR("ImageConvert::BGRAToHSL only for <unsgined char> not "<<
          source.GetStorageType());
  return -1;
}


// H [0,255]  0 = red, 85 = green , 171 = blue
// saturation [0,255]
// lighting [0,255]
// by Daniel Grest, Feb 2005
template <>
int ImageConvert::BGRAToHSL_(const Image<unsigned char>& source,
                             Image<unsigned char> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_BGRA){
    BIASERR("source not in BGRA format");
    return -1;
  }
#endif
  register const unsigned char *B = source.GetImageData();
  register const unsigned char *ImageDataEnd =
      B + source.GetSizeByte();
  register unsigned char *H = dest.GetImageData();
  register const unsigned char *G = NULL, *R = NULL;
  register unsigned char *S = NULL, *L = NULL;
  register unsigned char Min = 0, Max = 0;
  register int delta,TmpH;
  
  if (source.IsPlanar()){
    BIASERR("BGRAToHSL_() not for planar images");
    return -2;
  } else {// source is interleaved
    G = B + 1;
    R = G + 1;
    S = H + 1;
    L = S + 1;
    while (B < ImageDataEnd){
      // calulate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      delta = Max - Min;
      if (delta==0){
        *H = 171;
        *S = 0;
      } else {
        // Saturation
        if ((*L) > 127)
          *S= (unsigned char)((255*delta)/ (Max+Min));
        else  *S= (unsigned char)((255*delta)/(510-delta));
        // H (ue)
        if (*R == Max)
          TmpH = (255 * (*G - *B)) / delta;
        else if (*G == Max)
          TmpH = 510 + (255 * (*B - *R)) / delta;
        else // (*B == Max)
          TmpH = 1020 + (255 * (*R - *G)) / delta;
        TmpH /= 6;
        if (TmpH < 0)
          *H = (unsigned char)(TmpH + 255);
        else
          *H = (unsigned char)TmpH;
      }
      
      // increase pointers
      H+=3;
      S+=3;
      L+=3;
      R+=4;G+=4;B+=4;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_HSL);
  }
  return 0;
}




///////////////////////////////////////////////////////////////////////////
/////////////////////////// hsL ///////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////

template <class StorageType>
BIASImageBase_EXPORT
int ImageConvert::TohsL(const Image<StorageType>& source,
                        Image<StorageType> &dest)
{
  int res = -1;
  
  if (dest.IsEmpty())
    dest.Init(source.GetWidth(), source.GetHeight(), source.GetChannelCount(),
              source.GetStorageType());
  
  BIASASSERT(source.GetBitDepth() == 8);
  
#ifdef BIAS_DEBUG
  if (!dest.SamePixelAndChannelCount(source)){
    BIASERR("ImageConvert::TohsL() should be called with images of"\
            <<" same size");
  }
#endif
  
  switch (source.GetColorModel()) {
  case ImageBase::CM_RGB:
    res = RGBTohsL_(source,dest);
    break;
  default:
    BIASERR("Image::TohsL(): color model not implemented for "<<
            source.GetColorModel());
    break;
  }
  dest.SetColorModel(ImageBase::CM_hsL);
  return res;
}

template <class StorageType>
int ImageConvert::RGBTohsL_(const Image<StorageType>& source,
                            Image<StorageType> &/*dest*/)
{
  BIASERR("ImageConvert::RGBTohsL only for <int> and <unsigned char> and <float> (bit depth 8 for each type)"<<
          " so far, not "<<
          source.GetStorageType());
  return -1;
}

// (h,s) is the euclidean vector of the(H,S) polar coordinates
// lighting [0,255]
// by Daniel Grest, June 2003
template <>
BIASImageBase_EXPORT
int ImageConvert::RGBTohsL_(const Image<int> &source,
                            Image<int> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_RGB){
    BIASERR("source not in RGB format");
    return -1;
  }
#endif
  register const int *R = source.GetImageData();
  register const int *ImageDataEnd =
      R + source.GetPixelCount();
  register int *h = dest.GetImageData();
  register const int *G = NULL, *B = NULL;
  register int *s = NULL, *L = NULL;
  register int Min = 0, Max = 0;
  register int delta,hTemp,sTemp;
  
  register float d=0,sat;
  if (source.IsPlanar()){
    BIASERR("not yet implemented");
  } else {// source is interleaved
    G = R + 1;
    B = G + 1;
    s = h + 1;
    L = s + 1;
    while (R < ImageDataEnd){
      // calulate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      // project RGB
      hTemp= *G - *B;
      sTemp= 2*(*R) - *G - *B;
      d=float(sqrt(double(hTemp*hTemp + sTemp*sTemp)));
      //      if (*L==21) cout<<"d:"<<d<<endl;
      delta = Max - Min;
      if ( (delta==0) || (d<1E-07) ) {
        *h = 0;
        *s = 0;
      } else {
        // Saturation
        if ((*L) > 127)
          sat= (float)((255*delta)/ (Max+Min));
        else  sat= (float)((255*delta)/(510-delta));
        // if (*L==21) cout<<"sat:"<<sat<<endl;
        *h= (int)((float)hTemp * sat / d + 0.5);
        *s= (int)((float)sTemp * sat / d + 0.5);
        
      }
      
      // increase pointers
      h+=3;
      s+=3;
      L+=3;
      R+=3;G+=3;B+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_hsL);
  }
  return 0;
  
  
}



// (h,s) is the euclidean vector of the(H,S) polar coordinates
// lighting [0,255]
// by Daniel Grest, June 2003
template <>
BIASImageBase_EXPORT
int ImageConvert::RGBTohsL_(const Image<float> &source,
                            Image<float> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_RGB){
    BIASERR("source not in RGB format");
    return -1;
  }
#endif
  register const float *R = source.GetImageData();
  register const float *ImageDataEnd =
      R + source.GetPixelCount();
  register float *h = dest.GetImageData();
  register const float *G = NULL, *B = NULL;
  register float *s = NULL, *L = NULL;
  register float Min = 0, Max = 0;
  register float delta,hTemp,sTemp;
  
  register float d=0,sat;
  if (source.IsPlanar()){
    BIASERR("not yet implemented");
  } else {// source is interleaved
    G = R + 1;
    B = G + 1;
    s = h + 1;
    L = s + 1;
    while (R < ImageDataEnd){
      // calulate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      // project RGB
      hTemp= *G - *B;
      sTemp= 2*(*R) - *G - *B;
      d=float(sqrt(double(hTemp*hTemp + sTemp*sTemp)));
      //      if (*L==21) cout<<"d:"<<d<<endl;
      delta = Max - Min;
      if ( (delta==0) || (d<1E-07) ) {
        *h = 0;
        *s = 0;
      } else {
        // Saturation
        if ((*L) > 127)
          sat= (float)((255*delta)/ (Max+Min));
        else  sat= (float)((255*delta)/(510-delta));
        // if (*L==21) cout<<"sat:"<<sat<<endl;
        *h= (float)hTemp * sat / d + 0.5f;
        *s= (float)sTemp * sat / d + 0.5f;
      }
      
      // increase pointers
      h+=3;
      s+=3;
      L+=3;
      R+=3;G+=3;B+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_hsL);
  }
  return 0;
  
  
}



// (h,s) is the euclidean vector of the(H,S) polar coordinates
// h,s in [-127..127] but stored  unsigned char (0..128) !
// lighting [0,255]
// by Daniel Grest, June 2003
template <>
BIASImageBase_EXPORT
int ImageConvert::RGBTohsL_(const Image<unsigned char> &source,
                            Image<unsigned char> &dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_RGB){
    BIASERR("source not in RGB format");
    return -1;
  }
#endif
  register const unsigned char *R = source.GetImageData();
  register const unsigned char *ImageDataEnd =
      R + source.GetSizeByte();
  register unsigned char *h = dest.GetImageData();
  register const unsigned char *G = NULL, *B = NULL;
  register unsigned char *s = NULL;
  register unsigned char *L = NULL;
  register unsigned char Min = 0, Max = 0;
  register unsigned char delta;
  register float hTemp,sTemp;
  
  register float d=0,sat;
  if (source.IsPlanar()){
    BIASERR("not yet implemented");
  } else {// source is interleaved
    G = R + 1;
    B = G + 1;
    s = h + 1;
    L = s + 1;
    while (R < ImageDataEnd){
      // calulate Min(R,G,B) and Max(R,G,B)
      Max = Min = *R;
      if (Max < *G) Max=*G;
      if (Max < *B) Max=*B;
      if (Min > *G) Min=*G;
      if (Min > *B) Min=*B;
      
      *L = (Max + Min) / 2;
      
      // project RGB
      hTemp= float((int)*G - (int)*B);
      sTemp= float(2*(int)(*R) - (int)*G - (int)*B);
      d=float(sqrt(double(hTemp*hTemp + sTemp*sTemp)));
      //      if (*L==21) cout<<"d:"<<d<<endl;
      delta = Max - Min;
      if ( (delta==0) || (d<1E-07) ) {
        *h = 127;
        *s = 127;
      } else {
        // Saturation
        if ((*L) > 127)
          sat= (float)((127*delta)/ (Max+Min));
        else  sat= (float)((127*delta)/(510-delta));
        // if (*L==21) cout<<"sat:"<<sat<<endl;
        *h= (unsigned char)(hTemp * sat / d + 127.5);
        *s= (unsigned char)(sTemp * sat / d + 127.5);
        
      }
      
      // increase pointers
      h+=3;
      s+=3;
      L+=3;
      R+=3;G+=3;B+=3;
    }
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_hsL);
  }
  return 0;
}

// works for interleaved float images, but not for unsigned char, since it
// assumes values for hue to be in [0,360], and for S and L in [0,1]
template <class StorageType>
int BIAS::ImageConvert::HSLToRGB_(const Image<StorageType>& source,
                                  Image<StorageType>& dest)
{
#ifdef BIAS_DEBUG
  if (source.GetColorModel() != ImageBase::CM_HSL){
    BIASERR("source not in HSL format");
    return -1;
  }
  if(source.GetStorageType() != ImageBase::ST_float){
    BIASERR("not implemented properly, fix first, only working for float right now")
    return -1;
  }
#endif
  unsigned width = source.GetWidth();
  unsigned height = source.GetHeight();
  Image<float> tmpIm(width,height,3);
  Image<float> tmpS(width,height,3);
  
  ImageConvert::ConvertST(source,tmpS,ImageBase::ST_float);
  register float **idaS = tmpS.GetImageDataArray();
  register StorageType        **idaD = dest.GetImageDataArray();  	 
  register float           **idaTmp = tmpIm.GetImageDataArray();
  
  
  if(source.IsPlanar()){
    BIASERR("Only for interleaved images!");
    return -1;
  }
  else{
    //tmpS.ScaleShiftBetween(0,1.0);
    // http://130.113.54.154/~monger/hsl-rgb.html    	
//#ifdef BIAS_HAVE_OPENMP
//#pragma omp parallel for
//#endif	  	
    for(int y = 0; y<(int)height; y++){
      for(int x = 0; x<(int)width; x++){
        //in range 0,1
        float H = float(idaS[y][x*3])/360.0f;
        float S = float(idaS[y][x*3+1]);
        float L = float(idaS[y][x*3+2]);
        
        //If S=0, define R, G, and B all to L  			 
        if(S==0){
          idaTmp[y][x*3] = idaTmp[y][x*3+1] = idaTmp[y][x*3+2] = L;
        } else {
          float tmp1=0.0,tmp2=0.0,tmp3R=0.0, tmp3G=0.0, tmp3B=0.0;
          if(L<0.5f) tmp2 = L*(1.0f+S);
          else if (L >=0.5f) tmp2 = (L+S)-(L*S);
          
          tmp1 = 2.0f*L-tmp2;
          
          tmp3R = H+(1.0f/3.0f); if(tmp3R<0) tmp3R+=1.0f; if(tmp3R>1) tmp3R-=1.0f;
          tmp3G = H;             if(tmp3G<0) tmp3G+=1.0f; if(tmp3G>1) tmp3G-=1.0f;
          tmp3B = H-(1.0f/3.0f); if(tmp3B<0) tmp3B+=1.0f; if(tmp3B>1) tmp3B-=1.0f;
          //R
          if((tmp3R*6.0f) < 1) idaTmp[y][x*3] = tmp1+(tmp2-tmp1)*6.0f*tmp3R;
          else if((tmp3R*2.0f) < 1) idaTmp[y][x*3]=tmp2;
          else if((tmp3R*3.0f) < 2) idaTmp[y][x*3]=tmp1+(tmp2-tmp1)*((2.0f/3.0f)-tmp3R)*6.0f;
          else idaTmp[y][x*3]=tmp1;
          //G
          if((tmp3G*6.0f) < 1) idaTmp[y][x*3+1] = tmp1+(tmp2-tmp1)*6.0f*tmp3G;
          else if((tmp3G*2.0f) < 1) idaTmp[y][x*3+1]=tmp2;
          else if((tmp3G*3.0f) < 2) idaTmp[y][x*3+1]=tmp1+(tmp2-tmp1)*((2.0f/3.0f)-tmp3G)*6.0f;
          else idaTmp[y][x*3+1]=tmp1;
          //B
          if((tmp3B*6.0f) < 1) idaTmp[y][x*3+2] = tmp1+(tmp2-tmp1)*6.0f*tmp3B;
          else if((tmp3B*2.0f) < 1) idaTmp[y][x*3+2]=tmp2;
          else if((tmp3B*3.0f) < 2) idaTmp[y][x*3+2]=tmp1+(tmp2-tmp1)*((2.0f/3.0f)-tmp3B)*6.0f;
          else idaTmp[y][x*3+2]=tmp1;
        }//else
        
        //scale back to 0 255
        //cout << "r " << idaTmp[y][x*3] << " g " << idaTmp[y][x*3+1] << " b " << idaTmp[y][x*3+2] << endl;
        idaD[y][x*3] = StorageType(idaTmp[y][x*3]*(255.0));
        idaD[y][x*3+1] = StorageType(idaTmp[y][x*3+1]*(255.0));
        idaD[y][x*3+2] = StorageType(idaTmp[y][x*3+2]*(255.0));
      }//x
    }//y
    dest.SetInterleaved(true);
    dest.SetColorModel(ImageBase::CM_RGB);
    return 0;
  }
}

} // namespace BIAS
