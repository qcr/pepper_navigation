#include "bias_config.h"

///
/// solve explicit instantiation
/// this file is included by ImageConvert.cpp
/// @author Jan Woetzel
///

#define INSTANCE_ImageConvert(type)\
template BIASImageBase_EXPORT int ImageConvert::IP_ToGrey(BIAS::Image<type> &img);\
template BIASImageBase_EXPORT int ImageConvert::ToGrey(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::GetChannel(BIAS::Image<type>& image, int channel, bool strip);\
template BIASImageBase_EXPORT int ImageConvert::GetChannel(const BIAS::Image<type>& source, BIAS::Image<type>& dest, unsigned int channel);\
template BIASImageBase_EXPORT int ImageConvert::ToRGB(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToRGBA(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToHSV(const BIAS::Image<type>& source,BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::HSVToRGB(const type h,const type s,const type v, type& r,type& g, type& b);\
template BIASImageBase_EXPORT int ImageConvert::ToHSL(const BIAS::Image<type>& source,BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::TohsL(const BIAS::Image<type>& source,BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToInterleaved(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToInterleavedRGB(const BIAS::Image<type>& red, const BIAS::Image<type>& green, const BIAS::Image<type>& blue, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToPlanar(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::FromInterleaved(const BIAS::Image<type>& source, BIAS::Image<type>& Image1, BIAS::Image<type>& Image2, BIAS::Image<type>& Image3);\
template BIASImageBase_EXPORT int ImageConvert::IP_RGBToGrey_(BIAS::Image<type> &img);\
template BIASImageBase_EXPORT int ImageConvert::IP_YUV422ToGrey_(BIAS::Image<type> &img);\
template BIASImageBase_EXPORT int ImageConvert::BGRToRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::YUV422ToRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::GreyToRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::RGBToGrey_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::HSLToGrey_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::YUV422ToGrey_(const BIAS::Image<type> &source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToPlanarYUYV422_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);    \
template BIASImageBase_EXPORT int ImageConvert::ToPlanarRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToInterleavedYUYV422_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::ToInterleavedRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::FromInterleavedYUYV422_(const BIAS::Image<type>& source, BIAS::Image<type>& Y, BIAS::Image<type>& U, BIAS::Image<type>& V);\
template BIASImageBase_EXPORT int ImageConvert::FromInterleavedRGB_(const BIAS::Image<type>& source, BIAS::Image<type>& R,  BIAS::Image<type>& G, BIAS::Image<type>& B);\
template BIASImageBase_EXPORT int ImageConvert::BayerToGrey_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::BayerToRGB_ (const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBSlow_(const BIAS::Image<type>& source, BIAS::Image<type>& dest);\
template BIASImageBase_EXPORT int ImageConvert::XYZToRGB(const BIAS::Image<float>& source, BIAS::Image<type>& dest, float scale);\
template BIASImageBase_EXPORT int ImageConvert::LUVToRGB(const BIAS::Image<float>& source, BIAS::Image<type>& dest, float scale);

//#ifdef BIAS_HAVE_OPENCV
#define INSTANCE_ImageConvert_OPENCV(cvt)\
template BIASImageBase_EXPORT int ImageConvert::BIAS2ipl(const BIAS::Image<cvt>& source, IplImage* &dest);\
template BIASImageBase_EXPORT int ImageConvert::Ipl2BIAS(IplImage* source, BIAS::Image<cvt>& dest);
//#else
//#define INSTANCE_ImageConvert_OPENCV(cvt)
//#endif

#ifdef BIAS_HAVE_DV
#define INSTANCE_ImageConvert_DV(dvtype)\
template BIASImageBase_EXPORT int ImageConvert::DVToRGB_(const BIAS::Image<dvtype>& source, BIAS::Image<dvtype>& dest,unsigned char quality);
#else
#define INSTANCE_ImageConvert_DV(dvtype)
#endif

// all instances for one type:
#define INSTI(atype)   INSTANCE_ImageConvert(atype) INSTANCE_ImageConvert_OPENCV(atype) INSTANCE_ImageConvert_DV(atype)

INSTI(unsigned char)
template BIASImageBase_EXPORT int ImageConvert::BayerToRGB (const BIAS::Image<unsigned char>& source, BIAS::Image<unsigned char>& dest, BayerDemosaicMethod method, bool flip);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBSimple_(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBSimple(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBBilinear_(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBNearestNeighbour_(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBAHD_(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile, int bits);
template BIASImageBase_EXPORT void ImageConvert::ClearBorders_<unsigned char>(unsigned char *rgb, int sx, int sy, int w);
template BIASImageBase_EXPORT void ImageConvert::CLIP(unsigned char& in, unsigned char& out);
template BIASImageBase_EXPORT void ImageConvert::CLIP16(unsigned char& in, unsigned char& out, unsigned char bits);

INSTI(float)

#ifdef BUILD_IMAGE_INT
INSTI(int)
#endif
#ifdef BUILD_IMAGE_CHAR
INSTI(char)
#endif
#ifdef BUILD_IMAGE_SHORT
INSTI(short)
#endif
#if defined(BUILD_IMAGE_USHORT)
INSTI(unsigned short)
template BIASImageBase_EXPORT int ImageConvert::BayerToRGB (const BIAS::Image<unsigned short>& source, BIAS::Image<unsigned short>& dest, BayerDemosaicMethod method, bool flip);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBSimple_(const unsigned short* source, unsigned short* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBBilinear_(const unsigned short* source, unsigned short* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBNearestNeighbour_(const unsigned short* source, unsigned short* dest, int sx, int sy, ImageBase::EColorModel tile);
template BIASImageBase_EXPORT int ImageConvert::BayerToRGBAHD_(const unsigned short* source, unsigned short* dest, int sx, int sy, ImageBase::EColorModel tile, int bits);
template BIASImageBase_EXPORT void ImageConvert::ClearBorders_(unsigned short *rgb, int sx, int sy, int w);
template BIASImageBase_EXPORT void ImageConvert::CLIP(unsigned short& in, unsigned short& out);
template BIASImageBase_EXPORT void ImageConvert::CLIP16(unsigned short& in, unsigned short& out, unsigned short bits);
#endif
#ifdef BUILD_IMAGE_DOUBLE
INSTI(double)
#endif
#ifdef BUILD_IMAGE_UINT
INSTI(unsigned int)
#endif

