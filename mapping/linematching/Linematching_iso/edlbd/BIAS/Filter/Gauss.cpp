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

#include "Gauss.hh"
#include <Base/Image/ImageConvert.hh>

using namespace BIAS;
using namespace std;

//////////////////////////////////////////////////////////////////////////
//                 implementation
//////////////////////////////////////////////////////////////////////////


template <class InputStorageType, class OutputStorageType>
Gauss<InputStorageType, OutputStorageType>::
Gauss()
  : Convolution<InputStorageType, OutputStorageType>() 
{
  _GaussSigma = 0.7;
  _GaussRatio = 0.01;
  _LastRatio = _LastSigma = -1.0;
  _GaussIO = NULL;
  _GaussOO = NULL;
}

template <class InputStorageType, class OutputStorageType>
Gauss<InputStorageType, OutputStorageType>::
Gauss(const Gauss<InputStorageType, OutputStorageType>& other)
  : _GaussSigma(other._GaussSigma),
    _GaussRatio(other._GaussRatio),
    _LastSigma(other._LastSigma),
    _LastRatio(other._LastRatio)
{
  if (other._GaussIO!=NULL) _GaussIO = other._GaussIO->Clone();
  else _GaussIO = NULL;
  if (other._GaussOO!=NULL) _GaussOO = other._GaussOO->Clone(); 
  else _GaussOO = NULL;
}

template <class StorageType, class KernelType>
Gauss<StorageType, KernelType>::~Gauss()
{
  if (_GaussIO!=NULL) delete _GaussIO;
  if (_GaussOO!=NULL) delete _GaussOO;
}


void DecreaseROI(const ROI& RoiIn, ROI& RoiOut,
                 int hws) {
  unsigned minx, miny, maxx, maxy;
  RoiIn.GetCorners(minx, miny, maxx, maxy);
 
  minx += hws;
  miny += hws;
  maxx -= hws;
  maxy -= hws;
  if (minx>maxx) minx = maxx = (minx+maxx)/2;
  if (miny>maxy) miny = maxy = (miny+maxy)/2;
  RoiOut.SetCorners(minx, miny, maxx, maxy);
}


////////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter\n");
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  if (_fm.IsSize1x1()){ // do not filter if filter mask is 1x1
    return ImageConvert::ConvertST(src, dst, dst.GetStorageType());
  } 
  if ((src.GetChannelCount()==1) && (this->GetBorderHandling() == TBH_valid))
    return FilterGrey(src, dst);
  return Convolution<InputStorageType, OutputStorageType>::Filter(src,dst);
}

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter\n");
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  return Convolution<InputStorageType,OutputStorageType>::FilterFloat(src,dst);
}

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
FilterInt(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter\n");
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  return Convolution<InputStorageType, OutputStorageType>::FilterInt(src,dst);
}


