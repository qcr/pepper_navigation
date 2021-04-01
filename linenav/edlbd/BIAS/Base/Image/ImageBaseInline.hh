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

// #include <bias_config.h>
// #ifdef BIAS_HAVE_OPENEXR
// #  ifdef WIN32
// #    pragma warning( push, 1)
// #    define PLATFORM_WINDOWS
// #  endif // WIN32
// #    include <half.h>
// #  ifdef WIN32
// #    pragma warning( pop)
// #  endif // WIN32
// #endif


#if !defined(_IMAGE_BASE_H_INCLUDED_)
# error do not include ImageBaseInline here.
#endif 

//
// Check for valid x and y coordinates (if BIAS_DEBUG is defined)
// In case of error display the functionname
// 
#ifdef BIAS_DEBUG
#define CheckCoordinates(x,y,functionname) \
   if (x < 0 || x > (int)GetWidth()-1) {                            \
    BIASERR("x-coordinate out of range in "<<functionname<<" "<<x); \
  } \
  if (y < 0 || y > (int)GetHeight()-1) { \
    BIASERR("y-coordinate out of range in "<<functionname<<" "<<y); \
  }
#else
#define CheckCoordinates(x,y,functionname)
#endif


// Check for valid channel nr. in Debug mode. Do nothing in Release
// In case of error display the functionname (JW 09/2003)
#ifdef BIAS_DEBUG
#define CheckChannel(channel,functionname) \
  if (channel >= GetChannelCount()) { \
    BIASERR("channel"<<channel<<" out of range in "<<functionname); \
  }
#else
#define CheckChannel(channel,functionname)
#endif



inline bool ImageBase::SamePixelAndChannelCount(const ImageBase& Image) const {
  return ( (GetPixelCount() == Image.GetPixelCount()) && 
           (GetChannelCount() == Image.GetChannelCount()) ); 
}

inline bool ImageBase::NotBiggerPixelAndSameChannelCount(const ImageBase& Image) const {
  return ( (GetPixelCount() <= Image.GetPixelCount()) && 
           (GetChannelCount() == Image.GetChannelCount()) ); 
}

inline void ImageBase::SetZero()
{
  if (!IsEmpty()){
    memset(GetImageData(), 0, GetWidthStep()*GetHeight());
  }
}

inline void ImageBase::ReleaseImageDataPointer()
{       
  delete[] (char**)ImageDataArray_;
  delete[] (char*)ImageData_;
  ImageDataArray_=NULL;
  ImageData_=NULL;
}

inline unsigned int 
ImageBase::GetPixelPosition(unsigned int x, unsigned int y, 
                            unsigned short int channel) const
{ 
  CheckCoordinates((int)x, (int)y, "GetPixelPosition");

  if (IsPlanar()) 
    return y*Width_+x+channel*GetPixelCount();
  else  
    return y*Width_*ChannelCount_ +x*(ChannelCount_)+channel; 
}

bool ImageBase::IsPositionInImage(const int& x,const int& y) const{
  return (x>=0 && x<(int)Width_ && y>=0 && y<(int)Height_);
}

int  ImageBase::MoveToClosestPositionInImage(int& x, int& y) const {
#ifdef BIAS_DEBUG
  if (!((Width_>0)&&(Height_>0))) {
    BIASERR("Image is not initialiozed correctly: "<<Width_<<"x"<<Height_);
    BIASABORT;
  }
#endif 
  int ret = 0;
  if (x<0) {
    x = 0;
    ret = 1;
  } else if (x>=(int)Width_) {
    x = Width_-1;
    ret = 2;
  }
  if (y<0) {
    y = 0;
    ret = 3;
  } else if (y>=(int)Height_) {
    y = Height_-1;
    ret = 4;
  }
  return ret;
}



