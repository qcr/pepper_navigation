/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003-2009    (see file CONTACT for details)
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
//#include <Base/Common/W32Compat.hh>
//#include <Base/Common/BIASpragmaStart.hh>

template <class StorageType> inline StorageType Image<StorageType>::MaxSTValue()
{
  BIASERR("no such StorageType");
  return (StorageType)0;
}

template <> inline unsigned char Image<unsigned char>::MaxSTValue()
{ return (unsigned char)(UCHAR_MAX); }

template <> inline float Image<float>::MaxSTValue()
{ return FLT_MAX; }

#ifdef BUILD_IMAGE_CHAR
template <> inline char Image<char>::MaxSTValue()
{ return SCHAR_MAX; }
template <> inline char Image<char>::MinSTValue()
{ return SCHAR_MIN; }
#endif
#ifdef BUILD_IMAGE_SHORT
template <> inline short Image<short>::MaxSTValue()
{ return SHRT_MAX; }
template <> inline short Image<short>::MinSTValue()
{ return SHRT_MIN; }
#endif
#ifdef BUILD_IMAGE_USHORT
template <> inline unsigned short Image<unsigned short>::MaxSTValue()
{ return USHRT_MAX; }
template <> inline unsigned short Image<unsigned short>::MinSTValue()
{ return 0; }
#endif
#ifdef BUILD_IMAGE_INT
template <> inline int Image<int>::MaxSTValue()
{ return INT_MAX; }
template <> inline int Image<int>::MinSTValue()
{ return INT_MIN; }
#endif
#ifdef BUILD_IMAGE_UINT
template <> inline unsigned int Image<unsigned int>::MaxSTValue()
{ return UINT_MAX; }
template <> inline unsigned int Image<unsigned int>::MinSTValue()
{ return 0; }
#endif
#ifdef BUILD_IMAGE_DOUBLE
template <> inline double Image<double>::MaxSTValue()
{ return DBL_MAX; }
template <> inline double Image<double>::MinSTValue()
{ return DBL_MIN; }
#endif

template <class StorageType> inline StorageType Image<StorageType>::MinSTValue()
{
  BIASERR("no such StorageType");
  return (StorageType)0;
}

template <> inline unsigned char Image<unsigned char>::MinSTValue()
{ return 0; }

template <> inline float Image<float>::MinSTValue()
{ return FLT_MIN; }


template <class StorageType> 
inline StorageType 
Image<StorageType>::PixelValue(const unsigned int x, const unsigned int y, 
           const unsigned short int channel) const 
{    
  CheckCoordinates((int)x, (int)y, "PixelValue");
  
  if (IsPlanar()) {
    return GetImageData()[channel * GetPixelCount() + y * GetWidth() + x];
  } else {
    return GetImageData()[GetChannelCount() * 
                                 ( y * GetWidth() + x ) + channel ];
  }
}

template <class StorageType>
inline StorageType Image<StorageType>::
PixelValueInterleaved(const int x, const int y, const int channel) const
{
  BIASASSERT(this->IsInterleaved());
  CheckCoordinates(x, y, "PixelValueInterleaved");

  return GetImageDataArray()[y][x*GetChannelCount()+channel];
}


template <class StorageType>
inline StorageType & Image<StorageType>::
PixelValueRefInterleaved(const int x, const int y, const int channel)
{
  BIASASSERT(this->IsInterleaved());
  CheckCoordinates(x, y, "PixelValueRefInterleaved");

  return GetImageDataArray()[y][x*GetChannelCount()+channel];
}


template <class StorageType>
inline const StorageType& Image<StorageType>::GetPixelNearestNeighbor(
  const double x, const double y, const unsigned short int channel) const  
{
  int x_res = (int) ((x - floor(x)) <= 0.5 ? floor(x) : floor(x) + 1);
  int y_res = (int) ((y - floor(y)) <= 0.5 ? floor(y) : floor(y) + 1);

  CheckCoordinates(x_res, y_res, "GetNearestNeighbor");

  if (IsPlanar()) {

    // The image is planar
    return 
      GetImageData()[GetPixelCount()*channel + x_res + GetWidth() * y_res];
  } else {

    // The image is interleaved      
    return 
      GetImageData()[GetChannelCount()*(x_res+GetWidth()*y_res)+channel];
  }
}


