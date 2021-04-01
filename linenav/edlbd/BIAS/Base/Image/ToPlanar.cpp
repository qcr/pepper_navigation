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
int ImageConvert::ToPlanar(const Image<StorageType>& source,
                           Image<StorageType>& dest)
{
  int res = -1;

  if (dest.IsEmpty())
    dest.Init(source.GetWidth(), source.GetHeight(), source.GetChannelCount(),
              source.GetStorageType(), false);

#ifdef BIAS_DEBUG
  if ( source.IsPlanar() )
    BIASERR("ImageConvert::ToPlanar() image already planar!");
  if (! dest.SamePixelAndChannelCount(source) )
    BIASERR("ImageConvert::ToPlanar() not same image size");
#endif

  switch (dest.GetColorModel())
  {
    case ImageBase::CM_HSV:
    case ImageBase::CM_BGR:
    case ImageBase::CM_RGB:
      res = ToPlanarRGB_(source,dest);
      break;
    case ImageBase::CM_YUYV422:
      res = ToPlanarYUYV422_(source,dest);
      break;
    default:
      BIASERR("conversion to planar only implemented for RGB, BGR and"
	     <<" YUYV422 images!");
      res = -1;
  }
  dest.SetInterleaved(false);
  return res;
}


template <class StorageType>
int ImageConvert::ToPlanarRGB_(const Image<StorageType>& source,
                               Image<StorageType>& dest)
{
  register StorageType* Red;

  // in place conversion needs extra memory
  if (source.GetImageData() == dest.GetImageData())
    Red = new StorageType[3 * source.GetPixelCount()];
  else
    Red = dest.GetImageData();

  BIASGDOUT(D_MI_TOPLANAR,"GetImageData()  "<<dest.GetImageData());
  register StorageType* Green = Red + source.GetPixelCount();
  register StorageType* Blue = Green + source.GetPixelCount();
  register const StorageType* ImageData = source.GetImageData();
  register const StorageType* ImageDataStop = ImageData +
    3 * source.GetPixelCount();

  // Copy the values to the corresponding areas in new memory
  while (ImageData < ImageDataStop) { // Schleife vielleicht rueckwaerts?
    *Red++   = *ImageData++;
    *Green++ = *ImageData++;
    *Blue++  = *ImageData++;
  }

  if (source.GetImageData() == dest.GetImageData()){
    // Change the pointer
    dest.ReleaseImageDataPointer();
    dest.SetInterleaved(false); 
    dest.RedirectImageDataPointer( Red - dest.GetPixelCount() );
  }

  BIASGDOUT(D_MI_TOPLANAR,"GetImageData()  "<<dest.GetImageData());
  return 0;
}


template <class StorageType>
int ImageConvert::ToPlanarYUYV422_(const Image<StorageType>& source,
                                   Image<StorageType>& dest)
{
#ifdef BIAS_DEBUG
  if (dest.GetWidth() % 2 != 0 )
    BIASERR("ImageConvert::ToPlanar() only implemented for images with even with");
#endif

  register const StorageType* ImageData = source.GetImageData();
  register const StorageType* ImageDataStop = ImageData + 2 * source.GetPixelCount();
  register StorageType *Y, *U, *V;

  if (source.GetImageData() == dest.GetImageData())
    Y = new StorageType[2 * source.GetPixelCount()];
  else
    Y = dest.GetImageData();
  U = Y + source.GetPixelCount();
  V = U + source.GetPixelCount()/2;

  while (ImageData < ImageDataStop){
    *Y++ = *ImageData++;
    *U++ = *ImageData++;
    *Y++ = *ImageData++;
    *V++ = *ImageData++;
  }

  if (source.GetImageData() == dest.GetImageData()){
    dest.ReleaseImageDataPointer();
    dest.SetInterleaved(false); 
    dest.RedirectImageDataPointer( Y - dest.GetPixelCount() );
  }
  return 0;
}

template <class StorageType>
int ImageConvert::FromInterleaved(const Image<StorageType>& source,
                                  Image<StorageType>& Image1,
                                  Image<StorageType>& Image2,
                                  Image<StorageType>& Image3)
{
  int res = 0;
#ifdef BIAS_DEBUG
  if ( source.GetChannelCount() == 1 )
    BIASERR("ImageConvert::FromInterleaved() does not make sense with one channel images");
  if ( Image1.IsEmpty() || Image2.IsEmpty() || Image3.IsEmpty() )
    BIASERR("ImageConvert::FromInterleaved() do not call with empty images");
  if (source.IsPlanar())
    BIASERR("ImageConvert::FromInterleaved() image is not interleaved");
#endif
  switch (source.GetColorModel()){
  case ImageBase::CM_YUYV422:
    res = FromInterleavedYUYV422_(source,Image1, Image2, Image3);
    break;
  case ImageBase::CM_RGB:
    res = FromInterleavedRGB_(source,Image1, Image2, Image3);
    break;
  default:
    BIASERR("ImageConvert::ToPlanar() color model not implemented");
    res = -1;
    break;
  }
  return res;
}

template <class StorageType>
int ImageConvert::FromInterleavedRGB_(const Image<StorageType>& source,
                                      Image<StorageType>& R,
                                      Image<StorageType>& G,
                                      Image<StorageType>& B)
{
#ifdef BIAS_DEBUG
  unsigned int Width = source.GetWidth();
  unsigned int Height = source.GetHeight();
  if ( ( Height != R.GetHeight() ) ||
       ( Height != G.GetHeight() ) ||
       ( Height != B.GetHeight() ) ||
       ( Width != R.GetWidth() ) ||
       ( Width != G.GetWidth() ) ||
       ( Width != B.GetWidth() ) )
    BIASERR("FromInterleavedRGB_: wrong argument image size");
#endif

  register StorageType* r = R.GetImageData();
  register StorageType* g = G.GetImageData();
  register StorageType* b = B.GetImageData();
  register const StorageType* ImageData = source.GetImageData();
  register const StorageType* ImageDataEnd = ImageData + source.GetPixelCount() * 3;
  while (ImageData < ImageDataEnd){
    *r++ = *ImageData++;
    *g++ = *ImageData++;
    *b++ = *ImageData++;
  }

  return 0;
}

template <class StorageType>
int ImageConvert::FromInterleavedYUYV422_(const Image<StorageType>& source,
                                          Image<StorageType>& Y,
                                          Image<StorageType>& U,
                                          Image<StorageType>& V)
{
#ifdef BIAS_DEBUG
  unsigned int Width = source.GetWidth();
  unsigned int Height = source.GetHeight();
  if ( ( Height != Y.GetHeight() ) ||
       ( Height != U.GetHeight() ) ||
       ( Height != V.GetHeight() ) ||
       ( Width != Y.GetWidth() ) ||
       ( Width != U.GetWidth() * 2 ) ||
       ( Width != V.GetWidth() * 2 ) )
    BIASERR("ImageConvert::FromInterleavedYUYV422 wrong argument image size");
#endif

  register StorageType* y = Y.GetImageData();
  register StorageType* u = U.GetImageData();
  register StorageType* v = V.GetImageData();
  register const StorageType* ImageData = source.GetImageData();
  register const StorageType* ImageDataEnd = ImageData + source.GetPixelCount() * 2;
  while (ImageData < ImageDataEnd){
    *y++ = *ImageData++;
    *u++ = *ImageData++;
    *y++ = *ImageData++;
    *v++ = *ImageData++;
  }

  return 0;
}


} // namespace BIAS
