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

#include "Binomial.hh"





using namespace BIAS;
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                 implementation
//////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
Binomial<InputStorageType, OutputStorageType>::
Binomial()
  : Convolution<InputStorageType, OutputStorageType>() 
{
  _HalfWinSize = 1;
  //this->AddDebugLevel(D_BINOMIAL_KERNEL);
  //this->AddDebugLevel(D_FILTERBASE_CALLSTACK);

  _CalculateKernel(_HalfWinSize);
}

template <class InputStorageType, class OutputStorageType>
Binomial<InputStorageType, OutputStorageType>::
Binomial(const Binomial<InputStorageType, OutputStorageType>& other)
  : _HalfWinSize(other._HalfWinSize),
    _LastHalfWinSize(other._LastHalfWinSize)
{
}

template <class StorageType, class KernelType>
Binomial<StorageType, KernelType>::~Binomial()
{
}

////////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter\n");

  int res=0;
  
  switch (_HalfWinSize){
  case 1:
    res = Filter3x3(src, dst);
    break;
  case 2:
    res = Filter5x5(src, dst);
    break;
  default:
    if ((_HalfWinSize != _LastHalfWinSize))
      _CalculateKernel(_HalfWinSize);
    res = Convolution<InputStorageType, OutputStorageType>::Filter(src,dst);
    break;
  }
  
  return res;
}

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
FilterInt(const Image<InputStorageType>& src, Image<OutputStorageType>& dst){
  BIASERR("Unsupported interface, use Filter");
  return Filter(src, dst);
}

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src, Image<OutputStorageType>& dst){
  BIASERR("Unsupported interface, use Filter");
  return Filter(src, dst);
}


template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter3x3(const Image<InputStorageType>& src, Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter3x3\n");
  int res=0;
  if (src.GetChannelCount()==1){
    if (!src.SamePixelAndChannelCount(dst)) 
      if (!dst.IsEmpty()) dst.Release();
    if (dst.IsEmpty()) dst.Init(src.GetWidth(), src.GetHeight(), 
                                src.GetChannelCount());
    if (this->CalculationInFloat()){
      switch (_FilterBorderHandling){
      case TBH_valid:
        res = Filter3x3ValidGreyFloat(src, dst);
        break;
      case TBH_same:
        res = Filter3x3SameGreyFloat(src, dst);
        break;
      default:
        BIASERR("unfinished");
        res = -1;
        break;
      }
    } else {
      _HalfWinSize=1;
      res = Convolution<InputStorageType, OutputStorageType>::Filter(src, dst); 
    }
  } else {
    _HalfWinSize=1;
    res = Convolution<InputStorageType, OutputStorageType>::Filter(src, dst); 
  }
  return res;
}
  
