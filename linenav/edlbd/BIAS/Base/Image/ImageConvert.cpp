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

#include "ImageConvert.hh"

#include <iostream>
//#include <Base/Debug/DebugSimple.hh>
#include "ToPlanar.cpp"
#include "ToInterleaved.cpp"
#include "ToHSV.cpp"
#include "ToHSL.cpp"

using namespace std;
using namespace BIAS;




/// one of the elemenatry switch case statements
#define SWIC(biasTypeEnum, cpptype, callfunc, src, dst) \
case ImageBase::biasTypeEnum :\
{\
  Image< cpptype > typedImg, destImg;\
  const ImageBase* psrc =  &src;                      \
  typedImg.StealImage(*const_cast<ImageBase*>(psrc)); \
  callfunc(typedImg, destImg);\
  const_cast<ImageBase*>(psrc)->StealImage(typedImg);  \
  dst.StealImage(destImg);\
}\
  break;



// use only those case whose instances are compiled
// create one macro for each type, use dby CALLFUNC macro
// @author Jan Woetzel 05/2006

// always compiled
#define CASE_UCHAR(func, src, dst)   SWIC(ST_unsignedchar, unsigned char, func, src, dst)
#define CASE_FLOAT(func, src, dst)   SWIC(ST_float,         float,         func, src, dst)

// compiled iff option enabled vias bias_config.h
#ifdef BUILD_IMAGE_CHAR
#  define CASE_CHAR(func, src, dst)  SWIC(ST_char, char, func, src, dst)
#else
#  define CASE_CHAR(func, src, dst)
#endif

#ifdef BUILD_IMAGE_SHORT
#  define CASE_SHORT(func, src, dst)  SWIC(ST_shortint, short int, func, src, dst)
#else
#  define CASE_SHORT(func, src, dst)
#endif

#ifdef BUILD_IMAGE_USHORT
#  define CASE_USHORT(func, src, dst)  SWIC(ST_unsignedshortint, unsigned short int, func, src, dst)
#else
#  define CASE_USHORT(func, src, dst)
#endif

#ifdef BUILD_IMAGE_INT
#  define CASE_INT(func, src, dst)  SWIC(ST_int, int, func, src, dst)
#else
#  define CASE_INT(func, src, dst)
#endif

#ifdef BUILD_IMAGE_UINT
#  define CASE_UINT(func, src, dst)  SWIC(ST_unsignedint, unsigned int, func, src, dst)
#else
#  define CASE_UINT(func, src, dst)
#endif

#ifdef BUILD_IMAGE_DOUBLE
#  define CASE_DOUBLE(func, src, dst)  SWIC(ST_double, double, func, src, dst)
#else
#  define CASE_DOUBLE(func, src, dst)
#endif



///
/// creates all switch cases given a function
/// no pretty implementation but backward compatible to old macro JW
#define CALLFUNC(afunc,source,dest) \
  CASE_UCHAR (afunc,source,dest) \
  CASE_FLOAT (afunc,source,dest) \
  CASE_CHAR  (afunc,source,dest) \
  CASE_SHORT (afunc,source,dest) \
  CASE_USHORT(afunc,source,dest) \
  CASE_INT   (afunc,source,dest) \
  CASE_UINT  (afunc,source,dest) \
  CASE_DOUBLE(afunc,source,dest)

//
// end of macros
//


// ----- implementation -----

int ImageConvert::Convert(ImageBase &source, ImageBase &dest,
                          enum ImageBase::EColorModel targetColorModel,
                          bool bPlanar)
{
  if ( (dest.GetStorageType()!=ImageBase::ST_invalid) &&
    (dest.GetStorageType()!=source.GetStorageType()) ) {
      BIASERR("color model conversion cannot convert image from StorageType "
        <<source.GetStorageType()<<" to an image with StorageType "
        <<dest.GetStorageType());
      return -1;
    }

    if (source==dest) { ///////////// call the In-Place function
      if (source.GetColorModel()!=targetColorModel)
        switch (targetColorModel)
      {
        case ImageBase::CM_Grey:
          switch (source.GetStorageType())
          {
            //CALLFUNC_IP(IP_ToGrey,source);
            // >> start of former CALLFUNC_IP jw
          case ImageBase::ST_unsignedchar:
            {
              Image<unsigned char> ucImg;
              ucImg.StealImage(source);
              IP_ToGrey(ucImg);
              source.StealImage(ucImg);
            }
            break;
          case ImageBase::ST_float:
            {
              Image<float> fImg;
              fImg.StealImage(source);
              IP_ToGrey(fImg);
              source.StealImage(fImg);
            }
            break;
#ifdef BUILD_IMAGE_CHAR
          case ImageBase::ST_char:
            {
              Image<char> cImg;
              cImg.StealImage(source);
              IP_ToGrey(cImg);
              source.StealImage(cImg);
            }
            break;
#endif // BUILD_IMAGE_CHAR
#ifdef BUILD_IMAGE_SHORT
          case ImageBase::ST_shortint:
            {
              Image<short int> sImg;
              sImg.StealImage(source);
              IP_ToGrey(sImg);
              source.StealImage(sImg);
            }
            break;
#endif // BUILD_IMAGE_SHORT
#ifdef BUILD_IMAGE_USHORT
          case ImageBase::ST_unsignedshortint:
            {
              Image<unsigned short int> usImg;
              usImg.StealImage(source);
              IP_ToGrey(usImg);
              source.StealImage(usImg);
            }
            break;
#endif // BUILD_IMAGE_USHORT
#ifdef BUILD_IMAGE_INT
          case ImageBase::ST_int:
            {
              Image<int> iImg;
              iImg.StealImage(source);
              IP_ToGrey(iImg);
              source.StealImage(iImg);
            }
            break;
#endif // BUILD_IMAGE_INT
#ifdef BUILD_IMAGE_UINT
          case ImageBase::ST_unsignedint:
            {
              Image<unsigned int> uiImg;
              uiImg.StealImage(source);
              IP_ToGrey(uiImg);
              source.StealImage(uiImg);
            }
            break;
#endif // BUILD_IMAGE_UINT
#ifdef BUILD_IMAGE_DOUBLE
          case ImageBase::ST_double:
            {
              Image<double> dImg;
              dImg.StealImage(source);
              IP_ToGrey(dImg);
              source.StealImage(dImg);
            }
            break;
#endif // BUILD_IMAGE_DOUBLE
            // << end of former CALLFUNC_IP jw

          default:
            BIASERR("invalid storage type in source image");
            return -2;
          }
          break;
        default:
          BIASERR("no In-Place-conversion for sourceColorModel:"<<
            source.GetColorModel()<<
            " to targetColorModel:"<<targetColorModel<< endl<<
            "can only In-Place-convert to CM_Grey");
          return -3;
      }

      if (source.IsPlanar() != bPlanar) {
        ImageBase destImgBase;
        if (source.IsPlanar()) {
          switch (source.GetStorageType()) {
            CALLFUNC(ToInterleaved,source,destImgBase);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
          }
        } else { // it is interleaved
          switch (source.GetStorageType()) {
            CALLFUNC(ToPlanar,source,destImgBase);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
          }
        }
        source.StealImage(destImgBase);
      }

    } else { ///////////////////////// not In-Place

      switch (targetColorModel)
      {
      case ImageBase::CM_Grey:
        switch (source.GetStorageType())
        {
          CALLFUNC(ToGrey,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;
      case ImageBase::CM_RGB:
        switch (source.GetStorageType())
        {
          CALLFUNC(ToRGB,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;
      case ImageBase::CM_RGBA:
        switch (source.GetStorageType())
        {
          CALLFUNC(ToRGBA,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;
      case ImageBase::CM_HSV:
        switch (source.GetStorageType())
        {
          CALLFUNC(ToHSV,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;
      case ImageBase::CM_HSL:
        switch (source.GetStorageType())
        {
          CALLFUNC(ToHSL,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;
      case ImageBase::CM_hsL:
        switch (source.GetStorageType())
        {
          CALLFUNC(TohsL,source,dest);
        default:
          BIASERR("invalid storage type in source image");
          return -2;
        }
        break;

      default:
        BIASERR("no conversion available to targetColorModel:"<<
          targetColorModel);
        return -3;
      }
    }
    if (source.IsPlanar() != bPlanar) {
      ImageBase destImgBase;
      if (source.IsPlanar()) {
        switch (source.GetStorageType()) {
          CALLFUNC(ToInterleaved,dest,destImgBase);
      default:
        BIASERR("invalid storage type in source image");
        return -2;
        }
      } else { // it is interleaved
        switch (source.GetStorageType()) {
          CALLFUNC(ToPlanar,dest,destImgBase);
      default:
        BIASERR("invalid storage type in source image");
        return -2;
        }
      }
      dest.StealImage(destImgBase);
    }
    return 0;
}

/**
Sony DFW-X700 :
r = y + 1.4022 v got
b = y + 1.7710 u
g = y - 0.3475 u - 0.7144 v got
*/
#define YUV2RGB(y, u, v, r, g, b)\
  r = y + ( ((v << 11) + (v << 10) - (v << 8) + (v << 6) - (v << 3) ) >> 11);\
  g = y - ( ((v<<10) + (v<<9) - (v<<6) -  (v<<3) ) >> 11 ) - \
  ( ((u << 9) + (u << 7) + (u << 6) + (u << 3) + u) >> 11 );\
  b = y + ( ((u << 12) - (u << 9) + (u << 5)+(u<<4)) >> 11);\
  r = r < 0 ? 0 : r;\
  g = g < 0 ? 0 : g;\
  b = b < 0 ? 0 : b;\
  r = r > 255 ? 255 : r;\
  g = g > 255 ? 255 : g;\
  b = b > 255 ? 255 : b




////////////////////
// ToGrey Section //
////////////////////
template<class StorageType>
int BIAS::ImageConvert::ToGrey(const Image<StorageType>& source,
                               Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT," ImageConvert::ToGrey()");
  int res = -1;

  if ( dest.GetPixelCount() != source.GetPixelCount() ||
       dest.GetWidth() != source.GetWidth() ||
       dest.GetHeight() != source.GetHeight() ){
    BIASGDOUT(D_CONVERT,
              "ToGrey() should be called with an image of same size");
    dest.Release();
  }
  if ((dest.GetChannelCount() != 1) &&
      (dest.GetImageData() != source.GetImageData())){
    BIASGDOUT(D_CONVERT,"ToGrey() should be called with an 1 channel image");
    dest.Release();
  }

  if (dest.IsEmpty()) {
    dest.Init(source.GetWidth(), source.GetHeight(),1,source.GetStorageType());
  }

  switch (source.GetColorModel()) {
  case ImageBase::CM_Grey:
    dest = source;
    res = 0;
    break;
  case ImageBase::CM_DV:
#ifdef BIAS_HAVE_DV
    res = DVToRGB_(source,dest);
    IP_ToGrey(dest);
#else
    BIASERR("CM_DV not supported because BIAS_HAVE_DV not defined!");
    BIASABORT;
#endif
    break;

  case ImageBase::CM_RGB:
  case ImageBase::CM_BGR:
    res = RGBToGrey_(source,dest);
    break;
  case ImageBase::CM_BGRA:
    res = BGRAToGrey_(source, dest);
    break;
  case ImageBase::CM_UYVY422:
  case ImageBase::CM_YUYV422:
    res = YUV422ToGrey_(source,dest);
    break;
  case ImageBase::CM_YUV420P:
    res = YUV420PToGrey_(source,dest);
    break;
  case ImageBase::CM_YUV411:
    res = YUV411ToGrey_(source,dest);
    break;
  case ImageBase::CM_Bayer_RGGB:
  case ImageBase::CM_Bayer_GBRG:
  case ImageBase::CM_Bayer_GRBG:
  case ImageBase::CM_Bayer_BGGR:
    // PtGrToGrey_ is not correct
    res = BayerToGrey_(source,dest);
    break;
  case ImageBase::CM_HSL:
  case ImageBase::CM_hsL:
    res = HSLToGrey_(source,dest);
    break;
  default:
    BIASERR("ImageConvert::ToGrey(): unknown color model:"<<
            source.GetColorModel());
    //BIASBREAK; // do NOT abort, here.
    return -1;
  }
  dest.ChannelCount_=1;
  dest.WidthStep_=dest.Width_*dest.Depth_;
  dest.SetColorModel(ImageBase::CM_Grey);
  // needed for inplace conversion
  dest.UpdateImageDataArrayFromImageData_();
  dest.SetROI(*source.GetROI());
  return res;
}


template<class StorageType>
int BIAS::ImageConvert::GetChannel(Image<StorageType>& image, int channel, bool strip)
{
  BIASGDOUT(D_CONVERT,"GetChannel()");
  register StorageType *sink = image.GetImageData();
  register StorageType *src  = image.GetImageData();

  int channelCount = image.GetChannelCount();

  if (channelCount < channel) {
    BIASERR("Error: Image has not enough channels.");
    return -1;
  }

  // in place not necessary , as ToRGB() created new dist already
  if (strip) {
    for (unsigned long int i = 0; i < image.GetPixelCount(); i++) {
      sink[0] = src[channel];
      src +=channelCount;
      sink++;
    }
    image.ChannelCount_= 1;
    image.WidthStep_ = image.Width_ * image.Depth_ * image.ChannelCount_;
    image.ImageData_ = realloc(image.ImageData_, sizeof(StorageType)*image.GetPixelCount());
    image.SetColorModel(ImageBase::CM_Grey);
    image.UpdateImageDataArrayFromImageData_();
  } else {
    for (unsigned long int i = 0; i < image.GetPixelCount(); i++) {
      for (int j = 0; j < channelCount; j++) {
        sink[j] = src[channel];
      }
      src += channelCount;
      sink+= channelCount;
    }
  }
  return 0;
}

int BIAS::ImageConvert::GetChannel(ImageBase& image, int channel, bool strip)
{
  BIASGDOUT(D_CONVERT,"GetChannel()");
  register unsigned char *sink = (unsigned char*)image.GetImageData();
  register unsigned char *src  = (unsigned char*)image.GetImageData();

  int channelCount = image.GetChannelCount();

  if (channelCount < channel) {
    BIASERR("Error: Image has not enough channels.");
    return -1;
  }

  int depth = image.GetDepth();

  if (strip) {
    for (unsigned long int i = 0; i < image.GetPixelCount(); i++) {
      for (int k=0; k<depth; k++) {
        sink[k] = src[channel*depth+k];
      }
      src += channelCount*depth;
      sink+= depth;
    }
    image.ChannelCount_=1;
    image.WidthStep_ = image.Width_ * image.Depth_ * image.ChannelCount_;
    image.ImageData_ = realloc(image.ImageData_, depth*image.GetPixelCount());
    image.SetColorModel(ImageBase::CM_Grey);
    image.UpdateImageDataArrayFromImageData_();
  } else {
    for (unsigned long int i = 0; i < image.GetPixelCount(); i++) {
      for (int j = 0; j < channelCount; j++) {
        for (int k=0; k<depth; k++) {
          sink[j*depth+k] = src[channel*depth+k];
        }
      }
      src += channelCount*depth;
      sink+= channelCount*depth;
    }
  }

  return 0;
}


////////////////////
// ToGrey Section //
////////////////////
template<class StorageType>
int BIAS::ImageConvert::GetChannel(const Image<StorageType>& source,
				   Image<StorageType>& dest,
				   const unsigned int channel)
{
  BIASASSERT(source.GetStorageType() == dest.GetStorageType());

    if (dest.GetImageData() == source.GetImageData()){
    BIASERR("GetChannel(src,dst,channel) should not be called with"
	    " the same images. In place does not work, currently.");
    dest.Release();
  } else if ((dest.GetWidth() != source.GetWidth() ||
       dest.GetHeight() != source.GetHeight() ))
  {
    BIASWARN("GetChannel(src,dst,channel) should be called"
            "with an image of same size.\n"
            "src: "<<source.GetWidth()<<"x"<<source.GetHeight()<<"\n"
            "dst: "<<dest.GetWidth()<<"x"<<dest.GetHeight() );
    dest.Release();
  } else if ((dest.GetChannelCount() != 1)
    && (dest.GetImageData() != source.GetImageData()) )
  {
    BIASWARN("GetChannel(src,dst,channel) should be called "
      "with a 1 channel dest image but has "
      <<dest.GetChannelCount()<<" channels" );
    dest.Release();
  };

  if (channel >= source.GetChannelCount()){
    BIASERR("GetChannel(src,dst,channel) - src doesn't have enough channels");
    return -1;
  }

  if (dest.IsEmpty()) {
    dest.Init(source.GetWidth(), source.GetHeight(),1,source.GetStorageType());
    dest.SetColorModel( ImageBase::CM_Grey );
  }

  const StorageType *s=source.GetImageData();
  StorageType *d=dest.GetImageData();

  if (source.IsPlanar()){
    s += channel*source.GetWidth()*source.GetHeight(); // start offset
    // TODO use memcpy instead
    for (unsigned int i=0; i < dest.GetWidth()*dest.GetHeight(); i++){
      d[0]=s[0];
      d++;
      s++;
    }
  } else {
    // interleaved
    const int step = source.GetChannelCount(); // in units of StorageType
    s += channel;
    for (unsigned int i=0; i < dest.GetWidth()*dest.GetHeight(); i++){
      *d = *s;
      d++;
      s+=step;
    }
  }

  dest.ChannelCount_=1;
  dest.WidthStep_=dest.Width_*dest.Depth_;
  dest.SetColorModel(ImageBase::CM_Grey);
  return 0;
}


int BIAS::ImageConvert::ToGrey(const ImageBase& source,
                               ImageBase& dest)
{
  int res = 0;
  switch (source.GetStorageType()){
  case ImageBase::ST_unsignedchar:
    {
//      BIASASSERT(dest.GetStorageType() == ImageBase::ST_unsignedchar);
      Image<unsigned char> s(source);
      Image<unsigned char> d;
      res = ToGrey(s, d);
      dest.StealImage(d);
    }
    break;
#ifdef BUILD_IMAGE_USHORT
  case ImageBase::ST_unsignedshortint:
    {
//      BIASASSERT(dest.GetStorageType() == ImageBase::ST_unsignedshortint);
      Image<unsigned short> s(source);
      Image<unsigned short> d;
      res = ToGrey(s, d);
      dest.StealImage(d);
    }
    break;
#endif // BUILD_IMAGE_USHORT
  case ImageBase::ST_float:
    {
//      BIASASSERT(dest.GetStorageType() == ImageBase::ST_float);
      Image<float> s(source);
      Image<float> d;
      res = ToGrey(s, d);
      dest.StealImage(d);
    }
    break;
  default:
    BIASERR("unfinished code");
    BIASABORT;
    break;
  }
  dest.SetROI(*source.GetROI());
  return res;
}

template <class StorageType>
int BIAS::ImageConvert::RGBToGrey_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"ImageConvert::RGBToGrey_()");
  StorageType *p=dest.GetImageData();
  const StorageType *r, *g, *b;

  if (source.IsPlanar()){
    switch (source.GetColorModel()){
    case ImageBase::CM_RGB:
      r=source.GetImageData();
      g=r+(source.GetWidth()*source.GetHeight());
      b=r+(2*source.GetWidth()*source.GetHeight());
      break;
    case ImageBase::CM_BGR:
      b=source.GetImageData();
      g=b+(source.GetWidth()*source.GetHeight());
      r=b+(2*source.GetWidth()*source.GetHeight());
      break;
    default:
      BIASERR("RGBToGrey_ must be called with RGB or BGR ColorModel_");
      return -1;
    }
    const unsigned num_pix = dest.GetWidth()*dest.GetHeight();
    // casting using rounding is necessary for integer data types to deal
    // with compile precision
    if (numeric_limits<StorageType>::is_integer){
      for (unsigned int i=0; i < num_pix; i++){
        // + 0.5 is for rounding
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0])+0.5);
        p++; r++; g++; b++;
      }
    } else {
      for (unsigned int i=0; i < num_pix; i++){
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0]));
        p++; r++; g++; b++;
      }
    }
  } else {
    switch (source.GetColorModel()) {
    case ImageBase::CM_RGB:
      r=source.GetImageData();
      g=r+1;
      b=r+2;
      break;
    case ImageBase::CM_BGR:
      b=source.GetImageData();
      g=b+1;
      r=b+2;
      break;
    default:
      BIASERR("RGBToGrey_ must be called with RGB or BGR ColorModel_");
      return -1;
    }
    const unsigned num_pix = dest.GetWidth()*dest.GetHeight();
    // casting is necessray for integer data types to deal
    // with compile precision
    if (numeric_limits<StorageType>::is_integer){
      for (unsigned int i=0; i < num_pix; i++){
        // + 0.5 is for rounding
        p[0]=(StorageType)((0.299*(double)(r[0])+
                            0.587*(double)(g[0])+
                            0.114*(double)(b[0]))+0.5);
        p++; r+=3; g+=3; b+=3;
      }
    } else {
      for (unsigned int i=0; i < num_pix; i++){
        p[0]=(StorageType)((0.299*(double)(r[0])+
                            0.587*(double)(g[0])+
                            0.114*(double)(b[0])));
        p++; r+=3; g+=3; b+=3;
      }
    }
  }
  return 0;
}

template<class StorageType>
int
BIAS::ImageConvert::BGRAToGrey_(const Image<StorageType>& source, Image<
                                StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"ImageConvert::BGRAToGrey_()");
  StorageType *p = dest.GetImageData();
  const StorageType *r, *g, *b;

  if (source.IsPlanar())
    {
      if (source.GetColorModel() == ImageBase::CM_BGRA)
        {
          b = source.GetImageData();
          g = b + (source.GetWidth() * source.GetHeight());
          r = b + (2 * source.GetWidth() * source.GetHeight());
        }
      else
        {
          BIASERR("RGBToGrey_ must be called with BGRA ColorModel_");
          return -1;
        }
      for (unsigned int i = 0; i < dest.GetWidth() * dest.GetHeight(); i++)
        {
          // casting using rint is necessray for integer data types to deal
          // with compile precision
          // this is solved by introducing a templated Cast function
          p[0] = Cast<StorageType> (0.299 * (double) (r[0]) +0.587
                                    * (double) (g[0]) +0.114 * (double) (b[0]));
          p++;
          r++;
          g++;
          b++;
        }
    }
  else
    {
      if (source.GetColorModel() == ImageBase::CM_BGRA)
        {
          b = source.GetImageData();
          g = b + 1;
          r = b + 2;
        }
      else
        {
          BIASERR("RGBToGrey_ must be called with BGRA ColorModel_");
          return -1;
        }
      for (unsigned int i = 0; i < dest.GetWidth() * dest.GetHeight(); i++)
        {
          p[0] = Cast<StorageType> (0.299 * (double) (r[0]) +0.587
                                    * (double) (g[0]) +0.114 * (double) (b[0]));
          //cout << "rgb: "<<(int)r[0]<<" "<<(int)g[0]<<" "<<(int)b[0]<<"\tgrey: "<<(int)p[0]<<endl;
          p++;
          r += 4;
          g += 4;
          b += 4;
        }
    }
  return 0;
}