template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
FilterGrey(const Image<InputStorageType>& src, 
           Image<OutputStorageType>& dst,
           bool ResetROI)
{
  if (_GaussIO==NULL)
    _GaussIO = new Gauss<InputStorageType, OutputStorageType>;
  if (_GaussOO==NULL)
    _GaussOO = new Gauss<OutputStorageType, OutputStorageType>;

  // this is the final target sigma:
  double smoothsigma = _GaussSigma;

  // compute maximum sigma for the different kernel sizes
  // approximate rule: 
  // if you want to neglect filter mask entries below 10% of the maximum,
  // your gauss'es half window size should be of size 5*sigma
  const double sqrtlogratio  = 1.0 / sqrt(-2.0 * log(_GaussRatio));
  const double maxsigma3x3   = sqrtlogratio;
  const double maxsigma5x5   = 2.0 * sqrtlogratio;
  const double maxsigma7x7   = 3.0 * sqrtlogratio;
  const double maxsigma9x9   = 4.0 * sqrtlogratio;
  const double maxsigma11x11 = 5.0 * sqrtlogratio;
  const double maxsigma13x13 = 6.0 * sqrtlogratio;
  const double maxsigmaperpass = maxsigma13x13;

  // compute how many passes we would need with the largest filter
  int MultiPass = (int)rint(smoothsigma * smoothsigma / 
                            (maxsigmaperpass*maxsigmaperpass));
  if (MultiPass>1) {
    // we need multiple passes
    smoothsigma /= sqrtf(float(MultiPass));
    BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass="<<MultiPass);
  } else {
    BIASDOUT(D_GAUSS_FAST_GREY,"Single pass is sufficient.");
  }
  _GaussIO->SetSigma(smoothsigma);
  _GaussIO->SetRatio(_GaussRatio);
  _GaussOO->SetSigma(smoothsigma);
  _GaussOO->SetRatio(_GaussRatio);
  if (_GaussSigma<maxsigma3x3) {
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 3x3");
    _GaussIO->Filter3x3Grey(src, dst);
  } else if (_GaussSigma<maxsigma5x5) {
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 5x5");
    _GaussIO->Filter5x5Grey(src, dst);
  } else if (_GaussSigma<maxsigma7x7) {
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 7x7");
    _GaussIO->Filter7x7Grey(src,dst);
  } else if (_GaussSigma<maxsigma9x9){
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 9x9");
    _GaussIO->Filter9x9Grey(src, dst);
  } else if (_GaussSigma<maxsigma11x11){
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 11x11");
    _GaussIO->Filter11x11Grey(src, dst);
  } else {
    BIASDOUT(D_GAUSS_FAST_GREY,"Run 13x13");
    _GaussIO->Filter13x13Grey(src, dst);
  }
	while (MultiPass-->1) {
	  if (ResetROI) dst.UnsetROI();
    if (_GaussSigma<maxsigma3x3) {
      BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 3x3");
      _GaussOO->Filter3x3Grey(dst, dst);
    } else if (_GaussSigma<maxsigma5x5) {
      BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 5x5");
      _GaussOO->Filter5x5Grey(dst, dst);
    } else if (_GaussSigma<maxsigma7x7) {
      BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 7x7");
      _GaussOO->Filter7x7Grey(dst, dst);
    } else if (_GaussSigma<maxsigma9x9) {
      BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 9x9");
      _GaussOO->Filter9x9Grey(dst, dst);
    } else if (_GaussSigma<maxsigma11x11){
	    BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 11x11");
      _GaussOO->Filter11x11Grey(dst, dst);
    } else {
	    BIASDOUT(D_GAUSS_FAST_GREY,"MultiPass: Run 13x13");
      _GaussOO->Filter13x13Grey(dst, dst);
    }
	}
  if (ResetROI) dst.UnsetROI();
  return 0;
}

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter3x3GreyThreshold(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst, 
                       float threshold) {
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter3x3\n");
  SetHalfWinSize(1, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);

  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first byte
  *pD++ = CONV_FLOAT(*pS);
  const int width = src.GetWidth(), height = src.GetHeight();
  const InputStorageType* pEndH = src.GetImageData() + width*height-1;
  while (++pS<pEndH) {
    if(pS[0] <= threshold || 
       pS[-1] <= threshold || 
       pS[1] <= threshold) 
      *pD++ = CONV_FLOAT(pS[0]);
    else
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1]));
  }
  // copy last byte
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-1);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst2 =  _tmpFloat.GetImageData() + width;
  const CONV_FLOAT* pEndLast2 =  _tmpFloat.GetImageData() + width*height;
  // copy first two lines
  while (pI<pEndFirst2) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
    if(pI[0] <= threshold || 
       pI[width] <= threshold || 
       pI[-width] <= threshold) {
      *pO++ = OutputStorageType(pI[0]);
    } else
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width]));  
  }
  // copy last line
  while (pI<pEndLast2) *pO++ = OutputStorageType(*pI++);

  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 1);

  return 0;
} 


