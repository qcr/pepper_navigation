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

#include <Base/Image/Image.hh>
#include <Base/Image/ImageConvert.hh>

// disabled warnings



namespace BIAS {

//////////////////////////////////////////////////////////////////////////////
//
// Assignment Operator
//
//////////////////////////////////////////////////////////////////////////////

template <class StorageType>
Image<StorageType>& Image<StorageType>::operator=(const ImageBase& Source)
{

  if (GetStorageType()==Source.GetStorageType()){
    this->ImageBase::operator=(Source);
  } else {
    // please ensure that TestImageAssign runs flawless when changing anything
    // in this function
    BEXCEPTION("Images have different storage type. Use ConvertST()");
  }
  return *this;
}


//////////////////////////////////////////////////////////////////////////////
//
//
//   Defining Operators -, -= ,+ ,+= ,* ,*= ,/ ,/= for both a scalar argument and an image
//
//   These operators are templated, so the work only on arguments of the same
//   type as the image itself.
//
//   The division operators have an addition a test for
//      - a zero scalar as argument
//      - for zero values in an argument image (if MIP_BOUNCE_CHECK is defined)
//
//
//////////////////////////////////////////////////////////////////////////////

//
// As long as color images are not supported, exit if operation is tried on non-gray images
//
#ifdef BIAS_DEBUG
#define GRAYTEST(OPERATOR) \
if (GetChannelCount() != 1) { \
  BIASERR("Operation " << OPERATOR << " is only defined on gray images. Leaving images unchanged! Aborting!"); \
  BIASBREAK;\
  BIASABORT; \
}
#else
#define GRAYTEST(OPERATOR)
#endif


//////////////////////////////////////////////////////////////////////////////


/// Test the the dimensions of the ROI of two images. If both have none
/// test the dimensions of the whole image
/// Abort in any case of error.
template <class StorageType>
inline bool HaveImagesMatchingROI(const Image<StorageType>& i1,
                                  const Image<StorageType>& i2)
{
  // Test for matching dimensions of ROI
  if ( ( (i1.GetROILowerRightX() - i1.GetROIUpperLeftX()) !=
         (i2.GetROILowerRightX() - i2.GetROIUpperLeftX()) ) ||
       ( (i1.GetROILowerRightY() - i1.GetROIUpperLeftY()) !=
         (i2.GetROILowerRightY() - i2.GetROIUpperLeftY()) ) ) {
    BIASERR("Computation not possible: 'Region Of Interest' do not "
            <<"match in size! Aborting!");
    i1.PrintROI();
    i2.PrintROI();
    return false;
  }
  return true;
}


//////////////////////////////////////////////////////////////////////////////
//
// The unary operators on pixel values of an image using a scalar
//
//////////////////////////////////////////////////////////////////////////////

/** Operator += with scalar on pixel values of an image */
template <class StorageType>
Image<StorageType>& Image<StorageType>::operator+= (const StorageType& Value)
{
  GRAYTEST("+=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  /*register*/ StorageType *p=GetImageData()+width*miny+minx;
  /*register*/ StorageType *le=GetImageData()+width*miny+maxx;
  /*register*/ StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  while (p<e){
    while (p<le){ // walk one line
      //*p++ += Value;

      (*p) += (Value);
      //*p = *p + Value;

      p++;
    }
    le+=width; // skip to beginning of next line
    p+=step;
  }

  return *this;
}


/** Operator -= with scalar on pixel values of an image */
template <class StorageType>
Image<StorageType>& Image<StorageType>::operator-= (const StorageType& Value)
{
  GRAYTEST("-=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ -= Value;
    }
    le+=width; // skip to beginning of next line
    p+=step;
  }

  return *this;
}


/** Operator *= with scalar on pixel values of an image */
template <class StorageType>
Image<StorageType>& Image<StorageType>::operator*= (const StorageType& Value)
{
  GRAYTEST("*=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  width *= GetChannelCount();
  step  *= GetChannelCount();
  register StorageType *p  = GetImageData()+width*miny +minx;
  register StorageType *le = GetImageData()+width*miny +maxx;
  register StorageType *e  = GetImageData()+width*(maxy-1) +maxx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ *= Value;
    }
    le+=width; // skip to beginning of next line
    p+=step;
  }