inline void ImageBase::BIASToTextureCoordinates(const double& biasx, 
                                                const double& biasy,
                                                double& gl_x, 
                                                double& gl_y) const {
  // to compute relative coordinates, need image initialized:
  BIASASSERT(Width_>0 && Height_>0);
  gl_x = (biasx + 0.5) / double(Width_);
  //gl_y = (double(Height_) - biasy - 0.5) / double(Height_);
  gl_y = (biasy + 0.5) / double(Height_);

//#ifdef BIAS_DEBUG
  //static bool recursion = true;
  //recursion = !recursion;
  //if (recursion) {
  //  return;
  //}
  //// check consistency
  //double biasx2, biasy2;
  //TextureToBIASCoordinates(gl_x,  gl_y, biasx2, biasy2);
  /**
     BIASASSERT(Equal(biasx, biasx2));
     BIASASSERT(Equal(biasy, biasy2));
  **/
//#endif

}

inline void ImageBase::TextureToBIASCoordinates(const double& gl_x, 
                                                const double& gl_y,
                                                double& biasx, 
                                                double& biasy) const {
  // to compute relative coordinates, need image initialized:
  BIASASSERT(Width_>0 && Height_>0);
  biasx = gl_x * double(Width_) - 0.5;
  biasy = double(Height_) - (gl_y * double(Height_) + 0.5);

#ifdef BIAS_DEBUG
  static bool recursion = true;
  recursion = !recursion;
  if (recursion) {
    return;
  }
  // check consitency
  double gl_x2, gl_y2;
  BIASToTextureCoordinates(biasx, biasy, gl_x2,  gl_y2);
  /**
     BIASASSERT(Equal(gl_x2, gl_x));
     BIASASSERT(Equal(gl_y2, gl_y));
  **/
#endif
}

