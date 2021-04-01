/* This file is part of the BIAS library (Basic ImageAlgorithmS).

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/
#include "ImageBase.hh"

#include "Image.hh" // required for static_casting




#include <math.h>
#include <string>
#include <sstream>
using std::string;
using std::stringstream;

using namespace BIAS;
using namespace std;


ImageBase::~ImageBase()
{
  if (ImageDataArray_ != NULL) {
    delete[] (char**)ImageDataArray_;
    ImageDataArray_ = NULL;
  }
  if (ImageData_!=NULL){
    delete[] (char*)ImageData_;
    ImageData_ = NULL;
  }
}



ImageBase::ImageBase()
  : Debug(), Roi_()
{

  ImageData_ = NULL;
  ImageDataArray_ = NULL;
  ChannelCount_ = 0;
  Width_ = 0;
  Height_ =0;
  Depth_ = 0;
  BitDepth_ = 0;
  WidthStep_ = 0;
  ColorModel_ = CM_invalid;
  StorageType_=ST_invalid;
  InterleavedDataOrder_ = true;
  //_UID=BIAS::UUID();
  _MetaData=MetaData();
}

ImageBase::ImageBase(const ImageBase& im)
{

  ImageData_ = NULL;
  ImageDataArray_ = NULL;
  Width_ = 0;
  Height_ = 0;
  ChannelCount_ = 0;
  StorageType_=ST_invalid;
    //  StorageType_ = im.StorageType_; //otherwise the =operator will fail.
  (*this) = im;
}


ImageBase::ImageBase(unsigned int width, unsigned int height,
                     unsigned int channels, enum EStorageType storageType,
                     bool interleaved)
{


  ImageData_      = NULL;
  ImageDataArray_ = NULL;

  ColorModel_ = CM_invalid;
  InterleavedDataOrder_ = interleaved;

  Init(width,height,channels,storageType);

  Roi_.Resize(width, height);
  Roi_.UnsetROI();
}


int ImageBase::GetStorageSizeByte(const unsigned int & width, const unsigned int & height,
                              const unsigned int nChannels,
                              const enum EStorageType storageType
                              /* ,const EColorModel colormodel*/
                              )
{
  const int nPixel       = width * height;
  const int nBytePerPixel = nChannels * GetSizeByte(storageType);
  // the nr. of Byte does not depend on colormodel, currently
  // as the number of channels is set according to nr. of Byte,
  // not number of values (jw, bartczak 11/2007)
  return nPixel * nBytePerPixel;
}



void ImageBase::ReInit(const unsigned int & width, const unsigned int & height,
      const unsigned int nChannels,
      const enum EStorageType storageType,
      const bool interleaved,
      const EColorModel colormodel)
{
  const int current_nBytes = GetStorageSizeByte(width, height,
    nChannels, storageType /* ,colormodel */);
  const int required_nBytes = GetStorageSizeByte(GetWidth(), GetHeight(),
    GetChannelCount(), GetStorageType() /* ,GetColorModel() */);
  if (current_nBytes  != required_nBytes){
    // need allocation
    if (!IsEmpty())
      Release();
    Init(width, height, nChannels, storageType, IsInterleaved() );
  } else {
    // no allocation, keep existing data area
    // take over descripive tags for data area
    SetStorageType(storageType);
    SetInterleaved(interleaved);
    SetColorModel(colormodel);
  }
}



void ImageBase::Init(unsigned int Width, unsigned int Height,
                     unsigned int nChannels, enum EStorageType storageType,
                     bool interleaved)
{
//   if ((ImageData_!=NULL) || (ImageDataArray_ !=NULL)){
//     BIASERR("Image<StorageType>::Init should only be called once"<<
//             "\nuse Release if you want to reinit");
//   }

  if (!IsEmpty()) { Release(); }

  //BIASASSERT(ImageData_==NULL);
  //BIASASSERT(ImageDataArray_==NULL);

#ifdef DISABLE_MODULO_4_SIZE
  if (((sizeof(StorageType)*Width) % 4) != 0){
    BIASERR("Because IplImage always uses aligned image data\n"
      << "  Image only supports images with \n"
      << "  (width * sizeof(StorageType)) modulo 4 = 0.\n"
      << "  Please choose a width which is divisible by four\n"
      << "  if multiplied with sizeof(StorageType) or use either\n"
      << "  the CopyIn function (internal only) or\n"
      << "  the ImportImage function.\n");
    return; // error
  };

#  if USE_OPENCV == true
  BIASERR("you cannot use -DDISABLE_MODULO_4_SIZE and -DUSE_OPENCV together");
#  endif // USE_OPENCV == true
#endif // DISABLE_MODULO_4_SIZE

  Width_        = Width;
  Height_       = Height;
  ChannelCount_ = nChannels;
  StorageType_  = storageType;
  Roi_.Resize(Width, Height);
  Roi_.UnsetROI();
  Depth_   = GetSizeByte( this->GetStorageType() );
  BitDepth_=8*Depth_; // default, might be smaller, e.g. save 12 bit image in 16 bit ushort
  InterleavedDataOrder_ = interleaved;

  delete[] (char**)ImageData_;
  ImageData_ = (void*) new char[ GetSizeByte() ];

  switch (nChannels){
  case 4:
    SetColorModel(CM_RGBA);
    break;
  case 3:
    SetColorModel(CM_RGB);
    break;
  case 2:
    SetColorModel(CM_YUYV422);
    break;
  case 1:
    SetColorModel(CM_Grey);
    break;
  default:
    SetColorModel(CM_invalid);
    break;
  };

  if (InterleavedDataOrder_)
    WidthStep_ = Width_ * Depth_ * ChannelCount_;
  else
    WidthStep_ = Width_ * Depth_;

  UpdateImageDataArrayFromImageData_();
}


int ImageBase::UpdateImageDataArrayFromImageData_()
{
  // catch the IsEmpty NULL data case: JW
  if (IsEmpty() || GetPixelCount()==0)
  {
    if (ImageDataArray_ != NULL) {
      delete[] ImageDataArray_;
      ImageDataArray_ = NULL;
    }
    // nothing to point to because no data pixels available
    return 0;
  }

  // we have ImageData
  BIASASSERT( !IsEmpty() );
  BIASASSERT( GetPixelCount()>0 );
  BIASASSERT( GetWidth()>0 );
  BIASASSERT( GetHeight()>0 );
  BIASASSERT( GetChannelCount()>0 );

  const unsigned height=(IsPlanar())?
    (GetHeight()*GetChannelCount()) : (GetHeight());
  if (ImageDataArray_ != NULL) {
    delete[] ImageDataArray_;
    ImageDataArray_=NULL;
  }
  ImageDataArray_ = (void**) new char*[height];
  ImageDataArray_[0] = GetImageData();

  for (register unsigned int i=1; i < height; i++) {
    ImageDataArray_[i] = (void*)((char*)(ImageDataArray_[i-1]) +
      WidthStep_);
  };
  return 0;
}


void ImageBase::Release(const bool reset_storage_type) 
{
  //cout << "ImageBase::Release()" << endl;

  // check if we really need to delete the mem
  if (ImageDataArray_!=NULL) {
    delete[] (char**)ImageDataArray_;
    ImageDataArray_ = NULL;
  }
  if (ImageData_!=NULL) {
    delete[] (char*)ImageData_;
    ImageData_=NULL;
  }
  ChannelCount_ = 0;
  Width_ = Height_ = Depth_ = 0;
  WidthStep_  = 0;
  ColorModel_ = CM_invalid;
  if (reset_storage_type){
    //cout << "ImageBase::Release() has to reset storage type" << endl;
    // ImageBase::Release() is also implicitly called for templated images
    // when releasing the storage type here, it becomes possible to assign
    // a float image to an unsigned char image via ImageBase
    // use 
    // Image<float> f;
    // Image<unsigned char> uc;
    // f.Release();
    //   // also happens when calling a function which expects an ImageBase 
    //   // with and Image<StorageType> implicitly
    // ImageBase *ibf= dynamic_cast<ImageBase *>(f); 

    // (*ibf) = uc;
    BitDepth_ = 0;
    StorageType_= ST_invalid;
  }

  Roi_.Release();

  _MetaData.clear();
}