template <class StorageType>
inline const StorageType& Image<StorageType>::GetPixelNearestNeighbor(
    const float x, const float y, const unsigned short int channel) const
{
  //CheckCoordinates(x, y, "GetNearestNeighbor");
    
  return GetPixelNearestNeighbor((double) x, (double) y, channel);
}


// template <class StorageType>
// inline StorageType& Image<StorageType>::GetPixelNearestNeighbor(
//   const float x, const float y, const enum TChannel channel) const 
// {
//   CheckCoordinates(x, y, "GetNearestNeighbor");

//   return GetPixelNearestNeighbor((double) x, (double) y, channel);
// }


template <class StorageType>
inline void
Image<StorageType>::SetPixel(const StorageType & value,
			     const unsigned int & x, 
			     const unsigned int & y, 
			     const unsigned short int channel)
{ 
  CheckCoordinates((int)x, (int)y, "SetPixel");
  CheckChannel(channel,"SetPixel");
  
  BIASASSERT( channel<GetChannelCount() );
  
  if (IsPlanar()) {
    ((StorageType*)GetImageData())[channel * GetPixelCount() + y*GetWidth() +x] = value;
  } else {
    ((StorageType*)GetImageData())[GetChannelCount() * (y*GetWidth() +x) +channel ] = value;
  };
}


template <class StorageType>
inline void 
Image<StorageType>::SetPixel(const StorageType & value0,
			     const StorageType & value1,
			     const StorageType & value2,
			     const unsigned int & x, 
			     const unsigned int & y )
{
  CheckChannel(2, "SetPixel"); // channels 0,1,2 must be there
  
  SetPixel(value0, x, y, 0);
  SetPixel(value1, x, y, 1);
  SetPixel(value2, x, y, 2);
}

template <class StorageType>
inline bool Image<StorageType>::
CheckBilinearInterpolation(const double x, const double y) const
{
  return ((floor(x) >= 0) && (floor(y) >= 0) &&
          (ceil(x) < GetWidth()) && (ceil(y) < GetHeight()));
}

template <class StorageType>
inline bool Image<StorageType>::
CheckBicubicInterpolation(const double x, const double y) const
{
  return ((floor(x) >= 1) && (floor(y) >= 1) &&
          ((floor(x)+2) < GetWidth()) && ((floor(y)+2) < GetHeight()));
}

template <class StorageType>
inline double Image<StorageType>::LinearInterpolation(
  const unsigned int x, const double y) const
{
#ifdef BIAS_DEBUG
  if ((x<0) || (x>(GetWidth()-1)))
    BIASERR("LinearInterpolation() x out of bounds: "<<x);
  if ((floor(y)<0) || (ceil(y)>(GetHeight()-1)))
    BIASERR("LinearInterpolation() y out of bounds: "<<y);
#endif
  double dy = y - floor(y);
  StorageType **ImageDataArray = (StorageType**)GetImageDataArray();

  return ((1-dy)*(double)ImageDataArray[(unsigned int)floor(y)][x] +
	  dy*(double)ImageDataArray[(unsigned int)ceil(y)][x]);
}

template <class StorageType>
inline double Image<StorageType>::LinearInterpolation(
  const double x, const unsigned int y) const
{
#ifdef BIAS_DEBUG
  if ((floor(x)<0) || ((ceil(x))>GetWidth()-1))
    BIASERR("LinearInterpolation() x out of bounds: "<<x);
  if ((y<0) || (y>(GetHeight()-1)))
    BIASERR("LinearInterpolation() y out of bounds: "<<y);
#endif
  double dx = x - floor(x);
  StorageType **ImageDataArray = (StorageType**)GetImageDataArray();

  return ((1-dx)*(double)ImageDataArray[y][(unsigned int)floor(x)] +
	  dx*(double)ImageDataArray[y][(unsigned int)ceil(x)]);
}

