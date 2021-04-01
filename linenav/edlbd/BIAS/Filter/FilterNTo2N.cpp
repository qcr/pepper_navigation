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


#include "FilterNTo2N.hh"


using namespace BIAS;

using namespace BIAS;
using namespace std;



template <class InputStorageType, class OutputStorageType>
FilterNTo2N<InputStorageType, OutputStorageType>::
FilterNTo2N()
  :FilterBase<InputStorageType, OutputStorageType>()
{
  _VecLengthType=VLT_L2;
}

template <class InputStorageType, class OutputStorageType>
FilterNTo2N<InputStorageType, OutputStorageType>::
FilterNTo2N(const FilterBase<InputStorageType, OutputStorageType>& other)
  : _VecLengthType(_VecLengthType)
{
}

template <class InputStorageType, class OutputStorageType>
FilterNTo2N<InputStorageType, OutputStorageType>::~FilterNTo2N()
{
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASERR("empty");
  return -1;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
FilterInt(const Image<InputStorageType>& src, Image<OutputStorageType>& dst)
{
  BIASERR("empty");
  return -1;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src,Image<OutputStorageType>& dst)
{
  BIASERR("empty");
  return -1;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
Filter(const Image<InputStorageType>& src,Image<OutputStorageType>& dst1, 
       Image<OutputStorageType>& dst2)
{
  BIASERR("empty");
  return -1;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
FilterInt(const Image<InputStorageType>& src,Image<OutputStorageType>& dst1, 
          Image<OutputStorageType>& dst2)
{
  BIASERR("empty");
  return -1;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src,Image<OutputStorageType>& dst1, 
            Image<OutputStorageType>& dst2)
{
  BIASERR("empty");
  return -1;
}


////////////////////////////////////////////////////////////////////
//   length algorithms
////////////////////////////////////////////////////////////////////

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType,OutputStorageType>::
VecLen(const Image<OutputStorageType>& gx, 
       const Image<OutputStorageType>& gy,
       Image<OutputStorageType>& absg)
{
  int res=0;
  switch (_VecLengthType){
    case VLT_L1:
      res=VecLenL1(gx, gy, absg);
      break;
    case VLT_L2:
      res=VecLenL2(gx, gy, absg);
      break;
    case VLT_max:
      res=VecLenMax(gx, gy, absg);
      break;
  default:
    BIASERR("unknown VecLengthType "<<_VecLengthType);
    res=-1;
    break;
  }

  return res;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType,OutputStorageType>::
VecLenL1(const Image<OutputStorageType>& gx, 
         const Image<OutputStorageType>& gy,
         Image<OutputStorageType>& absg)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "FilterNTo2N::VecLenL1\n");
  unsigned htlx, htly, hbrx, hbry, vtlx, vtly, vbrx, vbry;
  gx.GetROI()->GetCorners(htlx, htly, hbrx, hbry);
  gy.GetROI()->GetCorners(vtlx, vtly, vbrx, vbry);
  const int minx=(htlx<vtlx)?vtlx:htlx;
  const int miny=(htly<vtly)?vtly:htly;
  const int maxx=(hbrx>vbrx)?vbrx:hbrx;
  const int maxy=(hbry>vbry)?vbry:hbry;
  
  register int x, y;
  register const OutputStorageType **gxi=gx.GetImageDataArray();
  register const OutputStorageType **gyi=gy.GetImageDataArray();
  register OutputStorageType **absi=absg.GetImageDataArray();

  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      absi[y][x]=(OutputStorageType)(fabs((double)(gxi[y][x]))+
                                     fabs((double)(gyi[y][x])));
    }
  }
  
  absg.GetROI()->SetCorners(minx, miny, maxx, maxy);

  return 0;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType, OutputStorageType>::
VecLenL2(const Image<OutputStorageType>& gx, 
         const Image<OutputStorageType>& gy,
         Image<OutputStorageType>& absg)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "FilterNTo2N::VecLenL2\n");
  unsigned htlx, htly, hbrx, hbry, vtlx, vtly, vbrx, vbry;
  gx.GetROI()->GetCorners(htlx, htly, hbrx, hbry);
  gy.GetROI()->GetCorners(vtlx, vtly, vbrx, vbry);
  const int minx=(htlx<vtlx)?vtlx:htlx;
  const int miny=(htly<vtly)?vtly:htly;
  const int maxx=(hbrx>vbrx)?vbrx:hbrx;
  const int maxy=(hbry>vbry)?vbry:hbry;
  
  register int x, y;
  register const OutputStorageType **gxi=gx.GetImageDataArray();
  register const OutputStorageType **gyi=gy.GetImageDataArray();
  register OutputStorageType **absi=absg.GetImageDataArray();

  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      absi[y][x]=
        (OutputStorageType)sqrt((double)(gxi[y][x])*(double)(gxi[y][x])+
                                (double)(gyi[y][x])*(double)(gyi[y][x]));
    }
  }
  
  absg.GetROI()->SetCorners(minx, miny, maxx, maxy);

  return 0;
}

template <class InputStorageType, class OutputStorageType>
int FilterNTo2N<InputStorageType,OutputStorageType>::
VecLenMax(const Image<OutputStorageType>& gx, 
          const Image<OutputStorageType>& gy,
          Image<OutputStorageType>& absg)
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK, "FilterNTo2N::VecLenMax\n");
  unsigned htlx, htly, hbrx, hbry, vtlx, vtly, vbrx, vbry;
  gx.GetROI()->GetCorners(htlx, htly, hbrx, hbry);
  gy.GetROI()->GetCorners(vtlx, vtly, vbrx, vbry);
  const int minx=(htlx<vtlx)?vtlx:htlx;
  const int miny=(htly<vtly)?vtly:htly;
  const int maxx=(hbrx>vbrx)?vbrx:hbrx;
  const int maxy=(hbry>vbry)?vbry:hbry;
  
  register int x, y;
  register const OutputStorageType **gxi=gx.GetImageDataArray();
  register const OutputStorageType **gyi=gy.GetImageDataArray();
  register OutputStorageType **absi=absg.GetImageDataArray();

  for (y=miny; y<maxy; y++){
    for (x=minx; x<maxx; x++){
      absi[y][x]=(gxi[y][x]>=gyi[y][x])?(gxi[y][x]):(gyi[y][x]);
    }
  }
  
  absg.GetROI()->SetCorners(minx, miny, maxx, maxy);

  return 0;
}

#define FILTER_INSTANTIATION_CLASS FilterNTo2N
#define FILTER_INSTANTIATION_NO_UNSIGNED_OUTPUT
#include "Filterinst.hh"