int ImageBase::StealImage(ImageBase &source) {
  if (source.IsEmpty()) {
    BIASERR("tried to steal empty image");
    return -1;
  }
  if (!IsEmpty())
    Release();
  ImageData_ = source.GetImageData();
  ImageDataArray_ = source.GetImageDataArray();
  ChannelCount_ = source.GetChannelCount();
  ColorModel_ = source.GetColorModel();
  StorageType_ = source.GetStorageType();
  InterleavedDataOrder_=!source.IsPlanar();

  Width_ = source.GetWidth();
  Height_ = source.GetHeight();
  Depth_ = source.GetDepth();
  BitDepth_ = source.GetBitDepth();
  WidthStep_ = source.GetWidthStep();

  Roi_ = *source.GetROI();

  source.ImageData_=NULL;
  source.ImageDataArray_=NULL;
  // copy(_MetaData.begin(), _MetaData.end(), source._MetaData.begin());
  _MetaData.resize(source._MetaData.size());
  copy(source._MetaData.begin(), source._MetaData.end(), _MetaData.begin());

  source._MetaData.clear();

  return 0;
}

int ImageBase::GetChannel(const ImageBase &source, const unsigned int channel)
{
  if (IsEmpty())
    Init(source.GetWidth(), source.GetHeight(), 1,source.GetStorageType());



  if (source.GetColorModel()==CM_YUYV422){
    switch (channel){
    case 0:
      memcpy(GetImageData(), source.GetImageData(),
             source.GetPixelCount() * source.GetDepth());
      break;
    case 1:
      memcpy(GetImageData(), (char*)(source.GetImageData()) +
             source.GetPixelCount()*source.GetDepth(),
             source.GetPixelCount() * source.GetDepth() / 2);
      break;
    case 2:
      memcpy(GetImageData(),
             (char*)(source.GetImageData()) + source.GetDepth()*
             (source.GetPixelCount() + source.GetPixelCount()/2),
             source.GetPixelCount() * source.GetDepth() / 2);
      break;
    default:
      BIASERR("ImageConvert<StorageType>::GetChannel() invalid Channel");
      return -1;
      break;
    }
  } else { // if
    if (source.IsPlanar()){
      switch (channel){
      case 0:
        memcpy(GetImageData(), source.GetImageData(),
               source.GetPixelCount() * source.GetDepth());
        break;
      case 1:
        memcpy(GetImageData(),
               (char*)(source.GetImageData()) +
               source.GetPixelCount()*source.GetDepth(),
               source.GetPixelCount() * source.GetDepth());
        break;
      case 2:
        memcpy(GetImageData(),
               (char*)(source.GetImageData()) + 2 * source.GetPixelCount()*
               source.GetDepth(),
               source.GetPixelCount() * source.GetDepth());
        break;
      default:
        BIASERR("ImageConvert<StorageType>::GetChannel() invalid Channel");
        return -1;
        break;
      }
    } else { // interleaved

      char *src=(char *)source.GetImageData();
      char *dst=(char *)GetImageData();
      char *end=dst+GetPixelCount()*GetDepth();
      register int depth=GetDepth();
      register int depth2=depth<<1;
      switch (channel){
      case 0:
        while (dst<end){
          for (int i=0; i<depth; i++) *dst++=*src++;
          src+=depth2;
        }
        break;
      case 1:
        while (dst<end){
          src+=depth;
          for (int i=0; i<depth; i++) *dst++=*src++;
          src+=depth;
        }
        break;
      case 2:
        while (dst<end){
          src+=depth2;
          for (int i=0; i<depth; i++) *dst++=*src++;
        }
        break;
      default: break;
      }
    } // end if IsPlanar()

  } //  end if

  return 0;
}


int ImageBase::GetCopyOfROI2(ImageBase &copy) const
{

  char **dst, **src;
  register int y;
  int ROIUpperLeftX, ROIUpperLeftY, ROILowerRightX, ROILowerRightY;
  GetROI()->GetCorners(ROIUpperLeftX, ROIUpperLeftY,
    ROILowerRightX, ROILowerRightY);
  if (!copy.IsEmpty()) copy.Release();
  copy.Init(ROILowerRightX - ROIUpperLeftX + 1,
    ROILowerRightY - ROIUpperLeftY + 1, GetChannelCount(),
    GetStorageType());
  copy.SetColorModel(GetColorModel());

  // following line must be changed if methods is extendend
  src = (char**)GetImageDataArray();
  dst = (char**)copy.GetImageDataArray();

  int length = (ROILowerRightX-ROIUpperLeftX + 1)
    * GetChannelCount() * GetDepth();
  for (y = ROIUpperLeftY; y <= ROILowerRightY; y++){
    memcpy(&(dst[y-ROIUpperLeftY][0]),
      &(src[y][ROIUpperLeftX*GetChannelCount()*GetDepth()]), length);
  }
  return 0;

}

int ImageBase::GetCopyOfROI(ImageBase &copy) const
{

  char **dst, **src;
  register int y;
  int ROIUpperLeftX, ROIUpperLeftY, ROILowerRightX, ROILowerRightY;
  GetROI()->GetCorners(ROIUpperLeftX, ROIUpperLeftY,
    ROILowerRightX, ROILowerRightY);

  if (!copy.IsEmpty()) copy.Release();
  copy.Init(ROILowerRightX - ROIUpperLeftX,
    ROILowerRightY - ROIUpperLeftY, GetChannelCount(),
    GetStorageType());
  copy.SetColorModel(GetColorModel());

  // following line must be changed if methods is extendend
  src = (char**)GetImageDataArray();
  dst = (char**)copy.GetImageDataArray();

  const int length =
    (ROILowerRightX-ROIUpperLeftX) * GetChannelCount() * GetDepth();

  for (y = ROIUpperLeftY; y < ROILowerRightY; y++){
    memcpy(&(dst[y-ROIUpperLeftY][0]),
      &(src[y][ROIUpperLeftX*GetChannelCount()*GetDepth()]), length);
  }
  return 0;
}


int ImageBase::Paste2ROI(const ImageBase& Image)
{

  int ROIUpperLeftX, ROIUpperLeftY, ROILowerRightX, ROILowerRightY;
  GetROI()->GetCorners(ROIUpperLeftX, ROIUpperLeftY,
    ROILowerRightX, ROILowerRightY);

  if ( ((int)Image.GetWidth() != (ROILowerRightX - ROIUpperLeftX)) ||
    ((int)Image.GetHeight() != (ROILowerRightY - ROIUpperLeftY)) ){
      BIASERR("ImageBase:Paste2ROI():  ROI size in destination is not equal "<<
        "to image size in source, aborting! ROI:WxH  "
        <<(ROILowerRightX - ROIUpperLeftX)<<"x"
        <<(ROILowerRightY - ROIUpperLeftY)<<"  Im:WxH "
        <<Image.GetWidth()<<"x"<<Image.GetHeight());
      BIASABORT;
      return(-1);
    }
    if (Image.GetColorModel() != GetColorModel()){
      BIASERR("ImageBase::Paste2ROI():  ColorModels do not match, aborting!");
      BIASABORT;
      return(-1);
    }

    char **dst, **src;
    register int y;

    dst = (char**)GetImageDataArray();
    src = (char**)Image.GetImageDataArray();
    int length= (ROILowerRightX-ROIUpperLeftX)*GetChannelCount()*GetDepth();
    //copy image line-wise with memcopy
    for (y = ROIUpperLeftY; y < ROILowerRightY; y++){
      memcpy(&(dst[y][ROIUpperLeftX*GetChannelCount()*GetDepth()]),
             &(src[y-ROIUpperLeftY][0]),length);
    }
    return 0;
}


