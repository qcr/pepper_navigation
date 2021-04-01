/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2004 (see file CONTACT for details)
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

#include "FilterMask.hh"

using namespace BIAS;
using namespace std;

#define SETZEROSHIFT _KernelRightShift=0;_HorizRightShift=0;_VertRightShift=0

FilterMask::FilterMask() {
  _Separable = false;
  SETZEROSHIFT;
}

void FilterMask::Init(const Matrix<FM_INT> &kernel, int rs, 
                      bool ResetOtherType){ 
  _sKernel   = kernel;
  _Separable = false;
  _HorizRightShift  = 0;
  _VertRightShift   = 0;
  _KernelRightShift = rs;
  if (ResetOtherType) ResetFloatFilter();
}

void FilterMask::Init(const Matrix<FM_FLOAT> &kernel, bool ResetOtherType) {
  _fKernel   = kernel;
  _Separable = false;
  SETZEROSHIFT;
  if (ResetOtherType) ResetIntFilter();
}

void FilterMask::Init(const Vector<FM_INT> &horz, const Vector<FM_INT> &vert,
                      int hrs, int vrs, bool ResetOtherType) {
  _sHoriz    = horz;
  _sVert     = vert;
  _Separable = true;
  _HorizRightShift  = hrs;
  _VertRightShift   = vrs;
  _KernelRightShift = 0;
  if (ResetOtherType) ResetFloatFilter();
}

void FilterMask::Init(const Vector<FM_FLOAT> &horz,
                      const Vector<FM_FLOAT> &vert, 
                      bool ResetOtherType) {
  _fHoriz    = horz;
  _fVert     = vert;
  _Separable = true;
  SETZEROSHIFT;
  if (ResetOtherType) ResetIntFilter();
}

FilterMask & FilterMask::operator=(const FilterMask& src) {
  _sKernel   = src._sKernel;
  _fKernel   = src._fKernel;
  _sHoriz    = src._sHoriz;
  _fHoriz    = src._fHoriz;
  _sVert     = src._sVert;
  _fVert     = src._fVert;
  _Separable = src._Separable;

  _KernelRightShift = src._KernelRightShift;
  _HorizRightShift  = src._HorizRightShift;
  _VertRightShift   = src._VertRightShift;

  return *this;
}

void FilterMask::CreateFloatFilter() {
  _fKernel.newsize(_sKernel.num_cols(),_sKernel.num_rows());
  _fHoriz.newsize(_sHoriz.Size());
  _fVert.newsize(_sVert.Size());

  FM_FLOAT shiftscale=powf(0.5,(float)_KernelRightShift);
  for(int x=0;x<_sKernel.num_cols();x++)
    for(int y=0;y<_sKernel.num_rows();y++){
      _fKernel[y][x]=_sKernel[y][x]*shiftscale;
    }

  shiftscale=powf(0.5,(float)_HorizRightShift);
  for(int x=0;x<_sHoriz.size();x++) {
    _fHoriz[x]=_sHoriz[x]*shiftscale;
  }

  shiftscale=powf(0.5,(float)_VertRightShift);
  for(int x=0;x<_sVert.size();x++){
    _fVert[x]=_sVert[x]*shiftscale;
  }
}

void FilterMask::CreateIntFilter(int rshift,
                                 int rshifth,
                                 int rshiftv) {
  BIASDOUT(D_FM_INTAPPROX,"Called with shifts "<<rshift<<" "<<rshiftv
           <<" "<<rshifth);
  FM_FLOAT shiftscale = 0;
  if (!_Separable) {
    // 2D kernel:
    _sKernel.newsize(_fKernel.num_cols(),_fKernel.num_rows());
    shiftscale = powf(2.0,(float)rshift);
    BIASDOUT(D_FM_INTAPPROX,"Shiftscale is "<<shiftscale);
    for(int x=0;x<_sKernel.num_cols();x++)
      for(int y=0;y<_sKernel.num_rows();y++){
        _sKernel[y][x]=(FM_INT)rintf(_fKernel[y][x]*shiftscale);
      }
    _KernelRightShift = rshift;
  } else {
    // horizontal part
    _sHoriz.newsize(_fHoriz.Size());
    shiftscale = powf(2.0,(float)rshifth);
    BIASDOUT(D_FM_INTAPPROX,"Shiftscale horizontal is "<<shiftscale);
    for(unsigned int x=0;x<_fHoriz.Size();x++) {
      _sHoriz[x] = (FM_INT)rintf(_fHoriz[x] * shiftscale);
    }
    _HorizRightShift = rshifth;  
    BIASDOUT(D_FM_INTAPPROX,"Horizontal filter is "<<_sHoriz);

    // vertical part
    _sVert.newsize(_fVert.Size());
    shiftscale = powf(2.0,(float)rshiftv);
    BIASDOUT(D_FM_INTAPPROX,"Shiftscale vertical is "<<shiftscale);
    for(unsigned int x=0;x<_fVert.Size();x++) {
      _sVert[x] = (FM_INT)rintf(_fVert[x] * shiftscale);
    }
    _VertRightShift = rshiftv;
    BIASDOUT(D_FM_INTAPPROX,"Vertical filter is "<<_sVert);
  }
}