template <class StorageType>
int BIAS::ImageConvert::YUV422ToGrey_(const Image<StorageType> &source,
                                      Image<StorageType>& dest)
{

  register const StorageType *y = source.GetImageData();
  StorageType *sink = dest.GetImageData();
  register StorageType *sinkp;
  bool IdenticalImages = false;
  BIASGDOUT(D_CONVERT,"ImageConvert::YUV422ToGrey_()");

  if (y == sink) {
    BIASGDOUT(D_MI_CONVGREY,"identical images");
    IdenticalImages = true;
    sink = new StorageType[source.GetWidth() * source.GetHeight()];
  }

  switch (source.GetColorModel()) {
    case ImageBase::CM_YUYV422:
      break;
    case ImageBase::CM_UYVY422:
      y++;
      break;
    default:
      BIASERR("ImageConvert::YUV422ToGrey_(): unsupported colormodel");
      return -1;
  }

  if (source.IsPlanar()){
    memcpy(sink, y, dest.GetPixelCount());
  } else {
    sinkp = sink;
    for (register unsigned long int i=0; i < dest.GetPixelCount(); i++){
      sinkp[0]=y[0];
      sinkp++;
      y+=2;
      // cerr << i << " " << endl;
    }
  }

  if (IdenticalImages) {
    dest.ReleaseImageDataPointer();
    dest.ChannelCount_=1;
    dest.RedirectImageDataPointer(sink);
  }
  return 0;
}



///////////////////////
// IP_ToGrey Section //
///////////////////////
template <class StorageType>
int BIAS::ImageConvert::IP_ToGrey(Image<StorageType> &img)
{
  int res = -1;

  BIASGDOUT(D_CONVERT,"ImageConvert::IP_ToGrey_()");

  //   if (sizeof(StorageType) != 1){
  //     BIASERR("IP_ToGrey makes only sense with <char> and <unsigned char> images");
  //     return -1;
  //   }

  switch (img.GetColorModel()) {
  case ImageBase::CM_Grey://ImageBase::ImageBase::CM_Grey:
    BIASERR("already in grey");
    break;
  case ImageBase::CM_BGR:
    res = IP_RGBToGrey_(img);
    break;
  case ImageBase::CM_BGRA:
    res = IP_RGBToGrey_(img);
    break;
  case ImageBase::CM_RGB:
    res = IP_RGBToGrey_(img);
    break;
  case ImageBase::CM_YUYV422:
  case ImageBase::CM_UYVY422:
    res = IP_YUV422ToGrey_(img);
    break;
  default:
    BIASERR("Image::IP_ToGrey(): unknown color model");
    res = -1;
    break;
  }
  if (res >=0) {
    img.ChannelCount_=1;
    img.WidthStep_ = img.Depth_ * img.Width_;
    img.SetColorModel(ImageBase::CM_Grey);
    img.UpdateImageDataArrayFromImageData_();
  }
  return res;
}


//template<class StorageType>
#define acc(x,y) ()
int ImageConvert::RGBToXYZ(const ImageBase& source, BIAS::Image<float>& dest,
                           float scale)
{
  unsigned int width = source.GetWidth();
  unsigned int height = source.GetHeight();
  unsigned int channels = source.GetChannelCount();

  if (source.GetColorModel() != ImageBase::CM_RGB)
    {
      BIASERR("the source image must be in rgb format: error");
      return -1;
    }

  if (channels != 3)
    {
      BIASERR("number of channels dont equal 3: error");
      return -1;
    }

  if (width != dest.GetWidth() || height != dest.GetHeight() || channels
      != dest.GetChannelCount())
    {
      dest.Release();
      dest.Init(width, height, channels);
    }

  dest.SetColorModel(ImageBase::CM_XYZ);

  float rgbToxyz[3][3] = {
    { 0.490f, 0.310f, 0.200f },
    { 0.177f, 0.812f, 0.011f },
    { 0.000f, 0.010f, 0.990f }
  };

  float* channelR = new float[width*height];
  ImageBase::GetChannel(source, 0, channelR);
  float* channelG = new float[width*height];
  ImageBase::GetChannel(source, 1, channelG);
  float* channelB = new float[width*height];
  ImageBase::GetChannel(source, 2, channelB);

  for (unsigned int i = 0; i < height; i++)
  {
    for (unsigned int j = 0; j < width; j++)
    {
      float s[3] = {
      channelR[i*width+j]/scale,
      channelG[i*width+j]/scale,
      channelB[i*width+j]/scale};

      dest.SetPixel(s[0] * rgbToxyz[0][0] + s[1] * rgbToxyz[0][1] + s[2]
      * rgbToxyz[0][2], s[0] * rgbToxyz[1][0] + s[1] * rgbToxyz[1][1]
      + s[2] * rgbToxyz[1][2], s[0] * rgbToxyz[2][0] + s[1]
      * rgbToxyz[2][1] + s[2] * rgbToxyz[2][2], j, i);
        }
  }
  delete [] channelR;
  delete [] channelG;
  delete [] channelB;


  return 0;
}

template<class StorageType>
int ImageConvert::XYZToRGB(const BIAS::Image<float>& source, BIAS::Image<StorageType>& dest,
                           float scale)
{
  unsigned int width = source.GetWidth();
  unsigned int height = source.GetHeight();
  unsigned int channels = source.GetChannelCount();

  if (source.GetColorModel() != ImageBase::CM_XYZ)
    {
      BIASERR("the source image must be in xyz format: error");
      return -1;
    }

  if (channels != 3)
    {
      BIASERR("number of channels must equal 3: error");
      return -1;
    }

  if (width != dest.GetWidth() || height != dest.GetHeight() || channels
      != dest.GetChannelCount())
    {
      dest.Release();
      dest.Init(width, height, channels);
    }

  dest.SetColorModel(ImageBase::CM_RGB);

  float xyzTorgb[3][3] =
    {
      { 2.3649f, -0.8971f, -0.4678f },
      { -0.5156f, 1.4273f, 0.0883f },
      { 0.0052f, -0.0144f, 1.0092f } };

  for (unsigned int i = 0; i < height; i++)
    {
      for (unsigned int j = 0; j < width; j++)
        {
          float s[3] =
            { (float) source.PixelValue(j, i, 0), source.PixelValue(j, i, 1),
              source.PixelValue(j, i, 2) };
          dest.SetPixel((StorageType) rint((s[0] * xyzTorgb[0][0] + s[1] * xyzTorgb[0][1] + s[2]
                                            * xyzTorgb[0][2])*scale), (StorageType) rint((s[0] * xyzTorgb[1][0] + s[1] * xyzTorgb[1][1]
                                                                                          + s[2] * xyzTorgb[1][2])*scale), (StorageType) rint((s[0] * xyzTorgb[2][0] + s[1]
                                                                                                                                               * xyzTorgb[2][1] + s[2] * xyzTorgb[2][2])*scale), j, i);
        }
    }

  return 0;
}

int
ImageConvert::XYZToLUV(const BIAS::Image<float>& source, BIAS::Image<float>& dest)
{

  unsigned int width = source.GetWidth();
  unsigned int height = source.GetHeight();
  unsigned int channels = source.GetChannelCount();
  if (source.GetColorModel() != ImageBase::CM_XYZ)
    {
        BIASERR("the source image must be in xyz format: error");
      return -1;
    }

  if (channels != 3)
    {
        BIASERR("number of channels must equal 3: error");
      return -1;
    }

  if (width != dest.GetWidth() || height != dest.GetHeight() || channels
      != dest.GetChannelCount())
    {
      dest.Release();
      dest.Init(width, height, channels);
    }

  dest.SetColorModel(ImageBase::CM_LUV);

  float rgbToxyz[3][3] =
    {
      { 0.490f, 0.310f, 0.200f },
      { 0.177f, 0.812f, 0.011f },
      { 0.000f, 0.010f, 0.990f } };

  float Xn, Yn, Zn, Un, Vn;

  Xn = rgbToxyz[0][0] + rgbToxyz[0][1] + rgbToxyz[0][2];
  Yn = rgbToxyz[1][0] + rgbToxyz[1][1] + rgbToxyz[1][2];
  Zn = rgbToxyz[2][0] + rgbToxyz[2][1] + rgbToxyz[2][2];

  Un = (4.0f * Xn) / (Xn + 15.0f * Yn + 3.0f * Zn);
  Vn = (9.0f * Yn) / (Xn + 15.0f * Yn + 3.0f * Zn);

  for (unsigned int i = 0; i < height; i++)
    {
      for (unsigned int j = 0; j < width; j++)
        {
          // now transform xyz to luv
          float x = source.PixelValue(j, i, 0);
          float y = source.PixelValue(j, i, 1);
          float z = source.PixelValue(j, i, 2);

          // compute L*
          float L = y / Yn;

          if (L > 0.008856)
            {
              L = 116.0f * pow(L, 1.0f / 3.0f) - 16.0f;
            }
          else
            {
              L = 903.3f * L;
            }

          //    //compute u* and v*
          float denominator = x + 15.0f * y + 3.0f * z;
          float u_star = 0;
          float v_star = 0;
          if (denominator != 0)
            {
              u_star = 13.0f * L * ((4.0f * x / denominator) - Un);
              v_star = 13.0f * L * ((9.0f * y / denominator) - Vn);

            }

          dest.SetPixel(L, u_star, v_star, j, i);
        }
    }

  return 0;
}

