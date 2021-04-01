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

#include "FilterBase.hh"


using namespace BIAS;
using namespace std;



//#define DEBUG_CALLSTACK

template <class InputStorageType, class OutputStorageType>
FilterBase<InputStorageType, OutputStorageType>::FilterBase()
  : Debug()
{
  _FilterBorderHandling = TBH_valid;
#ifdef DEBUG_CALLSTACK
  AddDebugLevel(D_FILTERBASE_CALLSTACK);
#endif
}

template <class InputStorageType, class OutputStorageType>
FilterBase<InputStorageType, OutputStorageType>::
FilterBase(const FilterBase<InputStorageType, OutputStorageType>& other)
  : _FilterBorderHandling(other._FilterBorderHandling)
{
}

template <class InputStorageType, class OutputStorageType>
FilterBase<InputStorageType, OutputStorageType>::~FilterBase()
{
}

template <class InputStorageType, class OutputStorageType>
void FilterBase<InputStorageType, OutputStorageType>::
GetBorders(int& border_x, int& border_y) const
{
  switch(_FilterBorderHandling){
  case TBH_same:
    border_x=0;
    border_y=0;
    break;
  case TBH_full:
    BIASERR("unfinished");
    BIASABORT;
    break;
  case TBH_valid:
    GetBordersValid_(border_x, border_y);
    break;
  default:
    BIASASSERT("invalid boder handling type" && false);
    BIASABORT;
    break;
  }
}

template <class InputStorageType, class OutputStorageType>
void  FilterBase<InputStorageType, OutputStorageType>::
SetNonROIToValue(Image<OutputStorageType>& dst,
                 const std::vector<OutputStorageType>& t) const {
  const unsigned int channelcount = dst.GetChannelCount();
  BIASASSERT(t.size()==channelcount);
  ROI* pROI = dst.GetROI();

  if (pROI->GetROIType()==ROI_Corners) {
    // roi corners is simple, just do upper part, left and right and then lower
    unsigned UpperLeftX=0, UpperLeftY=0, LowerRightX=0, LowerRightY=0;
    pROI->GetCorners(UpperLeftX, UpperLeftY, LowerRightX, LowerRightY);

    // upper rows
    for (unsigned int y=0; y<UpperLeftY; y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = t[c];
      }
    }
    for (unsigned int y=UpperLeftY; y<LowerRightY; y++) {
      // left border
      for (unsigned int x=0; x<UpperLeftX; x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = t[c];
      }
      // right border
      for (unsigned int x=LowerRightX; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = t[c];
      }
    }
    // lower rows
    for (unsigned int y=LowerRightY; y<dst.GetHeight(); y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = t[c];
      }
    }
  } else {
    // generic roi: check every pixel
    for (unsigned int y=0; y<dst.GetHeight(); y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        if (!pROI->IsInROI(x,y))
          for (unsigned int c=0; c<channelcount; c++) 
            dst.GetImageDataArray()[y][(x*channelcount)+c] = t[c];
      }
    }
  }
}

template <class InputStorageType, class OutputStorageType>
void  FilterBase<InputStorageType, OutputStorageType>::
CopyNonROIFromSource(const Image<InputStorageType>& src, 
                     Image<OutputStorageType>& dst) const {
  const unsigned int channelcount = dst.GetChannelCount();
  BIASASSERT(src.GetWidth()==dst.GetWidth());
  BIASASSERT(src.GetHeight()==dst.GetHeight());
  BIASASSERT(src.GetChannelCount()==dst.GetChannelCount());
  ROI* pROI = dst.GetROI();

  if (pROI->GetROIType()==ROI_Corners) {
    // roi corners is simple, just do upper part, left and right and then lower
    unsigned UpperLeftX=0, UpperLeftY=0, LowerRightX=0, LowerRightY=0;
    pROI->GetCorners(UpperLeftX, UpperLeftY, LowerRightX, LowerRightY);

    // upper rows
    for (unsigned int y=0; y<UpperLeftY; y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = 
            src.GetImageDataArray()[y][(x*channelcount)+c];
      }
    }
    for (unsigned int y=UpperLeftY; y<LowerRightY; y++) {
      // left border
      for (unsigned int x=0; x<UpperLeftX; x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = 
            src.GetImageDataArray()[y][(x*channelcount)+c];
      }
      // right border
      for (unsigned int x=LowerRightX; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = 
            src.GetImageDataArray()[y][(x*channelcount)+c];
      }
    }
    // lower rows
    for (unsigned int y=LowerRightY; y<dst.GetHeight(); y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        for (unsigned int c=0; c<channelcount; c++) 
          dst.GetImageDataArray()[y][(x*channelcount)+c] = 
            src.GetImageDataArray()[y][(x*channelcount)+c];
      }
    }
  } else {
    // generic roi: check every pixel
    for (unsigned int y=0; y<dst.GetHeight(); y++) {
      for (unsigned int x=0; x<dst.GetWidth(); x++) {
        if (!pROI->IsInROI(x,y))
          for (unsigned int c=0; c<channelcount; c++) 
            dst.GetImageDataArray()[y][(x*channelcount)+c] = 
              src.GetImageDataArray()[y][(x*channelcount)+c];
      }
    }
  }
}




#define FILTER_INSTANTIATION_CLASS FilterBase
#include "Filterinst.hh"