template<class castType>
castType ImageBase::GetValue(const ImageBase & im,
                     const unsigned int x,
                     const unsigned int y,
                     const unsigned int channel)
{
  BIASASSERT(im.IsInterleaved());
  unsigned int C = im.GetChannelCount();
  // determine storage type
  switch ( im.GetStorageType())
  {
  case ImageBase::ST_unsignedchar: {
    unsigned char** p = (unsigned char**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }

  case ImageBase::ST_float: {
    float** p = (float**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }

#ifdef BUILD_IMAGE_DOUBLE
  case ImageBase::ST_double: {
    double** p = (double**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif


#ifdef BUILD_IMAGE_INT
  case ImageBase::ST_int: {
    int** p = (int**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif

#ifdef BUILD_IMAGE_UINT
  case ImageBase::ST_unsignedint: {
    unsigned int** p = (unsigned int**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif

#ifdef BUILD_IMAGE_SHORT
  case ImageBase::ST_shortint: {
    short** p = (short**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif

#ifdef BUILD_IMAGE_USHORT
  case ImageBase::ST_unsignedshortint: {
    unsigned short** p = (unsigned short**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
  case ImageBase::ST_char: {
    char** p = (char**)im.GetImageDataArray();
    return static_cast<castType>(p[y][x*C+channel]);
    break; }
#endif

  default:
    {
      BIASERR("unknown StorageType "<<im.GetStorageType()<<"detected");      
    }
  };
  return 0;
}

template<class inputType>
void ImageBase::SetValue(const ImageBase & im,
                         const unsigned int x,
                         const unsigned int y,
                         const unsigned int channel,
                         const inputType val)
{
  BIASASSERT(im.IsInterleaved());
  unsigned int C = im.GetChannelCount();
  // determine storage type
  switch ( im.GetStorageType())
  {
  case ImageBase::ST_unsignedchar: {
    unsigned char** p = (unsigned char**)im.GetImageDataArray();
    p[y][x*C+channel] = static_cast<unsigned char>(val);
    break; }

  case ImageBase::ST_float: {
    float** p = (float**)im.GetImageDataArray();
    p[y][x*C+channel] = static_cast<float>(val);
    break; }

#ifdef BUILD_IMAGE_DOUBLE
  case ImageBase::ST_double: {
    double** p = (double**)im.GetImageDataArray();    
    p[y][x*C+channel] = static_cast<double>(val);
    break; }
#endif


#ifdef BUILD_IMAGE_INT
  case ImageBase::ST_int: {
    int** p = (int**)im.GetImageDataArray();    
    p[y][x*C+channel] = static_cast<int>(val);
    break; }
#endif

#ifdef BUILD_IMAGE_UINT
  case ImageBase::ST_unsignedint: {
    unsigned int** p = (unsigned int**)im.GetImageDataArray();    
    p[y][x*C+channel] = static_cast<unsigned int>(val);
    break; }
#endif

#ifdef BUILD_IMAGE_SHORT
  case ImageBase::ST_shortint: {
    short** p = (short**)im.GetImageDataArray();    
    p[y][x*C+channel] = static_cast<short>(val);
    break; }
#endif

#ifdef BUILD_IMAGE_USHORT
  case ImageBase::ST_unsignedshortint: {
    unsigned short** p = (unsigned short**)im.GetImageDataArray();
    p[y][x*C+channel] = static_cast<unsigned short>(val);
    break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
  case ImageBase::ST_char: {
    char** p = (char**)im.GetImageDataArray();
    p[y][x*C+channel] = static_cast<char>(val);
    break; }
#endif

  default:
    {
      BIASERR("unknown StorageType "<<im.GetStorageType()<<"detected");      
    }
  };
 
}


template<class inputType>
void ImageBase::SetChannel(const BIAS::ImageBase & im,                           
                           const unsigned int channelId,
                           const inputType* channelIn)
{

  BIASASSERT(im.IsInterleaved());
  unsigned int C = im.GetChannelCount();
  unsigned int width = im.GetWidth();
  unsigned int height = im.GetHeight();
  // determine storage type
  switch ( im.GetStorageType())
    {
    case BIAS::ImageBase::ST_unsignedchar: {
      unsigned char** p = (unsigned char**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<unsigned char>(channelIn[y*width+x]);
        }
      }
      break; }

    case BIAS::ImageBase::ST_float: {
      float** p = (float**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<float>(channelIn[y*width+x]);
        }
      }
      break; }

#ifdef BUILD_IMAGE_DOUBLE
    case BIAS::ImageBase::ST_double: {
      double** p = (double**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<double>(channelIn[y*width+x]);
        }
      }
      break; }
#endif


#ifdef BUILD_IMAGE_INT
    case BIAS::ImageBase::ST_int: {
      int** p = (int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<int>(channelIn[y*width+x]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_UINT
    case BIAS::ImageBase::ST_unsignedint: {
      unsigned int** p = (unsigned int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<unsigned int>(channelIn[y*width+x]);
        }
      }

      break; }
#endif

#ifdef BUILD_IMAGE_SHORT
    case BIAS::ImageBase::ST_shortint: {
      short** p = (short**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<short>(channelIn[y*width+x]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_USHORT
    case BIAS::ImageBase::ST_unsignedshortint: {
      unsigned short** p = (unsigned short**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<unsigned short>(channelIn[y*width+x]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
    case BIAS::ImageBase::ST_char: {
      char** p = (char**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          p[y][x*C+channelId] = static_cast<char>(channelIn[y*width+x]);
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


template<class outputType>
void ImageBase::GetChannel(const BIAS::ImageBase & im,
                           const unsigned int channelId,
                           outputType* channelOut)
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
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }

    case BIAS::ImageBase::ST_float: {
      float** p = (float**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }

#ifdef BUILD_IMAGE_DOUBLE
    case BIAS::ImageBase::ST_double: {
      double** p = (double**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif


#ifdef BUILD_IMAGE_INT
    case BIAS::ImageBase::ST_int: {
      int** p = (int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_UINT
    case BIAS::ImageBase::ST_unsignedint: {
      unsigned int** p = (unsigned int**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }

      break; }
#endif

#ifdef BUILD_IMAGE_SHORT
    case BIAS::ImageBase::ST_shortint: {
      short** p = (short**)im.GetImageDataArray();    
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_USHORT
    case BIAS::ImageBase::ST_unsignedshortint: {
      unsigned short** p = (unsigned short**)im.GetImageDataArray();
      for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
        }
      }
      break; }
#endif

#ifdef BUILD_IMAGE_CHAR // signed!
    case BIAS::ImageBase::ST_char: {
      char** p = (char**)im.GetImageDataArray();
     for(unsigned int y=0; y<height; y++) {
        for(unsigned int x=0; x<width; x++) {
          channelOut[y*width+x] = static_cast<outputType>(p[y][x*C+channelId]);
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