int ImageConvert::XYZToLAB(const BIAS::Image<float>& source, BIAS::Image<float>& dest){
  unsigned int width = source.GetWidth();
    unsigned int height = source.GetHeight();
    unsigned int channels = source.GetChannelCount();
    if (source.GetColorModel() != ImageBase::CM_XYZ)
      {
          BIASERR("the source image must be in xyz format: error");
        return -1;
      }

    if (channels != 3)
      {
          BIASERR("number of channels must equal 3: error");
        return -1;
      }

    if (width != dest.GetWidth() || height != dest.GetHeight() || channels
        != dest.GetChannelCount())
      {
        dest.Release();
        dest.Init(width, height, channels);
      }

    dest.SetColorModel(ImageBase::CM_LAB);

    float rgbToxyz[3][3] =
      {
        { 0.490f, 0.310f, 0.200f },
        { 0.177f, 0.812f, 0.011f },
        { 0.000f, 0.010f, 0.990f } };

    float Xn, Yn, Zn;
        Xn = rgbToxyz[0][0] + rgbToxyz[0][1] + rgbToxyz[0][2];
        Yn = rgbToxyz[1][0] + rgbToxyz[1][1] + rgbToxyz[1][2];
        Zn = rgbToxyz[2][0] + rgbToxyz[2][1] + rgbToxyz[2][2];
    float th_root = 1.0f/3.0f;

    for (unsigned int i = 0; i < height; i++)
      {
        for (unsigned int j = 0; j < width; j++)
          {
            float x = source.PixelValue(j, i, 0);
            float y = source.PixelValue(j, i, 1);
            float z = source.PixelValue(j, i, 2);

            float x_scaled, y_scaled, z_scaled, x_root, y_root, z_root;
            x_scaled = (x/Xn);
            y_scaled = (y/Yn);
            z_scaled = (z/Zn);


            if(x_scaled < 0.008856f){
              x_root = (903.296f*x_scaled+16.0f)/116.0f;
            }else{
              x_root = x_scaled * th_root;
            }

            if(y_scaled < 0.008856f){
              y_root = (903.296f*y_scaled+16.0f)/116.0f;
            }else{
              y_root = y_scaled * th_root;
            }

            if(z_scaled < 0.008856f){
              z_root = (903.296f*z_scaled+16.0f)/116.0f;
            }else{
              z_root = z_scaled * th_root;
            }

            // compute L
            float L = 116.0f * y_root - 16.0f;

            float a = 500.0f * (x_root - z_root);

            float b = 200.0f * (y_root - z_root);

            dest.SetPixel(L, a, b, j, i);
          }
      }

    return 0;
}

int
ImageConvert::LUVToXYZ(const BIAS::Image<float>& source, BIAS::Image<float>& dest)
{
  unsigned int width = source.GetWidth();
  unsigned int height = source.GetHeight();
  unsigned int channels = source.GetChannelCount();

  if (source.GetColorModel() != ImageBase::CM_LUV)
    {
        BIASERR("the source image must be in luv format: error");
      return -1;
    }

  if (channels != 3)
    {
        BIASERR("number of channels must equal 3: error");
      return -1;
    }

  if (width != dest.GetWidth() || height != dest.GetHeight() || channels
      != dest.GetChannelCount())
    {
      dest.Release();
      dest.Init(width, height, channels);
    }

  dest.SetColorModel(ImageBase::CM_XYZ);

  float rgbToxyz[3][3] =
    {
      { 0.490f, 0.310f, 0.200f },
      { 0.177f, 0.812f, 0.011f },
      { 0.000f, 0.010f, 0.990f } };

  float Xn, Yn, Zn, Un, Vn;

  Xn = rgbToxyz[0][0] + rgbToxyz[0][1] + rgbToxyz[0][2];
  Yn = rgbToxyz[1][0] + rgbToxyz[1][1] + rgbToxyz[1][2];
  Zn = rgbToxyz[2][0] + rgbToxyz[2][1] + rgbToxyz[2][2];

  Un = (4.0f * Xn) / (Xn + 15.0f * Yn + 3.0f * Zn);
  Vn = (9.0f * Yn) / (Xn + 15.0f * Yn + 3.0f * Zn);

  for (unsigned int i = 0; i < height; i++)
    {
      for (unsigned int j = 0; j < width; j++)
        {
          float luv[3];
          luv[0] = source.PixelValue(j, i, 0);
          luv[1] = source.PixelValue(j, i, 1);
          luv[2] = source.PixelValue(j, i, 2);

          //-------------------------------
          // transform luv to xyz

          float L = luv[0];
          float u_star = luv[1];
          float v_star = luv[2];
          float x = 0;
          float y = 0;
          float z = 0;

          if (L > 7.9996248)
            {
              y = Yn * pow((float) ((L + 16.0) / 116.0), 3);
            }
          else
            {
              y = Yn * (L / 903.3f);
            }

          if (L > 0)
            {
              float u = u_star / (13.0f * L) + Un;
              float v = v_star / (13.0f * L) + Vn;
              z = 3.0f * y / v - 5.0f * y - (3.0f / 4.0f) * u * y / v;
              x = 9.0f * y / v - 15.0f * y - 3.0f * z;
            }

          dest.SetPixel(x, y, z, j, i);
        }
    }

  return 0;
}

//template<class StorageType>
int ImageConvert::RGBToLUV(const BIAS::ImageBase& source,
                           BIAS::Image<float>& dest, float scale)
{
  Image<float> xyz;
  int res = RGBToXYZ(source, xyz, scale);
  if (res != 0)
    return res;
  XYZToLUV(xyz, dest);
  return res;

  return 0;
}

//template<class StorageType>
int ImageConvert::RGBToLAB(const BIAS::ImageBase& source,
                           BIAS::Image<float>& dest, float scale)
{
  Image<float> xyz;
  int res = RGBToXYZ(source, xyz, scale);
  if (res != 0)
    return res;
  XYZToLAB(xyz, dest);
  return res;

  return 0;
}



template<class StorageType>
int ImageConvert::LUVToRGB(const BIAS::Image<float>& source,
                           BIAS::Image<StorageType>& dest, float scale)
{
  Image<float> xyz;
  int res = LUVToXYZ(source, xyz);
  if (res != 0)
    return res;
  res = XYZToRGB(xyz, dest, scale);

  return res;
}


template <class StorageType>
int BIAS::ImageConvert::IP_RGBToGrey_(Image<StorageType> &img)
{
  StorageType *p=img.GetImageData();
  StorageType *r, *g, *b;
  if (img.IsPlanar()){
    switch (img.GetColorModel()){
    case ImageBase::CM_RGB:
      r=img.GetImageData();
      g=r+(img.GetWidth()*img.GetHeight());
      b=r+(2*img.GetWidth()*img.GetHeight());
      break;
    case ImageBase::CM_BGR:
      b=img.GetImageData();
      g=b+(img.GetWidth()*img.GetHeight());
      r=b+(2*img.GetWidth()*img.GetHeight());
      break;
    case ImageBase::CM_BGRA:
      b = img.GetImageData();
      g = b + (img.GetWidth() * img.GetHeight());
      r = b + (2 * img.GetWidth() * img.GetHeight());
      break;
    default:
      BIASERR("IP_RGBToGrey_ must be called with RGB or BGR ColorModel_");
      return -1;
    }
    const unsigned num_pix = img.GetWidth()*img.GetHeight();
    // casting using rounding is necessary for integer data types to deal
    // with compile precision
    if (numeric_limits<StorageType>::is_integer){
      for (unsigned int i=0; i < num_pix; i++){
        // + 0.5 is for rounding
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0])+0.5);
        p++; r++; g++; b++;
      }
    } else {
      for (unsigned int i=0; i < num_pix; i++){
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0]));
        p++; r++; g++; b++;
      }
    }
  } else {
    switch (img.GetColorModel()) {
    case ImageBase::CM_RGB:
      r=img.GetImageData();
      g=r+1;
      b=r+2;
      break;
    case ImageBase::CM_BGR:
      b=img.GetImageData();
      g=b+1;
      r=b+2;
      break;
    case ImageBase::CM_BGRA:
      b = img.GetImageData();
      g = b + 1;
      r = b + 2;
      break;
    default:
      BIASERR("IP_RGBToGrey_ must be called with RGB or BGR or BGRA ColorModel_");
      return -1;
    }

    const unsigned num_pix = img.GetWidth()*img.GetHeight();
    // casting using rounding is necessary for integer data types to deal
    // with compile precision
    if (numeric_limits<StorageType>::is_integer){
      // + 0.5 is for rounding
      for (unsigned int i=0; i < num_pix; i++){
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0])+0.5);
        p++; r+=3; g+=3; b+=3;
      }
    } else {
      for (unsigned int i=0; i < num_pix; i++){
        p[0]=(StorageType)(0.299*(double)(r[0])+
                           0.587*(double)(g[0])+
                           0.114*(double)(b[0]));
        p++; r+=3; g+=3; b+=3;
      }
    }
  }
  return 0;
}


int BIAS::ImageConvert::RGBToI1I2I3(const BIAS::ImageBase& source, BIAS::Image<float>& dest)
{
  if(source.GetColorModel() != BIAS::ImageBase::CM_RGB)
    {
      cerr<<"Error in rgb2i3! ColorModel should be CM_RGB"<< "\r\n";
      return -1;
    }
	int
		width = source.GetWidth(),
    height = source.GetHeight();
  int size = width*height;
  dest.Init(width, height, 3);
  //float* siP = sink.GetImageData();
  switch(source.GetStorageType()) {
    case ImageBase::ST_float:
      ConvertRGBToI1I2I3Worker_<float>(static_cast<const float*>(source.GetImageData()),
        dest.GetImageData(), size);
      break;
    case ImageBase::ST_unsignedchar:
      ConvertRGBToI1I2I3Worker_<unsigned char>(static_cast<const unsigned char*>(source.GetImageData()),
        dest.GetImageData(), size);
      break;
    default:
      BIASERR("unimplemented case!");
      return -1;
      break;
  }

  return 0;
}


template<class sourceType>
void BIAS::ImageConvert::ConvertRGBToI1I2I3Worker_(const sourceType* soP, float* siP, int size)
{
  sourceType sR, sG, sB;
  for(int i = 0; i < size; i++)
  {
    sR = *soP;
    soP++;
    sG = *soP;
    soP++;
    sB = *soP;
    soP++;
    // I1
    /*		intensity = (sR + sG + sB)/3;
    *siP = (lastIntensity - intensity + 255)/2;
    lastIntensity = intensity;*/
    *siP = static_cast<float>(sR + sG + sB)/3.f;
    siP++;
    // I2 + shift
    *siP = static_cast<float>(sR - sB)/2.f;
    siP++;
    // I3 + shift
    *siP = static_cast<float>(2*sG - sR - sB)/4.f;
    siP++;
  }
}


template <class StorageType>
int BIAS::ImageConvert::IP_YUV422ToGrey_(Image<StorageType> &img)
{
  StorageType *y;

  BIASGDOUT(D_CONVERT,"ImageConvert::IP_YUV422ToGrey_()");


  if (img.IsPlanar()){
    switch (img.GetColorModel()) {
    case ImageBase::CM_YUYV422:
      img.ChannelCount_=1;
      break;
    default:
      BIASERR("ImageConvert::IP_YUV422ToGrey_()  color model not yet implemented");
      break;
    }
  } else {
    StorageType *dest=img.GetImageData();
    switch (img.GetColorModel()) {
    case ImageBase::CM_YUYV422:
      y = img.GetImageData();
      break;
    case ImageBase::CM_UYVY422:
      y = img.GetImageData() + 1;
      break;
    default:
      BIASERR("ImageConvert::IP_YUV422ToGrey_(): unsupported colormodel");
      return -1;
    }
    for (unsigned int i=0; i < img.GetWidth()*img.GetHeight(); i++){
      dest[0]=y[0];
      dest++;
      y+=2;
    }
  }
  return 0;

}

////////////////////
//  ToRGBA Section //
////////////////////


template <class StorageType>
int BIAS::ImageConvert::ToRGBA(const Image<StorageType>& source,
                               Image<StorageType>& dest)
{
  // only implmented for interleaved, out-of-place conversion so far

  BIASWARNONCE("inefficient code: -> RGBA conversion");
  int res = 0;
  Image<StorageType> rgb;
  if (source.GetColorModel()!=ImageBase::CM_RGB) {
    res = ToRGB(source, rgb);
    if (res!=0) return res;
  } else {
    rgb = source;
  }

  BIASASSERT(rgb.IsInterleaved());
  // add alphachannel
  if (!dest.IsEmpty()) dest.Release();
  dest.Init(source.GetWidth(), source.GetHeight(), 4);
  dest.SetColorModel(ImageBase::CM_RGBA);

  StorageType *pD = rgb.GetImageData();
  StorageType *pDA = dest.GetImageData();
  for (unsigned int y = 0; y < dest.GetHeight(); y++) {
    for (unsigned int x = 0; x < dest.GetWidth(); x++) {
      *pDA++ = *pD++;
      *pDA++ = *pD++;
      *pDA++ = *pD++;
      *pDA++ = 0;
    }
  }
  return res;
}
////////////////////
//  ToRGB Section //
////////////////////


template <class StorageType>
int BIAS::ImageConvert::ToRGB(const Image<StorageType>& source,
                              Image<StorageType>& dest)
{
  int res = -1;
  bool inplace=(source.GetImageData()==dest.GetImageData());
  Image<StorageType>* tmpdest;
  if (inplace)
    tmpdest = new Image<StorageType>;
  else
    tmpdest = &dest;


  BIASGDOUT(D_CONVERT,"ImageConvert::ToRGB()");
  if (!tmpdest->IsEmpty()) {
    if (tmpdest->GetChannelCount() != 3 ||
      tmpdest->GetWidth()!=source.GetWidth() ||
      tmpdest->GetHeight() != source.GetHeight() ){
        tmpdest->Release();
      }
  }

  if (tmpdest->IsEmpty()) {
    if (source.GetColorModel() == ImageBase::CM_DV) {
#ifdef BIAS_HAVE_DV
      tmpdest->Init(720,576, 3,source.GetStorageType()); // PAL
#else
      BIASERR("BIAS_HAVE_DV not defined!!!");
      BIASABORT;
#endif
    } else {
      tmpdest->Init(source.GetWidth(), source.GetHeight(), 3,
        source.GetStorageType());
    }
  }

  switch (source.GetColorModel()) {
  case ImageBase::CM_Grey:
  case ImageBase::CM_DepthAndVariance:
  case ImageBase::CM_Disparity:
  case ImageBase::CM_Depth:
    res = GreyToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_BGR:
    res = BGRToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_RGB:
    *tmpdest = source;
    res = 0;
    break;
  case ImageBase::CM_HSL:
     res = HSLToRGB_(source,*tmpdest);
     break;
  case ImageBase::CM_UYVY422:
    res = YUV422ToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_YUYV422:
    res = YUV422ToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_YUV420P:
    res = YUV420PToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_HSV:
    res = HSVToRGB_(source, *tmpdest);
    break;
  case ImageBase::CM_DV:
#ifdef BIAS_HAVE_DV
    res = DVToRGB_(source,*tmpdest);
#else
    BIASERR("BIAS_HAVE_DV not defined!!");
    BIASABORT;
#endif
    break;
  case ImageBase::CM_Bayer_RGGB:
  case ImageBase::CM_Bayer_GBRG:
  case ImageBase::CM_Bayer_GRBG:
  case ImageBase::CM_Bayer_BGGR:
    res = BayerToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_RGBA:
    res = RGBAToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_BGRA:
    res = BGRAToRGB_(source,*tmpdest);
    break;
  case ImageBase::CM_PGR_XB3_F7M3_GBRG:  // tri-image
    {
      // workaround template implementation:
      Image<unsigned char>* ps_uc =
        const_cast<Image<unsigned char>*>
        (dynamic_cast< const Image<unsigned char>* >(&source));
      Image<unsigned char>* pd_uc =
        dynamic_cast< Image<unsigned char>* >(&dest);
      if ((ps_uc!=NULL) && (pd_uc!=NULL)){
        // use unsigned char special implemetation
        BIAS::Image<unsigned char> tmp3;
        res = DeinterleaveHorizontal(*ps_uc, tmp3);
        if (res!=0){
          BIASERR("could not Deinterleave the 3 Bayer patterns.");
          return res;
        }
        // 3 horizontal Bayer pattern images, now.
        // convert from Bayer pattern to interpolated RGB
        // BayerToRGB_
        res = BIAS::ImageConvert::ToRGB(tmp3, *pd_uc);
        if (res!=0){
          BIASERR("could not Deinterlace");
          return res;
        }
      } else {
        BIASERR("CM_PGR_XB3_F7M3_GBRG implemented only for unsigned char.");
        return -1;
      }
    }
    break;

  default:
    BIASERR("ImageConvert::ToRGB(): unknown color model:"<<
      source.GetColorModel());
    break;
  }
  tmpdest->SetColorModel(ImageBase::CM_RGB);

  if (inplace){
    dest = *tmpdest;
    delete tmpdest;
  }

  return res;
}