template <class StorageType>
inline double Image<StorageType>::
DistanceWeightedInterpolationGrey(const double x, const double y) const
{
  StorageType **ida = (StorageType**)GetImageDataArray();
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  StorageType ul = ida[y_floor][x_floor];
  StorageType ur = ida[y_floor][x_ceil];
  StorageType ll = ida[y_ceil][x_floor];
  StorageType lr = ida[y_ceil][x_ceil];
  double dy = y - y_floor;
  double dx = x - x_floor;
  double ulw, urw, llw, lrw;
  double sum=0.0;
  llw=sqrt(pow(1.0-dx,2.0)+pow(dy,2.0));
  lrw=sqrt(pow(dx,2.0)+pow(dy,2.0));
  ulw=sqrt(pow(1.0-dx,2.0)+pow(1.0-dy,2.0));
  urw=sqrt(pow(dx,2.0)+pow(1.0-dy,2.0));
  sum+=llw;
  sum+=lrw;
  sum+=ulw;
  sum+=urw;
  llw/=sum;
  lrw/=sum;
  ulw/=sum;
  urw/=sum;
  return (ul*ulw + ll*llw + lr*lrw + ur*urw);
}

template <class StorageType> inline double Image<StorageType>::
BilinearInterpolationGrey(const double x, const double y) const
{
  StorageType **ida = (StorageType**)GetImageDataArray();
  BIASASSERT(ida!=NULL);
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=x_floor+1;
  unsigned int y_ceil=y_floor+1;
  StorageType *p = ida[y_floor];
  StorageType ul = p[x_floor];
  StorageType ur = p[x_ceil];
  p = ida[y_ceil];
  StorageType ll = p[x_floor];
  StorageType lr = p[x_ceil];
  double dy = y - y_floor;
  double dx = x - x_floor;
        
  double dl = ll - ul;
  double val =  ul + dy*dl + dx*(ur - ul + dy*(lr - dl - ur ));

/*
#ifdef BIAS_DEBUG
  {
    unsigned int x_floor=(unsigned int)floor(x);
    unsigned int y_floor=(unsigned int)floor(y);
    unsigned int x_ceil=(unsigned int)ceil(x);
    unsigned int y_ceil=(unsigned int)ceil(y);
    StorageType ul = ida[y_floor  ][x_floor  ];
    StorageType ur = ida[y_floor  ][x_ceil];
    StorageType ll = ida[y_ceil][x_floor  ];
    StorageType lr = ida[y_ceil][x_ceil];
    double dy = y - y_floor;
    double dx = x - x_floor;
    
    double val2 = ul + dy*(ll - ul) + dx*(ur - ul) + dy*dx*(ul + lr - ll - ur );
    BIASASSERT(Equal(val, val2, 1e-6));
  }
#endif
*/
    return val;
}

#define IMAGEPOINT_UP(x,y,width,height) x<=(width-1)&& y<=(height-1)
#define IMAGEPOINT_DOWN(x,y,width,height) x>=0&& y>=0


template <class StorageType> inline 
double Image<StorageType>::
BilinearInterpolationGreyMinVal(const double x, const double y, 
                                const StorageType & minVal) const
{
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  if(!(IMAGEPOINT_UP(x_ceil, y_ceil, Width_, Height_))) return minVal;
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  if(!(IMAGEPOINT_DOWN(x_floor, y_floor, Width_, Height_))) return minVal;

  StorageType **ida = (StorageType**)GetImageDataArray();
  // pixel values: 
  StorageType ul = ida[y_floor  ][x_floor  ];
  if (ul<=minVal) return minVal;
  StorageType ur = ida[y_floor  ][x_ceil];
  if (ur<=minVal) return minVal;
  StorageType ll = ida[y_ceil][x_floor  ];
  if (ll<=minVal) return minVal;
  StorageType lr = ida[y_ceil][x_ceil];
  if (lr<=minVal) return minVal;  
  
  // distance weights:
  double dy = y - y_floor;
  double dx = x - x_floor;
  
  // OK, compute sum for bilinear interpolation: 
  return ul 
    + dy*(ll - ul) 
    + dx*(ur - ul) 
    + dy*dx*(ul + lr - ll - ur );
}