int ImageBase::Cut2ROI()
{
  int ROIUpperLeftX, ROIUpperLeftY, ROILowerRightX, ROILowerRightY;
  GetROI()->GetCorners(ROIUpperLeftX, ROIUpperLeftY,
    ROILowerRightX, ROILowerRightY);

  const int bytesInRow= (ROILowerRightX-ROIUpperLeftX)*
    GetChannelCount() * GetDepth();
  char *dst = new char[(ROILowerRightY - ROIUpperLeftY)*bytesInRow];
  char **src = (char**)GetImageDataArray();

  register int y;
  int start=ROIUpperLeftX*GetChannelCount()*GetDepth();

  for (y = ROIUpperLeftY; y < ROILowerRightY; y++){
    memcpy(dst+(y-ROIUpperLeftY)*bytesInRow, &(src[y][start]),
      bytesInRow);
  }

  Width_ =  ROILowerRightX - ROIUpperLeftX;
  Height_ = ROILowerRightY - ROIUpperLeftY;
  WidthStep_=Width_*ChannelCount_*Depth_;
  GetROI()->Resize(GetWidth(),GetHeight());
  UnsetROI();
  ReleaseImageDataPointer();
  RedirectImageDataPointer((void*)dst);
  return 0;
}

void ImageBase::SetOutsideROIZero()
{
  switch (GetROI()->GetROIType()){
  case ROI_Corners:
    {
      // just for debugging
      //ImageBase tmp = *this;

      char *id = (char *)GetImageData();
      unsigned ulx, uly, lrx, lry;
      GetROI()->GetCorners(ulx, uly, lrx, lry);
      // top 
      unsigned size = uly*GetWidthStep();
      memset((void *)id, 0, size);
      // and bottom
      size = (GetHeight()-lry)*GetWidthStep();
      memset((void *)(&id[lry*GetWidthStep()]), 0, size);
      // left and right
      unsigned left_size, right_offs, right_size, y_offs;
      left_size = ulx*GetChannelCount()*GetDepth();
      right_offs = lrx*GetChannelCount()*GetDepth();
      right_size = (GetWidth()-lrx)*GetChannelCount()*GetDepth();
      for (unsigned y=uly; y<lry; y++){
        y_offs = y*GetWidthStep();
        memset((void *)(&id[y_offs]), 0, left_size);
        memset((void *)(&id[y_offs+right_offs]), 0, right_size);
      }

      /* just for debugging from here
      id = (char *)tmp.GetImageData();
      for (unsigned y=0; y<GetHeight(); y++){
        for (unsigned x=0; x<GetWidth(); x++){
          if (!GetROI()->IsInROI(x, y))
            memset((void *)
                   (&id[y*GetWidthStep()+x*GetChannelCount()*GetDepth()]),
                   0, GetChannelCount()*GetDepth());
        }
      }
      size = GetWidth()*GetHeight()*GetChannelCount()*GetDepth();
      for (unsigned i=0; i<size; i++){
        if (((char *)tmp.GetImageData())[i] != 
            ((char *)GetImageData())[i]){
          ABORT;
        } else if (i%25==0){
          cout << "." << flush;
        }
      }*/
    }
    break;
  default:
    {
      char *id = (char *)GetImageData();
      for (unsigned y=0; y<GetHeight(); y++){
        for (unsigned x=0; x<GetWidth(); x++){
          if (!GetROI()->IsInROI(x, y))
            memset((void *)
                   (&id[y*GetWidthStep()+x*GetChannelCount()*GetDepth()]),
                   0, GetChannelCount()*GetDepth());
        }
      }
    }
    break;
  }
}

//////////////////////////////////////////////////////////////////////////////
//
// Assignment Operator
//
//////////////////////////////////////////////////////////////////////////////
ImageBase& ImageBase::operator=(const ImageBase& Source)
{
  //cout << "ImageBase operator= " << endl;
  /// always copy the members that do not depend on image data
  /// to allow feeding Image* into ImageBase*. JW 01/2005


  bool skipAlloc=false;
  // determine if we can keep the data area or need to realllcoate JW:
  if ( !(this->IsEmpty()) && (Source.IsEmpty()) ){
    // src is empty, so should be this.
  /*  cout << "JW calls Release() now: Source empty? " << boolalpha << Source.IsEmpty()
	 << ", this empty? " << boolalpha << IsEmpty() << endl;*/
    Release();
    // not done because the members need to be set.
    //return *this;
    skipAlloc=true;
  };

  // skip allocation if there is no data in source image,
  // required for STL vector allocation
  if (Source.IsEmpty()) {
    skipAlloc=true;
  }

  if (GetStorageType()!=ST_invalid){
  /*  cout << "ImageBase operator=(): storage type source = " << Source.GetStorageType()
	 << ", this = " << GetStorageType() << endl;*/
    if (Source.GetStorageType()!=GetStorageType()){
      BEXCEPTION("Can not assign images of different storage type! Call Release(true) on destination image, if this is intented.");
    }
  }

  if ((!this->IsEmpty()) && (!SamePixelAndChannelCount(Source) )) {
    // sizes or type don't match, realloc
    Release();
  };

  if (this->IsEmpty() && !skipAlloc){
    Init(Source.GetWidth(), Source.GetHeight(), Source.GetChannelCount(),
         Source.GetStorageType());
  }

  if (!Source.IsEmpty()){
    // copy image data:
    memcpy(this->GetImageData(), // dest
           Source.GetImageData(), //src
           Source.GetPixelCount() * Source.GetDepth() *
           Source.GetChannelCount());
  }


  // copy remaining member values:
  StorageType_  = Source.GetStorageType();
  ColorModel_   = Source.GetColorModel();
  ChannelCount_ = Source.GetChannelCount();
  Depth_        = Source.GetDepth();
  BitDepth_     = Source.GetBitDepth();
  Width_        = Source.GetWidth();
  Height_       = Source.GetHeight();
  InterleavedDataOrder_ = ! Source.IsPlanar();
  WidthStep_    = Source.WidthStep_;
  //_UID          = Source.GetUID();
  _MetaData     = Source._MetaData;
  Roi_          = *Source.GetROI();
  // update pointers to begin of line for new allocated image data area.
  this->UpdateImageDataArrayFromImageData_();

  return *this;
}


void ImageBase::CopyIn_NoInit(void *data)
{
  BIASDOUT(D_COPYIN," entering CopyIn_NoInit()");
  memcpy(GetImageData(), data, GetPixelCount()*GetChannelCount()*GetDepth());
}


int ImageBase::Flip()
{
  int size=GetPixelCount() * GetChannelCount() * GetDepth(); // in byte
  if (size<=0) return -1;

  char *newData = NULL;
  //TODO: this is causing a bad allocation on OpenGL screenshot... (JW DBG)
  //try {
  newData  = new char[size];
  //} catch( bad_alloc &ba) {
  // BIASERR( ba.what() );
  // return -1;
  //};


  register unsigned int length =  GetWidth() * GetChannelCount() * GetDepth();
  if (InterleavedDataOrder_) {
    register char *targetData = newData + size - length;
    for (register unsigned int i=0; i < Height_; i++) {
      memcpy(targetData,(char*)(ImageDataArray_[i]),length);
      targetData -= length;
    }
  }
  else { // planar
    BIASERR("Flip not implemented for planar images.");
    /*
    register int rowSize=Width_*sizeof(StorageType);
    register StorageType *targetData = newData+
    (Height_-1)*rowSize;
    register StorageType *sourceData = ImageData_;
    for (int channel=0;channel<ChannelCount_;channel++) {
    for (register int i=0; i < Height_; i++) {
    memcpy(targetData,sourceData,rowSize);
    targetData-=rowSize;
    sourceData+=rowSize;
    }
    targetData+=(2*Height_-1)*rowSize;
    } */
  }
  ReleaseImageDataPointer();
  RedirectImageDataPointer((void*)newData);
  return 0;
}