void FilterMask::ResetFloatFilter() {
  if (!_Separable) {
    _fKernel.newsize(_sKernel.num_cols(),_sKernel.num_rows());
    for(int x=0;x<_sKernel.num_cols();x++)
      for(int y=0;y<_sKernel.num_rows();y++){
        _fKernel[y][x] = 0;
      }
  } else {
    _fHoriz.newsize(_sHoriz.Size());
    for(int x=0;x<_sHoriz.size();x++) {
      _fHoriz[x]=0;
    }
    _fVert.newsize(_sVert.Size());
    for(int x=0;x<_sVert.size();x++){
      _fVert[x]=0;
    }
  }
}

void FilterMask::ResetIntFilter() {
  if (!_Separable) {
    // 2D kernel:
    _sKernel.newsize(_fKernel.num_cols(),_fKernel.num_rows());
    for(int x=0;x<_sKernel.num_cols();x++)
      for(int y=0;y<_sKernel.num_rows();y++){
        _sKernel[y][x]=0;
      }
    _KernelRightShift = 0;
  } else {
    // horizontal part
    _sHoriz.newsize(_fHoriz.Size());
    for(unsigned int x=0;x<_fHoriz.Size();x++) {
      _sHoriz[x] = 0;
    }
    _HorizRightShift = 0;  

    // vertical part
    _sVert.newsize(_fVert.Size());
    for(unsigned int x=0;x<_fVert.Size();x++) {
      _sVert[x] = 0;
    }
    _VertRightShift = 0;
  }
}

void FilterMask::Transpose() {
  _sKernel = _sKernel.Transpose();
  _fKernel = _fKernel.Transpose();
  Vector<FM_INT> stmp(_sHoriz);
  _sHoriz = _sVert;
  _sVert = stmp;
  Vector<FM_FLOAT> ftmp(_fHoriz);
  _fHoriz = _fVert;
  _fVert = ftmp;
  int tmp(_HorizRightShift);
  _HorizRightShift = _VertRightShift;
  _VertRightShift = tmp;
}
  
int FilterMask::ComputeIntPrecisionBits(int NumberOfBitsInInputData,
                                        int NumberOfBitsInTempData) {
  BIASDOUT(D_FM_PRECISION,"Called with NumberOfBitsInInputData="
           <<NumberOfBitsInInputData<<" and NumberOfBitsInTempData="
           <<NumberOfBitsInTempData);
  float norml1 = 0.0, maxentry=0.0;
  if (_Separable) {
    // get maximum of l1 norms of vectors
    float normhori = _fHoriz.NormL1();
    float normvert = _fVert.NormL1();
    norml1 = (normvert>normhori)?normvert:normhori;
    for (int i=0; i<_fHoriz.size(); i++) 
      if (maxentry<fabs(_fHoriz[i])) maxentry = fabs(_fHoriz[i]);
    for (int i=0; i<_fHoriz.size(); i++) 
      if (maxentry<fabs(_fVert[i])) maxentry = fabs(_fVert[i]);
  } else {
    float maxval=0, minval=0;
    // get sum of absolute values
    norml1 = _fKernel.NormL1();
    _fKernel.GetMaxMin(maxval, minval);
    maxentry = (fabs(maxval)>fabs(minval))?fabs(maxval):fabs(minval);
  }
  // compute dual logarithm of norm
  norml1 = (float)(log(norml1)/log(2.0));
  int ScaleBits = (int)ceil(norml1);
 
  // if the filter mask does not sum to unity, ScaleBits contains the number
  // of bits to be subtracted from the maximum possible number here.
  // ScaleBits can also be negative here, if the float entries sum up to
  // a very small number, e.g. 0.2

  // now subtract from buffer size: 1.) input data size 2.)norm of mask bits
  ScaleBits = NumberOfBitsInTempData-NumberOfBitsInInputData-ScaleBits;
  BIASDOUT(D_FM_PRECISION,"ScaleBits are "<<ScaleBits<<" MaxEntry:"<<maxentry);

  // in principle we are ready, but in an int of type FM_INT, we can only hold
  // a limited number of bits, so check that limit also:
  double thetest1 = maxentry * pow(2.0, (double)ScaleBits);
  FM_INT thetest2 = 
    (FM_INT)trunc((double)maxentry*pow(2.0, (double)ScaleBits));
  if ((long)thetest1!=(long)thetest2) {
    // compute dual logarithm of absolutely largest entry
    norml1 = (float)(log(maxentry)/log(2.0));
    // largest power-of-2-scaling of largest entry which fits into FM_INT
    ScaleBits = ((sizeof(FM_INT)*8-1)-(int)ceil(norml1));
    BIASDOUT(D_FM_PRECISION,"int filter content check failed:"
             <<(FM_INT)thetest1<<" != "
             <<thetest2<<" maxentry is "<<maxentry<<" reducing scalebits to "
             <<ScaleBits);

  }
  return ScaleBits;
}




BIASFilter_EXPORT ostream& BIAS::operator<<(ostream& os, const FilterMask& fm) {
  if (fm._Separable) {
    os<<"filter mask in separated representation (integer): h="<< fm._sHoriz
      <<" with right shift "<< fm._HorizRightShift
      <<" v="<<fm._sVert<<" with right shift "<<fm._VertRightShift<<endl;
    os <<" the filter mask in separated representation (floating point) h="
       <<fm._fHoriz<<" v="<<fm._fVert<<endl;
  } else {
    os<<"filter mask for integer type convolution:"<<fm._sKernel
      <<" with right shift of "<<fm._KernelRightShift<<endl
      <<"filter mask for floating point type convolution: " << fm._fKernel;
  }

  return os;
}
