/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003, 2004    (see file CONTACTS for details)
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

using namespace BIAS;

namespace BIAS {

template <class StorageType>
BIASImageBase_EXPORT 
int ImageConvert::ToInterleaved(const Image<StorageType>& source,
                                Image<StorageType>& dest) {
   
  switch (source.GetColorModel()) {
  case ImageBase::CM_Grey:
    BIASERR("Does not make sense to convert grey images to interleaved or planar. Target image left unchanged!");
    return -1;
    break;

  case ImageBase::CM_HSV:
  case ImageBase::CM_BGR:
  case ImageBase::CM_RGB:
    return ToInterleavedRGB_(source,dest);
    break;

//   case MIP_YUYV422:
//     return ToInterleavedYUYV422_(source);
//     break;

  default:
    BIASERR("Conversion To Interleaved only implemented for ImageBase::CM_HSV, ImageBase::CM_BGR, ImageBase::CM_RGB. Target image left unchanged!");
    return -1;
  }
    
  //return 0;
}

template <class StorageType>
int ImageConvert::ToInterleavedRGB(const Image<StorageType>& red, 
                                   const Image<StorageType>& green, 
                                   const Image<StorageType>& blue,
                                   Image<StorageType>& dest) 
{
#ifdef BIAS_DEBUG
  if (!red.SamePixelAndChannelCount(green) || 
      !red.SamePixelAndChannelCount(blue)){
    BIASERR("image sizes of input images does not match");
    return -2;
  }
#endif
  register const StorageType *r=red.GetImageData();
  register const StorageType *g=green.GetImageData();
  register const StorageType *b=blue.GetImageData();
  if (!dest.IsEmpty()){
    if (dest.GetHeight() != red.GetHeight() || 
        dest.GetWidth() != red.GetWidth()){
      dest.Release();
      dest.Init(red.GetWidth(), red.GetHeight(), 3,dest.GetStorageType());
    }
  } else {
    dest.Init(red.GetWidth(), red.GetHeight(), 3,dest.GetStorageType());
  }
  register StorageType *sink = dest.GetImageData();
  register StorageType *end = dest.GetImageData() + 3 * dest.GetPixelCount();

  while (sink<end){
    *sink++=*r++;
    *sink++=*g++;
    *sink++=*b++;
  }
  dest.SetInterleaved(true);
  dest.SetColorModel(ImageBase::CM_RGB);
  return 0;
}

template <class StorageType>
int 
ImageConvert::ToInterleavedYUYV422_(const Image<StorageType>& /*source*/,
                                    Image<StorageType>& /*dest*/) 
{
  BIASERR("not implemented");  
  return 0;
}


template <class StorageType>
int ImageConvert::ToInterleavedRGB_(const Image<StorageType>& source,
                                    Image<StorageType>& dest) {
  
  bool IsInplace = source.GetImageData()==dest.GetImageData();
  Image<StorageType>* tmpdest;
  if (IsInplace) 
    tmpdest = new Image<StorageType>;
  else
    tmpdest = &dest;
  if (!tmpdest->IsEmpty()) tmpdest->Release();  
  tmpdest->Init(source.GetWidth(), source.GetHeight(), source.GetChannelCount(),
              source.GetStorageType());
  tmpdest->SetColorModel(source.GetColorModel());
  
  // Is it already interleaved?
  if ( !source.IsPlanar() ) {
    BIASERR("Source image was already interleaved.");
    register StorageType* pDest = tmpdest->GetImageData();    
    register StorageType* destStop = pDest + 3 * tmpdest->GetPixelCount();
    register const StorageType* src = source.GetImageData();
    for(;pDest < destStop; pDest++, src++) 
      *pDest = *src;

  } else {

    register StorageType* pDest = tmpdest->GetImageData();    
    register StorageType* destStop = pDest + 3 * tmpdest->GetPixelCount();

    register const StorageType* src_red   = source.GetImageData();
    register const StorageType* src_green = source.GetImageData() + source.GetPixelCount();
    register const StorageType* src_blue  = source.GetImageData() + 2*source.GetPixelCount();

    while (pDest < destStop) {

      *(pDest++) = *(src_red++);
      *(pDest++) = *(src_green++);
      *(pDest++) = *(src_blue++);
    }
  }
  
  tmpdest->SetInterleaved(true);
  if (IsInplace)
  {
    dest = *tmpdest;
    delete tmpdest;
  }
  return 0;
}

} // namespace BIAS