int ImageBase::FlipHorizontal()
{
  int size=GetPixelCount() * GetChannelCount() * GetDepth(); // in byte
  if (size<=0) return -1;
  int pixelsize=GetChannelCount() * GetDepth();
  char *newData = new char[size];
  if (InterleavedDataOrder_) {
    register char *targetData;
    register char *srcData;
    for (register unsigned int i=0; i < Height_; i++) {
      targetData = newData + (i+1)*WidthStep_ - pixelsize;
      srcData = (char *)(ImageDataArray_[i]);
      for (register int x=0; x<(int)Width_; x++){
        memcpy(targetData-x*pixelsize, srcData+x*pixelsize, pixelsize);
      }
    }
  } else { // planar
    BIASERR("FlipHorizontal not implemented for planar images.");
  }
  ReleaseImageDataPointer();
  RedirectImageDataPointer((void*)newData);
  return 0;
}




void ImageBase::PrintHeader(ostream& os) const
{
  os
    << "\nVersion_ : \t\t" << Version_
    << "\nStorageType_ : \t\tImageBase::"<<StorageType_
    << "\nColorModel_ : \t\tImageBase::"<< ColorModel_
    << " ("<<(int)ColorModel_<<")"
    << "\nWidth_: \t\t" << Width_
    << "\nHeight_: \t\t" << Height_
    << "\nnChannelCount_: \t" << ChannelCount_
    << "\nWidthStep_: \t\t" << WidthStep_
    << "\nDepth_: \t\t" << Depth_
    << "\nBitDepth_: \t\t" << BitDepth_
    << "\nPixelCount_: \t\t" << GetPixelCount()
    << "\nInterleavedDataOrder_: \t" << InterleavedDataOrder_
    << "\nROI: \t" << Roi_
    <<   "\nUUID: \t\t\t"<<"0"//GetUID()
    << " ("<</*(GetUID().IsValid()?"valid":"invalid")*/"5"<<")";
  os<<"\nImageData_ ptr:       \t\t"
    //<<hex
    <<(long)ImageData_;
  os<< "\nImageDataArray_ ptr: :\t\t"
    //<<hex
    <<(long)ImageDataArray_;
#ifdef BIASDEBUG
  os << "\nDebuglevel_:\t" << Debuglevel_;
#endif
  os<<"\n"<<dec;
}




// JW 09/2003
bool
ImageBase::IsPowerOfTwoSize() const {
  //unsigned int w2, h2;
  //return PowerOfTwoSize(w2,h2);
  int w=(int)GetWidth();
  int h=(int)GetHeight();
  return IsPowerOfTwoSize(w, h);
}

// static JW
bool
ImageBase::IsPowerOfTwoSize(const unsigned int w,
                            const unsigned int h)
{
  // JW 09/03: more efficient is this with bitmasks :-)
  return (((w&(w-1))==0) && ((h&(h-1))==0));
}



// Jan Woetzel
bool
ImageBase::PowerOfTwoSize(unsigned int & width2, unsigned int & height2) const {
  // next pow2 sizes:
  width2  = PowerOfTwoSize( GetWidth() );
  height2 = PowerOfTwoSize( GetHeight() );

  return ( (GetWidth()==width2) && (GetHeight()==height2));
}

// Jan Woetzel
unsigned int
ImageBase::PowerOfTwoSize(const unsigned int & val) {
  // special case
  if (val==0)
    return 0;

  // loop is faster than ldexp etc...
  unsigned int res = 1;
  while (res < val)
    res <<= 1;
  return res;
}


// JW
bool ImageBase::DimensionMatch(const BIAS::ImageBase & other) const {
  // return treu of width and height matches. don't are about types or channel count
  return (  (this->GetWidth() == other.GetWidth())
    && (this->GetHeight() == other.GetHeight()) );
}


bool ImageBase::FormatMatch(const BIAS::ImageBase & d ) const {
  if (!DimensionMatch(d))
    return false;
  // check twice just to be sure.
  if (GetWidth() != d.GetWidth())
    return false;
  if (GetHeight() != d.GetHeight())
    return false;
  if (GetDepth() != d.GetDepth())
    return false;
  if (GetBitDepth() != d.GetBitDepth())
    return false;
  if (GetChannelCount() != d.GetChannelCount())
    return false;
  if (GetSizeByte() != d.GetSizeByte())
    return false;
  if (GetColorModel() != d.GetColorModel())
    return false;
  if (GetWidthStep() != d.GetWidthStep())
    return false;
  //if (s.GetPixelFormat() != d.GetPixelFormat())
  //  return false;

  return true; // fits.
}


void *ImageBase::PixelValueBase(unsigned x, unsigned y, unsigned channel)
{
  BIASERR("warnig, PixelValueBase is deprecated and will be removed in the "
    "future,\nuse GetImageDataArray() instead!");
  if (InterleavedDataOrder_)
    return (void *)(&((char*)ImageData_)[(y*WidthStep_*Depth_+
    x*Depth_*ChannelCount_+
    channel)]);
  else
    return (void *)(&((char*)ImageData_)[channel*Width_*Height_*Depth_+
    y*Width_*Depth_+x*Depth_]);
}

/// JW: there's no reason to drop this interface - be API backward compatible!
int ImageBase::SetROI(unsigned int UpperLeftX, unsigned int UpperLeftY,
                      unsigned int LowerRightX, unsigned int LowerRightY)
{
  return SetROICorners(
    UpperLeftX,  UpperLeftY,
    LowerRightX,  LowerRightY);
}

int
ImageBase::SetROI(const ROI& roi) {
  *GetROI() = roi;	
  return 0;
}


int ImageBase::SetROICorners(unsigned int UpperLeftX, unsigned int UpperLeftY,
                             unsigned int LowerRightX, unsigned int LowerRightY)
{
  return GetROI()->SetCorners(
    UpperLeftX,  UpperLeftY,
    LowerRightX,  LowerRightY);
}


void ImageBase::UnsetROI()
{
  GetROI()->UnsetROI();
}

void ImageBase::GetROI(unsigned int& UpperLeftX, unsigned int& UpperLeftY,
                       unsigned int& LowerRightX, unsigned int& LowerRightY) const
{
  GetROICorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
}
void ImageBase::GetROICorners(unsigned int& UpperLeftX, unsigned int& UpperLeftY,
                              unsigned int& LowerRightX, unsigned int& LowerRightY) const
{
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
}


void ImageBase::GetROI( int& UpperLeftX,  int& UpperLeftY,
                       int& LowerRightX,  int& LowerRightY) const
{
  GetROICorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
}
void ImageBase::GetROICorners(int& UpperLeftX, int& UpperLeftY,
                              int& LowerRightX, int& LowerRightY) const
{
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
}



void ImageBase::PrintROI(std::ostream& os) const
{
  os << *GetROI();
}

const unsigned int ImageBase::GetROIUpperLeftX() const
{
  unsigned UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY;
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
  return UpperLeftX;
}

const unsigned int ImageBase::GetROIUpperLeftY() const
{
  unsigned UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY;
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
  return UpperLeftY;
}

const unsigned int ImageBase::GetROILowerRightX() const
{
  unsigned UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY;
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
  return LowerRightX;
}