  return *this;
}


/** Operator /= with scalar on pixel values of an image */
template <class StorageType>
Image<StorageType>& Image<StorageType>::operator/= (const StorageType& Value)
{
  if (Value == (StorageType) 0) {
    BIASERR("Division by zero attempted on image. Aborting!");
    BIASABORT;
  }

  // GRAYTEST("/=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ /= Value;
    }
    le+=width; // skip to beginning of next line
    p+=step;
  }

  return *this;
}



//////////////////////////////////////////////////////////////////////////////
//
//  The unary operators on pixel values of an  image using the pixel
//  values of another image
//
//////////////////////////////////////////////////////////////////////////////

/** Operator += operating on pixel values with pixel values of another image */
template <class StorageType>
Image<StorageType>&
Image<StorageType>::operator+=(const Image<StorageType>& argimage)
{
  GRAYTEST("+=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  register const StorageType *argimagedata=argimage.GetImageData()+width*miny+minx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ += *argimagedata++;
    }
    le+=width; // skip to beginning of next line
    p+=step;
    argimagedata+=step;
  }

  return *this;
}

/** Operator -= operating on pixel values with pixel values of another image */
template <class StorageType> Image<StorageType>&
Image<StorageType>::operator-= (const Image<StorageType>& argimage)
{
  GRAYTEST("+=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  register const StorageType *argimagedata=argimage.GetImageData()+width*miny+minx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ -= *argimagedata++;
    }
    le+=width; // skip to beginning of next line
    p+=step;
    argimagedata+=step;
  }

  return *this;
}