template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter3x3Grey(const Image<InputStorageType>& src, 
              Image<OutputStorageType>& dst) {
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter3x3\n");
  SetHalfWinSize(1, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);
  

  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first byte
  *pD++ = CONV_FLOAT(*pS);
  const int width = src.GetWidth(), height = src.GetHeight();
  const InputStorageType* pEndH = src.GetImageData() + width*height-1;
  while (++pS<pEndH) {
    *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1]));
  }
  // copy last byte
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-1);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst2 =  _tmpFloat.GetImageData() + width;
  const CONV_FLOAT* pEndLast2 =  _tmpFloat.GetImageData() + width*height;
  // copy first two lines
  while (pI<pEndFirst2) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
    *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width]));     			
  }
  // copy last line
  while (pI<pEndLast2) *pO++ = OutputStorageType(*pI++);

  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 1);
  

  return 0;
} 

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter5x5GreyThreshold(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst,
                       float threshold) {
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter5x5\n");
  SetHalfWinSize(2, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);
  


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first two bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);
  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-2;
  while (++pS<pEndH) {
    if(pS[0] <= threshold || 
       pS[-1] <= threshold || 
       pS[1] <= threshold ||
       pS[-2] <= threshold || 
       pS[2] <= threshold )       
      *pD++ = CONV_FLOAT(pS[0]);
    else
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])+h2*(pS[-2]+pS[2]));
  }
  // copy last two bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-2);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst2 =  _tmpFloat.GetImageData() + 2*width;
  const CONV_FLOAT* pEndLast2 =  _tmpFloat.GetImageData() + width*height;
  // copy first two lines
  while (pI<pEndFirst2) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
    if(pI[0] <= threshold || 
       pI[width] <= threshold || 
       pI[-width] <= threshold ||
       pI[width2] <= threshold || 
       pI[-width2] <= threshold ) 
      *pO++ = OutputStorageType(pI[0]);
    else
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
                                + v2*(pI[-width2]+pI[width2]));
			
  }
  // copy last two lines
  while (pI<pEndLast2) *pO++ = OutputStorageType(*pI++);

  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 2);

  return 0;
} 

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter5x5Grey(const Image<InputStorageType>& src, 
              Image<OutputStorageType>& dst) {
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter5x5\n");
  SetHalfWinSize(2, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);
  


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first two bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);
  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-2;
  while (++pS<pEndH) {
    *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])+h2*(pS[-2]+pS[2]));
  }
  // copy last two bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-2);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst2 =  _tmpFloat.GetImageData() + 2*width;
  const CONV_FLOAT* pEndLast2 =  _tmpFloat.GetImageData() + width*height;
  // copy first two lines
  while (pI<pEndFirst2) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
    *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
                              + v2*(pI[-width2]+pI[width2]));
			
  }
  // copy last two lines
  while (pI<pEndLast2) *pO++ = OutputStorageType(*pI++);

  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 2);

  return 0;
} 

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter7x7Grey(const Image<InputStorageType>& src, 
              Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter7x7\n");
  SetHalfWinSize(3, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[3]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h3 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[3]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v3 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first three bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);
  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width, width3=3*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-3;
  while (++pS<pEndH) {
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])
			 +h2*(pS[-2]+pS[2])+h3*(pS[-3]+pS[3]));
  }
  // copy last three bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-3);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst3 =  _tmpFloat.GetImageData() + 3*width;
  const CONV_FLOAT* pEndLast3 =  _tmpFloat.GetImageData() + width*height;
  // copy first three lines
  while (pI<pEndFirst3) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
				+ v2*(pI[-width2]+pI[width2])
				+ v3*(pI[-width3]+pI[width3]));
  }
  // copy last three lines
  while (pI<pEndLast3) *pO++ = OutputStorageType(*pI++);
  
  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 3);

  return 0;
} 