const unsigned int ImageBase::GetROILowerRightY() const
{
  unsigned UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY;
  GetROI()->GetCorners(UpperLeftX,  UpperLeftY,  LowerRightX,  LowerRightY);
  return LowerRightY;
}


// JW
int ImageBase::ZeroPad(const unsigned int newwidth,
                       const unsigned int newheight,
                       unsigned char bgcolor )
{
  return this->Pad(newwidth, newheight, bgcolor );
}


// JW
int ImageBase::PadToPowerOfTwo(const int & padVal)
{
  unsigned int w2, h2;
  // do nothing if image is already correct size
  if (PowerOfTwoSize(w2,h2)) {
    // do nothing.
    return 0;
  }

  // create a temporary
  // to use the existing out of place impl.
  ImageBase img;
  int result=PadToPowerOfTwo( img, padVal );

  // assignment to emulaet in-place
  *this = img;

  return  result;
}


// JW
int
ImageBase::PadToPowerOfTwo(BIAS::ImageBase & dest,
                           const int & padVal) const
{

  BIASASSERT( IsInterleaved() );

  unsigned int w2, h2; // desired dest size
  if (PowerOfTwoSize(w2,h2))
  {
    // is already a power of two - just copy, no padding
    // constructor is always safe, operator= not
    dest=ImageBase( *this );
    return 0;
  };

  // call general Padding routine
  return Pad(dest, w2, h2, padVal);
}




// JW
int
ImageBase::Pad(const unsigned int & newwidth,
               const unsigned int & newheight,
               const int & padVal)
{
  // do nothing if image is already correct size
  if ((newwidth==GetWidth()) && (newheight==GetHeight()))
  {
    // do nothing.
    return 0;
  }

  // create a temporary
  // to use the existing out of place impl.
  ImageBase img;
  int result=Pad( img, newwidth, newheight, padVal );

  // assignment to emulaet in-place
  *this = img;

  return  result;
}


// JW
int
ImageBase::Pad(BIAS::ImageBase & dest,
               const unsigned int & newwidth,
               const unsigned int & newheight,
               const int & padVal) const
{

  if (!IsInterleaved())
  {
    BIASERR("can work on interleaved images, only. Sorry.");
    return -1;
  }
  if (newwidth < GetWidth() || newheight < GetHeight() )
  {
    BIASERR("can only increase image size. Sorry.");
    return -1;
  }
  BIASASSERT( IsInterleaved() );
  BIASASSERT( GetWidth()  <= newwidth  );
  BIASASSERT( GetHeight() <= newheight );

  //
  // OK
  //

  // pad and copy from upper left origin
  dest=ImageBase(newwidth, newheight, GetChannelCount(), GetStorageType(), 
                 IsInterleaved() );
  dest.SetColorModel( GetColorModel() );
  dest.SetBitDepth( GetBitDepth() );

  // compute bytes per row for src and dest:
  const unsigned int src_BytePerRow  = GetDepth() * GetWidth() * GetChannelCount();
  const unsigned int dest_BytePerRow = GetDepth() * dest.GetWidth() * dest.GetChannelCount();

  //
  // dest image is:
  //   SSS R
  //   BBBBB
  //
  // S,R:  for all source rows
  // copy overlap part from left and init non-overlap data.
  for (unsigned int ys=0; ys<GetHeight(); ys++)
  {
    // S:
    // copy:   dest* , src*, n
    memcpy(
      (void*) dest.GetImageDataArray()[ys],
      (void*) GetImageDataArray()[ys],
      src_BytePerRow );

    // R:
    // for all lines of orig image: pad right part
    if (dest_BytePerRow > src_BytePerRow) {
      // set remainder of row to zero in dest img.
      // set    dest*, val, n
      memset(
        (void*) (& ((unsigned char*)(dest.GetImageDataArray()[ys]))[src_BytePerRow])
        ,padVal
        ,dest_BytePerRow-src_BytePerRow );
    };
  };

  // B: bottom rectangle
  if (GetHeight()<dest.GetHeight())
  {
    // set    dest*, val, n
    memset((void*) dest.GetImageDataArray()[GetHeight()],
      padVal,
      dest_BytePerRow*(dest.GetHeight()-GetHeight()) );
  };
  return 0;
}


// JW
int ImageBase::PadToPowerOfTwoAndFlip(const int & padVal)
{
  /// \todo replace with linewise memmov
  if (0==Flip())
  {
    return PadToPowerOfTwo(padVal);
  } else {
    BIASERR("could not flip. skipped padding.");
    return -1;
  };
}


// JW
// static
int ImageBase::GetSizeByte(const enum BIAS::ImageBase::EStorageType & storagetype)
{
  switch (storagetype)
  {
  case BIAS::ImageBase::ST_unsignedchar:
  case BIAS::ImageBase::ST_char:
    //BIASASSERT(sizeof(char)==1);
    return sizeof(char);
  case BIAS::ImageBase::ST_unsignedshortint:
  case BIAS::ImageBase::ST_shortint:
    //BIASASSERT(sizeof(short int)==2);
    return sizeof(short int);
  case BIAS::ImageBase::ST_unsignedint:
  case BIAS::ImageBase::ST_int:
    //BIASASSERT(sizeof(int)==4);
    return sizeof(int);
  case BIAS::ImageBase::ST_float:
    //BIASASSERT(sizeof(float)==4);
    return sizeof(float);
  case BIAS::ImageBase::ST_double:
    //BIASASSERT(sizeof(double)==8);
    return sizeof(double);
  case BIAS::ImageBase::ST_invalid:
    // useful e.g. for creating vector of empty images
    return 0;
  default:
    BIASERR( "unsupported StorageType:"<<storagetype);
    BIASBREAK;
  };
  return 0;
}

int ImageBase::GetSizeUnits(const enum BIAS::ImageBase::EColorModel & colormodel){
  switch (colormodel)
  {
  case BIAS::ImageBase::CM_RGBA:
  case BIAS::ImageBase::CM_BGRA:
    return 4;

  case BIAS::ImageBase::CM_HSV:
  case BIAS::ImageBase::CM_HSL:
  case BIAS::ImageBase::CM_hsL:
    BIASWARN("you are possibly treating a HSV/HSL/hsL as RGB.");
  case BIAS::ImageBase::CM_BGR:
  case BIAS::ImageBase::CM_RGB:
    return 3;

  case BIAS::ImageBase::CM_GreyA:
    return 2;

  case BIAS::ImageBase::CM_Grey:
    return 1;

  case BIAS::ImageBase::CM_Bayer_BGGR:
  case BIAS::ImageBase::CM_Bayer_RGGB:
  case BIAS::ImageBase::CM_Bayer_GRBG:
  case BIAS::ImageBase::CM_Bayer_GBRG:
    return 1; // packed: 3 values in 1 Bytes
  case BIAS::ImageBase::CM_PGR_XB3_F7M3_GBRG:
    return 3; // packed: 9 values in 3 Bytes

  default:
    BIASERR( "unsupported/unknown color model "<<colormodel);
  };
  return 0;
}


// JW
int ImageBase::GetChannelcount(const enum BIAS::ImageBase::EColorModel & colormodel)
{
  switch (colormodel)
  {
  case BIAS::ImageBase::CM_RGBA:
  case BIAS::ImageBase::CM_BGRA:
    return 4;

  case BIAS::ImageBase::CM_HSV:
  case BIAS::ImageBase::CM_HSL:
  case BIAS::ImageBase::CM_hsL:
    BIASWARN("you are possibly treating a HSV/HSL/hsL as RGB.");
  case BIAS::ImageBase::CM_BGR:
  case BIAS::ImageBase::CM_RGB:
    return 3;

  case BIAS::ImageBase::CM_GreyA:
    return 2;

  case BIAS::ImageBase::CM_Grey:
    return 1;

  case BIAS::ImageBase::CM_Bayer_BGGR:
  case BIAS::ImageBase::CM_Bayer_RGGB:
  case BIAS::ImageBase::CM_Bayer_GRBG:
  case BIAS::ImageBase::CM_Bayer_GBRG:
    //BIASWARN("you are possibly treating a Bayer image as RGB.");
    return 3;
  case BIAS::ImageBase::CM_PGR_XB3_F7M3_GBRG:
    // 3 Bytes/channels containing 9 packed values
    // nChannels is determined by raw data size,
    // not descriptive meta tags such as ColorModel
    return 3;


  default:
    BIASERR( "unsupported/unknown color model "<<colormodel);
  };
  return 0;
}