template <class StorageType>
int BIAS::ImageConvert::BGRToRGB_(const Image<StorageType>& source,
                                  Image<StorageType>& dest)
{

  BIASGDOUT(D_CONVERT,"BGRToRGB_()");
  StorageType *sink = dest.GetImageData();
  const StorageType *src = source.GetImageData();
  const unsigned long int pixel_count = dest.GetPixelCount();
  // in place
  if(src == sink){
    StorageType tmp;
    for (unsigned long int i=0;i<pixel_count; i++) {
      tmp = src[2];
      sink[2] = src[0];
      sink[0] = tmp;
      sink+=3;
      src+=3;
    }
  } else {
    for (unsigned long int i=0;i<pixel_count; i++) {
      sink[2] = src[0];
      sink[1] = src[1];
      sink[0] = src[2];
      sink+=3;
      src+=3;
    }
  }
  return 0;
}

template <class StorageType>
int BIAS::ImageConvert::RGBAToRGB_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"RGBAToRGB()");
  register StorageType *sink = dest.GetImageData();
  register const StorageType *src = source.GetImageData();
  // in place not necessary , as ToRGB() created new dist already
  for (unsigned long int i=0;i<dest.GetPixelCount(); i++) {
    sink[0] = src[0];
    sink[1] = src[1];
    sink[2] = src[2];
    sink+=3;
    src+=4;
  }

  return 0;
}



template <class StorageType>
int BIAS::ImageConvert::BGRAToRGB_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"RGBAToRGB()");
  register StorageType *sink = dest.GetImageData();
  register const StorageType *src = source.GetImageData();
  // in place not necessary , as ToRGB() created new dist already
  for (unsigned long int i=0;i<dest.GetPixelCount(); i++) {
    sink[2] = src[0];
    sink[1] = src[1];
    sink[0] = src[2];
    sink+=3;
    src+=4;
  }

  return 0;
}


template <class StorageType>
int BIAS::ImageConvert::YUV422ToRGB_(const Image<StorageType>& source,
                                     Image<StorageType>& dest)
{


  BIASGDOUT(D_CONVERT,"YUV422ToRGB_()");
  StorageType *sink;
  bool IdenticalImages = false;
  const StorageType *sourcedata = source.GetImageData();


  if (dest.GetImageData() == sourcedata) {
    IdenticalImages = true;
    sink = new StorageType[source.GetWidth() * source.GetHeight() * 3];
  }
  else
    sink = dest.GetImageData();


#define FASTYUV

#ifndef FASTYUV
  float y,u = 0.0,v=0.0;
  float r,g,b;

  int sinkoffset =0;
  int yoffset = 0;
  int uvoffset = 1;



  if (source.ColorModel_ == ImageBase::CM_UYVY422) {
    yoffset = 1;
    uvoffset = 0;
  }


  for (int j = 0; j < (source.GetWidth() * source.GetHeight()) ; j++)
  {
    //        y = 1.164 *(float) sourcedata[j*2 + yoffset]-16;
    y = (float)sourcedata[j*2 + yoffset];
    if ( j%2 == 0){
      u=  (float)sourcedata[j*2+ uvoffset]- 128;
    }
    else {
      v = (float) sourcedata[j*2+ uvoffset ]- 128;
    }

    b = rint(y              + 1.77 * u );
    g = rint(y - 0.714 * v  - 0.345 * u );
    r = rint(y + 1.4 * v );
    //        b = rint(y              + 2.018 * v );
    //        g = rint(y - 0.813 * u  - 0.391 * v );
    //        r = rint(y + 1.596 * u );
    r = r >255 ? 255: r;
    g = g >255 ? 255: g;
    b = b >255 ? 255: b;

    r =  r <0 ? 0: r;
    g =  g <0 ? 0: g;
    b =  b <0 ? 0: b;


    sink[sinkoffset++ ] = (unsigned char) r;
    sink[sinkoffset++] = (unsigned char) g;
    sink[sinkoffset++] = (unsigned char) b;
  }

#else
  unsigned int i,j;
  register int y0,y1,u,v,r,g,b;
  if (source.GetColorModel() == ImageBase::CM_UYVY422) {
    for (i = 0, j = 0; i < (source.GetWidth()*source.GetHeight() << 1); i += 4, j += 6)     {
      u = (unsigned char) sourcedata[i + 0] - 128;
      y0 = (unsigned char) sourcedata[i + 1];
      v = (unsigned char) sourcedata[i + 2] - 128;
      y1 = (unsigned char) sourcedata[i + 3];
      YUV2RGB (y0, u, v, r, g, b);
      sink[j + 0] = StorageType(r);
      sink[j + 1] = StorageType(g);
      sink[j + 2] = StorageType(b);
      YUV2RGB (y1, u, v, r, g, b);
      sink[j + 3] = StorageType(r);
      sink[j + 4] = StorageType(g);
      sink[j + 5] = StorageType(b);
    }

  }
  else {
    for (i = 0, j = 0; i < (dest.GetWidth()*dest.GetHeight() << 1); i += 4, j += 6)     {
      u = (unsigned char) sourcedata[i + 1] - 128;
      y0 = (unsigned char) sourcedata[i + 0];
      v = (unsigned char) sourcedata[i + 3] - 128;
      y1 = (unsigned char) sourcedata[i + 2];
      YUV2RGB (y0, u, v, r, g, b);
      sink[j + 0] = StorageType(r);
      sink[j + 1] = StorageType(g);
      sink[j + 2] = StorageType(b);
      YUV2RGB (y1, u, v, r, g, b);
      sink[j + 3] = StorageType(r);
      sink[j + 4] = StorageType(g);
      sink[j + 5] = StorageType(b);
    }
  }
#endif
  if (IdenticalImages) {
    dest.ReleaseImageDataPointer();
    dest.ChannelCount_=3;
    dest.RedirectImageDataPointer(sink);
  }

  return 0;
}


template <class StorageType>
int BIAS::ImageConvert::GreyToRGB_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"GreyToRGB_()");

  register const StorageType* src_start = source.GetImageData();
  register const StorageType* src_stop  = src_start + dest.GetPixelCount();
  register StorageType* pDest = dest.GetImageData();

  for (;src_start < src_stop; src_start++) {
    *pDest++ = *src_start;
    *pDest++ = *src_start;
    *pDest++ = *src_start;
  }

  return 0;
}

#ifdef BIAS_HAVE_DV
template <class StorageType>
int BIAS::ImageConvert::DVToRGB_(const Image<StorageType>& source,
                                 Image<StorageType>& dest,
                                 unsigned char quality )
{
  BIASERR("DVToRGB_ only makes sense with unsigned char images");
  return -1;
}


namespace BIAS {
template <>
int ImageConvert::DVToRGB_(const Image<unsigned char>& source,
                                 Image<unsigned char>& dest,
                                 unsigned char quality )
{
  BIASGDOUT(D_CONVERT,"DVToRGB_()");

  if (!dest.IsEmpty())
    if (dest.GetWidth() != 720 || dest.GetHeight() != 576
      || dest.GetChannelCount() != 3)
      dest.Release();


  if (dest.IsEmpty()) dest.Init(720, 576,3);


  unsigned char *sink = dest.GetImageData();
  //        4 ? from  avi2pics
  unsigned char *pixels[3];
  int pitches[3];

  pixels[0] = sink;  //target
  pixels[1] = NULL;
  pixels[2] = NULL;

  pitches[0] = 720 * 3;
  pitches[1] = 0;
  pitches[2] = 0;
  if (DVDecoder_ == NULL) DVDecoder_ = dv_decoder_new(0,0,0);
  if (DVDecoder_ == NULL) BIASERR("dv_decoder_new() failed.");
  //       else cout <<"Got new DVDecoder"<<endl;
  //     dv_init(0,0);// new style for 0.9.8
  switch (quality) {
  case 0:  DVDecoder_->quality = DV_QUALITY_FASTEST ;
    break;
  case 1: DVDecoder_->quality = DV_QUALITY_FASTEST | DV_QUALITY_AC_1;
    break;
  case 2: DVDecoder_->quality = DV_QUALITY_COLOR;
    break;
  case 3: DVDecoder_->quality = DV_QUALITY_COLOR | DV_QUALITY_AC_1;
    break;
  case 4: DVDecoder_->quality = DV_QUALITY_BEST;
    break;
  }
  //
  //  decoder->quality = DV_QUALITY_COLOR | DV_QUALITY_DC;
  //decoder->quality = DV_QUALITY_COLOR | DV_QUALITY_AC_1;
  //  decoder->quality = DV_QUALITY_BEST;

  dv_parse_header(DVDecoder_, source.GetImageData());
  dv_decode_full_frame(DVDecoder_, source.GetImageData(),
    e_dv_color_rgb, pixels, pitches);

  return 0;
}

} // namespace BIAS

#endif // of BIAS_HAVE_DV





template <class StorageType>
int BIAS::ImageConvert::BayerToGrey_(const Image<StorageType>& source ,
                                     Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT," Image::BayerToGreySlow_()");

  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;


  int Mask[8];
  StorageType R=0,G=0,B=0;
  unsigned int srcskip;
  srcskip = source.GetWidth() ;

  Mask[0] = - (int(source.GetWidth()) +1);
  Mask[1] = - int(source.GetWidth());
  Mask[2] = - (int(source.GetWidth()) -1);
  Mask[3] = -1;
  Mask[4] = +1;
  Mask[5] = int(source.GetWidth()) -1;
  Mask[6] = + int(source.GetWidth());
  Mask[7] = int(source.GetWidth()) +1;

  unsigned int xoffset = 0;
  unsigned int yoffset = 0;
  switch (source.GetColorModel()) {
  case ImageBase::CM_Bayer_RGGB:
    // first pixel in first line
    R = *src;
    G = (*(src+srcskip)+*(src+1))/2;
    B = *(src+srcskip+1);
    break;
  case ImageBase::CM_Bayer_GBRG:
    yoffset = 1;
    // first pixel in first line
    G = *src;
    R = *(src+srcskip);
    B = *(src+1);
    break;
  case ImageBase::CM_Bayer_GRBG:
    xoffset = 1;
    // first pixel in first line
    G = *src;
    B = *(src+srcskip);
    R = *(src+1);
    break;
  case ImageBase::CM_Bayer_BGGR:
    xoffset = 1;yoffset = 1;
    // first pixel in first line
    B = *src;
    G = (*(src+srcskip)+*(src+1))/2;
    R = *(src+srcskip+1);
    break;
  default:; }
  *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
  src++;
  // first line
  if (0 == yoffset) {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src+Mask[3])+ *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = ( *(src+Mask[5]) + *(src+Mask[7])) /2;
      }
      else { // staying on Green
        R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
        G = *src;
        B = (*(src+Mask[6]) ) ;
      }
      *sink++ = Cast<StorageType>(0.299*(double)R+
                                  0.587*(double)G+0.114*(double)B);
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[6]) ) ;
        G = *src;
        B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
      }
      else { // staying on Blue
        R = (  *(src+Mask[5]) + *(src+Mask[7])) /2;
        G = ( *(src+Mask[3]) + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = *src;
      }
      *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
      src++;
    }
  }
  // last pixel in first line
  if (0 == yoffset) {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[3])+  *(src+Mask[6]) ) /2;
      B =  *(src+Mask[5])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[3]);
      G = *src;
      B = (*(src+Mask[6]) ) ;
    }
  }
  else {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[6]) ) ;
      G = *src;
      B =  *(src+Mask[3]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[5]);
      G = ( *(src+Mask[3]) + *(src+Mask[6]) ) /2;
      B = *src;
    }
  }
  *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
  src++;


  //loop
  for (unsigned int y=1; y<(unsigned int)source.GetHeight()-1; y++) {
    // first pixel in line
    if (y%2 == yoffset)
      if (0%2 == xoffset) {  // staying on Red
        R = *src;
        G = (*(src+Mask[1])  + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = ( *(src+Mask[2]) + *(src+Mask[7])) /2;
      } else { // staying on Green
        R =  *(src+Mask[4]);
        G = *src;
        B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
      }
    else
      if (0 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
        G = *src;
        B = *(src+Mask[4]);
      }
      else { // staying on Blue
        R = ( *(src+Mask[0]) + *(src+Mask[7])) /2;
        G = (*(src+Mask[1])  + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = *src;
      }
      *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
      src++;
      // main part of line
      for (unsigned int x=1; x<(unsigned int)source.GetWidth()-1; x++) {
        if (y%2 == yoffset)
          if (x%2 == xoffset) {  // staying on Red
            R = *src;
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
            B = ( *(src+Mask[0]) + *(src+Mask[2])
              + *(src+Mask[5]) + *(src+Mask[7])) /4;
          } else { // staying on Green
            R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
            G = *src;
            B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
          }
        else
          if (x%2 == xoffset) {  // staying on Green
            R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
            G = *src;
            B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
          }
          else { // staying on Blue
            R = ( *(src+Mask[0]) + *(src+Mask[2])
              + *(src+Mask[5]) + *(src+Mask[7])) /4;
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
            B = *src;
          }
          *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
          src++;
      }
      // last pixel in line
      if (y%2 == yoffset)
        if ((srcskip-1)%2 == xoffset) {  // staying on Red
          R = *src;
          G = (*(src+Mask[1]) + *(src+Mask[3]) + *(src+Mask[6]) ) /3;
          B = ( *(src+Mask[0]) + *(src+Mask[5])) /2;
        } else { // staying on Green
          R =  *(src+Mask[3]) ;
          G = *src;
          B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
        }
      else
        if ((srcskip-1)%2 == xoffset) {  // staying on Green
          R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
          G = *src;
          B = *(src+Mask[3]);
        }
        else { // staying on Blue
          R = ( *(src+Mask[0]) + *(src+Mask[5])) /2;
          G = (*(src+Mask[1]) + *(src+Mask[3])+ *(src+Mask[6]) ) /3;
          B = *src;
        }
        *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
        src++;


  }
  // first pixel in last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    if (0 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[1])+  *(src+Mask[4]) ) /2;
      B =  *(src+Mask[2])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[4]);
      G = *src;
      B = (*(src+Mask[1]) ) ;
    }
  }
  else {
    if (0 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[1]) ) ;
      G = *src;
      B =  *(src+Mask[4]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[2]);
      G = ( *(src+Mask[1]) + *(src+Mask[4]) ) /2;
      B = *src;
    }
  }
  *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
  src++;
  // last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src+Mask[3])+ *(src+Mask[1]) + *(src+Mask[4]) ) /3;
        B = ( *(src+Mask[0]) + *(src+Mask[2])) /2;
      }
      else { // staying on Green
        R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
        G = *src;
        B = (*(src+Mask[1]) ) ;
      }
      *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[1]) ) ;
        G = *src;
        B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
      }
      else { // staying on Blue
        R = (  *(src+Mask[0]) + *(src+Mask[2])) /2;
        G = ( *(src+Mask[1]) + *(src+Mask[3]) + *(src+Mask[4]) ) /3;
        B = *src;
      }
      *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
      src++;
    }
  }
  // last pixel in last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[3])+  *(src+Mask[1]) ) /2;
      B =  *(src+Mask[0])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[3]);
      G = *src;
      B = (*(src+Mask[1]) ) ;
    }
  }
  else {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[1]) ) ;
      G = *src;
      B =  *(src+Mask[3]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[0]);
      G = ( *(src+Mask[3]) + *(src+Mask[1]) ) /2;
      B = *src;
    }
  }
  *sink++ = Cast<StorageType>(0.299*(double)R+0.587*(double)G+0.114*(double)B);
  src++;
  return 0;
}