template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter9x9Grey(const Image<InputStorageType>& src, 
	      Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter9x9\n");
  SetHalfWinSize(4, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[4]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[3]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h3 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h4 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[4]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[3]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v3 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v4 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }


  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first four bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);

  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width, width3=3*width, width4=4*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-4;
  while (++pS<pEndH) {
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])+h2*(pS[-2]+pS[2])
			 +h3*(pS[-3]+pS[3])+h4*(pS[-4]+pS[4]));
  }
  // copy last four bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-4);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst4 =  _tmpFloat.GetImageData() + 4*width;
  const CONV_FLOAT* pEndLast4 =  _tmpFloat.GetImageData() + width*height;
  // copy first four lines
  while (pI<pEndFirst4) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
				+ v2*(pI[-width2]+pI[width2])
				+ v3*(pI[-width3]+pI[width3])
				+ v4*(pI[-width4]+pI[width4]));
  }
  // copy last four lines
  while (pI<pEndLast4) *pO++ = OutputStorageType(*pI++);

  // update roi 
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 4);

  return 0;
}

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter11x11Grey(const Image<InputStorageType>& src, 
	      Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter11x11\n");
  SetHalfWinSize(5, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[5]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[4]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[3]);
  const FM_FLOAT h3 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h4 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h5 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);

  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[5]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[4]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[3]);
  const FM_FLOAT v3 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v4 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v5 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first five bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);

  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width, width3=3*width, width4=4*width, width5=5*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-5;
  while (++pS<pEndH) {
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])+h2*(pS[-2]+pS[2])
			 +h3*(pS[-3]+pS[3])+h4*(pS[-4]+pS[4])+h5*(pS[-5]+pS[5]));
  }
  // copy last five bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-5);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst5 =  _tmpFloat.GetImageData() + 5*width;
  const CONV_FLOAT* pEndLast5 =  _tmpFloat.GetImageData() + width*height;
  // copy first four lines
  while (pI<pEndFirst5) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
				+ v2*(pI[-width2]+pI[width2])
				+ v3*(pI[-width3]+pI[width3])
				+ v4*(pI[-width4]+pI[width4])
				+ v5*(pI[-width5]+pI[width5]));
  }
  // copy last four lines
  while (pI<pEndLast5) *pO++ = OutputStorageType(*pI++);
  
  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 5);

  return 0;
}

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter13x13Grey(const Image<InputStorageType>& src, 
		Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter13x13\n");
  SetHalfWinSize(6, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[6]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[5]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[4]);
  const FM_FLOAT h3 = (const FM_FLOAT)((*_fm.GetSepfh())[3]);
  const FM_FLOAT h4 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h5 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h6 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);

  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[6]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[5]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[4]);
  const FM_FLOAT v3 = (const FM_FLOAT)((*_fm.GetSepfv())[3]);
  const FM_FLOAT v4 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v5 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v6 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first five bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS);

  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width, width3=3*width, width4=4*width, 
    width5=5*width, width6=6*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-6;
  while (++pS<pEndH) {
      *pD++ = CONV_FLOAT(h0*pS[0]+h1*(pS[-1]+pS[1])+h2*(pS[-2]+pS[2])
			 +h3*(pS[-3]+pS[3])+h4*(pS[-4]+pS[4])
			 +h5*(pS[-5]+pS[5])+h6*(pS[-6]+pS[6]));
  }
  // copy last five bytes
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++);
  *pD++ = CONV_FLOAT(*pS++); 
  *pD++ = CONV_FLOAT(*pS++);
  *pD   = CONV_FLOAT(*pS);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-6);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst6 =  _tmpFloat.GetImageData() + 6*width;
  const CONV_FLOAT* pEndLast6 =  _tmpFloat.GetImageData() + width*height;
  // copy first four lines
  while (pI<pEndFirst6) *pO++ = OutputStorageType(*pI++);
  // convolve main
  pI--;
  while (++pI<pEndV) {
      *pO++ = OutputStorageType(v0*pI[0] + v1*(pI[-width]+pI[width])
				+ v2*(pI[-width2]+pI[width2])
				+ v3*(pI[-width3]+pI[width3])
				+ v4*(pI[-width4]+pI[width4])
				+ v5*(pI[-width5]+pI[width5])
				+ v6*(pI[-width6]+pI[width6]));
  }
  // copy last four lines
  while (pI<pEndLast6) *pO++ = OutputStorageType(*pI++);
  
  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 6);

  return 0;
}