/*int ImageBase::Display(const std::string & DestWin,
                       const bool & autoresize,
                       const bool & moveToTopLeft,
                       const bool & waitForKey,
                       const unsigned int & delayMsec,
                       const float &scale,
                       const bool & allowAlphaWindow
                       ) const
{
  // TODO: avoid copy for const
  //ImageBase tmp(*this);
  //WrapBias2Ipl wrap (&tmp);

  const WrapBias2Ipl wrap (this);
  return wrap.Display(
    DestWin,
    autoresize,
    moveToTopLeft,
    waitForKey,
    delayMsec,
    scale,
    allowAlphaWindow
    );
}
*/

/*int ImageBase::Display(const std::string & DestWin) const
{
  // wait just one msec for update, do not nlock program run.
  return this->Display(
    DestWin
    ,true // autoresize
    ,false // moveToTopLeft
    ,false // don't waitForKey
    ,1     // delayMsec
    );
}*/

/*int ImageBase::Display(const bool & waitForKey,
                       const unsigned int & delayMsec,
                       const float &scale ) const
{
  return this->Display(
    string( DEFAULT_WrapBias2Ipl_WINNAME ),
    true, false,
    waitForKey, delayMsec, scale );
}

*/
/*int ImageBase::Display() const
{
  return this->Display(
    true,
   DEFAULT_Display_delay,
   DEFAULT_32to8_scale
    );
}*/



// fixes for building dlls
namespace BIAS {