/** Operator *= operating on pixel values with pixel values of another image */
template <class StorageType> Image<StorageType>&
Image<StorageType>::operator*= (const Image<StorageType>& argimage)
{
  GRAYTEST("+=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  register const StorageType *argimagedata=argimage.GetImageData()+width*miny+minx;
  while (p<e){
    while (p<le){ // walk one line
      *p++ *= *argimagedata++;
    }
    le+=width; // skip to beginning of next line
    p+=step;
    argimagedata+=step;
  }

  return *this;
}

/** Operator /= operating on pixel values with pixel values of another image */template <class StorageType> Image<StorageType>&
Image<StorageType>::operator/= (const Image<StorageType>& argimage)
{
  GRAYTEST("+=");

  unsigned int minx, miny, maxx, maxy;
  GetROICorners(minx, miny, maxx, maxy);
  register int width=(int)GetWidth(), step=width-maxx+minx;
  register StorageType *p=GetImageData()+width*miny+minx;
  register StorageType *le=GetImageData()+width*miny+maxx;
  register StorageType *e=GetImageData()+width*(maxy-1)+maxx;
  register const StorageType *argimagedata=argimage.GetImageData()+width*miny+minx;
  while (p<e){
    while (p<le){ // walk one line
      if(*argimagedata != (StorageType) 0)
	*p /= *argimagedata;
      argimagedata++;
      p++;
    }
    le+=width; // skip to beginning of next line
    p+=step;
    argimagedata+=step;
  }

  return *this;
}

//////////////////////////////////////////////////////////////////////////////
//
// The binary operators on pixel values of an image using a scalar
//
//////////////////////////////////////////////////////////////////////////////


/** Binary operator + operating on pixel values with a scalar,
    returning a new image */
template <class StorageType>
Image<StorageType> Image<StorageType>::operator+ (const StorageType& Value) const
{
  if (Value == (StorageType) 0) {
    BIASERR("Division by zero attempted on image. Aborting!");
    BIASABORT;
  }

  GRAYTEST("+");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(GetWidth(), GetHeight(), GetChannelCount());

  /* Define a new result image and use the unary version of this operator */
  result = (*this);
  result += Value;

  return result;
}


/** Binary operator - operating on pixel values with a scalar,
    returning a new image */
template <class StorageType>
Image<StorageType> Image<StorageType>::operator- (const StorageType& Value) const
{
  GRAYTEST("-");

  /* In any case we need a new result image */
  Image<StorageType> result(GetWidth(), GetHeight(), GetChannelCount());

  /* Define a new result image and use the unary version of this operator */
  result = (*this);
  result -= Value;

  return result;
}


/** Binary operator * operating on pixel values with a scalar,
    returning a new image */
template <class StorageType>
Image<StorageType> Image<StorageType>::operator* (const StorageType& Value) const
{
  GRAYTEST("*");

  /* In any case we need a new result image */
  Image<StorageType> result(GetWidth(), GetHeight(), GetChannelCount());

  /* Define a new result image and use the unary version of this operator */
  result = (*this);
  result *= Value;

  return result;
}


/** Binary operator / operating on pixel values with a scalar,
    returning a new image */
template <class StorageType>
Image<StorageType> Image<StorageType>::operator/ (const StorageType& Value) const
{
  GRAYTEST("/");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(GetWidth(), GetHeight(), GetChannelCount());

  /* Define a new result image and use the unary version of this operator */
  result = (*this);
  result /= Value;

  return result;
}


//////////////////////////////////////////////////////////////////////////////
//
// The binary operators on pixel values of an image using pixel values of
//   another image, returning a new image
//
//////////////////////////////////////////////////////////////////////////////

/** Binary Operator + operating pixel values with pixel values of another
    image, returning a new image */
template <class StorageType> Image<StorageType>
Image<StorageType>::operator+ (const Image<StorageType>& argimage) const
{
  GRAYTEST("+");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());


  if ( HaveImagesMatchingROI(*this, argimage) ) {
    /* Define a new result image and use the unary version of this operator */
    result = (*this);
    result += argimage;
  } else {
    BIASERR("unmatching ROIs");
  }
  return result;
}


/** Binary Operator - operating pixel values with pixel values of another
    image, returning a new image */
template <class StorageType> Image<StorageType>
Image<StorageType>::operator-(const Image<StorageType>& argimage)const
{
  GRAYTEST("-");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());

  if ( HaveImagesMatchingROI(*this, argimage) ) {
    /* Define a new result image and use the unary version of this operator */
    result = *this;
    result -= argimage;
  } else {
    BIASERR("unmatching ROIs");
  }
  return result;
}


/** Binary Operator * operating pixel values with pixel values of another
    image, returning a new image */
template <class StorageType> Image<StorageType>
Image<StorageType>::operator* (const Image<StorageType>& argimage) const
{
  GRAYTEST("*");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());

  if ( HaveImagesMatchingROI(*this, argimage) ) {

    /* Define a new result image and use the unary version of this operator */
    result = (*this);
    result *= argimage;
  } else {
    BIASERR("unmatching ROIs");
  }
  return result;
}


/** Binary Operator / operating pixel values with pixel values of another
    image, returning a new image */
template <class StorageType> Image<StorageType>
Image<StorageType>::operator/ (const Image<StorageType>& argimage) const
{
  GRAYTEST("/");

  /* In any case we need a new result image */
  Image<StorageType> result;
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());

  if ( HaveImagesMatchingROI(*this, argimage) ) {
    /* Define a new result image and use the unary version of this operator */
    result = (*this);
    result /= argimage;
  } else {
    BIASERR("unmatching ROIs");
  }
  return result;
}



/** logical Operator || operating pixel values with pixel values of another
    image, returning a new image */
