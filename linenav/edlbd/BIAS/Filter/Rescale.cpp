/* This file is part of the BIAS library (Basic ImageAlgorithmS).

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/
#include "Rescale.hh"




#include <Base/Common/CompareFloatingPoint.hh>
#include <Filter/Mean.hh>
#include <Filter/Gauss.hh>
#include <Filter/Binomial.hh>
#include <Filter/GaussThreshold.hh>

using namespace BIAS;
using namespace std;

namespace BIAS {

//////////////////////////////////////////////////////////////////////////
//                 implementation
//////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
Rescale<InputImageType, OutputImageType>::
Rescale()
  : FilterBase<InputImageType, OutputImageType>()
{
  _RescaleMeanSigmaFactor=0.9;
  _RescaleFactor= 2.0;
  Mean<InputImageType, OutputImageType> *pMean =
    new Mean<InputImageType, OutputImageType>;
  _lowpass = pMean;
  // set offset to 0.5 if we have an even mean filter:
  _LastPositionOffset = 0.5 * double((pMean->GetSize()+1)%2);
}

template <class InputImageType, class OutputImageType>
Rescale<InputImageType, OutputImageType>::
Rescale(const Rescale<InputImageType, OutputImageType>& other)
{
   _lowpass = NULL;
   (*this) = other;
}

template <class InputStorageType, class OutputStorageType>
Rescale<InputStorageType, OutputStorageType>&
Rescale<InputStorageType, OutputStorageType>::
operator=(const Rescale<InputStorageType, OutputStorageType>& other) {
  _RescaleFactor =  other._RescaleFactor;
  _RescaleMeanSigmaFactor = other._RescaleMeanSigmaFactor;
  _LastPositionOffset = other._LastPositionOffset;
  if (_lowpass) delete _lowpass;
  if (other._lowpass)
    _lowpass = other._lowpass->Clone();
  else
    _lowpass = NULL;
  return (*this);
}


template <class InputImageType, class OutputImageType>
Rescale<InputImageType, OutputImageType>::~Rescale()
{
  if (_lowpass) delete _lowpass;
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Filter(const Image<InputImageType>& src, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::Filter\n");
  int res=0;
  if (_RescaleFactor<1.0){
    res=Upsample(src, dst);
  } else if (_RescaleFactor>1.0){
    res=Downsample(src, dst);
  } else {
    dst=src;
    res=0;
    _LastPositionOffset=0.0;
  }
  return res;
}

//////////////////////////////////////////////////////////////////////////
//               downsampling functions
//////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Downsample(const Image<InputImageType>& src, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::Downsample(src, dst)\n");
  double pow2 = log(_RescaleFactor)/log(2.0);

  if (dynamic_cast<Mean<InputImageType, OutputImageType>*>(_lowpass)){
    unsigned newwidth, newheight;
    if (fabs(_RescaleFactor-2.0)<=DBL_EPSILON){ // factor is 2
      newwidth=src.GetWidth()>>1;
      newheight=src.GetHeight()>>1;
      if (dst.GetWidth()!=newwidth || dst.GetHeight()!=newheight
          || dst.GetChannelCount()!=src.GetChannelCount()){
        if (!dst.IsEmpty()) dst.Release();
        dst.Init(newwidth, newheight, src.GetChannelCount(),
                 src.GetStorageType(), src.IsInterleaved() );
      }
      return DownsampleBy2(src, dst);
    } else if (fabs(_RescaleFactor-4.0)<=DBL_EPSILON ){ // factor is 4
      newwidth=src.GetWidth()>>2;
      newheight=src.GetHeight()>>2;
      if (dst.GetWidth()!=newwidth || dst.GetHeight()!=newheight
          || dst.GetChannelCount()!=src.GetChannelCount()){
        if (!dst.IsEmpty()) dst.Release();
        dst.Init(newwidth, newheight, src.GetChannelCount(),
                 src.GetStorageType(), src.IsInterleaved());
      }
      return DownsampleBy4(src, dst);
    } else if ( pow2 - floor(pow2) <DBL_EPSILON ) { // factor is a power of 2
      return DownsampleBPoT(src,dst);
    }
  }
  return  Downsample(src, dst, _RescaleFactor);
}


template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Downsample(const Image<InputImageType>& src, Image<OutputImageType>& dst,
           const double factor)
{
  const double old_width = (double)src.GetWidth();
  const double old_height = (double)src.GetHeight();
  const unsigned newwidth = (unsigned)floor(old_width/factor);
  const unsigned newheight = (unsigned)floor(old_height/factor);

  Image<OutputImageType> meanim;
  int res = 0;
  if ((res=_ApplyMeanFilter(src, meanim, factor))!=0){
    BIASERR("error applying mean filter");
    return res;
  }
  int tlx, tly, brx, bry;
  meanim.GetROI()->GetCorners(tlx, tly, brx, bry);

  tlx=(int)ceil((double)tlx/factor);
  tly=(int)ceil((double)tly/factor);
  brx=(int)floor((double)brx/factor);
  bry=(int)floor((double)bry/factor);

  dst.Init(newwidth, newheight, src.GetChannelCount(), src.GetStorageType(),
           src.IsInterleaved());

  dst.GetROI()->SetCorners(tlx, tly, brx, bry);
  return _FillInterpolated(meanim, factor, dst);
}



template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Downsample(const Image<InputImageType>& src, Image<OutputImageType>& dst,
           const unsigned newwidth, const unsigned newheight)
{ 

  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::Downsample(src, dst, width, height)\n");

  if (src.GetWidth() == newwidth && src.GetHeight() == newheight) {
      dst = src;
      return 0;
  }


  const double hfactor = (double)src.GetWidth() / (double)newwidth;
  const double vfactor = (double)src.GetHeight() / (double)newheight;
  const double factor = (vfactor>hfactor)?vfactor:hfactor;

  Image<OutputImageType> meanim;
  if (_ApplyMeanFilter(src, meanim, factor)!=0){
    BIASERR("error applying mean filter");
  }

  Mean<InputImageType, OutputImageType> *pMean =
    dynamic_cast<Mean<InputImageType, OutputImageType> *>(_lowpass);
  if (pMean) {
    // set offset to 0.5 if we have an even mean filter:
    _LastPositionOffset = 0.5 * double((pMean->GetSize()+1)%2);
  } else {
    _LastPositionOffset = 0.0;
  }

  int tlx, tly, brx, bry;
  meanim.GetROI()->GetCorners(tlx, tly, brx, bry);

  tlx=(int)ceil((double)tlx/hfactor);
  tly=(int)ceil((double)tly/vfactor);
  brx=(int)floor((double)brx/hfactor);
  bry=(int)floor((double)bry/vfactor);

  if (dst.GetWidth()!=newwidth || dst.GetHeight()!=newheight){
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(newwidth, newheight, src.GetChannelCount(), src.GetStorageType(),
             src.IsInterleaved());
  }
  dst.GetROI()->SetCorners(tlx, tly, brx, bry);

  int res=0;
  if (src.GetChannelCount()==1 && src.GetColorModel()==ImageBase::CM_Grey){
    res=_FillInterpolatedGrey(meanim, dst);
  } else {
    res=_FillInterpolatedColor(meanim, dst);
  }

  return res;
}


////////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBy2(const Image<InputImageType>& src, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2(src, dst)\n");
  int res=0;
  unsigned nwidth=src.GetWidth()>>1;
  unsigned nheight=src.GetHeight()>>1;
  if (dst.GetWidth()!=nwidth || dst.GetHeight()!=nheight){
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(nwidth, nheight, src.GetChannelCount(), src.GetStorageType(),
             src.IsInterleaved());
  }
  if (src.GetColorModel()==ImageBase::CM_Grey){
    res = DownsampleBy2Grey(src, dst);
  } else {
    if (src.IsInterleaved() && src.GetChannelCount()==3){
      res = DownsampleBy2Color(src, dst);
    } else {
      // this is slow
      int newwidth=src.GetWidth()>>1;
      int newheight=src.GetHeight()>>1;
      res = Downsample(src, dst, newwidth, newheight);
    }
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;
  return res;
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBy2Grey(const Image<InputImageType>& Source,
                  Image<OutputImageType>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Grey\n");
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%2!=0) minx++;
  if (maxx%2!=0) maxx--;
  if (miny%2!=0) miny++;
  if (maxy%2!=0) maxy--;

  dx=minx>>1;
  dy=miny>>1;
  mdx=maxx>>1;
  mdy=(maxy>>1)-1;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy+1);

  register double sum;
  register const InputImageType *srcu, *srcl;
  register OutputImageType *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth(), dstwidth=Destination.GetWidth(),
    dststep;
  dst = Destination.GetImageData();

  srcl = srcu = Source.GetImageData()+minx+miny*srcwidth;
  srcl += srcwidth;
  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dstend = Destination.GetImageData() + mdx + mdy*dstwidth;
  dst+=dx;

  srcwidth+=(srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0.0;
      sum += (double)*srcu++;
      sum += (double)*srcu++;
      sum += (double)*srcl++;
      sum += (double)*srcl++;
      *dst++ = (OutputImageType)(sum / 4.0);
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;
  return 0;
}


template <>
int Rescale<unsigned char, unsigned char>::
DownsampleBy2Grey(const Image<unsigned char>& Source,
                  Image<unsigned char>& Destination) {
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Grey\n");
  register unsigned  sum;
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%2!=0) minx++;
  if (maxx%2!=0) maxx--;
  if (miny%2!=0) miny++;
  if (maxy%2!=0) maxy--;

  dx=minx>>1;
  dy=miny>>1;
  mdx=maxx>>1;
  mdy=(maxy>>1)-1;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy+1);

  register const unsigned char *srcu, *srcl;
  register unsigned char *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth(), dstwidth=Destination.GetWidth(),
    dststep;

  srcl = srcu = Source.GetImageData()+minx+miny*srcwidth;
  srcl += srcwidth;
  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dstend = Destination.GetImageData() + mdx + mdy*dstwidth;
  dst+=dx;

  srcwidth+=(srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0;
      sum += *srcu++;
      sum += *srcu++;
      sum += *srcl++;
      sum += *srcl++;
      *dst++ = sum >> 2;
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;

  return 0;
}


#ifdef BUILD_IMAGE_USHORT
template <>
int Rescale<unsigned short, unsigned short>::
DownsampleBy2Grey(const Image<unsigned short>& Source,
                      Image<unsigned short>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Grey\n");
  register unsigned  sum;
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%2!=0) minx++;
  if (maxx%2!=0) maxx--;
  if (miny%2!=0) miny++;
  if (maxy%2!=0) maxy--;

  dx=minx>>1;
  dy=miny>>1;
  mdx=maxx>>1;
  mdy=(maxy>>1)-1;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy+1);

  register const unsigned short *srcu, *srcl;
  register unsigned short *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth(), dstwidth=Destination.GetWidth(),
    dststep;

  srcl = srcu = Source.GetImageData()+minx+miny*srcwidth;
  srcl += srcwidth;
  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dstend = Destination.GetImageData() + mdx + mdy*dstwidth;
  dst+=dx;

  srcwidth+=(srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0;
      sum += *srcu++;
      sum += *srcu++;
      sum += *srcl++;
      sum += *srcl++;
      *dst++ = sum >> 2;
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;

  return 0;
}
#endif

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBy2Color(const Image<InputImageType>& Source,
                   Image<OutputImageType>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Color\n");
  BIASASSERT(Source.IsPlanar()==Destination.IsPlanar());




  register double sum1, sum2, sum3;
  register const InputImageType *srcu, *srcl;
  register OutputImageType   *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth()*3, dstwidth=Destination.GetWidth()*3;
  dst = Destination.GetImageData();
  srcl = srcu = Source.GetImageData();
  srcl += srcwidth;
  dstend = dst + Destination.GetPixelCount() * 3;
  dstlend = dst + dstwidth * 3;

  if (srcwidth%2!=0){
    srcwidth+=3;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum1 = sum2 = sum3 = 0.0;
      sum1 += (double)*srcu++;
      sum2 += (double)*srcu++;
      sum3 += (double)*srcu++;
      sum1 += (double)*srcu++;
      sum2 += (double)*srcu++;
      sum3 += (double)*srcu++;
      sum1 += (double)*srcl++;
      sum2 += (double)*srcl++;
      sum3 += (double)*srcl++;
      sum1 += (double)*srcl++;
      sum2 += (double)*srcl++;
      sum3 += (double)*srcl++;
      *dst++ = (OutputImageType)(sum1 / 4.0);
      *dst++ = (OutputImageType)(sum2 / 4.0);
      *dst++ = (OutputImageType)(sum3 / 4.0);
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;
  return 0;
}

template <>
int Rescale<unsigned char, unsigned char>::
DownsampleBy2Color(const Image<unsigned char>& Source,
                   Image<unsigned char>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Color\n");
  BIASASSERT(Source.IsPlanar()==Destination.IsPlanar());

  register unsigned int sum1, sum2, sum3;
  register const unsigned char *srcu, *srcl;
  register unsigned char *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth() * 3,
    dstwidth=Destination.GetWidth() *3;
  dst = Destination.GetImageData();
  srcl = srcu = Source.GetImageData();
  srcl += srcwidth;
  dstend = dst + Destination.GetPixelCount() * 3;
  dstlend = dst + dstwidth;

  if (srcwidth%2!=0){
    srcwidth+=3;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum1 = sum2 = sum3 = 0;
      sum1 += *srcu++;
      sum2 += *srcu++;
      sum3 += *srcu++;
      sum1 += *srcu++;
      sum2 += *srcu++;
      sum3 += *srcu++;
      sum1 += *srcl++;
      sum2 += *srcl++;
      sum3 += *srcl++;
      sum1 += *srcl++;
      sum2 += *srcl++;
      sum3 += *srcl++;
      *dst++ = sum1 >> 2;
      *dst++ = sum2 >> 2;
      *dst++ = sum3 >> 2;
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dstlend += dstwidth;
  }

//   for (unsigned int y=0;y<Destination.GetHeight(); y++){
//     for (unsigned int x=0;x<Destination.GetWidth(); x++){
//       for (unsigned int c=0;c<3;c++){
// 	sum1 = srcu[y *
//       }
//     }
//   }

  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;

  return 0;
}

#ifdef BUILD_IMAGE_USHORT
template <>
int Rescale<unsigned short, unsigned short>::
DownsampleBy2Color(const Image<unsigned short>& Source,
                   Image<unsigned short>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy2Color\n");

  register unsigned int sum1, sum2, sum3;
  register const unsigned short *srcu, *srcl;
  register unsigned short *dst, *dstend, *dstlend;
  register int srcwidth=Source.GetWidth() * 3,
    dstwidth=Destination.GetWidth() *3;
  dst = Destination.GetImageData();
  srcl = srcu = Source.GetImageData();
  srcl += srcwidth;
  dstend = dst + Destination.GetPixelCount() * 3;
  dstlend = dst + dstwidth;

  if (srcwidth%2!=0){
    srcwidth+=3;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum1 = sum2 = sum3 = 0;
      sum1 += *srcu++;
      sum2 += *srcu++;
      sum3 += *srcu++;
      sum1 += *srcu++;
      sum2 += *srcu++;
      sum3 += *srcu++;
      sum1 += *srcl++;
      sum2 += *srcl++;
      sum3 += *srcl++;
      sum1 += *srcl++;
      sum2 += *srcl++;
      sum3 += *srcl++;
      *dst++ = sum1 >> 2;
      *dst++ = sum2 >> 2;
      *dst++ = sum3 >> 2;
    }
    srcu += srcwidth;
    srcl += srcwidth;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=0.5;
  return 0;
}
#endif

/////////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBy4(const Image<InputImageType>& src, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy4\n");
  int res=0;
  unsigned nwidth=(int)src.GetWidth()>>2;
  unsigned nheight=(int)src.GetHeight()>>2;
  if (dst.GetWidth()!=nwidth || dst.GetHeight()!=nheight){
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(nwidth, nheight, src.GetChannelCount(), src.GetStorageType(),
             src.IsInterleaved());
  }
  if (src.GetColorModel()==ImageBase::CM_Grey){
    res = DownsampleBy4Grey(src, dst);
  } else {
    if (src.IsInterleaved() && src.GetChannelCount()==3){
      double oldFactor = GetFactor();
      SetFactor(4.0);
      res = DownsampleBPoT(src, dst);
      SetFactor(oldFactor);
    } else {
      // this is slow
      int newwidth=src.GetWidth()>>2;
      int newheight=src.GetHeight()>>2;
      res = Downsample(src, dst, newwidth, newheight);
    }
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=1.5;
  return res;
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBy4Grey(const Image<InputImageType>& Source,
                  Image<OutputImageType>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy4Grey\n");
  BIASASSERT(Source.GetChannelCount()==1 &&
             Source.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Destination.GetChannelCount()==1 &&
             Destination.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Source.GetWidth()>>2==Destination.GetWidth());
  BIASASSERT(Source.GetHeight()>>2==Destination.GetHeight());

  /// \todo warning kerneltype sum needed here

  double  sum;
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%4!=0) minx+=(4-(minx%4));
  if (maxx%4!=0) maxx-=(maxx%4);
  if (miny%4!=0) miny+=(4-(miny%4));
  if (maxy%4!=0) maxy-=(maxy%4);

  dx=minx>>2;
  dy=miny>>2;
  mdx=maxx>>2;
  mdy=maxy>>2;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy);

  register const InputImageType *src1, *src2, *src3, *src4;
  register OutputImageType *dst, *dstend, *dstlend;
  register int srcwidth=(int)Source.GetWidth();
  register const int dstwidth=(int)Destination.GetWidth();
  register int dststep;

  src1 = Source.GetImageData()+minx+miny*srcwidth;
  src2 = src1 + srcwidth;
  src3 = src2 + srcwidth;
  src4 = src3 + srcwidth;

  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dst+=dx;
  dstend = Destination.GetImageData() + mdx + (mdy-1)*dstwidth;

  srcwidth+=(3*srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }

  /// \todo warning kerneltype div needed here

  const double div=(1.0/16.0);
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0.0;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;

      sum += *src2++;
      sum += *src2++;
      sum += *src2++;
      sum += *src2++;

      sum += *src3++;
      sum += *src3++;
      sum += *src3++;
      sum += *src3++;

      sum += *src4++;
      sum += *src4++;
      sum += *src4++;
      sum += *src4++;

      *dst++ = (OutputImageType)(sum * div);
    }
    src1 += srcwidth;
    src2 += srcwidth;
    src3 += srcwidth;
    src4 += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=1.5;

  return 0;
}


template <>
int Rescale<unsigned char, unsigned char>::
DownsampleBy4Grey(const Image<unsigned char>& Source,
                  Image<unsigned char>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy4Grey\n");
  BIASASSERT(Source.GetChannelCount()==1 &&
             Source.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Destination.GetChannelCount()==1 &&
             Destination.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Source.GetWidth()>>2==Destination.GetWidth());
  BIASASSERT(Source.GetHeight()>>2==Destination.GetHeight());
  register unsigned  sum;
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%4!=0) minx+=(4-(minx%4));
  if (maxx%4!=0) maxx-=(maxx%4);
  if (miny%4!=0) miny+=(4-(miny%4));
  if (maxy%4!=0) maxy-=(maxy%4);

  dx=minx>>2;
  dy=miny>>2;
  mdx=maxx>>2;
  mdy=maxy>>2;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy);

  register const unsigned char *src1, *src2, *src3, *src4;
  register unsigned char *dst, *dstend, *dstlend;
  register int srcwidth=(int)Source.GetWidth();
  register const int dstwidth=(int)Destination.GetWidth();
  register int dststep;

  src1 = Source.GetImageData()+minx+miny*srcwidth;
  src2 = src1 + srcwidth;
  src3 = src2 + srcwidth;
  src4 = src3 + srcwidth;

  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dst+=dx;
  dstend = Destination.GetImageData() + mdx + (mdy-1)*dstwidth;

  srcwidth+=(3*srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;

      sum += *src2++;
      sum += *src2++;
      sum += *src2++;
      sum += *src2++;

      sum += *src3++;
      sum += *src3++;
      sum += *src3++;
      sum += *src3++;

      sum += *src4++;
      sum += *src4++;
      sum += *src4++;
      sum += *src4++;

      *dst++ = sum >> 4;
    }
    src1 += srcwidth;
    src2 += srcwidth;
    src3 += srcwidth;
    src4 += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=1.5;

  return 0;
}

#ifdef BUILD_IMAGE_USHORT
template <>
int Rescale<unsigned short, unsigned short>::
DownsampleBy4Grey(const Image<unsigned short>& Source,
                      Image<unsigned short>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBy4Grey\n");
  BIASASSERT(Source.GetChannelCount()==1 &&
             Source.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Destination.GetChannelCount()==1 &&
             Destination.GetColorModel()==ImageBase::CM_Grey);
  BIASASSERT(Source.GetWidth()>>2==Destination.GetWidth());
  BIASASSERT(Source.GetHeight()>>2==Destination.GetHeight());
  register unsigned  sum;
  unsigned minx, miny, maxx, maxy, dx, dy, mdx, mdy;
  Source.GetROI()->GetCorners(minx, miny, maxx, maxy);

  // make sure the image is not shifted
  if (minx%4!=0) minx+=(4-(minx%4));
  if (maxx%4!=0) maxx-=(maxx%4);
  if (miny%4!=0) miny+=(4-(miny%4));
  if (maxy%4!=0) maxy-=(maxy%4);

  dx=minx>>2;
  dy=miny>>2;
  mdx=maxx>>2;
  mdy=maxy>>2;

  Destination.GetROI()->SetCorners(dx, dy, mdx, mdy);

  register const unsigned short *src1, *src2, *src3, *src4;
  register unsigned short *dst, *dstend, *dstlend;
  register int srcwidth=(int)Source.GetWidth();
  register const int dstwidth=(int)Destination.GetWidth();
  register int dststep;

  src1 = Source.GetImageData()+minx+miny*srcwidth;
  src2 = src1 + srcwidth;
  src3 = src2 + srcwidth;
  src4 = src3 + srcwidth;

  dst = Destination.GetImageData() + dy*dstwidth;
  dstlend = dst + mdx;
  dst+=dx;
  dstend = Destination.GetImageData() + mdx + (mdy-1)*dstwidth;

  srcwidth+=(3*srcwidth-maxx+minx);
  dststep=dstwidth-mdx+dx;

  if (srcwidth%2!=0){
    srcwidth+=1;
  }
  while (dst < dstend){
    while (dst < dstlend){
      sum = 0;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;
      sum += *src1++;

      sum += *src2++;
      sum += *src2++;
      sum += *src2++;
      sum += *src2++;

      sum += *src3++;
      sum += *src3++;
      sum += *src3++;
      sum += *src3++;

      sum += *src4++;
      sum += *src4++;
      sum += *src4++;
      sum += *src4++;

      *dst++ = sum >> 4;
    }
    src1 += srcwidth;
    src2 += srcwidth;
    src3 += srcwidth;
    src4 += srcwidth;
    dst += dststep;
    dstlend += dstwidth;
  }
  // see DownsampleBy2 for details
  _LastPositionOffset=1.5;

  return 0;
}
#endif





template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
DownsampleBPoT(const Image<InputImageType>& Source,
               Image<OutputImageType>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::DownsampleBPoT\n");
  int res = 0;
  unsigned int channels = Source.GetChannelCount();
  int PowerOfTwo = (int)rint(log(_RescaleFactor)/log(2.0));
  if (PowerOfTwo == 0){

    // No downsample required, just copy
    Destination = Source;
    _LastPositionOffset=0.0;

  } else { // downsample

    unsigned int newwidth, newheight;
    unsigned int oldwidth,oldheight;

    oldwidth = Source.GetWidth();
    oldheight = Source.GetHeight();

    newwidth = (oldwidth >> PowerOfTwo);
    newheight = (oldheight >> PowerOfTwo);

    if (Destination.GetWidth() != newwidth
        || Destination.GetHeight() != newheight
        ||  Destination.GetChannelCount() != channels ){
      if ( ! Destination.IsEmpty())
        Destination.Release();
      Destination.Init(newwidth,newheight,channels,
                       Source.GetStorageType(),
                       Source.IsInterleaved());
    }
    OutputImageType *sink;
    const InputImageType *src;
    bool IdenticalImages = false;

    sink = Destination.GetImageData();
    src = Source.GetImageData();

    if ((void*)src==(void*)sink){
      IdenticalImages = true;
      sink = new OutputImageType[newwidth * newheight];
    }

    unsigned min_oldx, min_oldy, max_oldx, max_oldy;
    const double div = pow(2.0,PowerOfTwo+PowerOfTwo);
    const int sze = pow(2.0,PowerOfTwo);
    double sum;
    if (Source.IsInterleaved()){

      for (unsigned y=0; y<newheight; y++){
        min_oldy = y<<PowerOfTwo;
        max_oldy = min_oldy + sze;
        for (unsigned x=0; x<newwidth; x++){
          min_oldx = x<<PowerOfTwo;
          max_oldx = min_oldx + sze;
          for (unsigned c=0; c<channels; c++){
            sum=0.0;
            // now sum up the values in the old image
            for (unsigned oy=min_oldy; oy<max_oldy; oy++){
              for (unsigned ox=min_oldx; ox<max_oldx; ox++){
                sum += (double) src[oy*oldwidth*channels + ox*channels+c];
              }
            }
            // now set the new value
            sink[y*newwidth*channels+x*channels+c] =
              Cast<OutputImageType>(sum / div);
          }
        }
      }

    } else {

      BIASERR("Rescale::DownsampleBPoT unfinished for planar images");
      BIASABORT;

    }


    if (IdenticalImages) {
      Destination.Release();
      Destination.Init(newwidth, newheight, channels);
      Destination.ReleaseImageDataPointer();

      Destination.RedirectImageDataPointer(sink);
    }
    _LastPositionOffset= pow(2.0, PowerOfTwo-1.0)-0.5;
  }

  return res;
}




//////////////////////////////////////////////////////////////////////////
//               upsampling functions
//////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Upsample(const Image<InputImageType>& src, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::Upsample(src, dst)\n");

  return Upsample(src, dst,
                  (unsigned int)floor((double)src.GetWidth()/
                                      _RescaleFactor),
                  (unsigned int)floor((double)src.GetHeight()/
                                      _RescaleFactor));
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
Upsample(const Image<InputImageType>& src, Image<OutputImageType>& dst,
         const unsigned newwidth, const unsigned newheight)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::Upsample(src, dst, w, h)\n");

  BIASASSERT(src.GetWidth()<=newwidth);
  BIASASSERT(src.GetHeight()<=newheight);

  int res=0;
  float oldx, oldy;
  float scalex, scaley;
  unsigned int channelcount=src.GetChannelCount();
  bool identicalimages=false;
  OutputImageType *sink;

  if ((void *)src.GetImageData() == (void *)dst.GetImageData())
    identicalimages=true;

  if (!identicalimages){
    if (!src.SamePixelAndChannelCount(dst))
      if (!dst.IsEmpty()) dst.Release();

    if (dst.IsEmpty())
      dst.Init(newwidth, newheight, channelcount,  src.GetStorageType(),
               src.IsInterleaved());

    sink = dst.GetImageData();
  } else {
    sink = new OutputImageType[ newwidth *  newheight * channelcount];
  }

  scalex = (float)(src.GetWidth() - 1)/ (float) (newwidth);
  scaley = (float)(src.GetHeight() - 1) / (float) (newheight);
  if (src.IsInterleaved()) {
    for (unsigned int y = 0; y<newheight; y++)
      for (unsigned int x = 0; x<newwidth; x++)
        for (unsigned int c = 0; c<channelcount; c++) {
          oldx = (float)x * scalex;
          oldy = (float)y * scaley;
//           oldx = (float)x / factor;
//           oldy = (float)y / factor;
          sink[(y * newwidth + x)* channelcount + c] =
      // (OutputImageType)rint(src.BilinearInterpolation(oldx, oldy, c));
            (OutputImageType)src.BilinearInterpolation(oldx, oldy, c);
        }
  } else { // planar
    BIASERR("Upscale only implemented on interleaved images");
    res = -1;
  }
  if (identicalimages && res==0){
    dst.Release();
    dst.Init(newwidth, newheight, channelcount);

    dst.ReleaseImageDataPointer();
    dst.RedirectImageDataPointer(sink);
  }

  return res;
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
UpsampleGrey(const Image<InputImageType>& src, Image<OutputImageType>& dst,
             const unsigned newwidth, const unsigned newheight)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::UpsampleGrey(src, dst, w, h)\n");

  BIASASSERT(src.GetWidth()<newwidth);
  BIASASSERT(src.GetHeight()<newheight);
  BIASASSERT(src.GetChannelCount()==1);
  BIASASSERT(src.GetColorModel()==ImageBase::CM_Grey);
  int res=0;
  float oldx, oldy;
  float scalex, scaley;
  bool identicalimages=false;
  OutputImageType *sink;

  if ((void *)src.GetImageData() == (void *)dst.GetImageData())
    identicalimages=true;

  if (!identicalimages){
    if (!src.SamePixelAndChannelCount(dst))
      if (!dst.IsEmpty()) dst.Release();

    if (dst.IsEmpty())
      dst.Init(newwidth, newheight, 1);

    sink = dst.GetImageData();
  } else {
    sink = new OutputImageType[ newwidth *  newheight ];
  }

  scalex = (float)(src.GetWidth() - 1)/ (float) (newwidth);
  scaley = (float)(src.GetHeight() - 1) / (float) (newheight);
  if (src.IsInterleaved()) {
    for (unsigned int y = 0; y<newheight; y++)
      for (unsigned int x = 0; x<newwidth; x++){
        oldx = (float)x * scalex;
        oldy = (float)y * scaley;
        sink[(y * newwidth + x)] =
          src.FastBilinearInterpolationGrey(oldx, oldy);
      }
  } else { // planar
    BIASERR("UpscaleGrey only implemented on interleaved images");
    res = -1;
  }
  if (identicalimages && res==0){
    dst.Release();
    dst.Init(newwidth, newheight, 1);

    dst.ReleaseImageDataPointer();
    dst.RedirectImageDataPointer(sink);
  }

  return res;
}


// bicubic weights for factor 2 upsampling
#define WY0 -0.03125
#define WY1  0.28125

#define WX0  -0.125
#define WX1  1.125

////////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
UpsampleBy2Grey(const Image<InputImageType>& Source,
                Image<OutputImageType>& Destination,
                InterpolationType interpolation_method,
                const double& valmin,
                const double& valmax) {
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::UpsampleBy2Grey\n");
  register OutputImageType *dstu=NULL, *dstl=NULL;
  register const InputImageType *src=NULL,  *srclend=NULL;
  const int srcwidth = Source.GetWidth(),
    dstwidth = Destination.GetWidth();

  dstu = dstl = Destination.GetImageData();
  src = Source.GetImageData();
  dstl += dstwidth;

  srclend = src + srcwidth;

  switch (interpolation_method) {
  case NearestNeighbour: {
    register const InputImageType* srcend = src + Source.GetPixelCount();
    while (src < srcend){
      while (src < srclend){
        *dstu++ = *dstl++ = *src;
        *dstu++ = *dstl++ = *src++;
      }
      dstu += dstwidth;
      dstl += dstwidth;
      srclend += srcwidth;
    }
    return 0;
  }
  case Bilinear: {


    register const InputImageType* srcend =
      src + Source.GetPixelCount()- srcwidth;
    // main part
    while (src < srcend){
      const InputImageType* c_srclend = srclend-1;
      while (src < c_srclend){
        *dstl++ =  OutputImageType(0.5 * (src[0] + src[srcwidth]));
        *dstu++ = *src;
        *dstu++ = OutputImageType(0.5  * (src[0] + src[1]));
        *dstl++ = OutputImageType(0.25 * (src[0] + src[1]+
                                          src[srcwidth] + src[srcwidth+1]));
        src++;
      }
      // last column
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = *src;
      *dstl = dstl[-1]; dstl++;
      src++;

      // dst: skip already completed line
      dstu += dstwidth;
      dstl += dstwidth;
      srclend += srcwidth;
    }
    srclend-=1;
    // last line only copied
    while (src < srclend){
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-1]+src[0]));
    }
    // last pixel (nearest neighbour)
    *dstu++ = *dstl++ = *src;
    *dstu++ = *dstl++ = *src;
    return 0;
  }
  case Bicubic: {


    // first line bilinear
    const InputImageType* c_firstsrclend = srclend-1;
    while (src < c_firstsrclend){
      *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = OutputImageType(0.5 * (src[0] + src[1]));
      *dstl++ = OutputImageType(0.25 * (src[0] + src[1] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;
    }
    // last column
    *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth]));
    *dstu++ = *src;
    *dstu++ = *src;
    *dstl   = dstl[-1]; dstl++;
    src++;
    dstu += dstwidth;
    dstl += dstwidth;
    srclend += srcwidth;

    register const InputImageType* srcend =
      Source.GetImageData() + Source.GetPixelCount()- (srcwidth*2+2);

    const int width2 = 2*srcwidth;
    const int mwidth = -srcwidth;
    // main part
    while (src < srcend){
      // first column bilinear
      *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = OutputImageType(0.5  * (src[0] + src[1]));
      *dstl++ = OutputImageType(0.25 * (src[0] + src[1] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;

      const InputImageType* c_srclend = srclend - 2;
      while (src < c_srclend){
        // lower left
        register double val = 2.0*((WY0*(src[mwidth    ]+src[width2    ])) +
                                   (WY1*(src[ 0        ]+src[srcwidth  ])));
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);

        // upper left is original pixel
        *dstu++ = OutputImageType(*src);

        // upper right
        val = (WX0*(src[-1] + src[2])+WX1*(src[0]+src[1]))*0.5;

        if (val>=valmax) *dstu++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstu++ = OutputImageType(valmin);
        else  *dstu++ = OutputImageType(val);

        // lower right
        val = ((WX0*(src[mwidth - 1] + src[mwidth + 2]) +
                WX1*(src[mwidth    ] + src[mwidth + 1])) +
               (WX0*(src[width2 - 1] + src[width2 + 2]) +
                WX1*(src[width2    ] + src[width2 + 1])))*WY0 +
          ((WX0*(src[-1        ] + src[ 2        ]) +
            WX1*(src[ 0        ] + src[ 1        ])) +
           (WX0*(src[srcwidth  - 1] + src[srcwidth  + 2]) +
            WX1*(src[srcwidth     ] + src[srcwidth  + 1])))*WY1;

        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);

        src++;
      }

      // last columns
      // bilinear
      *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = OutputImageType(0.5 * (src[0] + src[1]));
      *dstl++ = OutputImageType(0.25 * (src[0] + src[1] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;
      // copy
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = *src;
      *dstl   = dstl[-1]; dstl++;
      src++;

      dstu += dstwidth;
      dstl += dstwidth;
      srclend += srcwidth;
    }

    // bilinear
    register const InputImageType* srcendlin =
      Source.GetImageData() + Source.GetPixelCount()- srcwidth;
    // main part
    while (src < srcendlin){
      const InputImageType* c_srclend = srclend - 1;
      while (src < c_srclend){
        *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth]));
        *dstu++ = *src++;
        *dstu++ = OutputImageType(0.5 * (src[-1] + src[0]));
        *dstl++ = OutputImageType(0.25 * (src[-1] + src[0] +
                                          src[srcwidth-1] + src[srcwidth]));
      }
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth]));
      *dstu++ = *src;
      *dstu++ = *src;
      *dstl   = dstl[-1]; dstl++;
      src++;

      dstu += dstwidth;
      dstl += dstwidth;
      srclend += srcwidth;
    }

    srclend-=1;
    // last line only copied
    while (src < srclend){
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-1]+src[0]));
    }
    // last pixel (nearest neighbour)
    *dstu++ = *dstl++ = *src;
    *dstu++ = *dstl++ = *src;
    return 0;
  }
  default:
    BIASERR("interpolation not implemented in rescale");
    BIASABORT;
  }
  return 0;
}


template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
UpsampleBy2RGBInterleaved(const Image<InputImageType>& Source,
                          Image<OutputImageType>& Destination,
                          InterpolationType interpolation_method,
                          const double& valmin,
                          const double& valmax) {
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::UpsampleBy2RGBInterleaved\n");
  register OutputImageType *dstu=NULL, *dstl=NULL;
  register const InputImageType *src=NULL,  *srclend=NULL;
  const int srcwidth = Source.GetWidth(),
    dstwidth = Destination.GetWidth();
  const int srcwidth3 = 3*srcwidth, dstwidth3=3*dstwidth;


  dstu = dstl = Destination.GetImageData();
  src = Source.GetImageData();
  dstl += dstwidth3;

  srclend = src + srcwidth3;

  switch (interpolation_method) {
  case NearestNeighbour: {
    register const InputImageType* srcend = src + Source.GetPixelCount()*3;
    while (src < srcend){
      while (src < srclend){
        *dstu++ = *dstl++ =  src[0];
        *dstu++ = *dstl++ =  src[1];
        *dstu++ = *dstl++ =  src[2];
        *dstu++ = *dstl++ = *src++;
        *dstu++ = *dstl++ = *src++;
        *dstu++ = *dstl++ = *src++;
      }
      dstu += dstwidth3;
      dstl += dstwidth3;
      srclend += srcwidth3;
    }
    return 0;
  }
  case Bilinear: {


    register const InputImageType* srcend =
      src + Source.GetPixelCount()*3 - srcwidth3;
    // main part
    while (src < srcend){
      const InputImageType* c_srclend = srclend-3;
      while (src < c_srclend){
        *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
        *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
        *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

        *dstu++ = src[0];
        *dstu++ = src[1];
        *dstu++ = src[2];

        *dstu++ = OutputImageType(0.5  * (src[0] + src[3]));
        *dstu++ = OutputImageType(0.5  * (src[1] + src[4]));
        *dstu++ = OutputImageType(0.5  * (src[2] + src[5]));

        *dstl++ = OutputImageType(0.25 * (src[0] + src[3]+
                                          src[srcwidth3] + src[srcwidth3+1]));
        src++;
        *dstl++ = OutputImageType(0.25 * (src[0] + src[3]+
                                          src[srcwidth3] + src[srcwidth3+1]));
        src++;
        *dstl++ = OutputImageType(0.25 * (src[0] + src[3]+
                                          src[srcwidth3] + src[srcwidth3+1]));
        src++;

      }
      // last column
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = src[0];
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstu++ = src[0];
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstl = dstl[-3]; dstl++;
      *dstl = dstl[-3]; dstl++;
      *dstl = dstl[-3]; dstl++;
      src+=3;
      // dst: skip already completed line
      dstu += dstwidth3;
      dstl += dstwidth3;
      srclend += srcwidth3;
    }
    srclend-=3;
    // last line only copied
    while (src < srclend){
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = *src++;

      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-3]+src[0]));
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-2]+src[1]));
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-1]+src[2]));
    }
    // last pixel (nearest neighbour)
    *dstu++ = *dstl++ = src[0];
    *dstu++ = *dstl++ = src[1];
    *dstu++ = *dstl++ = src[2];
    *dstu++ = *dstl++ = src[0];
    *dstu++ = *dstl++ = src[1];
    *dstu++ = *dstl++ = src[2];

    return 0;
  }
  case Bicubic: {


    // first line bilinear
    const InputImageType* c_firstsrclend = srclend-3;
    while (src < c_firstsrclend){
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = src[0];
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstu++ = OutputImageType(0.5 * (src[0] + src[3]));
      *dstu++ = OutputImageType(0.5 * (src[1] + src[4]));
      *dstu++ = OutputImageType(0.5 * (src[2] + src[5]));

      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+3]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+3]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+3]));
      src++;
    }
    // last column
    *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
    *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
    *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));
    *dstu++ = src[0];
    *dstu++ = src[1];
    *dstu++ = src[2];
    *dstu++ = *src++;
    *dstu++ = *src++;
    *dstu++ = *src++;
    *dstl = dstl[-3]; dstl++;
    *dstl = dstl[-3]; dstl++;
    *dstl = dstl[-3]; dstl++;

    dstu += dstwidth3;
    dstl += dstwidth3;
    srclend += srcwidth3;

    register const InputImageType* srcend =
      Source.GetImageData() + Source.GetPixelCount()*3 - (srcwidth3*2+6);

    const int width2 = 2*srcwidth3;
    const int mwidth = -srcwidth3;
    // main part
    while (src < srcend){
      // first column bilinear
      *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = src[0];
      *dstu++ = src[0];
      *dstu++ = src[0];

      *dstu++ = OutputImageType(0.5  * (src[0] + src[3]));
      *dstu++ = OutputImageType(0.5  * (src[1] + src[4]));
      *dstu++ = OutputImageType(0.5  * (src[2] + src[5]));

      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+1]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+1]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth3] + src[srcwidth3+1]));
      src++;

      const InputImageType* c_srclend = srclend - 6;
      while (src < c_srclend){
        // lower left
        register double val = 2.0*((WY0*(src[mwidth    ]+src[width2    ])) +
                                   (WY1*(src[ 0        ]+src[srcwidth3  ])));
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);

        val = 2.0*((WY0*(src[mwidth+1    ]+src[width2+1    ])) +
                   (WY1*(src[ 1        ]+src[srcwidth3+1  ])));
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);

        val = 2.0*((WY0*(src[mwidth+2    ]+src[width2+2    ])) +
                   (WY1*(src[ 2        ]+src[srcwidth3+2  ])));
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);

        // upper left is original pixel
        *dstu++ = OutputImageType(src[0]);
        *dstu++ = OutputImageType(src[1]);
        *dstu++ = OutputImageType(src[2]);

        // upper right
        val = (WX0*(src[-3] + src[6])+WX1*(src[0]+src[3]))*0.5;
        if (val>=valmax) *dstu++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstu++ = OutputImageType(valmin);
        else  *dstu++ = OutputImageType(val);

        val = (WX0*(src[-2] + src[7])+WX1*(src[1]+src[4]))*0.5;
        if (val>=valmax) *dstu++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstu++ = OutputImageType(valmin);
        else  *dstu++ = OutputImageType(val);

        val = (WX0*(src[-1] + src[8])+WX1*(src[2]+src[5]))*0.5;
        if (val>=valmax) *dstu++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstu++ = OutputImageType(valmin);
        else  *dstu++ = OutputImageType(val);

        // lower right
        val = ((WX0*(src[mwidth - 3] + src[mwidth + 6]) +
                WX1*(src[mwidth    ] + src[mwidth + 3])) +
               (WX0*(src[width2 - 3] + src[width2 + 6]) +
                WX1*(src[width2    ] + src[width2 + 3])))*WY0 +
          ((WX0*(src[-3        ] + src[ 6        ]) +
            WX1*(src[ 0        ] + src[ 3        ])) +
           (WX0*(src[srcwidth3  - 3] + src[srcwidth3  + 6]) +
            WX1*(src[srcwidth3     ] + src[srcwidth3  + 3])))*WY1;
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);
        src++;
        val = ((WX0*(src[mwidth - 3] + src[mwidth + 6]) +
                WX1*(src[mwidth    ] + src[mwidth + 3])) +
               (WX0*(src[width2 - 3] + src[width2 + 6]) +
                WX1*(src[width2    ] + src[width2 + 3])))*WY0 +
          ((WX0*(src[-3        ] + src[ 6        ]) +
            WX1*(src[ 0        ] + src[ 3        ])) +
           (WX0*(src[srcwidth3  - 3] + src[srcwidth3  + 6]) +
            WX1*(src[srcwidth3     ] + src[srcwidth3  + 3])))*WY1;
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);
        src++;
        val = ((WX0*(src[mwidth - 3] + src[mwidth + 6]) +
                WX1*(src[mwidth    ] + src[mwidth + 3])) +
               (WX0*(src[width2 - 3] + src[width2 + 6]) +
                WX1*(src[width2    ] + src[width2 + 3])))*WY0 +
          ((WX0*(src[-3        ] + src[ 6        ]) +
            WX1*(src[ 0        ] + src[ 3        ])) +
           (WX0*(src[srcwidth3  - 3] + src[srcwidth3  + 6]) +
            WX1*(src[srcwidth3     ] + src[srcwidth3  + 3])))*WY1;
        if (val>=valmax) *dstl++ = OutputImageType(valmax);
        else if (val<=valmin)  *dstl++ = OutputImageType(valmin);
        else  *dstl++ = OutputImageType(val);
        src++;
      }

      // last columns
      // bilinear
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = src[0];
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstu++ = OutputImageType(0.5 * (src[0] + src[3]));
      *dstu++ = OutputImageType(0.5 * (src[1] + src[4]));
      *dstu++ = OutputImageType(0.5 * (src[2] + src[5]));

      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;
      *dstl++ = OutputImageType(0.25 * (src[0] + src[3] +
                                        src[srcwidth] + src[srcwidth+1]));
      src++;


      // copy
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = *src;
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstu++ = src[0];
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstl = dstl[-3]; dstl++;
      src++;
      *dstl = dstl[-3]; dstl++;
      src++;
      *dstl = dstl[-3]; dstl++;
      src++;

      dstu += dstwidth3;
      dstl += dstwidth3;
      srclend += srcwidth3;
    }

    // bilinear
    register const InputImageType* srcendlin =
      Source.GetImageData() + Source.GetPixelCount()- srcwidth3;
    // main part
    while (src < srcendlin){
      const InputImageType* c_srclend = srclend - 3;
      while (src < c_srclend){
        *dstl++ = OutputImageType(0.5 * (*src + src[srcwidth3]));
        *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
        *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

        *dstu++ = *src++;
        *dstu++ = *src++;
        *dstu++ = *src++;

        *dstu++ = OutputImageType(0.5 * (src[-3] + src[0]));
        *dstu++ = OutputImageType(0.5 * (src[-2] + src[1]));
        *dstu++ = OutputImageType(0.5 * (src[-1] + src[2]));

        *dstl++ = OutputImageType(0.25 * (src[-3] + src[0] +
                                          src[srcwidth3-3] + src[srcwidth3]));
        *dstl++ = OutputImageType(0.25 * (src[-2] + src[1] +
                                          src[srcwidth3-2] + src[srcwidth3]));
        *dstl++ = OutputImageType(0.25 * (src[-1] + src[2] +
                                          src[srcwidth3-1] + src[srcwidth3]));
      }
      *dstl++ = OutputImageType(0.5 * (src[0] + src[srcwidth3]));
      *dstl++ = OutputImageType(0.5 * (src[1] + src[srcwidth3+1]));
      *dstl++ = OutputImageType(0.5 * (src[2] + src[srcwidth3+2]));

      *dstu++ = *src;
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstu++ = *src;
      *dstu++ = src[1];
      *dstu++ = src[2];

      *dstl = dstl[-3]; dstl++;
      src++;
      *dstl = dstl[-3]; dstl++;
      src++;
      *dstl = dstl[-3]; dstl++;
      src++;

      dstu += dstwidth3;
      dstl += dstwidth3;
      srclend += srcwidth3;
    }

    srclend-=3;
    // last line only copied
    while (src < srclend){
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = *src++;
      *dstu++ = *dstl++ = *src++;

      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-3]+src[0]));
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-2]+src[1]));
      *dstu++ = *dstl++ = OutputImageType(0.5*(src[-1]+src[2]));
    }
    // last pixel (nearest neighbour)
    *dstu++ = *dstl++ = *src;
    *dstu++ = *dstl++ = src[1];
    *dstu++ = *dstl++ = src[2];

    *dstu++ = *dstl++ = *src;
    *dstu++ = *dstl++ = src[1];
    *dstu++ = *dstl++ = src[2];



    return 0;
  }
  default:
    BIASERR("interpolation not implemented in rescale");
    BIASABORT;
  }
  return 0;
}


template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
UpsampleBy2(const Image<InputImageType>& Source,
            Image<OutputImageType>& Destination)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::UpsampleBy2\n");

  register OutputImageType *dstu=NULL, *dstl=NULL;
  register const InputImageType *src=NULL, *srcend=NULL,  *srclend=NULL;
  register int srcwidth=Source.GetWidthStep(),
    dstwidth=Destination.GetWidthStep(),
    cc=Source.GetChannelCount();
  dstu = dstl = Destination.GetImageData();
  src = Source.GetImageData();
  dstl += dstwidth;
  srcend = src + Source.GetPixelCount() * cc - cc;
  srclend = src + srcwidth - cc;

  while (src < srcend){
    while (src < srclend){
      for (int i=0; i<cc; i++){
        *dstu = *dstl = dstu[cc] = dstl[cc] = *src;
        src++; dstu++; dstl++;
      }
      dstu+=cc;
      dstl+=cc;
    }
    dstu += dstwidth;
    dstl += dstwidth;
    srclend += srcwidth;
  }
  return 0;
}


//////////////////////////////////////////////////////////////////////////
//               protected functions
//////////////////////////////////////////////////////////////////////////

template <class InputImageType, class OutputImageType>
void Rescale<InputImageType, OutputImageType>::
UpdateLowpass(double factor)
{
  Gauss<InputImageType, OutputImageType>* gauss =
    dynamic_cast<Gauss<InputImageType, OutputImageType>*>(_lowpass);
  if (gauss)
  {
    gauss->SetSigma(_RescaleMeanSigmaFactor * factor);
  }

  Binomial<InputImageType, OutputImageType>* binomial =
    dynamic_cast<Binomial<InputImageType, OutputImageType>*>(_lowpass);
  if (binomial)
  {
    binomial->SetHalfWinSize((int)ceil(1.0*factor));
  }

  Mean<InputImageType, OutputImageType>* mean =
    dynamic_cast<Mean<InputImageType, OutputImageType>*>(_lowpass);
  if (mean)
  {
    mean->SetSize((int)ceil(1.0*factor));
  }
}

template <class InputImageType, class OutputImageType>
void Rescale<InputImageType, OutputImageType>::
SetLowPassFilter(const FilterNToN<InputImageType, OutputImageType>& lowpass)
{
  if (_lowpass)
    delete _lowpass;
  _lowpass = lowpass.Clone();
  UpdateLowpass(_RescaleFactor);
}

template <class InputImageType, class OutputImageType>
void Rescale<InputImageType, OutputImageType>::
SetLowPassType(int lpt)
{
  if (_lowpass)
    delete _lowpass;

  switch (lpt)
  {
  case LPT_Mean:
    _lowpass = new Mean<InputImageType,OutputImageType>;
    break;
  case LPT_Gauss:
    _lowpass = new Gauss<InputImageType,OutputImageType>;
    break;
  case LPT_Binomial:
    _lowpass = new Binomial<InputImageType,OutputImageType>;
    break;
  case LPT_GaussThreshold:
    _lowpass = new GaussThreshold<InputImageType,OutputImageType>;
    break;
  default:
      BIASERR("Invalid LowPassType: " << lpt);
      break;
  }

  UpdateLowpass(_RescaleFactor) ;
}

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
_ApplyMeanFilter(const Image<InputImageType>& src,
                 Image<OutputImageType>& dst, const double factor)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_ApplyMeanFilter\n");
  UpdateLowpass(factor);
  //_lowpass->AddDebugLevel(D_CONV_KERNEL);
//  ImageIO::Save("beforeDownsamplingSrc.mip",src);
//  cout << "---------------" << endl;
  int res=_lowpass->Filter(src, dst);
//  ImageIO::Save("beforeDownsamplingDst.mip",dst);
//  cout << "---------------" << endl;

  return res;
}


template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
_FillInterpolatedGrey(const Image<OutputImageType>& src,
                      Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedGrey\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  OutputImageType **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      idst[y][x]=(OutputImageType)
        src.FastBilinearInterpolationGrey(x*facx, y*facy);

      //      cerr << "( "<<x<<", "<<y<<")  ->  ( "<<x*facx<<", "<<y*facy<<")\n";
    }
  }
  return 0;
}

template <>
int Rescale<unsigned char, unsigned char>::
_FillInterpolatedGrey(const Image<unsigned char>& src,
                      Image<unsigned char>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedGrey\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  unsigned char **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      idst[y][x]=(unsigned char)
        rint(src.FastBilinearInterpolationGrey(x*facx, y*facy));
    }
  }
  return 0;
}

#ifdef BUILD_IMAGE_USHORT
template <>
int Rescale<unsigned short, unsigned short>::
_FillInterpolatedGrey(const Image<unsigned short>& src,
                      Image<unsigned short>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedGrey\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  unsigned short **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      idst[y][x]=(unsigned short)
        rint(src.FastBilinearInterpolationGrey(x*facx, y*facy));
    }
  }
  return 0;
}
#endif

template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
_FillInterpolatedColor(const Image<OutputImageType>& src,
                       Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedColor\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  OutputImageType **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  const unsigned int cc = src.GetChannelCount();
  unsigned int c;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      for (c=0;c<cc; c++)
      idst[y][x*cc+c]=(OutputImageType)
        src.BilinearInterpolationRGBInterleaved(x*facx, y*facy, c);
    }
  }
  return 0;
}

template <>
int Rescale<unsigned char, unsigned char>::
_FillInterpolatedColor(const Image<unsigned char>& src,
                       Image<unsigned char>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedColor\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  unsigned char **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  const unsigned int cc = src.GetChannelCount();
  unsigned int c;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      for (c=0;c<cc; c++)
        idst[y][x*cc+c]=(unsigned char)rint
          (src.BilinearInterpolationRGBInterleaved(x*facx, y*facy, c));
    }
  }
  return 0;
}

#ifdef BUILD_IMAGE_USHORT
// exactly the same as <unsigned char, int>, but partial specializations
// are not supported by many compilers
template <>
int Rescale<unsigned short,unsigned short>::
_FillInterpolatedColor(const Image<unsigned short>& src,
                       Image<unsigned short>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedColor\n");


  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  unsigned short **idst=dst.GetImageDataArray();
  const double facx = (double)src.GetWidth() / (double)dst.GetWidth();
  const double facy = (double)src.GetHeight() / (double)dst.GetHeight();
  register int x, y;
  const unsigned int cc = src.GetChannelCount();
  unsigned int c;
  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      for (c=0;c<cc; c++)
        idst[y][x*cc+c]=(unsigned char)rint
          (src.BilinearInterpolationRGBInterleaved(x*facx, y*facy, c));
    }
  }


  return 0;
}
#endif

template <class InputImageType, class OutputImageType>
void Rescale<InputImageType, OutputImageType>::
GetBordersValid_(int &border_x, int &border_y) const
{
  if (_RescaleFactor<=1.0){
    border_x = border_y = 0;
  } else {
    _lowpass->GetBorders(border_x, border_y);
  }
}


template <class InputImageType, class OutputImageType>
int Rescale<InputImageType, OutputImageType>::
_FillInterpolated(const Image<OutputImageType>& src,
                  const double &factor, Image<OutputImageType>& dst)
{
  BIASDOUT(D_FILTERBASE_CALLSTACK, "Rescale::_FillInterpolatedColor\n");

  int tlx, tly, brx, bry;
  dst.GetROI()->GetCorners(tlx, tly, brx, bry);
  const int minx=tlx, maxx=brx, miny=tly, maxy=bry;
  OutputImageType **idst=dst.GetImageDataArray();
  register int x, xc, y;
  const unsigned cc = src.GetChannelCount();
  register unsigned c;
  double src_x, src_y;
  for (y=miny; y<maxy; y++){
    src_y = (y+0.5)*factor-0.5;
    for (x=minx; x<maxx; x++){
      src_x = (x+0.5)*factor-0.5;
      xc = x*cc;
      for (c=0; c<cc; c++){
        idst[y][xc+c] = Cast<OutputImageType>
          (src.BilinearInterpolationRGBInterleaved(src_x, src_y, c));
      }
    }
  }
  _LastPositionOffset = 0.5*factor-0.5;
  return 0;
}


//////////////////////////////////////////////////////////////////////////
//                 instantiation
//////////////////////////////////////////////////////////////////////////
#define FILTER_INSTANTIATION_CLASS Rescale
#include "Filterinst.hh"

} // namespace BIAS