template <class StorageType> inline 
double Image<StorageType>::
BilinearInterpolationGreyMaxVal(const double x, const double y,
                                const StorageType & maxVal) const
{
  StorageType **ida = (StorageType**)GetImageDataArray();
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  // pixel values: 
  // return invalid if at least one of the four neighbors is invalid
  StorageType ul = ida[y_floor  ][x_floor  ];
  if (ul>=maxVal) return maxVal;
  StorageType ur = ida[y_floor  ][x_ceil];
  if (ur>=maxVal) return maxVal;
  StorageType ll = ida[y_ceil][x_floor  ];
  if (ll>=maxVal) return maxVal;
  StorageType lr = ida[y_ceil][x_ceil];
  if (lr>=maxVal) return maxVal;  
  
  // distance weights:
  double dy = y - y_floor;
  double dx = x - x_floor;
  
  // OK, compute sum for bilinear interpolation: 
  return ul 
    + dy*(ll - ul) 
    + dx*(ur - ul) 
    + dy*dx*(ul + lr - ll - ur );
}


template <class StorageType>
inline StorageType Image<StorageType>::
FastBilinearInterpolationGrey(const double x, const double y) const
{
  const StorageType **ida = GetImageDataArray();
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  StorageType ul = ida[y_floor][x_floor];
  StorageType ur = ida[y_floor][x_ceil];
  StorageType ll = ida[y_ceil][x_floor];
  StorageType lr = ida[y_ceil][x_ceil];
  double dy = y - y_floor;
  double dx = x - x_floor;
        
  return (StorageType)(ul + dy*(ll - ul) + dx*(ur - ul) + dy*dx*(ul + lr - ll - ur ));
}

/* Specific implementation is ca. 20 % faster than the generic function
   using double values above (author: woelk, 01/2003)! */
template <> inline unsigned char
Image<unsigned char>::FastBilinearInterpolationGrey(const double x, 
						    const double y) const
{
  register const unsigned char **ida = GetImageDataArray();
  register unsigned int x_floor = (unsigned int)floor(x);
  register unsigned int y_floor = (unsigned int)floor(y);
  register unsigned int x_ceil = x_floor+1;
  register unsigned int y_ceil = y_floor+1;
  register unsigned int ul = (unsigned int)ida[y_floor][x_floor];
  register unsigned int ur = (unsigned int)ida[y_floor][x_ceil];
  register unsigned int ll = (unsigned int)ida[y_ceil][x_floor];
  register unsigned int lr = (unsigned int)ida[y_ceil][x_ceil];
  register unsigned int dy = (unsigned int)rint((y - y_floor)*256);
  register unsigned int dx = (unsigned int)rint((x - x_floor)*256);

  return (unsigned char)(((ul<<16) + (dy<<8)*(ll - ul) + (dx<<8)*(ur - ul) + dy*dx*(ul - ll - ur + lr))>>16);
}

template <class StorageType> inline double Image<StorageType>::
BilinearInterpolationRGBPlanar(const double x, const double y, 
			       unsigned int channel) const
{
  StorageType **ida = ((StorageType**)GetImageDataArray())+channel*GetPixelCount();
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  StorageType ul = ida[y_floor][x_floor];
  StorageType ur = ida[y_floor][x_ceil];
  StorageType ll = ida[y_ceil][x_floor];
  StorageType lr = ida[y_ceil][x_ceil];
  double dy = y - y_floor;
  double dx = x - x_floor;
        
  return ul + dy*(ll - ul) + dx*(ur - ul) + dy*dx*(ul - ll - ur + lr);
}