////////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
void Gauss<InputStorageType, OutputStorageType>::
_CalculateKernels(double Sigma, double Ratio)
{
  const int hws = (int)(floor(sqrt(-2.0*Sigma*Sigma*log(Ratio))));
  const int size = (hws<<1)+1;
  
  Vector<FM_FLOAT> H(size), V(size);
  BIASCDOUT(D_CONV_KERNEL, "Sigma = "<<Sigma<<"\tRatio = "<<Ratio<<endl);
  BIASCDOUT(D_CONV_KERNEL, "size = "<<size<<"\thws = "<<hws<<endl);

  double sum=0.0;
  int i, ip, im;
  if (Sigma==0.0){
    H[0]=1.0; V[0]=1.0;
  } else { 
    double fac=1.0/(2.0*Sigma*Sigma);
    register double g;
    // calculate the filter coefficients as gauss function
    for (i=0; i<=hws; i++){
      ip = hws+i;
      im = hws-i;
      g  = expf((float)(-(double)(i*i)*fac));
      V[ip] = V[im] = H[im] = H[ip] = (FM_FLOAT)g;
      //BIASCDOUT(D_CONV_KERNEL, i<<"v:"<<this->_VVec[im]<<"  "<<endl);
      sum += g*2.0;
    }
    sum -= V[hws]; // added twice
    BIASCDOUT(D_CONV_KERNEL, "sum = "<<sum<<endl);

    // normalize filter, so that the sum is 1.0
    sum = 1.0/sum;
    BIASCDOUT(D_CONV_KERNEL, "sum = "<<sum<<endl);
    for (i=0; i<=hws; i++){
      ip=hws+i;
      im=hws-i;
      H[ip] = H[im] = V[ip] = V[im] = V[im] * (FM_FLOAT)sum;
      BIASCDOUT(D_CONV_KERNEL, H[im]<<"  "<<endl);
    }
  }
  _fm.Init(H,V);
  // compute the int approximation
  int thebits = _fm.ComputeIntPrecisionBits(sizeof(InputStorageType)*8,
                                            sizeof(CONV_INT)*8-1);
  _fm.CreateIntFilter(thebits,thebits,thebits);

  _LastSigma = Sigma;
  _LastRatio = Ratio;


  //#define COMPARE
#ifdef COMPARE
  fprintf(stdout, "gauss: \n");
  for (i=-hws; i<=hws; i++){
    fprintf(stdout, "%f ",HVec[i+hws]); 
  }
  fprintf(stdout, "\n");
#endif
}

#define MIN_NORM 0.7