template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter5x5(const Image<InputStorageType>& src, Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter5x5\n");
  int res=0;
  if (src.GetChannelCount()==1){
    if (!src.SamePixelAndChannelCount(dst)) 
      if (!dst.IsEmpty()) dst.Release();
    if (dst.IsEmpty()) dst.Init(src.GetWidth(), src.GetHeight(), 
                                src.GetChannelCount());
    if (this->CalculationInFloat()){
      switch (_FilterBorderHandling){
      case TBH_valid:
        res = Filter5x5ValidGreyFloat(src, dst);
        break;
      default:
        BIASERR("Binomial::Filter5x5: border handling unfinished, "
                <<"using valid");
        res = Filter5x5ValidGreyFloat(src, dst);
        break;
      }
    } else {
      BIASASSERT("unfinished");
      _HalfWinSize=2;
      res = Convolution<InputStorageType, OutputStorageType>::Filter(src,dst);
    }
  } else {
    _HalfWinSize=2;
    res = Convolution<InputStorageType, OutputStorageType>::Filter(src, dst); 
  }
  return res;
}

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter3x3ValidGreyFloat(const Image<InputStorageType>& src, 
                        Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter3x3ValidGreyFloat\n");

  //  AddDebugLevel(D_BINOMIAL_WRITE_IM);
  const unsigned width=src.GetWidth();
  int nwidth = - int(width);
  const unsigned height=src.GetHeight();

  unsigned minx, miny, maxx, maxy;
  src.GetROI()->GetCorners(minx, miny, maxx, maxy);
  dst.GetROI()->SetCorners(minx+1, miny+1, maxx-1, maxy-1);

  if (width>_tmpFloat.GetWidth() || 
      height>_tmpFloat.GetHeight()){
    if (!_tmpFloat.IsEmpty()) _tmpFloat.Release();
    _tmpFloat.Init(width, height, src.GetChannelCount());
  }

  const InputStorageType *im=src.GetImageData();
  const InputStorageType *p, *end, *lend;
  OutputStorageType *pg, *oend, *olend;
  CONV_FLOAT *ph, tmp;
  int offset, step;

  // separated binomial 3x3 mask = 1 2 1
  offset=(int)minx+(int)miny*(int)width+1;
  p=im+offset;
  ph=_tmpFloat.GetImageData()+offset;
  step=width-(int)maxx+(int)minx+2;
  end=im+(int)maxx+((int)maxy-1)*(int)width-1;
  lend=im+(int)maxx+((int)miny*(int)width)-1;
  // vertical part
  while (p<end){
    while (p<lend){
      *ph++ = (CONV_FLOAT)(p[-1]) +(CONV_FLOAT)(*p) * 2.0f + (CONV_FLOAT)(p[1]);
      p++;
    }
    ph+=step;
    p+=step;
    lend+=width;
  }



  offset=(int)minx+((int)miny+1)*(int)width+1;
  ph=_tmpFloat.GetImageData()+offset;
  OutputStorageType *s=dst.GetImageData();
  pg=s+offset;
  oend=s+(int)maxx+((int)maxy-2)*(int)width-1;
  olend=s+(int)maxx+((int)miny+1)*(int)width-1;
  CONV_FLOAT div = 1.0 / 16.0;
  // horizontal part
  while (pg<oend){
    while (pg<olend){
      tmp = ph[nwidth] + *ph * 2.0f + ph[width];
      *pg++ = (OutputStorageType)(tmp * div);
      ph++;
    }
    pg+=step;
    ph+=step;
    olend+=width;
  }


  return 0;
}

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter3x3SameGreyFloat(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter3x3SameGreyFloat\n");
  int res =0;

  res = Filter3x3ValidGreyFloat(src, dst);
  
  // filter mask is 
  // | 1 2 1 |
  // | 2 4 2 |
  // | 1 2 1 |
  int minx, miny, maxx, maxy;
  src.GetROI()->GetCorners(minx, miny, maxx, maxy);
  register int x, y, xp, xm, yp, ym;

  const InputStorageType **srcida=src.GetImageDataArray();
  OutputStorageType **dstida=dst.GetImageDataArray();
  
  // upper left pixel
  x = minx;
  y = miny;
  xp = x+1;
  yp = y+1;
  dstida[y][x] = 
    OutputStorageType((4.0*float(srcida[y][x])+
                       2.0*(float(srcida[y][xp])+float(srcida[yp][x]))+
                       float(srcida[yp][xp]))/9.0);
  
  // upper right pixel
  x = maxx-1;
  //y = miny;
  xm = x-1;
  //yp = y+1;
  dstida[y][x] = 
    OutputStorageType((4.0*float(srcida[y][x])+
                       2.0*(float(srcida[y][xm])+float(srcida[yp][x]))+
                       float(srcida[yp][xm]))/9.0);

  // lower right pixel
  //x = maxx-1;
  y = maxy-1;
  //xm = x-1;
  ym = y-1;
  dstida[y][x] = 
    OutputStorageType((4.0*float(srcida[y][x])+
                       2.0*(float(srcida[y][xm])+float(srcida[ym][x]))+
                       float(srcida[ym][xm]))/9.0);

  // lower left pixel
  x = minx;
  //y = maxy-1;
  xp = x+1;
  //ym = y-1;
  dstida[y][x] = 
    OutputStorageType((4.0*float(srcida[y][x])+
                       2.0*(float(srcida[y][xp])+float(srcida[ym][x]))+
                       float(srcida[ym][xp]))/9.0);
   
 

  // upper row
  y = miny;
  yp = y+1;
  for (x=minx+1; x<maxx-1; x++){
    xp = x+1;
    xm = x-1;
    dstida[y][x] =
      OutputStorageType((4.0*float(srcida[y][x])+
                         2.0*(float(srcida[y][xm])+float(srcida[y][xp])+
                              float(srcida[yp][x]))+
                         float(srcida[yp][xp])+float(srcida[yp][xm]))/12.0);
  }
  // lower row
  y = maxy-1;
  ym = y-1;
  for (x=minx+1; x<maxx-1; x++){
    xp = x+1;
    xm = x-1;
    dstida[y][x] =
      OutputStorageType((4.0*float(srcida[y][x])+
                         2.0*(float(srcida[y][xm])+float(srcida[y][xp])+
                              float(srcida[ym][x]))+
                         float(srcida[ym][xp])+float(srcida[ym][xm]))/12.0);
  }
  // left column
  x = minx;
  xp = x+1;
  for (y=miny+1; y<maxy-1; y++){
    yp = y+1;
    ym = y-1;
    dstida[y][x] =
      OutputStorageType((4.0*float(srcida[y][x])+
                         2.0*(float(srcida[ym][x])+float(srcida[yp][x])+
                              float(srcida[y][xp]))+
                         float(srcida[ym][xp])+float(srcida[yp][xp]))/12.0);
  }

  // right column
  x = maxx-1;
  xm = x-1;
  for (y=miny+1; y<maxy-1; y++){
    yp = y+1;
    ym = y-1;
    dstida[y][x] =
      OutputStorageType((4.0*float(srcida[y][x])+
                         2.0*(float(srcida[ym][x])+float(srcida[yp][x])+
                              float(srcida[y][xm]))+
                         float(srcida[ym][xm])+float(srcida[yp][xm]))/12.0);
  }

  dst.GetROI()->SetCorners(minx, miny, maxx, maxy);

  return res;
}

