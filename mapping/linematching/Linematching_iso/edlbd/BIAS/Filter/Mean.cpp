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


#include "Mean.hh"



using namespace BIAS;
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                 implementation
//////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
Mean<InputStorageType, OutputStorageType>::
Mean()
  : Convolution<InputStorageType, OutputStorageType>()
{
  _MeanSize = 2;
}

template <class InputStorageType, class OutputStorageType>
Mean<InputStorageType, OutputStorageType>::
Mean(const Mean<InputStorageType, OutputStorageType>& other)
  : _MeanSize(other._MeanSize)
{
}

template <class InputStorageType, class OutputStorageType>
Mean<InputStorageType, OutputStorageType>::
~Mean()
{
}


template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
 BIASCDOUT(D_FILTERBASE_CALLSTACK, "Mean::Filter\n");
  int res=0;
  switch (_MeanSize){
  case 1:
    dst=src;
    break;
  case 2:
    // \bug mean filter moves image content to left top corner by at least one pixel!
    res=FilterMean2x2(src, dst);
    break;
  default:
    _SetKernel();
    res=Convolution<InputStorageType, OutputStorageType>::Filter(src, dst);
    break;
  }
  return res;
}

template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
FilterInt(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASERR("Unimplemented interface, calling generic Filter");
  return Filter(src, dst);
}

template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASERR("Unimplemented interface, calling generic Filter");
  return Filter(src, dst);
}

template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
FilterMean(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Mean::FilterMean\n");
  BIASERR("deprecated");
  return -10;
}

////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
FilterMean2x2Grey(const Image<InputStorageType>& src,
                  Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Mean::FilterMean2x2Grey\n");

  unsigned minx, miny, maxx, maxy;
  src.GetROI()->GetCorners(minx, miny, maxx, maxy);

  //why?
  maxx--;
  maxy--;

  dst.GetROI()->SetCorners(minx, miny, maxx, maxy);

  register double sum, lsum;
  register const InputStorageType *srcu, *srcl;
  register OutputStorageType *pdst, *dstend, *dstlend;
  register const int width=src.GetWidth();
  register const int step=width-(maxx-minx);

  //first line start + offset from roi minx (row nr.) miny*width (col nr.),
  srcl = srcu = src.GetImageData() + minx+(miny*width);
  //second starting row
  srcl += width;
  //first row pointer in destination
  pdst = dst.GetImageData() +  minx+(miny*width);
  //end of dest line
  dstlend = pdst + width-step;
  //end of dest
  dstend = dst.GetImageData() +  maxx+(maxy-1)*width;

  while (pdst < dstend){ //goes over the image
    lsum = (double)*srcu++ + (double)*srcl++; //first two elements
    while (pdst < dstlend){
      sum = (double)*srcu++ + (double)*srcl++; //second two elements
      // BUG !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      //*pdst++ = (OutputStorageType)((sum + lsum)/ 4.0);
      *pdst++ = (OutputStorageType)((sum + lsum)/ 2.0); // tried to correct
      lsum = sum =0.0;
    }
    srcu += step;
    srcl += step;
    pdst += step+1;
    dstlend += width;
  }

  return 0;
}


////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int Mean<InputStorageType, OutputStorageType>::
FilterMean2x2(const Image<InputStorageType>& src,
              Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Mean::FilterMean2x2\n");
  int res=0;
  if (src.GetChannelCount() == 1 && src.GetColorModel()==ImageBase::CM_Grey){
    if (! src.SamePixelAndChannelCount(dst)) {
      if (!dst.IsEmpty()) dst.Release();
      dst.Init(src.GetWidth(), src.GetHeight(), src.GetChannelCount());
    }
    // \bug moves content to left top corner
    res = FilterMean2x2Grey(src, dst);
  } else {
    _SetKernel();
    res = Convolution<InputStorageType, OutputStorageType>::Filter(src, dst);
  }
  return res;
}

//////////////////////////////////////////////////////////////////////////
//               protected functions
//////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
void Mean<InputStorageType, OutputStorageType>::_SetKernel() {
  const FM_FLOAT val = 1.0f/(FM_FLOAT)(_MeanSize);
  Vector<FM_FLOAT> H(_MeanSize), V(_MeanSize);
  for (int i=0; i< _MeanSize; i++){
    H[i] = V[i] = val;
  }
  _fm.Init(H,V);
  // compute the int approximation
  int thebits = _fm.ComputeIntPrecisionBits(sizeof(InputStorageType)*8,
                                            sizeof(CONV_INT)*8-1);
  _fm.CreateIntFilter(thebits,thebits,thebits);
}

template <class InputStorageType, class OutputStorageType>
void Mean<InputStorageType, OutputStorageType>::
GetBordersValid_(int &border_x, int &border_y) const
{
  border_x = border_y = _MeanSize/2;
}

//////////////////////////////////////////////////////////////////////////
//                 instantiation
//////////////////////////////////////////////////////////////////////////
#define FILTER_INSTANTIATION_CLASS Mean
#include "Filterinst.hh"