template <class StorageType>
int BIAS::ImageConvert::BayerToRGB_(const Image<StorageType>& source,
                                    Image<StorageType>&  dest )
{
  BIASGDOUT(D_CONVERT," Image::BayerToRGB_()");

  //BayerToRGB(source,dest,BAYER_DEMOSAIC_METHOD_BILINEAR);

  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;

 // BayerToRGBSimple_((unsigned char*)source.GetImageData(),(unsigned char*)dest.GetImageData(),
 //   source.GetWidth(),source.GetHeight(),source.GetColorModel());


  //BayerToRGBPGR_(source, dest );



  BIASASSERT(sink!=NULL);
  BIASASSERT(src!=NULL);


  int Mask[8];
  StorageType R=0,G=0,B=0;
  unsigned int srcskip;
  srcskip = source.GetWidth() ;

  Mask[0] = - (int(source.GetWidth()) +1);
  Mask[1] = - int(source.GetWidth());
  Mask[2] = - (int(source.GetWidth()) -1);
  Mask[3] = -1;
  Mask[4] = +1;
  Mask[5] = int(source.GetWidth()) -1;
  Mask[6] = + int(source.GetWidth());
  Mask[7] = int(source.GetWidth()) +1;

  unsigned int xoffset = 0;
  unsigned int yoffset = 0;
  switch (source.GetColorModel()) {
  case ImageBase::CM_Bayer_RGGB:
    // first pixel in first line
    R = *src;
    G = (*(src+srcskip)+*(src+1))/2;
    B = *(src+srcskip+1);
    break;
  case ImageBase::CM_Bayer_GBRG:
    yoffset = 1;
    // first pixel in first line
    G = *src;
    R = *(src+srcskip);
    B = *(src+1);
    break;
  case ImageBase::CM_Bayer_GRBG:
    xoffset = 1;
    // first pixel in first line
    G = *src;
    B = *(src+srcskip);
    R = *(src+1);
    break;
  case ImageBase::CM_Bayer_BGGR:
    xoffset = 1;yoffset = 1;
    // first pixel in first line
    B = *src;
    G = (*(src+srcskip)+*(src+1))/2;
    R = *(src+srcskip+1);
    break;
  default:; }
  sink[0]=R; sink[1]=G; sink[2]=B;
  sink+=3; src++;
  // first line
  if (0 == yoffset) {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src+Mask[3])+ *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = ( *(src+Mask[5]) + *(src+Mask[7])) /2;
      }
      else { // staying on Green
        R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
        G = *src;
        B = (*(src+Mask[6]) ) ;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[6]) ) ;
        G = *src;
        B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
      }
      else { // staying on Blue
        R = (  *(src+Mask[5]) + *(src+Mask[7])) /2;
        G = ( *(src+Mask[3]) + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  // last pixel in first line
  if (0 == yoffset) {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[3])+  *(src+Mask[6]) ) /2;
      B =  *(src+Mask[5])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[3]);
      G = *src;
      B = (*(src+Mask[6]) ) ;
    }
  }
  else {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[6]) ) ;
      G = *src;
      B =  *(src+Mask[3]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[5]);
      G = ( *(src+Mask[3]) + *(src+Mask[6]) ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;


  //loop
  for (unsigned int y=1; y<(unsigned int)source.GetHeight()-1; y++) {
    // first pixel in line
    if (y%2 == yoffset)
      if (0%2 == xoffset) {  // staying on Red
        R = *src;
        G = (*(src+Mask[1])  + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = ( *(src+Mask[2]) + *(src+Mask[7])) /2;
      } else { // staying on Green
        R =  *(src+Mask[4]);
        G = *src;
        B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
      }
    else
      if (0 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
        G = *src;
        B = *(src+Mask[4]);
      }
      else { // staying on Blue
        R = ( *(src+Mask[0]) + *(src+Mask[7])) /2;
        G = (*(src+Mask[1])  + *(src+Mask[4]) + *(src+Mask[6]) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
      // main part of line
      for (unsigned int x=1; x<(unsigned int)source.GetWidth()-1; x++) {
        if (y%2 == yoffset)
          if (x%2 == xoffset) {  // staying on Red
            R = *src;
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
            B = ( *(src+Mask[0]) + *(src+Mask[2])
              + *(src+Mask[5]) + *(src+Mask[7])) /4;
          } else { // staying on Green
            R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
            G = *src;
            B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
          }
        else
          if (x%2 == xoffset) {  // staying on Green
            R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
            G = *src;
            B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
          }
          else { // staying on Blue
            R = ( *(src+Mask[0]) + *(src+Mask[2])
              + *(src+Mask[5]) + *(src+Mask[7])) /4;
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
            B = *src;
          }
          *sink++ =R;
          *sink++ =G;
          *sink++ =B;
          src++;
      }
      // last pixel in line
      if (y%2 == yoffset)
        if ((srcskip-1)%2 == xoffset) {  // staying on Red
          R = *src;
          G = (*(src+Mask[1]) + *(src+Mask[3]) + *(src+Mask[6]) ) /3;
          B = ( *(src+Mask[0]) + *(src+Mask[5])) /2;
        } else { // staying on Green
          R =  *(src+Mask[3]) ;
          G = *src;
          B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
        }
      else
        if ((srcskip-1)%2 == xoffset) {  // staying on Green
          R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
          G = *src;
          B = *(src+Mask[3]);
        }
        else { // staying on Blue
          R = ( *(src+Mask[0]) + *(src+Mask[5])) /2;
          G = (*(src+Mask[1]) + *(src+Mask[3])+ *(src+Mask[6]) ) /3;
          B = *src;
        }
        *sink++ =R;
        *sink++ =G;
        *sink++ =B;
        src++;


  }
  // first pixel in last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    if (0 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[1])+  *(src+Mask[4]) ) /2;
      B =  *(src+Mask[2])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[4]);
      G = *src;
      B = (*(src+Mask[1]) ) ;
    }
  }
  else {
    if (0 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[1]) ) ;
      G = *src;
      B =  *(src+Mask[4]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[2]);
      G = ( *(src+Mask[1]) + *(src+Mask[4]) ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;
  // last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src+Mask[3])+ *(src+Mask[1]) + *(src+Mask[4]) ) /3;
        B = ( *(src+Mask[0]) + *(src+Mask[2])) /2;
      }
      else { // staying on Green
        R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
        G = *src;
        B = (*(src+Mask[1]) ) ;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<(unsigned int)dest.GetWidth()-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src+Mask[1]) ) ;
        G = *src;
        B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
      }
      else { // staying on Blue
        R = (  *(src+Mask[0]) + *(src+Mask[2])) /2;
        G = ( *(src+Mask[1]) + *(src+Mask[3]) + *(src+Mask[4]) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  // last pixel in last line
  if ((source.GetHeight()-1)%2 == yoffset) {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+Mask[3])+  *(src+Mask[1]) ) /2;
      B =  *(src+Mask[0])  ;
    }
    else { // staying on Green
      R =  *(src+Mask[3]);
      G = *src;
      B = (*(src+Mask[1]) ) ;
    }
  }
  else {
    if ((dest.GetWidth()-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src+Mask[1]) ) ;
      G = *src;
      B =  *(src+Mask[3]) ;
    }
    else { // staying on Blue
      R =  *(src+Mask[0]);
      G = ( *(src+Mask[3]) + *(src+Mask[1]) ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;


  return 0;
}

template <class StorageType>
int BIAS::ImageConvert::BayerToRGB(const Image<StorageType>& source, Image<StorageType>& dest,BayerDemosaicMethod method, bool flip)
{
  switch(method){
    case BAYER_DEMOSAIC_METHOD_SIMPLE: 
      if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBSimple_((unsigned char*)source.GetImageData(),(unsigned char*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel());
      }else if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBSimple_((unsigned short*)source.GetImageData(),(unsigned short*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel());
      }else{
        BIASERR("ONLY IMPLEMENTED FOR unsigned char and unsigned short");
      }
      break;
    case BAYER_DEMOSAIC_METHOD_NEAREST:
      BayerToRGBNearestNeighbour_(source.GetImageData(),dest.GetImageData(),
    source.GetWidth(),source.GetHeight(),source.GetColorModel());
      break;
    case BAYER_DEMOSAIC_METHOD_BILINEAR:
      if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBBilinear_((unsigned char*)source.GetImageData(),(unsigned char*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel());
      }else if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBBilinear_((unsigned short*)source.GetImageData(),(unsigned short*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel());
      }else{
        BIASERR("ONLY IMPLEMENTED FOR unsigned char and unsigned short");
      }
      break;
    case BAYER_DEMOSAIC_METHOD_HQLINEAR:
      if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBHQLinear_((unsigned char*)source.GetImageData(),(unsigned char*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel());
      }else if(source.GetStorageType() == ImageBase::ST_unsignedchar){
        BayerToRGBHQLinear_((unsigned short*)source.GetImageData(),(unsigned short*)dest.GetImageData(),
          source.GetWidth(),source.GetHeight(),source.GetColorModel(),source.GetBitDepth());
      }else{
        BIASERR("ONLY IMPLEMENTED FOR unsigned char and unsigned short");
      }
      break;
    case BAYER_DEMOSAIC_METHOD_AHD:
      BIASERR("NOT IMPLEMENTED");
      break;

    case BAYER_DEMOSAIC_METHOD_VNG:
      BIASERR("NOT IMPLEMENTED YET");
      break;

    default:
      BIASERR("NO DEFAULT OPERATION");
      break;
  }

  if(flip)
    dest.FlipHorizontal();

  return 0;

}






// 2 pixel border in dest is black
template <class StorageType>
int BIAS::ImageConvert::BayerToRGBSlow_(const Image<StorageType>& source,
                                        Image<StorageType>& dest )
{
  BIASGDOUT(D_CONVERT," Image::BayerToRGBSlow_()");

  StorageType *sink = dest.GetImageData() + 2 * dest.GetWidthStep() + 6;
  const StorageType *src = source.GetImageData() + 2 * source.GetWidth() + 2;

  int Mask[12];
  StorageType R,G,B, d1, d2;

  Mask[0] = - (int(source.GetWidth()) +1);
  Mask[1] = - int(source.GetWidth());
  Mask[2] = - (int(source.GetWidth()) -1);
  Mask[3] = -1;
  Mask[4] = +1;
  Mask[5] = int(source.GetWidth()) -1;
  Mask[6] = + int(source.GetWidth());
  Mask[7] = int(source.GetWidth()) +1;
  Mask[8] = - 2 * int(source.GetWidth());
  Mask[9] = - 2 ;
  Mask[10] = + 2 ;
  Mask[11] = + 2 * int(source.GetWidth());

#define D1 (*(src+Mask[8])>*(src+Mask[11]))?\
  (*(src+Mask[8])-*(src+Mask[11])):\
  (*(src+Mask[11])-*(src+Mask[8]))
#define D2 (*(src+Mask[9])>*(src+Mask[10]))?\
  (*(src+Mask[9])-*(src+Mask[10])):\
  (*(src+Mask[10])-*(src+Mask[9]))

  unsigned int xoffset = 0;
  unsigned int yoffset = 0;
  switch (source.GetColorModel()) {
  case ImageBase::CM_Bayer_GBRG: yoffset = 1; break;
  case ImageBase::CM_Bayer_GRBG: xoffset = 1; break;
  case ImageBase::CM_Bayer_BGGR: xoffset = 1;yoffset = 1; break;
  default:; }

  for (unsigned int y=2; y<(unsigned int)source.GetHeight()-2; y++) {
    for (unsigned int x=2; x<(unsigned int)source.GetWidth()-2; x++) {
      if (y%2 == yoffset)
        if (x%2 == xoffset) {  // staying on Red
          R = *src;
          d1 = D1;
          d2 = D2;
          if (d1<d2){
            G = (*(src+Mask[1]) + *(src+Mask[6]) ) /2;
          } else if (d2<d1){
            G = (*(src+Mask[3]) + *(src+Mask[4]) ) /2;
          } else {
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
          }
          B = ( *(src+Mask[0]) + *(src+Mask[2])
            + *(src+Mask[5]) + *(src+Mask[7])) /4;
        }
        else { // staying on Green
          R =  ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
          G = *src;
          B = ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
        }
      else
        if (x%2 == xoffset) {  // staying on Green
          R =  ( *(src+Mask[1]) + *(src+Mask[6]) ) /2;
          G = *src;
          B = ( *(src+Mask[3]) + *(src+Mask[4]) ) /2;
        }
        else { // staying on Blue
          R = ( *(src+Mask[0]) + *(src+Mask[2])
            + *(src+Mask[5]) + *(src+Mask[7])) /4;
          d1 = D1;
          d2 = D2;
          if (d1<d2){
            G = (*(src+Mask[1]) + *(src+Mask[6]) ) /2;
          } else if (d2<d1){
            G = (*(src+Mask[3]) + *(src+Mask[4]) ) /2;
          } else {
            G = (*(src+Mask[1]) + *(src+Mask[3])
              + *(src+Mask[4]) + *(src+Mask[6]) ) /4;
          }
          B = *src;
        }
        *sink++ =R;
        *sink++ =G;
        *sink++ =B;
        src++;
    }
    sink += 12;
    src += 4;
  }
  dest.SetROI(2,2,dest.GetWidth()-2,dest.GetHeight()-2);
  return 0;
}

template <class StorageType>
int BIAS::ImageConvert::YUV420PToRGB_(const Image<StorageType>& source,
                                      Image<StorageType>&  dest )
{
  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;
  float y,u = 0.0,v=0.0;
  float r,g,b;
  int uoffset=0 ;
  int voffset=0 ;
  int width = source.GetWidth();
  int height = source.GetHeight();

  for (int z = 0; z < height ; z++) {
    if (z%2==0) {
      voffset = width*height +z/2*width/2;
      uoffset = width*height + width*height/4+z/2*width/2;
    }
    for (int x=0; x< width; x++) {
      y = 1.164f * (float) (src[z*width + x] -16) ;
      if (  x%2==0 ) {
        v = (float) (src[voffset + x/2] -128);
        u = (float) (src[uoffset + x/2 ] -128);
      }
      b =  float(rint( y + 2.018f*v ));
      g =  float(rint( y - 0.813f*u  - 0.391f*v));
      r =  float(rint( y + 1.596f*u));
      if (r>255) r=255;
      if (g>255) g=255;
      if (b>255) b=255;
      if (r<0) r = 0;
      if (g<0) g = 0;
      if (b<0) b = 0;

      sink[(z*width + x)*3 ] = (unsigned char) r;
      sink[(z*width + x)*3 +1] =(unsigned char)g;
      sink[(z*width + x)*3 +2] =(unsigned char) b;
    }
  }
  return 0;
}

template <class StorageType>
int BIAS::ImageConvert::YUV420PToGrey_(const Image<StorageType>& source,
                                       Image<StorageType>&  dest )
{
  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;

  int width = source.GetWidth();
  int height = source.GetHeight();

  for (int z = 0; z < height ; z++)
    for (int x=0; x< width; x++)
      sink[(z*width + x) ] =  src[z*width + x];

  return 0;
}


template <class StorageType>
int BIAS::ImageConvert::YUV411ToGrey_(const Image<StorageType>& source,
                                       Image<StorageType>&  dest )
{
  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;

  int width = source.GetWidth();
  int height = source.GetHeight();

  for (int z = 0; z < height ; z++)
    for (int x=0; x< width; x++)
      sink[(z*width + x) ] =  src[z*width*2 + x*2];

  return 0;
}




int ImageConvert::FloatToUC_(const ImageBase &source, ImageBase &dest)
{
  unsigned int values = source.GetPixelCount() * source.GetChannelCount();

  unsigned char *d = (unsigned char*)dest.GetImageData();
  float *s =(float*) source.GetImageData();
  for (unsigned int i=0; i<values; i++){
    d[i] = (unsigned char) *s;
    s ++;
  }

  return 0;
}



//  Now storage type conversion
int ImageConvert::ConvertST(const ImageBase &source, ImageBase &dest,
                            ImageBase::EStorageType targetST)
{
  if (!dest.IsEmpty()) dest.Release();
  dest.Init(source.GetWidth(), source.GetHeight(), source.GetChannelCount(),
            targetST);
  if (targetST==source.GetStorageType()){
    dest=source;
    return 0;
  }

  dest.SetColorModel(source.GetColorModel());

  // this is NumPixels * Channels
  unsigned int values = source.GetPixelCount() * source.GetChannelCount();

  switch(targetST) {
#ifdef BUILD_IMAGE_DOUBLE
	case ImageBase::ST_double: {
    double* destPel = (double*)dest.GetImageData();
    float* sourcePel = (float*)source.GetImageData();
        
    switch (source.GetStorageType()) 
      {
      case ImageBase::ST_float: {
        for (unsigned int i=0; i<values; i++)
          destPel[i] = (double) sourcePel[i];      
      } break;
      default: {
        //this is quite unefficient: implement a specialized case (see above) if 
        //you need it faster 
        const unsigned int width = source.GetWidth();
        
        for(unsigned int y=0;y<source.GetHeight(); y++) {
          for(unsigned int x=0;x<source.GetWidth(); x++) {
            for(unsigned int c=0;x<source.GetChannelCount(); c++) {
              destPel[(width*y+x)*source.GetChannelCount()+c] = 
                ImageBase::GetValue<double>(source, x,y,c);
            }
          }
        }//y
      } break;
      };
  }break; //end of double
#endif
	case ImageBase::ST_unsignedchar:
    {   //target
	    unsigned char *d = (unsigned char*)dest.GetImageData();
	    switch (source.GetStorageType()){
      case ImageBase::ST_float:
		    return FloatToUC_(source,dest); break;
#ifdef BUILD_IMAGE_INT
      case ImageBase::ST_int:
        {
          int *s =(int*) source.GetImageData();
          for (unsigned int i=0; i<values; i++)
            d[i] = (unsigned char) s[i];
        }
        break;
#endif // BUILD_IMAGE_INT
#ifdef BUILD_IMAGE_USHORT
      case ImageBase::ST_unsignedshortint:
        {
          unsigned short int *s =(unsigned short int*) source.GetImageData();
          for (unsigned int i=0; i<values; i++)
            d[i] = (unsigned char) s[i];
        }
        break;
#endif // BUILD_IMAGE_SHORT
#ifdef BUILD_IMAGE_CHAR
      case ImageBase::ST_char:
        {
          char *s = (char*)source.GetImageData();
          for (unsigned int i=0; i<values; i++)
            d[i] = (unsigned char) s[i];
        }
        break;
#endif // BUILD_IMAGE_CHAR
#ifdef BUILD_IMAGE_SHORT
      case ImageBase::ST_shortint:
        {
          short *s=(short*)source.GetImageData();
          for (unsigned int i=0; i<values; i++)
            d[i] = (unsigned char) s[i];
        }
        break;
#endif // BUILD_IMAGE_SHORT
      default:
		    BIASERR("ConvertST() not fully implemented "<<source.GetStorageType());
        BIASABORT;
		    return -1;
	    }

    }
    break;
	case ImageBase::ST_int:
    {
	    int *d = (int*)dest.GetImageData();
	    switch (source.GetStorageType()){
      case ImageBase::ST_unsignedchar:
        {
          unsigned char *s =(unsigned char*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (int) s[i];
        }
        break;
      case ImageBase::ST_char:
        {
          char *s =(char*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (int) s[i];
        }
        break;
      case ImageBase::ST_unsignedshortint:
        {
          unsigned short *s =(unsigned short*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (int) s[i];
        }
        break;
      case ImageBase::ST_float:
	{
          float *s =(float*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (int) s[i];
	}
	break;

      default:
		    BIASERR("ConvertST() not fully implemented");  return -1;
	    }
    } break;
	case ImageBase::ST_unsignedshortint:
    {
	    unsigned short int *d = (unsigned short int*)dest.GetImageData();
	    switch (source.GetStorageType())
        {
        case ImageBase::ST_unsignedchar:
          {
            unsigned char *s =(unsigned char*) source.GetImageData();
            for (unsigned int i=0; i<values; i++)
              d[i] = (unsigned short int) s[i];
          }
          break;
        default:
          BIASERR("ConvertST() not fully implemented");  return -1;
        }
    } break;
	case ImageBase::ST_float: // target
    {
	    float *d = (float*)dest.GetImageData();
	    switch (source.GetStorageType())   {
        case ImageBase::ST_unsignedchar:   {
            unsigned char *s =(unsigned char*) source.GetImageData();
            unsigned char *e = s+values;
            while (s<e)
              *d++=(float)*s++;
          }
          break;
        case ImageBase::ST_char:  {
            char *s =(char*) source.GetImageData();
            char *e = s+values;
            while (s<e) *d++=(float)*s++;
          }
          break;
        case ImageBase::ST_unsignedshortint:   {
            unsigned short *s =(unsigned short*) source.GetImageData();
            for (unsigned int i=0; i<values; i++) d[i] = (float) s[i];
          }
          break;  
#ifdef BUILD_IMAGE_SHORT
        case ImageBase::ST_shortint:{
            short *s =(short*) source.GetImageData();
            for (unsigned int i=0; i<values; i++) d[i] = (float) s[i];
          }
          break;  
#endif
        
#ifdef BUILD_IMAGE_INT
        case ImageBase::ST_int: {
          int *s =(int*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (float) s[i];
         }
	     break;
#endif // BUILD_IMAGE_INT
#ifdef BUILD_IMAGE_DOUBLE
        case ImageBase::ST_double: {
          double *s =(double*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (float) s[i];
         }
	     break;
#endif // BUILD_IMAGE_INT

      default:
        BIASERR("unhandled storage type: "<<source.GetStorageType()<<endl);
        BIASABORT;
        break;
        } //swutch (sourceST)
    } // target float
    break;
#ifdef BUILD_IMAGE_SHORT
    case ImageBase::ST_shortint:  //target
    {  
	    unsigned char *d = (unsigned char*)dest.GetImageData();
	    switch (source.GetStorageType()){
      case ImageBase::ST_float:{
          float *s =(float*) source.GetImageData();
          for (unsigned int i=0; i<values; i++) d[i] = (unsigned char) s[i];
         }
	     break;

        default:
          BIASERR("ConvertST() not fully implemented "<<source.GetStorageType());
          return -1;
        } // switch soureceST
    } //target shortint
    break;
#endif // BUILD_IMAGE_SHORT
    
    
    default:
    BIASERR("ConvertST() not implemented for this target ST"); return -1;
    break;
  }
  int tlx, tly, brx, bry;
  source.GetROI()->GetCorners(tlx, tly, brx, bry);
  dest.GetROI()->SetCorners(tlx, tly, brx, bry);

  return 0;
}


template <class StorageType>
int BIAS::ImageConvert::HSLToGrey_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
  BIASGDOUT(D_CONVERT,"ImageConvert::hsLToGrey_()");
  StorageType *p=dest.GetImageData();
  const StorageType *h, *s, *L;

  if (source.IsPlanar()){
    switch (source.GetColorModel()){
    case ImageBase::CM_hsL:
    case ImageBase::CM_HSL:
      h=source.GetImageData();
      s=h+(source.GetWidth()*source.GetHeight());
      L=s+(source.GetWidth()*source.GetHeight());
      break;
    default:
      BIASERR("HSLToGrey_ must be called with HSL or hsL ColorModel_");
      return -1;
    }
    for (unsigned int i=0; i < dest.GetWidth()*dest.GetHeight(); i++){
      *p=(StorageType)*L;
      L++; p++;
    }
  } else { // interleaved
    switch (source.GetColorModel()) {
    case ImageBase::CM_hsL:
    case ImageBase::CM_HSL:
      h=source.GetImageData();
      s=h+1;
      L=h+2;
      break;
    default:
      BIASERR("HSLToGrey_ must be called with HSL or hsL ColorModel_");
      return -1;
    }
    for (unsigned int i=0; i < dest.GetWidth()*dest.GetHeight(); i++){
      *p=(StorageType)(*L);
      p++; L+=3;
    }
  }
  return 0;
}

#ifdef BIAS_HAVE_IMAGEMAGICKLIB

int BIAS::ImageConvert::
BIAS2ImageMagick(const ImageBase& input, Magick::Image& image)
{
	#ifdef WIN32
	//initialize DLL loaded library (not required on Linux but once on Windows)
	Magick::InitializeMagick(NULL);
	#endif

  // manipulate image information, e.g. compression

  image.colorSpace(sRGBColorspace); // default

  // determine export image format/channels:
  std::string pixmap;
  switch (input.GetChannelCount())
  {
  case 1:
  	pixmap="K";
  	image.colorSpace(GRAYColorspace); // default
  	break;
  case 2:
  	pixmap="KA"; break;
  case 3:
  	pixmap="RGB"; break;
  case 4:
  	pixmap="RGBA"; break;
  default:
  	BIASERR("unsupported channelcount. Could not convert image to ImageMagick++");
  	return -1;
  }

  // refine the color space used
  // there is a difference between color model and channel order
  switch (input.GetColorModel())
  {
  case BIAS::ImageBase::CM_RGB :
  	// color model
        //TEST image.colorSpace(RGBColorspace);
  	// channel order (within colro model)
  	pixmap="RGB";
  	break;
  case BIAS::ImageBase::CM_BGR :
        image.colorSpace(sRGBColorspace);
  	pixmap="BGR";
  	break;
  case BIAS::ImageBase::CM_RGBA :
        image.colorSpace(sRGBColorspace);
  	pixmap="RGBA";
  	break;
  	//case BIAS::ImageBase::CM::CM_BGBA :
  	//	pixmap="BGRA";
  	//	//image.colorSpace(RGBColorspace);
  	//	break;
  case BIAS::ImageBase::CM_Grey :
  	pixmap="K";
  	image.colorSpace(GRAYColorspace);
  	image.type(GrayscaleType);
  	break;
  case BIAS::ImageBase::CM_GreyA :
  	pixmap="KA";
  	//image_info->colorspace=GRAYColorspace;
  	image.colorSpace(UndefinedColorspace);
  	image.type(GrayscaleMatteType);
  	break;
  case BIAS::ImageBase::CM_HSV :
  	// HSV=HSB (which is different from HSI and HSL!)
  	image.colorSpace(HSBColorspace);
  	break;
  case BIAS::ImageBase::CM_HSL :
  	// HSL=HSI (which is different from HSV nor HSB!)
  	image.colorSpace(HSLColorspace);
  	break;
  default:
  	BIASERR("unsupported color model. Please add switch for "<<input.GetColorModel());
  }

  // Read a file into image object using Magick++ library function:
  unsigned char* p_imageData     =  (unsigned char*)input.GetImageData();
  BIASASSERT(p_imageData!=NULL);

  int depth = 8;
  // determine per pixel input/output storagetype:
  Magick::StorageType magick_ST=Magick::CharPixel; // default

  // CharPixel, ShortPixel, IntegerPixel, FloatPixel, or DoublePixel
  switch (input.GetStorageType())
    {
    case BIAS::ImageBase::ST_unsignedchar:
    case BIAS::ImageBase::ST_char:
      magick_ST=Magick::CharPixel;
      depth = (8*sizeof(char)); // usually 8 bit
      break;
    case BIAS::ImageBase::ST_unsignedshortint:
    case BIAS::ImageBase::ST_shortint:
      magick_ST=Magick::ShortPixel;
      depth = (8*sizeof(short)); // usually 16bit
      break;
    case BIAS::ImageBase::ST_unsignedint:
    case BIAS::ImageBase::ST_int:
      magick_ST=Magick::IntegerPixel;
      depth = (8*sizeof(int)); // usually 32bit
      break;
    case BIAS::ImageBase::ST_float:
      magick_ST=Magick::FloatPixel;
      depth = (8*sizeof(float)); // usually 32bit
      break;
    case BIAS::ImageBase::ST_double:
      magick_ST=Magick::DoublePixel;
      depth = (8*sizeof(double)); // usually 64 bit
      break;
    default:
      BIASERR("unsupported input image BIAS StorageType "<<input.GetStorageType() );
    };
  // create ImageMagick pixel buffer object from our image data:
  image.read(input.GetWidth(), // dimension
             input.GetHeight(),
             pixmap, // output map type. e.g. RGBA
             magick_ST, // desired storagetype
             p_imageData  // pointer to our own data array to be written to disk
             );
  image.depth(depth); // default depth in bit

  // input depth and switch case depth should match
  // check if ImageMagick has enough bits to store our data
  if (8*input.GetDepth() != image.depth()){
    BIASERR("ImageMagick bitdepth="<<image.depth()
            <<" seems to be smaller than requested "<<8*input.GetDepth()<<" bit input"<<endl
            <<"Do you use a bit-limited ImageMagick version , e.g. ImageMagick-16bit?"<<endl
            <<"You cannot convert this image to ImageMagick Image with your version of ImageMagick due to dynamic range problems!"
            );
    return -1;
  }
  BIASASSERT( 8*input.GetDepth() == image.depth() );
  //image.write("conversiontest.png");
	return 0;
}
//////////////////////////////////////////////////////////////////////////
int BIAS::ImageConvert::
ImageMagick2BIAS(Magick::Image& image, BIAS::ImageBase& result)
{
	#ifdef WIN32
	//initialize DLL loaded library (not required on Linux but once on Windows)
	Magick::InitializeMagick(NULL);
	#endif

	try {
		// now extract image info from Magick::Image
		unsigned width=(unsigned int)image.columns();
		unsigned height=(unsigned int)image.rows();
		unsigned bitdepth=(unsigned int)image.depth();
		unsigned depth=0;
		// bitdepth may ba a fractional part of the Byte depth, eg. 12/15 bits are filled.
		while (depth<bitdepth)
			depth+=8;

		//this divides by 8 and cuts off rest
		//e.g. making 1 from 12 or 2 from 17
		depth /= 8; // bit To Byte

		unsigned cc=0; // ChannelCount
                //ImageBase::EColorModel cm=ImageBase::CM_invalid;
		ImageBase::EStorageType st=ImageBase::ST_invalid;
		Magick::StorageType imst=MagickLib::UndefinedPixel;

		/// LibMagick: imcs is RGB even for one channel images (bug?)
		Magick::ColorspaceType imcs=image.image()->colorspace;
		std::string cm_map="";

		// choose appropriate storage type
		switch (depth)
		{
		case sizeof(unsigned char):
      st=ImageBase::ST_unsignedchar;
		  imst=MagickLib::CharPixel;
		break;
#ifdef BUILD_IMAGE_USHORT
		case sizeof(unsigned short):
      st=ImageBase::ST_unsignedshortint;
		  imst=MagickLib::ShortPixel;
		  break;
#endif
		case sizeof(float): // TODO This could also be int!
      st=ImageBase::ST_float;
		  imst=MagickLib::FloatPixel;
		  break;
		default:
			BIASERR("Invalid depth "<<depth<<" byte given (maybe unfinished code)!");   
      
      //very strange behaviour, bitdepth varies on different computers /virtual machines
      //I think it is a bug ion ImageMagick, which reads bitdepth=8 and depth =1 on one machine
      //and bitdepth=16 and depth=2 -> Error on the other machine
      //\todo fix this error. 
      /*if(depth==2){ 
        depth=1;
        st=ImageBase::ST_unsignedchar;
		    imst=MagickLib::CharPixel;
      }
      else */
      return -4;
			break;
		}

		// choose appropriate color model
		switch (imcs)
		{
		case MagickLib::GRAYColorspace:
                        //cm=ImageBase::CM_Grey;
			cm_map="R";
			cc=1;
			break;

		case MagickLib::RGBColorspace:
                case MagickLib::sRGBColorspace:
		{
			// TODO:  or 4, e.g. for RGBA
			if (image.channelDepth(MagickLib::AlphaChannel)>1){
                                //cm=ImageBase::CM_RGBA;
				cm_map="RGBA";
				cc=4;
			} else {
                                //cm=ImageBase::CM_RGB;
				cm_map="RGB";
				cc=3;
			}
			break;
		}

		default:
			// look in ImageMagick.h for further ColorspaceType's
			BIASERR("Color model:"<<imcs<<" not implemented. You may want to add a colorspace/map handler.");
			return -3;
			break;
		}
		


		if (!result.IsEmpty()) result.Release(true);

		// do error checking if result has already a specific StorageType
		if (result.GetStorageType()!=ImageBase::ST_invalid){
			if (result.GetDepth()!=depth){
				BIASERR("Error: You tried to load a "<<depth<<" byte "
						<<"("<<bitdepth<<"bit) "
						<<"image into an image of storage type "
						<<result.GetStorageType()<<" which has depth "
						<<result.GetDepth()<<" byte.");
				result.PrintHeader(cout);
				//BIASABORT;
				return -2;
			}
		}

		result.Init(width, height, cc, st, true);
		//result.PrintHeader(cout);
		result.SetBitDepth(bitdepth);
		image.write(0, 0, width, height, cm_map, imst, result.GetImageData());
        BIASASSERT(result.GetImageData()!=NULL);
		// verbose?
	}//end try
	catch ( Magick::Exception & error_) {
		// be silent about the reason why Magick couldn't load the file.
    BIASERR("ImageMagick2BIAS failed."<<endl
				<<"  Caught Magick exception: "<<endl
				<<"  "<<error_.what()<<endl );
		return -1; // error or unsupported format
	}
	catch ( std::exception & e) {
    BIASERR("ImageMagick2BIAS failed."<<endl
				<<"  Caught std exception: "<<endl
				<<"  "<<e.what()<<endl );
		return -3;
	}
	catch (...) {
    BIASERR("ImageMagick2BIAS failed."<<endl
			<<"  Caught unknown exception. "<<endl);
		return -4;
	}

	return 0; //OK
}

#endif



template <class StorageType>
int ImageConvert::BIAS2ipl(const BIAS::Image<StorageType>& source, IplImage*  &dest)
{
  //#ifdef USE_MY_OPENCV
  //BIASDOUT(D_IMCONV_IPL, "using own CreateIplImage");
  //  cerr << "using own CreateIplImage\n";
  //dest = CreateIplImage(source.GetWidth(), source.GetHeight(),
  //                      sizeof(StorageType) * 8 , source.GetChannelCount());
  //#else
  //BIASDOUT(D_IMCONV_IPL, "using official CreateIplImage");
  //cerr << "using official CreateIplImage\n";
  if (source.GetColorModel() == ImageBase::CM_BGRA)
    {
      dest = cvCreateImage(cvSize(source.GetWidth(), source.GetHeight()),
                           sizeof(StorageType) * 8, source.GetChannelCount() - 1);
    }
  else
    {
      dest = cvCreateImage(cvSize(source.GetWidth(), source.GetHeight()),
                           sizeof(StorageType) * 8, source.GetChannelCount());
    }
  //#endif

  StorageType* newImageData = (StorageType*) dest->imageDataOrigin;

  //
  // IplImages in OpenCV are always:
  //   - GREY
  //   - RGB  (with byte Order BGR due to Windows DIB format)
  //   - RGBA (with byte Order BGRA)
  //
  // As an limitation of opencv compared to IPL, color images are ALWAYS
  //   interleaved!

  //cerr << "source.GetColorModel() :"<<source.GetColorModel()<<endl;
  switch (source.GetColorModel()) {

  case ImageBase::CM_Grey:
    //cerr <<"grey\n";
    memcpy(newImageData, source.GetImageData(), source.GetSizeByte() );
    break;

  case ImageBase::CM_BGR :
    //cerr <<"BGR\n";
    if (source.IsPlanar()) {
      //cerr <<"Planar\n";
      // planar BGR image
      for(int channel = 0; channel < 3; channel++) {
        register const StorageType* src  = source.GetImageData() +
          channel*source.GetPixelCount();
        register StorageType* act = newImageData + channel;
        register const StorageType* stop = src + source.GetPixelCount();
        for(;src < stop; src++) {
          *act = *src;
          act += 3;
        }
      }
    } else {
      //cerr <<"Interleaved\n";
      // interleaved BGR image - the same format as IplImage itself
      memcpy((void*)newImageData, (void*)source.GetImageData(), source.GetSizeByte());
    };
    break;

  case ImageBase::CM_RGB :
    //cerr <<"RGB\n";
    if (source.IsPlanar()) {
      //cerr <<"Planar\n";
      // planar RGB image
      for(int channel = 0; channel < 3; channel++) {
        register const StorageType* src  = source.GetImageData() + (2 - channel) *
          source.GetPixelCount(); // RGB instead of BGR order!
        register StorageType* act = newImageData + channel;
        register const StorageType* stop = src + source.GetPixelCount();
        for(;src < stop; src++) {
          *act = *src;
          act += 3;
        }
      }
    } else {
      //cerr <<"Interleaved\n";
      // interleaved RGB image
      register const StorageType* src  = source.GetImageData();
      register const StorageType* stop = src + 3 * source.GetPixelCount();
      register StorageType* act = newImageData;
      while (src < stop) {
        // swap RGB because OpenCV uses BGR as native channel order
        // only the color conversion routines need this, I fixed highgui to handle channelSeq correctly (jw)
        act[0]=src[2];
        act[1]=src[1];
        act[2]=src[0];
        act+=3;
        src+=3;
      }
    }
    break;
  case ImageBase::CM_BGRA:
    { // drop alpha channel
      const StorageType *b = source.GetImageData();
      StorageType *d = newImageData;
      const int size = source.GetPixelCount();
      if (source.IsPlanar())
        {
          const StorageType *g = b + size, *r = g + size;
          for (int i = 0; i < size; i++)
            {
              *d = *b;
              d++;
              b++;
              *d = *g;
              d++;
              g++;
              *d = *r;
              d++;
              r++;
            }
        }
      else
        {
          for (int i = 0; i < size; i++)
            {
              *d = *b;
              d++;
              b++;
              *d = *b;
              d++;
              b++;
              *d = *b;
              d++;
              b += 2;
            }
        }
    }
    break;
  default:
    BIASERR("Only Grey, BGR and RGB images can be converted to IplImages yet."
            <<" Aborting!");
    return -1;
  }

  return 0;
}

template <class StorageType>
int ImageConvert::Ipl2BIAS(IplImage* source, BIAS::Image<StorageType>& dest)
{
  // Does the color depth match?
  if (8*sizeof(StorageType) != source->depth) {
    BIASERR("Cannot convert an IplImage into an Image of different color "
      <<"depth!");
    return -1;
  }

  if (source->width <=0 || source->height <= 0) {
    BIASERR("IplImage to be converted to Image has no size!");
    return -1;
  }
  if ( !dest.IsEmpty() )
    dest.Release();
  // What kind of image do we have?
  switch (source->nChannels) {
  case 1: // gray image
    // Release the old image data and allocate new memory
    dest.Init(source->width, source->height, 1);
    memcpy(dest.GetImageData(), source->imageDataOrigin, dest.GetSizeByte());
    break;
  case 3:
    if (strcmp(source->colorModel, "RGB")==0 &&
      strcmp(source->channelSeq, "BGR")==0 &&
      source->dataOrder==IPL_DATA_ORDER_PIXEL &&
      (source->depth>>3)==sizeof(StorageType) ){
        dest.Init(source->width, source->height, 3);
        register unsigned char* src = (unsigned char*) source->imageDataOrigin;
        register unsigned char* stop = src + 3 * source->width * source->height;
        register StorageType* dst = dest.GetImageData();
        while (src < stop) {
          dst[0]=src[2];
          dst[1]=src[1];
          dst[2]=src[0];
          dst+=3; src+=3;
        }
      } else {
        BIASERR("Ipl2BIAS not implemented for this color model or inavlid storage type");
        return -1;
      }
      break;
  default:
    BIASERR("Only gray IplImages can be converted to Images yet!");
    return -1;
  }
  return 0;
}




int ImageConvert::DeinterleaveHorizontal(const BIAS::ImageBase & raw,
                                         BIAS::ImageBase & out)
{
  if (raw.GetColorModel()!= vidap_DEFAULT_PACKED_COLORMODEL)
    return -1;
  if (raw.GetChannelCount()!= 3)
    return -2;
  if ( ImageBase::GetSizeByte(raw.GetStorageType()) != 1 ){
    BIASERR("storagetypes size must be 1 for now due to simple pointer arithmetics.\n"
      "Other pointer sizes need to be implemented.");
    return -3;
  }
  if (raw.GetStorageType()!= out.GetStorageType()){
    BIASERR("storagetypes must match");
    return -4;
  }

  BIASASSERT(raw.GetColorModel()== vidap_DEFAULT_PACKED_COLORMODEL );
  BIASASSERT(raw.GetChannelCount()== 3);
  BIASASSERT(ImageBase::GetSizeByte(raw.GetStorageType()) == 1);

  // check if we need re-allocation
  if (!out.IsEmpty())
    out.Release();

  // triple width for appeding 3 camera images horizontally
  out.Init( 3* raw.GetWidth(), raw.GetHeight(), 1 );
  out.SetColorModel( vidap_DEFAULT_BAYER_COLORMODEL );

  // abcABC --> aAbBcC
  unsigned char *s =NULL, *d0=NULL, *d1=NULL, *d2=NULL;

  for (unsigned int y=0; y<raw.GetHeight(); y++){
    s  = (unsigned char*)raw.GetImageDataArray()[y];
    d0 = (unsigned char*)out.GetImageDataArray()[y];
    d1 = (unsigned char*)out.GetImageDataArray()[y] + raw.GetWidth();
    d2 = (unsigned char*)out.GetImageDataArray()[y] + 2*raw.GetWidth();
    for (unsigned int x=0; x<raw.GetWidth(); x++){
      *d0 = *s; s++; d0++;
      *d1 = *s; s++; d1++;
      *d2 = *s; s++; d2++;
    }
  }

  return 0;
}
/*
template <class StorageType>
int BIAS::ImageConvert::BayerToRGBPGR_(const Image<StorageType>& source,
                                     Image<StorageType>&  dest )
{
  StorageType *sink = dest.GetImageData() ;
  const StorageType *src = source.GetImageData() ;

  unsigned int width= source.GetWidth();
  unsigned int srcheight=source.GetHeight() -1;
  unsigned int xoffset = 0, yoffset = 1;

  // first pixel in first line
  StorageType G = *src;
  StorageType R = *(src+width);
  StorageType B = *(src+1);

  sink[0]=0; sink[1]=0; sink[2]=0;
  sink+=3; src++;
  // first line
  if (0 == yoffset) {
    for (unsigned int x=1; x<width-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src+-1)+ *(src+1) + *(src+ width) ) /3;
        B = ( *(src+(width -1)) + *(src+(width +1))) /2;
      }
      else { // staying on Green
        R =  ( *(src+-1) + *(src+1) ) /2;
        G = *src;
        B = (*(src+width) ) ;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<width-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src+width) ) ;
        G = *src;
        B = ( *(src+(-1)) + *(src+1) ) /2;
      }
      else { // staying on Blue
        R = (  *(src+(width -1)) + *(src+(width +1))) /2;
        G = ( *(src-1) + *(src+1) + *(src+width) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  // last pixel in first line
  if (0 == yoffset) {
    if ((width-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src+(-1))+  *(src+(+ width)) ) /2;
      B =  *(src+(width -1))  ;
    }
    else { // staying on Green
      R =  *(src+(-1));
      G = *src;
      B = (*(src+(+ width)) ) ;
    }
  }
  else {
    if ((width-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src+(+ width)) ) ;
      G = *src;
      B =  *(src+(-1)) ;
    }
    else { // staying on Blue
      R =  *(src+(width -1));
      G = ( *(src+(-1)) + *(src+(+ width)) ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;


  //loop
  for (unsigned int y=1; y<srcheight; y++) {
    // first pixel in line
    if (y%2 == yoffset)
      if (0%2 == xoffset) {  // staying on Red
        R = *src;
        G = (*(src - width )  + *(src+1) + *(src+width) ) /3;
        B = ( *(src - width -1)  + *(src+(width +1))) /2;
      } else { // staying on Green
        R =  *(src+1);
        G = *src;
        B = ( *(src - width ) + *(src+  width ) ) /2;
      }
    else
      if (0 == xoffset) {  // staying on Green
        R =  ( *(src - width ) + *(src+ width ) ) /2;
        G = *src;
        B = *(src+1);
      }
      else { // staying on Blue
        R = ( *(src - width +1)  + *(src+(width +1))) /2;
        G = (*(src - width )  + *(src+(+1)) + *(src+(+ width)) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;

      // main part of line
      for (unsigned int x=1; x<width-1; x++) {
        if (y%2 == yoffset)
          if (x%2 == xoffset) {  // staying on Red
            R = *src;
            G = (*(src - width ) + *(src -1 )
              + *(src+(+1)) + *(src+ (+ width)) ) /4;
            B = ( *(src - width +1 ) + *(src - width -1 )
              + *(src+(width -1)) + *(src+(width +1))) /4;
          } else { // staying on Green
            R =  ( *(src -1 ) + *(src+ 1 ) ) /2;
            G = *src;
            B = ( *(src - width ) + *(src+  width ) ) /2;
          }
        else

          if (x%2 == xoffset) {  // staying on Green
            R =  ( *(src - width ) + *(src+  width ) ) /2;
            G = *src;
            B = ( *(src+(-1)) + *(src+(+1)) ) /2;
          }
          else { // staying on Blue
            R = ( *(src - width +1 ) + *(src - width -1 )
              + *(src+(width -1)) + *(src+(width +1))) /4;
            G = (*(src - width ) + *(src -1 )
              + *(src+(+1)) + *(src+(+ width)) ) /4;
            B = *src;
          }
          *sink++ =R;
          *sink++ =G;
          *sink++ =B;
          src++;
      }

      // last pixel in line
      if (y%2 == yoffset)
        if ((width-1)%2 == xoffset) {  // staying on Red
          R = *src;
          G = (*(src - width ) + *(src -1 ) + *(src+  width ) ) /3;
          B = ( *(src - width +1 ) + *(src+(width -1))) /2;
        } else { // staying on Green
          R =  *(src -1 ) ;
          G = *src;
          B = ( *(src - width ) + *(src+  width ) ) /2;
        }
      else
        if ((width-1)%2 == xoffset) {  // staying on Green
          R =  ( *(src - width ) + *(src+  width ) ) /2;
          G = *src;
          B = *(src -1 );
        }
        else {


          // staying on Blue
          R = ( *(src - width +1 ) + *(src+(width -1))) /2;
          G = (*(src - width ) + *(src -1 )+ *(src+(  width)) ) /3;
          B = *src;
        }
        *sink++ =R;
        *sink++ =G;
        *sink++ =B;
        src++;


  }

  // first pixel in last line
  if ((srcheight)%2 == yoffset) {
    if (0 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src - width )+  *(src +1 ) ) /2;
      B =  *(src - width -1 )  ;
    }
    else { // staying on Green
      R =  *(src +1 );
      G = *src;
      B = (*(src - width ) ) ;
    }
  }
  else {
    if (0 == xoffset) {  // staying on Green
      R =  ( *(src - width ) ) ;
      G = *src;
      B =  *(src+ 1 ) ;
    }
    else { // staying on Blue
      R =  *(src - width -1 );
      G = ( *(src - width ) + *(src +1 ) ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;
  // last line

  if ((srcheight)%2 == yoffset) {
    for (unsigned int x=1; x<width-1; x++){
      if (x%2 == xoffset) {  // staying on Red
        R = *src;
        G = ( *(src -1 )+ *(src - width ) + *(src +1 ) ) /3;
        B = ( *(src - width +1 ) + *(src - width -1 )) /2;
      }
      else { // staying on Green
        R =  ( *(src -1 ) + *(src+ 1 ) ) /2;
        G = *src;
        B = (*(src - width ) ) ;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  else {
    for (unsigned int x=1; x<width-1; x++){
      if (x%2 == xoffset) {  // staying on Green
        R =  ( *(src - width ) ) ;
        G = *src;
        B = ( *(src -1 ) + *(src +1 ) ) /2;
      }
      else { // staying on Blue
        R = (  *(src - width +1 ) + *(src - width -1 )) /2;
        G = ( *(src - width ) + *(src -1 ) + *(src +1 ) ) /3;
        B = *src;
      }
      *sink++ =R;
      *sink++ =G;
      *sink++ =B;
      src++;
    }
  }
  // last pixel in last line
  if ((srcheight-1)%2 == yoffset) {
    if ((width-1)%2 == xoffset) {  // staying on Red
      R = *src;
      G = ( *(src -1 )+  *(src - width ) ) /2;
      B =  *(src - width +1 )  ;
    }
    else { // staying on Green
      R =  *(src -1 );
      G = *src;
      B = (*(src - width ) ) ;
    }
  }
  else {
    if ((width-1)%2 == xoffset) {  // staying on Green
      R =  ( *(src - width ) ) ;
      G = *src;
      B =  *(src+(-1)) ;
    }
    else { // staying on Blue
      R =  *(src - width +1 );
      G = ( *(src -1 ) + *(src - width)  ) /2;
      B = *src;
    }
  }
  *sink++ =R;
  *sink++ =G;
  *sink++ =B;
  src++;

  return 0;
}
*/
template<class StorageType>
int ImageConvert::BayerToRGBSimple(const StorageType* source, StorageType* dest,
                                int sx, int sy, ImageBase::EColorModel tile){
return BayerToRGBSimple_(  source,  dest, sx,   sy,  tile);

}

template<class StorageType>
int ImageConvert::BayerToRGBSimple_(const StorageType* source, StorageType* dest,
                                int sx, int sy, ImageBase::EColorModel tile){

    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == ImageBase::CM_Bayer_BGGR
        || tile == ImageBase::CM_Bayer_GBRG ? -1 : 1;
    int start_with_green = tile == ImageBase::CM_Bayer_GBRG
        || tile == ImageBase::CM_Bayer_GRBG;
    int i, imax, iinc;

    // add black border
    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
        dest[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
        dest[i++] = 0;
        dest[i++] = 0;
        dest[i++] = 0;
    }

    dest += 1;
    width -= 1;
    height -= 1;

    for (; height--; source += bayerStep, dest += rgbStep) {
        const StorageType *bayerEnd = source + width;

        if (start_with_green) {
            dest[-blue] = source[1];
            dest[0] = (source[0] + source[bayerStep + 1] + 1) >> 1;
            dest[blue] = source[bayerStep];
            source++;
            dest += 3;
        }

        if (blue > 0) {
            for (; source <= bayerEnd - 2; source += 2, dest += 6) {
                dest[-1] = source[0];
                dest[0] = (source[1] + source[bayerStep] + 1) >> 1;
                dest[1] = source[bayerStep + 1];

                dest[2] = source[2];
                dest[3] = (source[1] + source[bayerStep + 2] + 1) >> 1;
                dest[4] = source[bayerStep + 1];
            }
        } else {
            for (; source <= bayerEnd - 2; source += 2, dest += 6) {
                dest[1] = source[0];
                dest[0] = (source[1] + source[bayerStep] + 1) >> 1;
                dest[-1] = source[bayerStep + 1];

                dest[4] = source[2];
                dest[3] = (source[1] + source[bayerStep + 2] + 1) >> 1;
                dest[2] = source[bayerStep + 1];
            }
        }

        if (source < bayerEnd) {
            dest[-blue] = source[0];
            dest[0] = (source[1] + source[bayerStep] + 1) >> 1;
            dest[blue] = source[bayerStep + 1];
            source++;
            dest += 3;
        }

        source -= width;
        dest -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }

  return 0;
}

template<class StorageType>
int ImageConvert::BayerToRGBBilinear_(const StorageType* bayer, StorageType* rgb,
                                int sx, int sy, ImageBase::EColorModel tile){

    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == ImageBase::CM_Bayer_BGGR
        || tile == ImageBase::CM_Bayer_GBRG ? -1 : 1;
    int start_with_green = tile == ImageBase::CM_Bayer_GBRG
        || tile == ImageBase::CM_Bayer_GRBG;


ClearBorders_(rgb, sx, sy, 1);
    rgb += rgbStep + 3 + 1;
    height -= 2;
    width -= 2;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const StorageType *bayerEnd = bayer + width;

        if (start_with_green) {
            /* OpenCV has a bug in the next line, which was
               t0 = (bayer[0] + bayer[bayerStep * 2] + 1) >> 1; */
            t0 = (bayer[1] + bayer[bayerStep * 2 + 1] + 1) >> 1;
            t1 = (bayer[bayerStep] + bayer[bayerStep + 2] + 1) >> 1;
            rgb[-blue] = (StorageType) t0;
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = (StorageType) t1;
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
                      bayer[bayerStep * 2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
                      2) >> 2;
                rgb[-1] = (StorageType) t0;
                rgb[0] = (StorageType) t1;
                rgb[1] = bayer[bayerStep + 1];

                t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[2] = (StorageType) t0;
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = (StorageType) t1;
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
                      bayer[bayerStep * 2 + 2] + 2) >> 2;
                t1 = (bayer[1] + bayer[bayerStep] +
                      bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
                      2) >> 2;
                rgb[1] = (StorageType) t0;
                rgb[0] = (StorageType) t1;
                rgb[-1] = bayer[bayerStep + 1];

                t0 = (bayer[2] + bayer[bayerStep * 2 + 2] + 1) >> 1;
                t1 = (bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                      1) >> 1;
                rgb[4] = (StorageType) t0;
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = (StorageType) t1;
            }
        }

        if (bayer < bayerEnd) {
            t0 = (bayer[0] + bayer[2] + bayer[bayerStep * 2] +
                  bayer[bayerStep * 2 + 2] + 2) >> 2;
            t1 = (bayer[1] + bayer[bayerStep] +
                  bayer[bayerStep + 2] + bayer[bayerStep * 2 + 1] +
                  2) >> 2;
            rgb[-blue] = (StorageType) t0;
            rgb[0] = (StorageType) t1;
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
    return 0;
}

template<class StorageType>
int ImageConvert::BayerToRGBNearestNeighbour_(const StorageType* bayer, StorageType* rgb,
                                int sx, int sy, ImageBase::EColorModel tile){

    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == ImageBase::CM_Bayer_BGGR
        || tile == ImageBase::CM_Bayer_GBRG ? -1 : 1;
    int start_with_green = tile == ImageBase::CM_Bayer_GBRG
        || tile == ImageBase::CM_Bayer_GRBG;
    int i, imax, iinc;

    imax = sx * sy * 3;
    for (i = sx * (sy - 1) * 3; i < imax; i++) {
        rgb[i] = 0;
    }
    iinc = (sx - 1) * 3;
    for (i = (sx - 1) * 3; i < imax; i += iinc) {
        rgb[i++] = 0;
        rgb[i++] = 0;
        rgb[i++] = 0;
    }

    rgb += 1;
    width -= 1;
    height -= 1;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
      //int t0, t1;
        const StorageType *bayerEnd = bayer + width;

        if (start_with_green) {
            rgb[-blue] = bayer[1];
            rgb[0] = bayer[bayerStep + 1];
            rgb[blue] = bayer[bayerStep];
            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[-1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[1] = bayer[bayerStep + 1];

                rgb[2] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[4] = bayer[bayerStep + 1];
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                rgb[1] = bayer[0];
                rgb[0] = bayer[1];
                rgb[-1] = bayer[bayerStep + 1];

                rgb[4] = bayer[2];
                rgb[3] = bayer[bayerStep + 2];
                rgb[2] = bayer[bayerStep + 1];
            }
        }

        if (bayer < bayerEnd) {
            rgb[-blue] = bayer[0];
            rgb[0] = bayer[1];
            rgb[blue] = bayer[bayerStep + 1];
            bayer++;
            rgb += 3;
        }

        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }
    return 0;
}

template<class StorageType>
void ImageConvert::ClearBorders_(StorageType *rgb, int sx, int sy, int w)
{
    int i, j;
    // black edges are added with a width w:
    i = 3 * sx * w - 1;
    j = 3 * sx * sy - 1;
    while (i >= 0) {
        rgb[i--] = 0;
        rgb[j--] = 0;
    }

    int low = sx * (w - 1) * 3 - 1 + w * 3;
    i = low + sx * (sy - w * 2 + 1) * 3;
    while (i > low) {
        j = 6 * w;
        while (j > 0) {
            rgb[i--] = 0;
            j--;
        }
        i -= (sx - 2 * w) * 3;
    }
}

int ImageConvert::BayerToRGBHQLinear_(const unsigned char* bayer, unsigned char* rgb, int sx, int sy, ImageBase::EColorModel tile){

    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == ImageBase::CM_Bayer_BGGR
        || tile == ImageBase::CM_Bayer_GBRG ? -1 : 1;
    int start_with_green = tile == ImageBase::CM_Bayer_GBRG
        || tile == ImageBase::CM_Bayer_GRBG;


    ClearBorders_<unsigned char>(rgb, sx, sy, 2);
    rgb += 2 * rgbStep + 6 + 1;
    height -= 4;
    width -= 4;

    /* We begin with a (+1 line,+1 column) offset with respect to bilinear decoding, so start_with_green is the same, but blue is opposite */
    blue = -blue;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        int t0, t1;
        const unsigned char *bayerEnd = bayer + width;
        const int bayerStep2 = bayerStep * 2;
        const int bayerStep3 = bayerStep * 3;
        const int bayerStep4 = bayerStep * 4;

        if (start_with_green) {
            /* at green pixel */
            rgb[0] = bayer[bayerStep2 + 2];
            t0 = rgb[0] * 5
                + ((bayer[bayerStep + 2] + bayer[bayerStep3 + 2]) << 2)
                - bayer[2]
                - bayer[bayerStep + 1]
                - bayer[bayerStep + 3]
                - bayer[bayerStep3 + 1]
                - bayer[bayerStep3 + 3]
                - bayer[bayerStep4 + 2]
                + ((bayer[bayerStep2] + bayer[bayerStep2 + 4] + 1) >> 1);
            t1 = rgb[0] * 5 +
                ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 3]) << 2)
                - bayer[bayerStep2]
                - bayer[bayerStep + 1]
                - bayer[bayerStep + 3]
                - bayer[bayerStep3 + 1]
                - bayer[bayerStep3 + 3]
                - bayer[bayerStep2 + 4]
                + ((bayer[2] + bayer[bayerStep4 + 2] + 1) >> 1);
            t0 = (t0 + 4) >> 3;
            CLIP<unsigned char>((unsigned char&)t0, rgb[-blue]);
            t1 = (t1 + 4) >> 3;
            CLIP<unsigned char>((unsigned char&)t1, rgb[blue]);

            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* B at B */
                rgb[1] = bayer[bayerStep2 + 2];
                /* R at B */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                    -
                    (((bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                     2]) * 3 + 1) >> 1)
                    + rgb[1] * 6;
                /* G at B */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2]) << 1)
                    - (bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                    + (rgb[1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t0, rgb[-1]);
                t1 = (t1 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t1, rgb[0]);
                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                    - bayer[3]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep4 + 3]
                    +
                    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                      1) >> 1);
                t1 = rgb[3] * 5 +
                    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                    - bayer[bayerStep2 + 1]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep2 + 5]
                    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t0, rgb[2]);
                t1 = (t1 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t1, rgb[4]);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* R at R */
                rgb[-1] = bayer[bayerStep2 + 2];
                /* B at R */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                    -
                    (((bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                     2]) * 3 + 1) >> 1)
                    + rgb[-1] * 6;
                /* G at R */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep * 3 +
                                                     2]) << 1)
                    - (bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                    + (rgb[-1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t0, rgb[1]);
                t1 = (t1 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t1, rgb[0]);

                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                    - bayer[3]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep4 + 3]
                    +
                    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                      1) >> 1);
                t1 = rgb[3] * 5 +
                    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                    - bayer[bayerStep2 + 1]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep2 + 5]
                    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t0, rgb[4]);
                t1 = (t1 + 4) >> 3;
                CLIP<unsigned char>((unsigned char&)t1, rgb[2]);
            }
        }

        if (bayer < bayerEnd) {
            /* B at B */
            rgb[blue] = bayer[bayerStep2 + 2];
            /* R at B */
            t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                   bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                -
                (((bayer[2] + bayer[bayerStep2] +
                   bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                 2]) * 3 + 1) >> 1)
                + rgb[blue] * 6;
            /* G at B */
            t1 = (((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                    bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2])) << 1)
                - (bayer[2] + bayer[bayerStep2] +
                   bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                + (rgb[blue] << 2);
            t0 = (t0 + 4) >> 3;
            CLIP<unsigned char>((unsigned char&)t0, rgb[-blue]);
            t1 = (t1 + 4) >> 3;
            CLIP<unsigned char>((unsigned char&)t1, rgb[0]);

            bayer++;
            rgb += 3;
        }


        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }


  return 0;
}