template <class InputStorageType, class OutputStorageType>
int Binomial<InputStorageType, OutputStorageType>::
Filter5x5ValidGreyFloat(const Image<InputStorageType>& src, 
                        Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Binomial::Filter5x5ValidGrey\n");

  //AddDebugLevel(D_BINOMIAL_WRITE_IM);
  const unsigned width=src.GetWidth();
  int nwidth = - int(width);
  const unsigned height=src.GetHeight();

  unsigned minx, miny, maxx, maxy;
  src.GetROI()->GetCorners(minx, miny, maxx, maxy);
  dst.GetROI()->SetCorners(minx+2, miny+2, maxx-2, maxy-2);

  // init internal memory if necessairy
  if (width>_tmpFloat.GetWidth() || 
      height>_tmpFloat.GetHeight()){
    if (!_tmpFloat.IsEmpty()) _tmpFloat.Release();
    _tmpFloat.Init(width, height, src.GetChannelCount());
  }

  const InputStorageType *im=src.GetImageData();
  const InputStorageType *p, *end, *lend;
  OutputStorageType *pg, *oend, *olend;
  CONV_FLOAT *ph, tmp;
  int offset, step;
  // separated binomial 5x5 mask = 1 4 6 4 1
  offset=(int)minx+(int)miny*(int)width+2;
  p=im+offset;
  ph=_tmpFloat.GetImageData()+offset;
  step=width-(int)maxx+(int)minx+4;
  end=im+(int)maxx+((int)maxy-1)*(int)width-2;
  lend=im+(int)maxx+((int)miny*(int)width)-2;
  // vertical part
  while (p<end){
    while (p<lend){
      *ph++ = (CONV_FLOAT)(p[-2]) + (CONV_FLOAT)(p[-1])*4.0f + (CONV_FLOAT)(*p) 
        * 6.0f + (CONV_FLOAT)(p[+1])*4.0f + (CONV_FLOAT)(p[2]);
      p++;
    }
    ph+=step;
    p+=step;
    lend+=width;
  }



  offset=(int)minx+((int)miny+2)*(int)width+2;
  ph=_tmpFloat.GetImageData()+offset;
  OutputStorageType *s=dst.GetImageData();
  pg=s+offset;
  oend=s+(int)maxx+((int)maxy-3)*(int)width-2;
  olend=s+(int)maxx+((int)miny+2)*(int)width-2;
  int dw=width<<1;
  CONV_FLOAT div = 1.0 / 256.0;
  // horizontal part
  while (pg<oend){
    while (pg<olend){
      tmp = ph[-dw] + ph[dw] + ph[nwidth]*4.0f + ph[width]*4.0f + (*ph)*6.0f;
      *pg++ = (OutputStorageType)(tmp * div);
      ph++;
    }
    pg+=step;
    ph+=step;
    olend+=width;
  }



  return 0;
}

////////////////////////////////////////////////////////////////////////////

template <class StorageType, class KernelType>
void Binomial<StorageType, KernelType>::
_CalculateKernel(int HalfWinSize)
{
  Vector<FM_INT> vec;
  int shift;
  const int size=(HalfWinSize<<1)+1;
  vec.newsize(size);
  vec[0]=vec[size-1]=1;

  int numerator=size-1, denominator=1;
  for (int i=1; i<=HalfWinSize; i++){
    vec[i]=vec[size-i-1]=(numerator/denominator);
    BIASCDOUT(D_BINOMIAL_KERNEL, "num "<<numerator<<" denom "<<denominator
              <<"  =  "<<vec[i]<<endl);
    denominator*=(i+1);
    numerator*=(size-i-1);
  }
  shift=(HalfWinSize<<1);

  BIASCDOUT(D_BINOMIAL_KERNEL, "size "<<size<<" : "<< vec << "  shift "
            <<shift<<endl<< "-------------\n");
  
  _fm.Init(vec, vec, 0, shift*shift, true);
  BIASCDOUT(D_BINOMIAL_KERNEL, "filter mask (int): "<<_fm<<endl);
  _fm.CreateFloatFilter();
  BIASCDOUT(D_BINOMIAL_KERNEL, "filter mask (float): "<<_fm<<endl);
  _LastHalfWinSize = HalfWinSize;
}

template <class StorageType, class KernelType>
void Binomial<StorageType, KernelType>::
GetBordersValid_(int &border_x, int &border_y) const
{
  border_x = border_y = _HalfWinSize;
}

//////////////////////////////////////////////////////////////////////////
//                 instantiation
//////////////////////////////////////////////////////////////////////////

#define FILTER_INSTANTIATION_CLASS Binomial
#include "Filterinst.hh"