template <class StorageType> inline double Image<StorageType>::
BilinearInterpolationRGBInterleaved(const double x, const double y, 
				    unsigned int channel) const
{
  unsigned int x_floor=(unsigned int)floor(x);
  unsigned int y_floor=(unsigned int)floor(y);
  unsigned int x_ceil=(unsigned int)ceil(x);
  unsigned int y_ceil=(unsigned int)ceil(y);
  StorageType ul =  
    ((StorageType**)GetImageDataArray())[y_floor][x_floor*
                                                GetChannelCount()+channel];
  StorageType ur = 
    ((StorageType**)GetImageDataArray())[y_floor][x_ceil*
                                                GetChannelCount()+channel];
  StorageType ll =  
    ((StorageType**)GetImageDataArray())[y_ceil][x_floor*
                                              GetChannelCount()+channel];
  StorageType lr =  
    ((StorageType**)GetImageDataArray())[y_ceil][x_ceil*
                                               GetChannelCount()+channel];
  double dy = y - y_floor;
  double dx = x - x_floor;
        
  return ul + dy*(ll - ul) + dx*(ur - ul) + dy*dx*(ul - ll - ur + lr);
}

template <>
inline void Image<unsigned char>::
BilinearInterpolationShiftRegion(const double& x, const double& y,
                                 unsigned int hws_x, unsigned int hws_y,
                                 unsigned char* buffer)
{
#ifdef BIAS_DEBUG
  if (ChannelCount_!=1) {
    BIASERR("invalid channel count != 1: channels="<<ChannelCount_);
    BIASABORT;
  }
#endif
  
  /*
  // XXXXXXX only for debug purposes: copy region without shift XXXXXXXX
  const unsigned int countx = int(x)+hws_x+1;
  const unsigned int county = int(y)+hws_y+1;
  for (unsigned int iy = int(y)-hws_y; iy< county; iy++) {
    for (unsigned int ix = int(x)-hws_x; ix< countx; ix++) {
      *buffer++ = GetImageDataArray()[iy][ix];
    }
  }
  // XXXXXXXXX end debug code XXXXXXXX
  */

  const unsigned char **ida  = (const unsigned char**)GetImageDataArray();

  // get pixel position of upper left corner of region
  const unsigned int x_floor = (unsigned int)floor(x) - hws_x;
  const unsigned int y_floor = (unsigned int)floor(y) - hws_y;
  const unsigned int x_ceil  = (unsigned int)ceil(x)  - hws_x;
  const unsigned int y_ceil  = (unsigned int)ceil(y)  - hws_y;

  // get pointers to four pixels surrounding upperleft corner of new region
  register unsigned char* ul = (unsigned char*) &ida[y_floor  ][x_floor  ];
  register unsigned char* ur = (unsigned char*) &ida[y_floor  ][x_ceil];
  register unsigned char* ll = (unsigned char*) &ida[y_ceil][x_floor  ];
  register unsigned char* lr = (unsigned char*) &ida[y_ceil][x_ceil];

  // get pointer to output buffer
  register unsigned char* result = buffer;

  // precompute frequently used weights
  const double dy = y - floor(y);
  const double dx = x - floor(x);
  const double dxdy = dx*dy;
  
  // make loop numbers and offset const:
  const unsigned int countx = 2*hws_x+1;
  const unsigned int county = 2*hws_y+1;
  const unsigned int lineoffset = GetWidth()-1-2*hws_x;

  // compute new grid pixel by pixel, shifting each pointer one to the right
  for (unsigned int iy = 0; iy< county; iy++) {
    for (unsigned int ix = 0; ix< countx; ix++, ul++, ur++, ll++, lr++) {
      *result = (unsigned char)(*ul + dy*(*ll - *ul) + dx*(*ur - *ul) + dxdy*(*ul + *lr - *ll - *ur));
      result++;
    }
    // end of a window row, go to start of next:
    ul += lineoffset;
    ur += lineoffset;
    ll += lineoffset;
    lr += lineoffset;
  }
  
}

template <class StorageType>
inline void Image<StorageType>::
BilinearInterpolationShiftRegion(const double& x, const double& y,
                                 unsigned int hws_x, unsigned int hws_y,
                                 StorageType* buffer)
{
  BIASERR("BilinearInterpolationShiftRegion is not implemented for other "
          "storage types than unsigned char!");
}


//#include <Base/Common/BIASpragmaEnd.hh>