int ImageConvert::BayerToRGBHQLinear_(const unsigned short* bayer, unsigned short* rgb, int sx, int sy, ImageBase::EColorModel tile, int bit){

    unsigned short bits = (unsigned short) bit;
    const int bayerStep = sx;
    const int rgbStep = 3 * sx;
    int width = sx;
    int height = sy;
    int blue = tile == ImageBase::CM_Bayer_BGGR
        || tile == ImageBase::CM_Bayer_GBRG ? -1 : 1;
    int start_with_green = tile == ImageBase::CM_Bayer_GBRG
        || tile == ImageBase::CM_Bayer_GRBG;


    ClearBorders_<unsigned short>(rgb, sx, sy, 2);
    rgb += 2 * rgbStep + 6 + 1;
    height -= 4;
    width -= 4;

    /* We begin with a (+1 line,+1 column) offset with respect to bilinear decoding, so start_with_green is the same, but blue is opposite */
    blue = -blue;

    for (; height--; bayer += bayerStep, rgb += rgbStep) {
        unsigned short t0, t1;
        const unsigned short *bayerEnd = bayer + width;
        const int bayerStep2 = bayerStep * 2;
        const int bayerStep3 = bayerStep * 3;
        const int bayerStep4 = bayerStep * 4;

        if (start_with_green) {
            /* at green pixel */
            rgb[0] = bayer[bayerStep2 + 2];
            t0 = rgb[0] * 5
                + ((bayer[bayerStep + 2] + bayer[bayerStep3 + 2]) << 2)
                - bayer[2]
                - bayer[bayerStep + 1]
                - bayer[bayerStep + 3]
                - bayer[bayerStep3 + 1]
                - bayer[bayerStep3 + 3]
                - bayer[bayerStep4 + 2]
                + ((bayer[bayerStep2] + bayer[bayerStep2 + 4] + 1) >> 1);
            t1 = rgb[0] * 5 +
                ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 3]) << 2)
                - bayer[bayerStep2]
                - bayer[bayerStep + 1]
                - bayer[bayerStep + 3]
                - bayer[bayerStep3 + 1]
                - bayer[bayerStep3 + 3]
                - bayer[bayerStep2 + 4]
                + ((bayer[2] + bayer[bayerStep4 + 2] + 1) >> 1);
            t0 = (t0 + 4) >> 3;
            CLIP16<unsigned short>((unsigned short&)t0,rgb[-blue],bits);
            t1 = (t1 + 4) >> 3;
            CLIP16<unsigned short>((unsigned short&)t1,rgb[blue],bits);

            bayer++;
            rgb += 3;
        }

        if (blue > 0) {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* B at B */
                rgb[1] = bayer[bayerStep2 + 2];
                /* R at B */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                    -
                    (((bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                     2]) * 3 + 1) >> 1)
                    + rgb[1] * 6;
                /* G at B */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2]) << 1)
                    - (bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                    + (rgb[1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t0, rgb[-1],bits);
                t1 = (t1 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t1, rgb[0],bits);
                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                    - bayer[3]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep4 + 3]
                    +
                    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                      1) >> 1);
                t1 = rgb[3] * 5 +
                    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                    - bayer[bayerStep2 + 1]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep2 + 5]
                    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t0,rgb[2],bits);

                t1 = (t1 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t1,rgb[4],bits);
            }
        } else {
            for (; bayer <= bayerEnd - 2; bayer += 2, rgb += 6) {
                /* R at R */
                rgb[-1] = bayer[bayerStep2 + 2];
                /* B at R */
                t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                       bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                    -
                    (((bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                     2]) * 3 + 1) >> 1)
                    + rgb[-1] * 6;
                /* G at R */
                t1 = ((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                       bayer[bayerStep2 + 3] + bayer[bayerStep * 3 +
                                                     2]) << 1)
                    - (bayer[2] + bayer[bayerStep2] +
                       bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                    + (rgb[-1] << 2);
                t0 = (t0 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t0,rgb[1],bits);
                t1 = (t1 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t1,rgb[0],bits);

                /* at green pixel */
                rgb[3] = bayer[bayerStep2 + 3];
                t0 = rgb[3] * 5
                    + ((bayer[bayerStep + 3] + bayer[bayerStep3 + 3]) << 2)
                    - bayer[3]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep4 + 3]
                    +
                    ((bayer[bayerStep2 + 1] + bayer[bayerStep2 + 5] +
                      1) >> 1);
                t1 = rgb[3] * 5 +
                    ((bayer[bayerStep2 + 2] + bayer[bayerStep2 + 4]) << 2)
                    - bayer[bayerStep2 + 1]
                    - bayer[bayerStep + 2]
                    - bayer[bayerStep + 4]
                    - bayer[bayerStep3 + 2]
                    - bayer[bayerStep3 + 4]
                    - bayer[bayerStep2 + 5]
                    + ((bayer[3] + bayer[bayerStep4 + 3] + 1) >> 1);
                t0 = (t0 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t0,rgb[4],bits);
                t1 = (t1 + 4) >> 3;
                CLIP16<unsigned short>((unsigned short&)t1,rgb[2],bits);
            }
        }

        if (bayer < bayerEnd) {
            /* B at B */
            rgb[blue] = bayer[bayerStep2 + 2];
            /* R at B */
            t0 = ((bayer[bayerStep + 1] + bayer[bayerStep + 3] +
                   bayer[bayerStep3 + 1] + bayer[bayerStep3 + 3]) << 1)
                -
                (((bayer[2] + bayer[bayerStep2] +
                   bayer[bayerStep2 + 4] + bayer[bayerStep4 +
                                                 2]) * 3 + 1) >> 1)
                + rgb[blue] * 6;
            /* G at B */
            t1 = (((bayer[bayerStep + 2] + bayer[bayerStep2 + 1] +
                    bayer[bayerStep2 + 3] + bayer[bayerStep3 + 2])) << 1)
                - (bayer[2] + bayer[bayerStep2] +
                   bayer[bayerStep2 + 4] + bayer[bayerStep4 + 2])
                + (rgb[blue] << 2);
            t0 = (t0 + 4) >> 3;
            CLIP16<unsigned short>((unsigned short&)t0,rgb[-blue],bits);
            t1 = (t1 + 4) >> 3;
            CLIP16<unsigned short>((unsigned short&)t1,rgb[0],bits);

            bayer++;
            rgb += 3;
        }


        bayer -= width;
        rgb -= width * 3;

        blue = -blue;
        start_with_green = !start_with_green;
    }


  return 0;
}

template<class StorageType>
int ImageConvert::BayerToRGBAHD_(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile, int bits){

  return 0;
}

template <class StorageType>
void ImageConvert::CLIP(StorageType& in, StorageType& out){
   in = in < 0 ? 0 : in;
   in = in > 255 ? 255 : in;
   out=in;
}

template <class StorageType>
void ImageConvert::CLIP16(StorageType& in, StorageType& out, StorageType bits){
   in = in < 0 ? 0 : in;
   in = in > ((1<<bits)-1) ? ((1<<bits)-1) : in;
   out=in;
}

//
// solve explicit instantiation of this class
// must be at the end of the file JW
#include "ImageConvertInstantiation.cpp"

