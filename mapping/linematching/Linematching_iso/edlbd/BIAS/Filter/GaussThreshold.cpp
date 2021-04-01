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

#include "GaussThreshold.hh"
#include <Base/Image/ImageConvert.hh>
//#include <Base/Image/ImageIO.hh>

using namespace BIAS;
using namespace std;

#define MIN_NORM 0.6

//////////////////////////////////////////////////////////////////////////
//                 implementation
//////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
GaussThreshold<InputStorageType, OutputStorageType>::
GaussThreshold()
  : Convolution<InputStorageType, OutputStorageType>() 
{
  _GaussSigma = 0.7;
  _GaussRatio = 0.01;
  _LastRatio = _LastSigma = -1.0;
  _GaussIO = NULL;
  _GaussOO = NULL;
  _Threshold = 0.0;
}

template <class InputStorageType, class OutputStorageType>
GaussThreshold<InputStorageType, OutputStorageType>::
GaussThreshold(const GaussThreshold<InputStorageType, OutputStorageType>& other)
  : _GaussSigma(other._GaussSigma),
    _GaussRatio(other._GaussRatio),
    _LastSigma(other._LastSigma),
    _LastRatio(other._LastRatio)
{
  _Threshold = 0.0;
}

template <class StorageType, class KernelType>
GaussThreshold<StorageType, KernelType>::~GaussThreshold()
{
  if (_GaussIO!=NULL) delete _GaussIO;
  if (_GaussOO!=NULL) delete _GaussOO;
}

////////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int GaussThreshold<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "Gauss::Filter\n");
  if ((_GaussSigma != _LastSigma)||(_GaussRatio != _LastRatio))
    _CalculateKernels(_GaussSigma, _GaussRatio);
  
  if (_fm.IsSize1x1()){ // do not filter if filter mask is 1x1
    return ImageConvert::ConvertST(src, dst, dst.GetStorageType());
  } 
  if ((src.GetChannelCount()==1) && (this->GetBorderHandling() == TBH_valid))
    return Filter7x7GreyIgnoreBelowThreshold(src, dst, _Threshold);
  BIASERR("Not yet implemented");
  BIASABORT;
  return Convolution<InputStorageType, OutputStorageType>::Filter(src,dst);
}


////////////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
void GaussThreshold<InputStorageType, OutputStorageType>::
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
      BIASCDOUT(D_CONV_KERNEL, H[im]<<" "<<endl);
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

template <class InputStorageType, class OutputStorageType>
int GaussThreshold<InputStorageType, OutputStorageType>::
Filter7x7GreyIgnoreBelowThreshold(const Image<InputStorageType>& src, 
                                  Image<OutputStorageType>& dst,
                                  const double thresh) 
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "GaussThreshold::Filter7x7\n");
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
  unsigned minx, miny, maxx, maxy;
  int hws = 3;
  
  src.GetROI()->GetCorners(minx, miny, maxx, maxy); 
  minx += hws;
  miny += hws;
  maxx -= hws;
  maxy -= hws;
  if (minx>maxx) minx = maxx = (minx+maxx)/2;
  if (miny>maxy) miny = maxy = (miny+maxy)/2;
  dst.GetROI()->SetCorners(minx, miny, maxx, maxy);
  
  return 0;
}


template <class InputStorageType, class OutputStorageType>
int GaussThreshold<InputStorageType, OutputStorageType>::
Filter7x7GreyOnlyBelowThreshold(const Image<InputStorageType>& src, 
                                Image<OutputStorageType>& dst,
                                const double thresh) 
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "GaussThreshold::Filter7x7 only below threshold\n");
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

    // only fill pixels with a value below threshold
    if (*pS <= OutputStorageType(thresh) && normalization>MIN_NORM)
      *pD++ = CONV_FLOAT(accumulator / normalization);
    else
      *pD++ = CONV_FLOAT(*pS);
    
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
  
  // copy first three lines of original image to output image
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
    
    // only fill pixels which are below threshold
    if (*pI <= OutputStorageType(thresh) && normalization > MIN_NORM)
      *pO++ = OutputStorageType(accumulator / normalization);
    else
      *pO++ = OutputStorageType(*pI);   
  }
  
  // copy last three lines of original image to output image
  while (pI < pEndLast3) {
    *pO++ = OutputStorageType(thresh);
    pI++;
  }
  
  // update roi
  unsigned minx, miny, maxx, maxy;
  int hws = 3;
  
  src.GetROI()->GetCorners(minx, miny, maxx, maxy); 
  minx += hws;
  miny += hws;
  maxx -= hws;
  maxy -= hws;
  if (minx>maxx) minx = maxx = (minx+maxx)/2;
  if (miny>maxy) miny = maxy = (miny+maxy)/2;
  dst.GetROI()->SetCorners(minx, miny, maxx, maxy);
  
  return 0;
} 


//////////////////////////////////////////////////////////////////////////
//                 instantiation
//////////////////////////////////////////////////////////////////////////
#define FILTER_INSTANTIATION_CLASS GaussThreshold
#include "Filterinst.hh"