template <class InputStorageType, class OutputStorageType>
int Gauss<InputStorageType, OutputStorageType>::
Filter7x7GreyIgnoreBelowThreshold(const Image<InputStorageType>& src, 
                                  Image<OutputStorageType>& dst,
                                  const InputStorageType& thresh) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter7x7\n");
  SetHalfWinSize(3, false);
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  const FM_FLOAT h0 = (const FM_FLOAT)((*_fm.GetSepfh())[3]);
  const FM_FLOAT h1 = (const FM_FLOAT)((*_fm.GetSepfh())[2]);
  const FM_FLOAT h2 = (const FM_FLOAT)((*_fm.GetSepfh())[1]);
  const FM_FLOAT h3 = (const FM_FLOAT)((*_fm.GetSepfh())[0]);
  const FM_FLOAT v0 = (const FM_FLOAT)((*_fm.GetSepfv())[3]);
  const FM_FLOAT v1 = (const FM_FLOAT)((*_fm.GetSepfv())[2]);
  const FM_FLOAT v2 = (const FM_FLOAT)((*_fm.GetSepfv())[1]);
  const FM_FLOAT v3 = (const FM_FLOAT)((*_fm.GetSepfv())[0]);


  //  cout<<"Kernel is "<<h0<<" "<<h1<<" "<<h2<<" "<<h3<<" "<<v0<<" "<<v1<<" "<<v2<<" "<<v3<<" "<<endl;


  if(!dst.SamePixelAndChannelCount(src)) {
    if (!dst.IsEmpty()) dst.Release();
    dst.Init(src.GetWidth(), src.GetHeight(), 1);
  }
  if(!_tmpFloat.SamePixelAndChannelCount(src)) {
    if (!_tmpFloat.IsEmpty())  _tmpFloat.Release();
    _tmpFloat.Init(src.GetWidth(),src.GetHeight(),1,
                   ImageBase::ST_float, src.IsInterleaved());
  }

  // horizontal convolution ignoring wrap-arounds
  const InputStorageType* pS  = src.GetImageData();
  CONV_FLOAT* pD = _tmpFloat.GetImageData();
  // copy first three bytes
  *pD++ = CONV_FLOAT(thresh);
  pS++;
  *pD++ = CONV_FLOAT(thresh);
  pS++;
  *pD++ = CONV_FLOAT(thresh);
  
  const int width = src.GetWidth(), height = src.GetHeight();
  const int width2 = 2*width, width3=3*width;
  const InputStorageType* pEndH = src.GetImageData() + width*height-3;
  
  while (++pS<pEndH) {
    register CONV_FLOAT normalization = 0; 
    register CONV_FLOAT accumulator = 0;
    if (pS[-3]>thresh) {
      normalization += h3;
      accumulator += h3*CONV_FLOAT(pS[-3]);
    }
    if (pS[-2]>thresh) {
      normalization += h2;
      accumulator += h2*CONV_FLOAT(pS[-2]);
    }
    if (pS[-1]>thresh) {
      normalization += h1;
      accumulator += h1*CONV_FLOAT(pS[-1]);
    }
    if (pS[0]>thresh) {
      normalization += h0;
      accumulator += h0*CONV_FLOAT(pS[0]);
    }
    if (pS[1]>thresh) {
      normalization += h1;
      accumulator += h1*CONV_FLOAT(pS[1]);
    }

    if (pS[2]>thresh) {
      normalization += h2;
      accumulator += h2*CONV_FLOAT(pS[2]);
    }
    if (pS[3]>thresh) {
      normalization += h3;
      accumulator += h3*CONV_FLOAT(pS[3]);
    }
    if (normalization>MIN_NORM)
      *pD++ = CONV_FLOAT(accumulator / normalization);
    else
      *pD++ = CONV_FLOAT(thresh);
  }
  // copy last three bytes
  *pD++ = CONV_FLOAT(thresh);pS++;
  *pD++ = CONV_FLOAT(thresh);pS++;
  *pD   = CONV_FLOAT(thresh);pS++;

  const CONV_FLOAT cfthresh =  CONV_FLOAT(thresh);

  // vertical convolution
  const CONV_FLOAT* pEndV = _tmpFloat.GetImageData() + width*(height-3);
  CONV_FLOAT* pI  = _tmpFloat.GetImageData();
  OutputStorageType* pO = dst.GetImageData();
  const CONV_FLOAT* pEndFirst3 =  _tmpFloat.GetImageData() + 3*width;
  const CONV_FLOAT* pEndLast3 =  _tmpFloat.GetImageData() + width*height;
  // copy first three lines
  while (pI<pEndFirst3) {
    *pO++ = OutputStorageType(thresh);
    pI++;
  }
  // convolve main
  pI--;
  while (++pI<pEndV) {
    register CONV_FLOAT normalization = 0;
    register CONV_FLOAT accumulator = 0;
    
    if (pI[-width3]>cfthresh) {
      normalization += v3;
      accumulator += v3*CONV_FLOAT(pI[-width3]);
    }
    if (pI[-width2]>cfthresh) {
      normalization += v2;
      accumulator += v2*CONV_FLOAT(pI[-width2]);
    }
    if (pI[-width]>cfthresh) {
      normalization += v1;
      accumulator += v1*CONV_FLOAT(pI[-width]);
    }
    if (pI[0]>cfthresh) {
      normalization += v0;
      accumulator += v0*CONV_FLOAT(pI[0]);
    }
    if (pI[width]>cfthresh) {
      normalization += v1;
      accumulator += v1*CONV_FLOAT(pI[width]);
    }

    if (pI[width2]>cfthresh) {
      normalization += v2;
      accumulator += v2*CONV_FLOAT(pI[width2]);
    }
    if (pI[width3]>cfthresh) {
      normalization += v3;
      accumulator += v3*CONV_FLOAT(pI[width3]);
    }

    if (normalization>MIN_NORM)
      *pO++ = OutputStorageType(accumulator / normalization);
    else
      *pO++ = OutputStorageType(thresh);

  }
  // copy last three lines
  while (pI<pEndLast3) {
    *pO++ = OutputStorageType(thresh);
    pI++;
  }
  
  // update roi
  DecreaseROI(*src.GetROI(), *dst.GetROI(), 3);

  return 0;
} 




//////////////////////////////////////////////////////////////////////////
//                 instantiation
//////////////////////////////////////////////////////////////////////////
#define FILTER_INSTANTIATION_CLASS Gauss
#include "Filterinst.hh"