template <class StorageType> Image<StorageType>&
Image<StorageType>::operator| (const Image<StorageType>& argimage)
{
  BIASERR("operator| not implemented for StorageType");
  return *(Image<StorageType>*)NULL;
}

template <> Image<short int>&
Image<short int>::operator| (const Image<short int>& argimage)
{
  GRAYTEST("|");

  /* In any case we need a new result image */
  Image<short int>& result = *(new Image<short int>);
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());

  if ( HaveImagesMatchingROI(*this, argimage) ) {
    BIASERR("Image::operator| not implemented for images with ROIs");
  } else {
    BIASERR("unmatching ROIs");
//     /* Get the pointer to the begin of the image (and of the newly created result image) and use it as counter */
//     register short int* ImageData       = (short int*)GetImageData();
//     register short int* ImageDataStop   = ImageData + GetPixelCount();
//     register short int* ArgImageData   = (short int*)argimage.GetImageData();
//     register short int* ResultImageData= (short int*)result.GetImageData();
//     for(;ImageData < ImageDataStop; ImageData++,
// 	  ArgImageData++, ResultImageData++) {
//       *ResultImageData = *ImageData | *ArgImageData;
//     }
  }
  return result;
}

template <> Image<unsigned char>&
Image<unsigned char>::operator| (const Image<unsigned char>& argimage)
{
  GRAYTEST("|");

  /* In any case we need a new result image */
  Image<unsigned char>& result = *(new Image<unsigned char>);
  result.Init(argimage.GetWidth(), argimage.GetHeight(),
	      argimage.GetChannelCount());

  if ( HaveImagesMatchingROI(*this, argimage) ) {
    BIASERR("Image::operator| not implemented for images with ROIs");
  } else {
    BIASERR("unmatching ROIs");
//     /* Get the pointer to the begin of the image (and of the newly created result image) and use it as counter */
//     register unsigned char* ImageData      = (unsigned char* )GetImageData();
//     register unsigned char* ImageDataStop  = ImageData + GetPixelCount();
//     register unsigned char* ArgImageData   =
//       (unsigned char* )argimage.GetImageData();
//     register unsigned char* ResultImageData=
//       (unsigned char* )result.GetImageData();
//     for(;ImageData < ImageDataStop; ImageData++,
// 	  ArgImageData++, ResultImageData++) {
//       *ResultImageData = *ImageData | *ArgImageData;
//     }
  }
  return result;
}


/** absolut difference */
template <class StorageType> void
Image<StorageType>::AbsDiff(const Image<StorageType>& im1,
                            const Image<StorageType>& im2)
{
#ifdef BIAS_DEBUG
  if (!im1.SamePixelAndChannelCount(im2))
    BIASERR("images are not of same size or channelcount");
#endif
  if (!IsEmpty()) Release();
  Init(im1.GetWidth(), im1.GetHeight(), im1.GetChannelCount());
  unsigned int cc = im1.GetChannelCount();

  /* Do the image dimensions (or the ROI dimensions if both images have one)
     fit */
  if ( HaveImagesMatchingROI(im1, im2) ) {
    unsigned minx,  miny, maxx, maxy;
    im1.GetROICorners(minx, miny, maxx, maxy);
    StorageType **ida=GetImageDataArray();
    const StorageType **ida1=im1.GetImageDataArray();
    const StorageType **ida2=im2.GetImageDataArray();
    maxx *= cc;
    for (unsigned y=miny; y<maxy; y++){
      for (unsigned x=minx; x<maxx; x+= cc){
	for (unsigned int c=0;c<cc; c++) {
	  ida[y][x+c]=(ida1[y][x+c]>ida2[y][x+c])?
	    (ida1[y][x+c]-ida2[y][x+c]):(ida2[y][x+c]-ida1[y][x+c]);
	}
      }
    }
  } else {
    BIASERR("unmatching ROIs");
  }
}

//////////////////////////////////////////////////////////////////////////////



} // namespace BIAS