  std::ostream& operator<<(std::ostream& os, const ImageBase& img2)
  {
    // TODO: remove dirty const_cast by making req. function in ImageBase const
    // to use reinterpret_cast<> we first have to unconst:
    ImageBase &img = const_cast<ImageBase&>(img2);

    //enum BIAS::ImageBase::EStorageType storageType = img.GetStorageType();
    int Version = img.GetVersionNumber();
    int depthondisk;
    // added in Version 300 to identify mipimage on disk similar to magic number
    os.write(IDENTIFIER, IDENTIFIER_LENGTH);
    os.write(reinterpret_cast<char*>(&img.StorageType_), sizeof(BIAS::ImageBase::EStorageType));
    os.write(reinterpret_cast<char*>(&Version), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.ChannelCount_), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.Width_), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.Height_), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.ColorModel_), sizeof(BIAS::ImageBase::EColorModel));
    depthondisk = img.Depth_ * 8;
    os.write(reinterpret_cast<char*>(& depthondisk), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.BitDepth_), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
    int PixelCount = img.GetPixelCount();
    os.write(reinterpret_cast<char*>(&PixelCount), sizeof(int));
    os.write(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));

    // version 311: now writing unique image id as a string,
    // make sure that id is valid, since IsValid_ is not written

    std::string id;
   // img._UID.GetString(id);
    os.write(id.c_str(), 36);
    if (img.GetROI()->WriteBinary(os)!=0){
      BIASERR("error writing ROI");
    }

    // finally write image data:
    BIASASSERT(img.GetImageData()!=NULL); // jw
    // TOOD seems to cause access to uninitialized value(s) by valgrind
    // Syscall param write(buf) points to uninitialized Bytes( ExampleMetaData
    //( JW)
    os.write(
      reinterpret_cast<char*>(img.GetImageData()),
      img.GetPixelCount() * img.GetChannelCount() * img.Depth_
      );

    return os;
  }

  std::ostream& operator<<(std::ostream& os,
    const ImageBase::EStorageType &st)
  {
    switch (st)
    {
    case ImageBase::ST_invalid: os<< "ST_invalid"; break;
    case ImageBase::ST_unsignedchar: os<< "ST_unsignedchar"; break;
    case ImageBase::ST_char: os<< "ST_char"; break;
    case ImageBase::ST_unsignedshortint: os<< "ST_unsignedshortint"; break;
    case ImageBase::ST_shortint: os<< "ST_shortint"; break;
    case ImageBase::ST_unsignedint: os<< "ST_unsignedint"; break;
    case ImageBase::ST_int: os<< "ST_int"; break;
    case ImageBase::ST_float: os<< "ST_float"; break;
    case ImageBase::ST_double: os<< "ST_double"; break;
    default: os<<"unknown StorageType"; break;
    }
    return os;
  }


  std::ostream& operator<<(std::ostream& os,
    const ImageBase::EColorModel &st)
  {
    switch (st)
    {
    case ImageBase::CM_invalid: os<< "CM_invalid"; break;
    case ImageBase::CM_Grey: os<< "CM_Grey"; break;
    case ImageBase::CM_RGB: os<< "CM_RGB"; break;
    case ImageBase::CM_BGR: os<< "CM_BGR"; break;
    case ImageBase::CM_YUYV422: os<< "CM_YUYV422"; break;
    case ImageBase::CM_UYVY422: os<< "CM_UYVY422"; break;
    case ImageBase::CM_YUV420P: os<< "CM_YUV420P"; break;
    case ImageBase::CM_YUV444: os<< "CM_YUV444"; break;
    case ImageBase::CM_YUV411: os<< "CM_YUV411"; break;
    case ImageBase::CM_HSV: os<< "CM_HSV"; break;
    case ImageBase::CM_HSI_OBS: os<< "CM_HSI is obsolete use HSL"; break;
    case ImageBase::CM_DV: os<< "CM_DV"; break;
    case ImageBase::CM_RGBA: os<< "CM_RGBA"; break;
    case ImageBase::CM_GreyA: os<< "CM_Bayer_GreyA"; break;
    case ImageBase::CM_Bayer_RGGB: os<< "CM_Bayer_RGGB"; break;
    case ImageBase::CM_Bayer_GBRG: os<< "CM_Bayer_GBRG"; break;
    case ImageBase::CM_Bayer_GRBG: os<< "CM_Bayer_GRBG"; break;
    case ImageBase::CM_Bayer_BGGR: os<< "CM_Bayer_BGGR"; break;
    case ImageBase::CM_HSL: os << "CM_HSL"; break;
    case ImageBase::CM_hsL: os << "CM_hsL"; break;
    case ImageBase::CM_SymTensor2x2: os << "CM_SymTensor2x2"; break;
    case ImageBase::CM_BGRA: os << "CM_BGRA"; break;
    case ImageBase::CM_RGBE: os << "CM_RGBE"; break;
    case ImageBase::CM_PGR_XB3_F7M3_GBRG: os << "CM_PGR_XB3_F7M3_GBRG"; break;
    case ImageBase::CM_DepthAndVariance:   os << "CM_DepthAndVariance";break;
    case ImageBase::CM_Disparity:   os << "CM_Disparity";break;
    case ImageBase::CM_Depth:   os << "CM_Depth";break;
    case ImageBase::CM_YUYV:  os<< "CM_YUYV"; break;
    case ImageBase::CM_LUV:   os << "CM_LUV";break;
    case ImageBase::CM_XYZ:   os << "CM_XYZ";break;
    case ImageBase::CM_LAB:   os << "CM_LAB";break;
    case ImageBase::CM_DOES_NOT_EXIST: os << "CM_DOES_NOT_EXIST (end of list marker)"; break;

    default: os<<"unknown ColorModel"; break;
    }
    return os;
  }

  void ImageBase::StringToColorModel(const std::string& str,
                                     ImageBase::EColorModel &cm)
  {
    
    cm=ImageBase::CM_invalid;
    if(str=="CM_Grey") cm = ImageBase::CM_Grey ;
    if(str=="CM_RGB") cm = ImageBase::CM_RGB;
    if(str=="CM_BGR") cm = ImageBase::CM_BGR;
    if(str=="CM_YUYV422") cm = ImageBase::CM_YUYV422;
    if(str=="CM_UYVY422") cm = ImageBase::CM_UYVY422;
    if(str=="CM_YUV420P") cm =ImageBase::CM_YUV420P;
    if(str=="CM_YUV444") cm =ImageBase::CM_YUV444;
    if(str=="CM_YUV411") cm = ImageBase::CM_YUV411;
    if(str=="CM_HSV") cm = ImageBase::CM_HSV;
    if(str=="CM_HSI is obsolete use HSL") cm =ImageBase::CM_HSI_OBS;
    if(str=="CM_DV") cm = ImageBase::CM_DV;
    if(str=="CM_RGBA") cm = ImageBase::CM_RGBA;
    if(str=="CM_Bayer_GreyA") cm = ImageBase::CM_GreyA;
    if(str=="CM_Bayer_RGGB") cm = ImageBase::CM_Bayer_RGGB;
    if(str=="CM_Bayer_GBRG") cm = ImageBase::CM_Bayer_GBRG;
    if(str=="CM_Bayer_GRBG") cm = ImageBase::CM_Bayer_GRBG;
    if(str=="CM_Bayer_BGGR") cm = ImageBase::CM_Bayer_BGGR;
    if(str=="CM_HSL") cm = ImageBase::CM_HSL;
    if(str=="CM_hsL") cm = ImageBase::CM_hsL;
    if(str=="CM_SymTensor2x2") cm = ImageBase::CM_SymTensor2x2;
    if(str=="CM_BGRA") cm = ImageBase::CM_BGRA;
    if(str=="CM_RGBE") cm = ImageBase::CM_RGBE;
    if(str=="CM_PGR_XB3_F7M3_GBRG") cm = ImageBase::CM_PGR_XB3_F7M3_GBRG;
    if(str== "CM_DepthAndVariance") cm = ImageBase::CM_DepthAndVariance;
    if(str== "CM_Disparity") cm = ImageBase::CM_Disparity;
    if(str== "CM_Depth") cm = ImageBase::CM_Depth;
    if(str== "CM_YUYV") cm = ImageBase::CM_YUYV;
    if(str== "CM_LUV") cm = ImageBase::CM_LUV;
    if(str== "CM_XYZ") cm = ImageBase::CM_XYZ;
    if(str== "CM_LAB") cm = ImageBase::CM_LAB;
  }

  void ImageBase::StringToStorageType(const std::string& str,
    ImageBase::EStorageType &st)  
  {
    st=ST_invalid;
    if(str=="ST_unsignedchar") st = ImageBase::ST_unsignedchar;
    if(str=="ST_char") st = ImageBase::ST_char;
    if(str=="ST_unsignedshortint") st = ImageBase::ST_unsignedshortint;
    if(str=="ST_shortint") st = ImageBase::ST_shortint;
    if(str=="ST_unsignedint") st = ImageBase::ST_unsignedint;
    if(str=="ST_int") st = ImageBase::ST_int;
    if(str=="ST_float") st = ImageBase::ST_float;
    if(str=="ST_double") st = ImageBase::ST_double;      
  }


  std::istream& operator>>(std::istream& is, BIAS::ImageBase& img)
  {
    enum BIAS::ImageBase::EStorageType FileStorageType;
    int Version;
    int Width, Height, ChannelCount, PixelCount;
    char identifier[IDENTIFIER_LENGTH];
    char cBuffer[37];
    bool roidefined;

    is.read(identifier, IDENTIFIER_LENGTH);
    if (identifier[0] != 'M' || identifier[1] != 'I' || identifier[2] != 'P'){
      BIASERR("no identifier in image, maybe old version or no MIP image"
        <<"\ntry mipconvertversion in Image/test");
      is.setstate(std::ios::badbit);
      return is;
    }
 
    is.read(reinterpret_cast<char*>(&FileStorageType),
            sizeof(BIAS::ImageBase::EStorageType));
    if (img.GetStorageType()!=ImageBase::ST_invalid){
      if (FileStorageType!=img.GetStorageType()){
        // please ensure that TestImageIO works correctly after changeing
        // code here
        //BIASERR("ImageBase operator>> cannot read "<<FileStorageType
        //      << " into ImageBase with storage type "<<img.GetStorageType());
        is.setstate(ios_base::failbit);
        return is;
      }
    }
   if ( ! img.IsEmpty() )
      img.Release();

    is.read(reinterpret_cast<char*>(&Version), sizeof(int));
    switch (Version)
    {
    case 320: // new ROI class
      //  std::cout <<"Opening file with version MIP320"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_),
              sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.BitDepth_), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      img.UpdateImageDataArrayFromImageData_();
      // read uid:
      is.read(cBuffer, 36);
      cBuffer[36]=0;
      //img._UID.SetFromString(cBuffer);
      if (img.GetROI()->ReadBinary(is)!=0){
        BIASERR("error reading roi");
      }
      // read image data
      is.read(reinterpret_cast<char*>(img.ImageData_),
        img.GetPixelCount() * img.ChannelCount_ * img.Depth_);
      break;
    case 313: // removed ROIIsDefined_
      //    std::cout <<"Opening file with version MIP313"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_),
        sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.BitDepth_), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      {
        unsigned UpperLeftX, UpperLeftY, LowerRightX, LowerRightY;
        is.read(reinterpret_cast<char*>(&UpperLeftX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&UpperLeftY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightY), sizeof(unsigned int));
        // not necessary
        //img.GetROI()->Resize(Width, Height);
        img.GetROI()->SetCorners(UpperLeftX, UpperLeftY,
          LowerRightX, LowerRightY);
        //      cout <<"("<<UpperLeftX<<", "<<UpperLeftY<<") <--> ("
        //   <<LowerRightX<<", "<<LowerRightY<<")"<<endl;
      }
      // read uid:
      is.read(cBuffer, 36);
      cBuffer[36]=0;
      //img._UID.SetFromString(cBuffer);

      // read image data
      is.read(reinterpret_cast<char*>(img.ImageData_),
        img.GetPixelCount() * img.ChannelCount_ * img.Depth_);
      break;
    case 312: // removed ROIIsDefined_
      //    std::cout <<"Opening file with version MIP312"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_), sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      {
        unsigned UpperLeftX, UpperLeftY, LowerRightX, LowerRightY;
        is.read(reinterpret_cast<char*>(&UpperLeftX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&UpperLeftY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightY), sizeof(unsigned int));
        // not necessary
        //img.GetROI()->Resize(Width, Height);
        img.GetROI()->SetCorners(UpperLeftX, UpperLeftY,
          LowerRightX, LowerRightY);
      }
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightX_), sizeof(unsigned int));
      //      is.read(reinterpret_cast<char*>(&img.ROILowerRightY_), sizeof(unsigned int));
      // read uid:
      is.read(cBuffer, 36);
      cBuffer[36]=0;
      //img._UID.SetFromString(cBuffer);

      // read image data
      is.read(reinterpret_cast<char*>(img.ImageData_), img.GetPixelCount() * img.ChannelCount_
        * img.Depth_);
      break;
    case 311: // solved stack corruption problem due to wrong id writing/loading
      //  std::cout <<"Opening file with version MIP311"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_), sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      {
        unsigned UpperLeftX, UpperLeftY, LowerRightX, LowerRightY;
        is.read(reinterpret_cast<char*>(&UpperLeftX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&UpperLeftY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
        // not necessary
        //img.GetROI()->Resize(Width, Height);
        if (roidefined)
          img.GetROI()->SetCorners(UpperLeftX, UpperLeftY,
          LowerRightX, LowerRightY);
      }
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
      //       if (!roidefined){
      //         img.ROIUpperLeftX_=img.ROIUpperLeftY_=0;
      //         img.ROILowerRightX_=img.Width_; img.ROILowerRightY_=img.Height_;
      //       }
      // read uid:
      is.read(cBuffer, 36);
      cBuffer[36]=0;
      //img._UID.SetFromString(cBuffer);

      // read image data
      is.read(reinterpret_cast<char*>(img.ImageData_), img.GetPixelCount() * img.ChannelCount_
        * img.Depth_);
      break;
    case 310: // introduced unique image id in this version
      //   std::cout <<"Opening file with version MIP310"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_), sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      {
        unsigned UpperLeftX, UpperLeftY, LowerRightX, LowerRightY;
        is.read(reinterpret_cast<char*>(&UpperLeftX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&UpperLeftY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
        // not necessary
        //img.GetROI()->Resize(Width, Height);
        if (roidefined)
          img.GetROI()->SetCorners(UpperLeftX, UpperLeftY,
          LowerRightX, LowerRightY);
      }
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
      //       if (!roidefined){
      //         img.ROIUpperLeftX_=img.ROIUpperLeftY_=0;
      //         img.ROILowerRightX_=img.Width_; img.ROILowerRightY_=img.Height_;
      //       }
      // do not read uid:
      { /// \bug fake reading of uuid because of bug, id is DISABLED !
        /*char *tmp=new char[sizeof(UUID)];
        is.read(tmp, sizeof(UUID));
        delete[] tmp;*/

        BIASERR("Cannot read image id in MIP version 310.");
      }

      // read image data
      is.read(reinterpret_cast<char*>(img.ImageData_), img.GetPixelCount() * img.ChannelCount_
        * img.Depth_);
      break;
    case 300:
      //std::cout <<"Opening file with version MIP300"<<std::endl;
      is.read(reinterpret_cast<char*>(&ChannelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&Width), sizeof(int));
      is.read(reinterpret_cast<char*>(&Height), sizeof(int));
      img.Init(Width, Height, ChannelCount, FileStorageType);
      is.read(reinterpret_cast<char*>(&img.ColorModel_), sizeof(BIAS::ImageBase::EColorModel));
      is.read(reinterpret_cast<char*>(&img.Depth_), sizeof(int));
      img.Depth_ /= 8;
      is.read(reinterpret_cast<char*>(&img.InterleavedDataOrder_), sizeof(bool));
      is.read(reinterpret_cast<char*>(&PixelCount), sizeof(int));
      is.read(reinterpret_cast<char*>(&img.WidthStep_), sizeof(int));
      {
        unsigned UpperLeftX, UpperLeftY, LowerRightX, LowerRightY;
        is.read(reinterpret_cast<char*>(&UpperLeftX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&UpperLeftY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightX), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&LowerRightY), sizeof(unsigned int));
        is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
        // not necessary
        //img.GetROI()->Resize(Width, Height);
        if (roidefined)
          img.GetROI()->SetCorners(UpperLeftX, UpperLeftY,
          LowerRightX, LowerRightY);
      }
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROIUpperLeftY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightX_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&img.ROILowerRightY_), sizeof(unsigned int));
      //       is.read(reinterpret_cast<char*>(&roidefined), sizeof(bool));
      //       if (!roidefined){
      //         img.ROIUpperLeftX_=img.ROIUpperLeftY_=0;
      //         img.ROILowerRightX_=img.Width_; img.ROILowerRightY_=img.Height_;
      //       }
      //        img.PrintHeader(cerr);
      //         cerr << endl << "size to read "<< img.PixelCount_ * img.ChannelCount_ *  img.Depth_<< endl;
      is.read(reinterpret_cast<char*>(img.ImageData_), img.GetPixelCount() * img.ChannelCount_
        * img.Depth_);
      break;
    default:
      BIASERR("operator>> invalid or unsupported version number ("<<Version
        <<")\ntry mipconvertversion in Image/test");

      break;
    }
    return is;
  }



  void ImageBase::PrintPixelValue(const ImageBase & im,
                     const unsigned int x,
                     const unsigned int y,
                     const unsigned short channel,
                     std::ostream & os)
{
  // determine storage type
  switch ( im.GetStorageType())
  {
  case ImageBase::ST_unsignedchar: {
    const Image<unsigned char> *
      p = static_cast< const Image<unsigned char> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }

  case ImageBase::ST_float: {
    const Image<float> *
      p = static_cast< const Image<float> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(float)p->PixelValue(x,y,channel);
    break; }

#ifdef BUILD_IMAGE_DOUBLE
  case ImageBase::ST_double: {
    const Image<double> *
      p = static_cast< const Image<double> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(double)p->PixelValue(x,y,channel);
    break; }
#endif


#ifdef BUILD_IMAGE_INT
  case ImageBase::ST_int: {
    const Image<int> *
      p = static_cast< const Image<int> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }
#endif

#ifdef BUILD_IMAGE_UINT
  case ImageBase::ST_unsignedint: {
    const Image<unsigned int> *
      p = static_cast< const Image<unsigned int> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }
#endif

#ifdef BUILD_IMAGE_SHORT
  case ImageBase::ST_shortint: {
    const Image<short> *
      p = static_cast< const Image<short> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }
#endif

#ifdef BUILD_IMAGE_USHORT
  case ImageBase::ST_unsignedshortint: {
    const Image<unsigned short> *
      p = static_cast< const Image<unsigned short> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
  case ImageBase::ST_char: {
    const Image<char> *
      p = static_cast< const Image<char> * >(&im);
    BIASASSERT(p!=NULL);
    os<<(int)p->PixelValue(x,y,channel);
    break; }
#endif

  default:
    {
      BIASERR("unknown StorageType "<<im.GetStorageType()<<"detected");
      os<<"?";
    }
  };
}


#ifdef BIAS_HAVE_OPENEXR
void  ImageBase::GetChannel(const BIAS::ImageBase & im,
                            const unsigned int channelId,
                            half* channelOut)
{
  BIASASSERT(im.IsInterleaved());
  unsigned int C = im.GetChannelCount();
  unsigned int width = im.GetWidth();
  unsigned int height = im.GetHeight();
  switch ( im.GetStorageType())
    {
    case BIAS::ImageBase::ST_unsignedchar: {
      unsigned char** p = (unsigned char**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }
      break; }

    case BIAS::ImageBase::ST_float: {
      float** p = (float**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = p[y][x*C+channelId];
        }
      }
      break; }

#ifdef BUILD_IMAGE_DOUBLE
    case BIAS::ImageBase::ST_double: {
      double** p = (double**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif


#ifdef BUILD_IMAGE_INT
    case BIAS::ImageBase::ST_int: {
      int** p = (int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_UINT
    case BIAS::ImageBase::ST_unsignedint: {
      unsigned int** p = (unsigned int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }

      break; }
#endif

#ifdef BUILD_IMAGE_SHORT
    case BIAS::ImageBase::ST_shortint: {
      short** p = (short**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_USHORT
    case BIAS::ImageBase::ST_unsignedshortint: {
      unsigned short** p = (unsigned short**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
    case BIAS::ImageBase::ST_char: {
      char** p = (char**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<float>(p[y][x*C+channelId]);
        }
      }

      break; }
#endif

    default:
      {
        BIASERR("unknown StorageType "<<im.GetStorageType()<<"detected");      
      }
    };
 
}
#endif 


} // end namespace
