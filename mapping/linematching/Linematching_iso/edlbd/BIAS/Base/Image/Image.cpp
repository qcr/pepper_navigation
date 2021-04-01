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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
#include "Image.hh"
// tell WIN32 to define math-constants
//--- math.h is included by <complex>

#include <typeinfo>

#include <assert.h>
#include <stdio.h>
#include <limits.h>
#include <typeinfo>
#include <string>
#include <Base/Debug/Error.hh>

//disables warning C4756: overflow in constant arithmetic in line 1132, function: InvertValues


using namespace BIAS;
using namespace std;

namespace BIAS {

template<class StorageType>
Image<StorageType>::
~Image()
{}

template<>
Image<unsigned char>::
Image() : ImageBase()
{

  StorageType_ = ST_unsignedchar;
  Depth_ = sizeof(unsigned char);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<float>::
Image() : ImageBase()
{

  StorageType_ = ST_float;
  Depth_ = sizeof(float);
  BitDepth_ = 8 * Depth_;
}


#ifdef BUILD_IMAGE_SHORT
template<>
Image<short int>::
Image() : ImageBase()
{
  StorageType_ = ST_shortint;
  Depth_ = sizeof(short int);
  BitDepth_ = 8 * Depth_;
}
#endif // BUILD_IMAGE_SHORT


template<class StorageType>
Image<StorageType>::
Image() : ImageBase()
{
  BIASERR("invalid StorageType, necessary instance of image not compiled:"
          <<PRINTTYPE(StorageType));
  //BIASABORT
}

template<>
Image<unsigned char>::
Image(const ImageBase& im) : ImageBase()
{

  StorageType_ = ST_unsignedchar;
  Depth_ = sizeof(unsigned char);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<unsigned char> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}

template<>
Image<float>::
Image(const ImageBase& im) : ImageBase()
{

  StorageType_ = ST_float;
  Depth_ = sizeof(float);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<float> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}

template<class StorageType>
Image<StorageType>::
Image(const ImageBase& im) : ImageBase()
{
  // this is the generic implementation which must NEVER be called !
  BIASERR("You tried to construct Image<"<<PRINTTYPE(StorageType)<<"> from "
          <<" ImageBase with StorageType "<<im.GetStorageType()
          <<". Not implemented for that type. Aborting.");
  BIASABORT;
}


template<class StorageType>
Image<StorageType>::
Image(unsigned int Width, unsigned int Height,
      unsigned int channels, bool interleaved)
{
  BIASERR("invalid StorageType="<<PRINTTYPE(StorageType));
  BIASABORT;
}


template<>
Image<unsigned char>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{

  StorageType_ = ST_unsignedchar;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}


template<>
Image<float>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{

  StorageType_ = ST_float;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}


#ifdef BUILD_IMAGE_CHAR
template<>
Image<char>::
Image() : ImageBase()
{
  StorageType_ = ST_char;
  Depth_ = sizeof(char);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<char>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_char;
  Depth_ = sizeof(char);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<char> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<char>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_char;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif


#ifdef BUILD_IMAGE_USHORT
template<>
Image<unsigned short int>::
Image() : ImageBase()
{
  StorageType_ = ST_unsignedshortint;
  Depth_ = sizeof(unsigned short int);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<unsigned short int>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_unsignedshortint;
  Depth_ = sizeof(unsigned short int);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<unsigned short int> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<unsigned short int>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_unsignedshortint;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif // BUILD_IMAGE_USHORT


#ifdef BUILD_IMAGE_SHORT
template<>
Image<short int>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_shortint;
  Depth_ = sizeof(short int);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<short int> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<short int>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_shortint;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif


#ifdef BUILD_IMAGE_UINT
template<>
Image<unsigned int>::
Image() : ImageBase()
{
  StorageType_ = ST_unsignedint;
  Depth_ = sizeof(unsigned int);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<unsigned int>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_unsignedint;
  Depth_ = sizeof(unsigned int);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<unsigned int> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<unsigned int>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_unsignedint;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif

#ifdef BUILD_IMAGE_INT
template<>
Image<int>::
Image() : ImageBase()
{
  StorageType_ = ST_int;
  Depth_ = sizeof(int);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<int>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_int;
  Depth_ = sizeof(int);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<int> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<int>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_int;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif


#ifdef BUILD_IMAGE_DOUBLE
template<>
Image<double>::
Image() : ImageBase()
{
  StorageType_ = ST_double;
  Depth_ = sizeof(double);
  BitDepth_ = 8 * Depth_;
}


template<>
Image<double>::
Image(const ImageBase& im) : ImageBase()
{
  StorageType_ = ST_double;
  Depth_ = sizeof(double);
  BitDepth_ = 8 * Depth_;
  if (GetStorageType() != StorageType_)
    {
      BIASERR("You tried to construct Image<double> from "
              <<" ImageBase with StorageType "<<im.GetStorageType()
              <<". Aborting.");
      BIASABORT;
    }
  this->ImageBase::operator=(im);
}


template<>
Image<double>::
Image(unsigned int Width, unsigned int Height,
      unsigned int Channels, bool interleaved)
{
  StorageType_ = ST_double;
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}
#endif


template<class StorageType>
void Image<StorageType>::
Init(unsigned int Width, unsigned int Height,
     unsigned int Channels, enum EStorageType storType, bool interleaved)
{
  // StorageType_ is set from constructor
  ImageBase::Init(Width, Height, Channels, StorageType_, interleaved);
}


template<class StorageType>
std::ostream &Image<StorageType>::
PrintPointer(std::ostream& os) const
{
  BIASASSERT(ImageData_ != NULL);BIASASSERT(ImageDataArray_ != NULL);
  os << "\nImageData_: \t\t" << (void *) GetImageData()
     << "\nimageDataArray_: \t" << (void *) (GetImageDataArray()[0]);
  return os;
}


template<class StorageType>
std::ostream& Image<StorageType>::
PrintData(std::ostream& os) const
{
  BIASASSERT(ImageData_ != NULL);
  os << "\nImageData_: \t\t" << GetImageData() << endl;
  for (unsigned long int i = 0; i < GetPixelCount(); i++)
    os << ((StorageType *) GetImageData())[i] << "  ";
  //      << (int)GetImageData()[i] << "   ";
  os << endl;
  return os;
}


template<class StorageType>
void Image<StorageType>::
FillImageWithConstValue(StorageType Value)
{
  int minx, miny, maxx, maxy, cc = GetChannelCount();
  GetROI()->GetCorners(minx, miny, maxx, maxy);
  minx *= cc;
  maxx *= cc;
  register int width = (int) GetWidth() * cc, step = width - maxx + minx;
  // beginning of ROI
  register StorageType *p = GetImageData() + width * miny + minx;
  // end of first row of ROI
  register StorageType *le = GetImageData() + width * miny + maxx;
  // end of last row of ROI
  register StorageType *e = GetImageData() + width * (maxy - 1) + maxx;
  //  int x=0, y=0;
  while (p < e)
    {
      while (p < le)
        { // walk width of ROI in one line
          *p++ = Value;
          //x++;
        }
      //y++; x=0;
      le += width; // set line end pointer one line further down
      p += step; // skip to beginning of ROI in next line
    }
}


template<class StorageType>
void Image<StorageType>::
FillImageWithConstValue(StorageType Value[])
{
  int minx, miny, maxx, maxy, i;
  register int cc = GetChannelCount();
  GetROI()->GetCorners(minx, miny, maxx, maxy);
  if (IsInterleaved())
    {
      minx *= cc;
      maxx *= cc;
      register int width = (int) GetWidth() * cc, step = width - maxx
        + minx;
      // beginning of ROI
      register StorageType *p = GetImageData() + width * miny + minx;
      // end of first row of ROI
      register StorageType *le = GetImageData() + width * miny + maxx;
      // end of last row of ROI
      register StorageType *e = GetImageData() + width * (maxy - 1) + maxx;
      //  int x=0, y=0;
      while (p < e)
        {
          while (p < le)
            { // walk width of ROI in one line
              for (i = 0; i < cc; i++)
                *p++ = Value[i];
            }
          //y++; x=0;
          le+=width; // set line end pointer one line further down
          p+=step; // skip to beginning of ROI in next line
        }
    }
  else
    {
      for (int i = 0; i < cc; i++)
        {
          register int width = (int) GetWidth(), step = width - maxx + minx;
          // beginning of ROI
          register StorageType *p = GetImageData() + i * GetPixelCount()
            + width * miny + minx;
          // end of first row of ROI
          register StorageType *le = GetImageData() + i * GetPixelCount()
            + width * miny + maxx;
          // end of last row of ROI
          register StorageType *e = GetImageData() + i * GetPixelCount()
            + width * (maxy - 1) + maxx;
          //  int x=0, y=0;
          while (p < e)
            {
              while (p < le)
                { // walk width of ROI in one line
                  *p++ = Value[i];
		}
              //y++; x=0;
              le += width; // set line end pointer one line further down
              p += step; // skip to beginning of ROI in next line
            }
        }
    }    
    }

template<class StorageType>
void Image<StorageType>::
ClearChannel(const unsigned int channel, const StorageType value)
{
  int minx=0, miny=0, maxx=0, maxy=0, i=0;
  register int cc = GetChannelCount();
  GetROI()->GetCorners(minx, miny, maxx, maxy);
  if (IsInterleaved())
    {
      minx *= cc;
      maxx *= cc;
      register int width = (int) GetWidth() * cc, step = width - maxx + minx;
      // beginning of ROI
      register StorageType *p = GetImageData() + width * miny + minx + channel;
      // end of first row of ROI
      register StorageType *le = GetImageData() + width * miny + maxx;
      // end of last row of ROI
      register StorageType *e = GetImageData() + width * (maxy - 1) + maxx;
      //  int x=0, y=0;
      while (p < e)
        {
          while (p < le)
            { // walk width of ROI in one line
              //for (i = 0; i < cc; i++)
                *p = value;
                p+=cc;
            }
          //y++; x=0;
          le+=width; // set line end pointer one line further down
          p+=step; // skip to beginning of ROI in next line
        }
    }
  else
    {

    register int width = (int) GetWidth(), step = width - maxx + minx;
    // beginning of ROI
    register StorageType *p = GetImageData() + i * GetPixelCount()
                              + width * miny + minx;
    // end of first row of ROI
    register StorageType *le = GetImageData() + i * GetPixelCount()
                               + width * miny + maxx;
    // end of last row of ROI
    register StorageType *e = GetImageData() + i * GetPixelCount()
                              + width * (maxy - 1) + maxx;
    //  int x=0, y=0;
    while (p < e)
    {
      while (p < le)
      { // walk width of ROI in one line
        *p++ = value;
      }
      //y++; x=0;
      le += width; // set line end pointer one line further down
      p += step; // skip to beginning of ROI in next line
    }

  }
}


  template<class StorageType>
    void
    Image<StorageType>::FillImageWithXValue()
    {
      StorageType *p = GetImageData();

      for (unsigned int y = 0; y < GetHeight(); y++)
        {
          for (unsigned int x = 0; x < GetWidth(); x++)
            {
              p[y * GetWidth() + x] = StorageType(x);
            }
        }
    }

  template<class StorageType>
    void
    Image<StorageType>::GetMeanPixelValue(StorageType mean[])
    {
      if (IsInterleaved())
        {
          register const unsigned cc = GetChannelCount();
          register unsigned int i;
          double *sum = new double[cc];
          for (i = 0; i < cc; i++)
            sum[i] = 0.0;
          int minx, miny, maxx, maxy;
          GetROI()->GetCorners(minx, miny, maxx, maxy);
          register double pc = (double) ((maxx - minx) * (maxy - miny));
          minx *= cc;
          maxx *= cc;
          register int width = (int) GetWidth() * cc, step = width - maxx
              + minx;
          // beginning of ROI
          register StorageType *p = GetImageData() + width * miny + minx;
          // end of first row of ROI
          register StorageType *le = GetImageData() + width * miny + maxx;
          // end of last row of ROI
          register StorageType *e = GetImageData() + width * (maxy - 1) + maxx;
          //  int x=0, y=0;
          while (p < e)
            {
              while (p < le)
                { // walk width of ROI in one line
                  for (i = 0; i < cc; i++)
                    sum[i] += (double) p[i];
                  p += cc;
                }
              le += width; // set line end pointer one line further down
              p += step; // skip to beginning of ROI in next line
            }

          for (i = 0; i < cc; i++)
            mean[i] = (StorageType)(sum[i] / pc);
          delete[] sum;
        }
      else
        {
            BIASERR("GetMeanPixelValue not implemented for planar images ");
        }
    }

  /////////////////////////////////////////////////////////////////////////////////
  template<class StorageType>
  void
  Image<StorageType>::GetMeanPixelValueIgnoreRange(StorageType mean[],
  		const StorageType ignoreStart, const StorageType ignoreEnd) const
  {
  	if (IsInterleaved())
  	{
  		register const unsigned cc = GetChannelCount();  		
  		register unsigned int i;
  		double *sum = new double[cc];
  		double *pc = new double[cc];
  		for (i = 0; i < cc; i++){
  			sum[i] = 0.0;
  			pc[i] = 0.0;
  		}
  		
  		int minx, miny, maxx, maxy;
  		GetROI()->GetCorners(minx, miny, maxx, maxy);  		
  		minx *= cc;
  		maxx *= cc;
  		register int width = (int) GetWidth() * cc, step = width - maxx + minx;
  		// beginning of ROI
  		const register StorageType *p = GetImageData() + width * miny + minx;
  		// end of first row of ROI
  		const register StorageType *le = GetImageData() + width * miny + maxx;
  		// end of last row of ROI
  		const register StorageType *e = GetImageData() + width * (maxy - 1) + maxx;
  		//  int x=0, y=0;
  		while (p < e)
  		{
  			while (p < le)
  			{ // walk width of ROI in one line
  				for (i = 0; i < cc; i++){
  					if(p[i] < ignoreStart || p[i] > ignoreEnd){
  						sum[i] += (double) p[i];
  						pc[i]++;
  					}
  				}
  				p += cc;
  			}
  			le += width; // set line end pointer one line further down
  			p += step; // skip to beginning of ROI in next line
  		}

  		for (i = 0; i < cc; i++)
  			mean[i] = (StorageType)(sum[i] / pc[i]);
  		delete[] sum;
  	}
  	else
  	{
  		BIASERR("GetMeanPixelValue not implemented for planar images ");
  	}
  }

  /////////////////////////////////////////////////////////////////////////////////
  // JW 01/2003
  template<class StorageType>
    bool
    Image<StorageType>::GetMinMaxPixelValueIgnoreRange(StorageType& min,
        StorageType& max, const StorageType ignoreStart,
        const StorageType ignoreEnd, const unsigned short int channel) const
    {


      // OK, work...
      // Planar implementation:

      const StorageType *p_act, *p_end; // pointer to actual and end position in image data.

      p_act = GetImageData() + channel * GetPixelCount(); // pointer to start of data
      p_end = p_act + GetPixelCount(); // pointer to end of data
      min = 0;
      max = 0;
      bool foundInRange = false;
      while ((!foundInRange) && (p_act != p_end))
        { // while no legal value found and pixels left..
          if((*p_act < ignoreStart) || (*p_act > ignoreEnd))
            {
              // found value out of ignore range, use it as initial min/max
              min = *p_act;
              max = *p_act;
              foundInRange = true;
            };
          p_act++;
        };
      if(!foundInRange) return foundInRange;
      	
      // initial valid min and max are set, find 'better' in the rest of the plane:
      for (; p_act != p_end; p_act++)
        {
          // check if new min found:
          // min and max can't be changed at once!
          if (*p_act < min)
            {
              if ((*p_act < ignoreStart) || (*p_act > ignoreEnd))
                {
                  min = *p_act; // found
                };
            }
          else if (*p_act > max)
            {
              if((*p_act < ignoreStart) || (*p_act > ignoreEnd))
                {
                  max = *p_act; // found
                };
            };
        }; // found min and max for planar image
      return foundInRange; // return if any value within range was found
    }

  template<class StorageType>
    void
    Image<StorageType>::GetMinMaxPixelValue(StorageType& min, StorageType& max,
        unsigned short int channel, unsigned int *mincoo, unsigned int *maxcoo) const
    {
      register int ChannelCount = GetChannelCount();
      //  register StorageType *ImageDataPointer, *EndImageData;
      //  register unsigned int x, y;

      unsigned minx, miny, maxx, maxy;
      GetROI()->GetCorners(minx, miny, maxx, maxy);
      const StorageType **ida = GetImageDataArray();

      if (IsInterleaved())
        {
          BIASCDOUT(D_IMAGE_MINMAXCOO," interleaved ("<<minx<<", "<<miny<<") <--> ("
              <<maxx<<", "<<maxy<<")\n");
          unsigned cc = ChannelCount;
          BIASASSERT(ida[miny]!=NULL);
          min = max = ida[miny][minx * cc + channel];
          const unsigned mmaxx = maxx * cc;
          if (mincoo != NULL || maxcoo != NULL)
            { // remember coordinates
              BIASCDOUT(D_IMAGE_MINMAXCOO, "remember coordinates\n");
              if (mincoo)
                {
                  mincoo[0] = minx;
                  mincoo[1] = miny;
                }
              if (maxcoo)
                {
                  maxcoo[0] = minx;
                  maxcoo[1] = miny;
                }
              for (unsigned y = miny; y < maxy; y++)
                {
                  for (unsigned x = minx * cc + channel; x < mmaxx; x += cc)
                    {
                      if (ida[y][x] > max)
                        {
                          max = ida[y][x];
                          if (maxcoo)
                            {
                              maxcoo[0] = (x - channel) / cc;
                              maxcoo[1] = y;
                            }
                        }
                      else if (ida[y][x] < min)
                        {
                          min = ida[y][x];
                          if (mincoo)
                            {
                              mincoo[0] = (x - channel) / cc;
                              mincoo[1] = y;
                            }
                        }
                    }
                }
            }
          else
            { // no need to remeber coordinates
              BIASCDOUT(D_IMAGE_MINMAXCOO, "don't remember coordinates\n");
              for (unsigned y = miny; y < maxy; y++)
                {
                  BIASASSERT(ida[y]!=NULL);
                  for (unsigned x = minx * cc + channel; x < mmaxx; x += cc)
                    {
                      //cout<<" ("<<(x-channel)/cc<<", "<<y<<"):" <<(int)(ida[y][x])<<flush;
                      /// @bug NULL pointer exception occurs, here (e.g. in fuse) JW
                      if (ida[y][x] > max)
                        max = ida[y][x];
                      if (ida[y][x] < min)
                        min = ida[y][x];
                    }
                }
            }
        }
      else
        { // planar image
          BIASCDOUT(D_IMAGE_MINMAXCOO," planar ("<<minx<<", "<<miny<<") <--> ("
              <<maxx<<", "<<maxy<<")\n");
          const StorageType *start = GetImageData() + channel * GetPixelCount()
              + miny * GetWidth() + minx;
          const StorageType *mendl = start + maxx - minx;
          const StorageType *end = GetImageData() + channel * GetPixelCount()
              + (maxy - 1) * GetWidth() + maxx;
          int step = (int) GetWidth() - (int) maxx + (int) minx;
          int x = minx;
          int y = miny;
          const int width = (int) GetWidth();
          max = min = *start;
          BIASCDOUT(D_IMAGE_MINMAXCOO, "starting with min: "<< (int)min <<"\tmax: "
              << (int)max <<endl);
          if (mincoo != NULL || maxcoo != NULL)
            { // remember coordinates
              BIASCDOUT(D_IMAGE_MINMAXCOO, "remember coordinates\n");
              if (mincoo)
                {
                  mincoo[0] = minx;
                  mincoo[1] = miny;
                }
              if (maxcoo)
                {
                  maxcoo[0] = minx;
                  maxcoo[1] = miny;
                }
              while (start < end)
                {
                  while (start < mendl)
                    {
                      //cerr << " ("<<x<<", "<<y<<") " << (int)(*start) << endl;
                      if (*start > max)
                        {
                          max = *start;
                          if (maxcoo)
                            {
                              maxcoo[0] = x;
                              maxcoo[1] = y;
                            }
                        }
                      else if (*start < min)
                        {
                          min = *start;
                          if (mincoo)
                            {
                              mincoo[0] = x;
                              mincoo[1] = y;
                            }
                        }
                      start++;
                      x++;
                    }
                  start += step;
                  mendl += width;
                  y++;
                  x = minx;
                }
            }
          else
            { // no need to remeber coordinates
              BIASCDOUT(D_IMAGE_MINMAXCOO, "don't remember coordinates\n");
              while (start < end)
                {
                  while (start < mendl)
                    {
                      if (*start > max)
                        max = *start;
                      else if (*start < min)
                        min = *start;
                      start++;
                    }
                  start += step;
                  mendl += width;
                }
            }
        }
    }

  template<class StorageType>
    StorageType
    Image<StorageType>::GetMinPixelValue(unsigned short int channel,
        unsigned int *coo) const
    {
      StorageType min, max;
      if (coo == NULL)
        {
          GetMinMaxPixelValue(min, max, channel);
        }
      else
        {
          unsigned int maxcoo[2];
          GetMinMaxPixelValue(min, max, channel, coo, maxcoo);
        }
      return min;
    }

  template<class StorageType>
    StorageType
    Image<StorageType>::GetMaxPixelValue(unsigned short int channel,
        unsigned int *coo) const
    {
      StorageType min, max;
      if (coo == NULL)
        {
          GetMinMaxPixelValue(min, max, channel);
        }
      else
        {
          unsigned int mincoo[2];
          GetMinMaxPixelValue(min, max, channel, mincoo, coo);
        }
      return max;
    }

  template<class StorageType>
    int
    Image<StorageType>::BelowThresholdToValue(StorageType Threshold,
        StorageType Value)
    {
      StorageType *p = GetImageData();
      unsigned int count = GetPixelCount() * GetChannelCount();
      for (register int i = count; i > 0; i--)
        {
          *p = (*p > Threshold) ? *p : Value;
          p++;
        }
      return 0;
    }

  template<class StorageType>
    int
    Image<StorageType>::AboveThresholdToValue(StorageType Threshold,
        StorageType Value)
    {
      StorageType *p = GetImageData();
      unsigned int count = GetPixelCount() * GetChannelCount();
      for (register int i = count; i > 0; i--)
        {
          *p = (*p < Threshold) ? *p : Value;
          p++;
        }
      return 0;
    }

  template<class StorageType>
    void
    Image<StorageType>::Binarize(StorageType threshold)
    {
      StorageType *ptr, *end;
      ptr = end = GetImageData();
      end += GetPixelCount();
      while (ptr < end)
        {
          *ptr = (*ptr > threshold) ? (MaxSTValue()) : (MinSTValue());
          ptr++;
        }
    }

  template<class StorageType>
    void
    Image<StorageType>::Binarise(StorageType threshold)
    {
  		this->Binarize(threshold);
    }

  template<class StorageType>
    int
    Image<StorageType>::ScaleShift(double Scale, double Shift)
    {
      register StorageType *ImageData = GetImageData();
      register StorageType *ImageDataEnd = ImageData + GetPixelCount()
          * GetChannelCount();
      while (ImageData < ImageDataEnd)
        {
          *ImageData = (StorageType)(((double) (*ImageData) + Shift) * Scale);
          ImageData++;
        }
      return 0;
    }

  template<class StorageType>
    int
    Image<StorageType>::ScaleShiftChannel(double Scale, double Shift,
        unsigned int channel)
    {
      StorageType *ImageData = GetImageData();
      StorageType *ImageDataEnd = ImageData + GetPixelCount()
          * GetChannelCount();

      if (channel >= GetChannelCount())
        {
            BIASERR("Not enough channels.");
          return -1;
        }

      if (IsPlanar())
        {
          ImageData += GetPixelCount() * channel;
          while (ImageData < ImageData + GetPixelCount() * (channel + 1))
            {
              *ImageData = (StorageType)(((double) (*ImageData) + Shift)
                  * Scale);
              ImageData++;
            }
        }
      else
        {
          ImageData += channel;
          while (ImageData < ImageDataEnd)
            {
              *ImageData = (StorageType)(((double) (*ImageData) + Shift)
                  * Scale);
              ImageData += GetChannelCount();
            }
        }

      return 0;
    }

  template<class StorageType>
    int
    Image<StorageType>::ScaleShiftBetween(double Min, double Max)
    {
      register double Scale, Shift;
      int res = CalcScaleShift(Min, Max, Scale, Shift);
      if (res != 0)
        {
          BIASWARNONCE("Cannot scale-shift, error in determining numbers.");
          return res;
        }
      res = ScaleShift(Scale, Shift);
      return res;
    }

  template<class StorageType>
    int
    Image<StorageType>::Transpose(Image<StorageType>& result)
    {

      int width = GetWidth();
      int height = GetHeight();
      int channels = GetChannelCount();

      if ((int) result.GetWidth() != height && (int) result.GetHeight()
          != width && (int) result.GetChannelCount() != channels)
        {
          result.Init(height, width, channels);
        }

      for (int x = 0; x < width; x++)
        {
          for (int y = 0; y < height; y++)
            {
              for (unsigned short ch = 0; ch < channels; ch++)
                {
                  StorageType val = PixelValue(x, y, ch);
                  result.SetPixel(val, y, x, ch);
                }
            }
        }

      return 0;
    }
/////////////////////////////////////////////////////////////////////////
template<class StorageType> 
int  Image<StorageType>::
InvertValues(BIAS::Image<StorageType>& inverted, StorageType* newMax)
{
	unsigned width = GetWidth();
	unsigned height = GetHeight();
	unsigned channels = GetChannelCount();
	StorageType max=0;
	if(newMax != NULL) 
		max = *newMax;
	else if (typeid(StorageType) == typeid(unsigned char))
		max = (StorageType)(UCHAR_MAX);
	else if(typeid(StorageType) == typeid(float))
		max = StorageType(FLT_MAX);	
	#ifdef BUILD_IMAGE_INT
	else if(typeid(StorageType) == typeid(int))
		max = StorageType(INT_MAX);
	#endif
	#ifdef BUILD_IMAGE_CHAR
	else if(typeid(StorageType) == typeid(char))
		max = StorageType(CHAR_MAX);
	#endif
	#ifdef BUILD_IMAGE_SHORT
	else if(typeid(StorageType) == typeid(short))
		max = (StorageType)(SHRT_MAX);
	#endif
	#if defined(BUILD_IMAGE_USHORT)
	else if(typeid(StorageType) == typeid(unsigned short))
		max = (StorageType)(USHRT_MAX);
	#endif
	#ifdef BUILD_IMAGE_DOUBLE
	else if(typeid(StorageType) == typeid(double))
			max = (StorageType)(DBL_MAX);
	#endif
	#ifdef BUILD_IMAGE_UINT
	else if(typeid(StorageType) == typeid(unsigned int))
				max = (StorageType)(UINT_MAX);
	#endif

	
	 
		
	if (inverted.GetWidth() != height &&inverted.GetHeight()
			!= width && inverted.GetChannelCount() != channels)
	{
		inverted.Init(height, width, channels);
	}
	StorageType ** ida = GetImageDataArray();
	StorageType **idaRes =  inverted.GetImageDataArray();
	for (unsigned int y = 0; y < height; y++){
		for (unsigned int x = 0; x < width; x++){	
			for (unsigned int c = 0; c < channels; c++){
				idaRes[y][x] = max - ida[y][x];
			}
		}
	}
	return 0;
}
 /////////////////////////////////////////////////////////////////////////  
  template<class StorageType>
    int
    Image<StorageType>::CalcScaleShift(double Min, double Max, double& Scale,
        double &Shift) const
    {
      StorageType min = 0, max = 0, MinVal = 0, MaxVal = 0;
      GetMinMaxPixelValue(MinVal, MaxVal, 0);
      for (unsigned int i = 1; i < GetChannelCount(); i++)
        {
          GetMinMaxPixelValue(min, max, (unsigned short) i);
          // unsigned int mincoo[2],maxcoo[2];
          // GetMinMaxPixelValue(min, max, (unsigned short)i,mincoo,maxcoo );
          // cout <<"Channel "<<i<<" Max: "<<max<<" at: ("<<maxcoo[0]<<","<<maxcoo[1]<<")  Min: "<<min<<" at: ("<<mincoo[0]<<","<<mincoo[1]<<")"<<endl;

          if (max > MaxVal)
            MaxVal = max;
          if (min < MinVal)
            MinVal = min;
        }

      if ((MaxVal - MinVal) == 0)
        {
          BIASWARNONCE("Image<StorageType>::CalcScaleShift(): minimum pixelvalue is"
              <<" equal to maximum pixel value -> no scaling possible");
          return -1;
        }
      Scale = (double) (Max - Min) / (double) (MaxVal - MinVal);
      Shift = (double) (Min - MinVal);

      return 0;
    }

  template<class StorageType>
    int
    Image<StorageType>::BinaryThreshold(StorageType Threshold,
        StorageType MinValue, StorageType MaxValue)
    {
      if (GetChannelCount() != 1)
        {

          return -1;
        }
      StorageType** dat = GetImageDataArray();
      //unsigned h = GetHeight();
      //unsigned w = GetWidth();
      unsigned minY = GetROIUpperLeftY();
      unsigned uppboundY = GetROILowerRightY();
      unsigned minX = GetROIUpperLeftX();
      unsigned uppboundX = GetROILowerRightX();
      for (unsigned y = minY; y < uppboundY; y++)
        {
          for (unsigned x = minX; x < uppboundX; x++)
            {
              dat[y][x] = (dat[y][x] >= Threshold) ? MaxValue : MinValue;
            }
        }

      /*
       //fixed but still not so easy to understand...
       //and it is questionable that this asm-like code is still so much faster:

       register StorageType* ImageData  = GetImageData() + (GetROILowerRightY()-1) *
       GetWidth()*sizeof(StorageType) + GetROIUpperLeftX ();
       register StorageType* ImageDataStop = GetImageData() +
       (GetROILowerRightY()-1) * GetWidth()*sizeof(StorageType) + GetROILowerRightX();
       register int ROIBreite   = GetROILowerRightX() - GetROIUpperLeftX() ;
       for(int i = (GetROILowerRightY() - GetROIUpperLeftY()); i > 0; i--) {
       for(;ImageData < ImageDataStop; ImageData+=sizeof(StorageType)) {
       *ImageData =*ImageData >= Threshold ? MaxValue : MinValue;
       }
       // Decrease the counter to proceed with above line
       ImageDataStop -= GetWidth()*sizeof(StorageType);
       ImageData      = ImageDataStop - ROIBreite;
       }
       */
      return 0;
    }

  template<class StorageType>
    void
    Image<StorageType>::MaskValues(StorageType Threshold,
        Image<unsigned char>& binaryImage)
    {
      unsigned int width = GetWidth();
      unsigned int height = GetHeight();
      unsigned int channels = GetChannelCount();

      if (binaryImage.GetWidth() != width || binaryImage.GetHeight() != height
          || binaryImage.GetChannelCount() != 1)
        {
          binaryImage.ReInit(width, height, 1);
        }

      for (unsigned int x = 0; x < width; x++)
        {
          for (unsigned int y = 0; y < height; y++)
            {
              bool flag = true;
              for (unsigned short ch = 0; ch < channels; ch++)
                {
                  StorageType val = PixelValue(x, y, ch);
                  if (val > Threshold)
                    {
                      flag = false;
                      break;
                    }
                }
              if (flag) {
                binaryImage.SetPixel(255,x,y);
              } else {
                binaryImage.SetPixel(0,x,y);
              }
            }
        }
    }

  ///////////////////////////////////////////////////////////////////////////
  template<class StorageType>
    double
    Image<StorageType>::BilinearInterpolation(const double x, const double y,
        const unsigned short int channel) const
    {

      double res = -1;

      switch (GetColorModel())
        {
      case CM_Grey:
        res = BilinearInterpolationGrey(x, y);
        break;
      case CM_RGB:
      case CM_BGR:
      case CM_RGBA:
        if (IsPlanar())
          {
            res = BilinearInterpolationRGBPlanar(x, y, channel);
          }
        else
          {
            res = BilinearInterpolationRGBInterleaved(x, y, channel);
          }
        ;
        break;
      default:
        if (IsInterleaved())
          {
            StorageType **ida = (StorageType**) GetImageDataArray();
            BIASASSERT(ida!=NULL);
            unsigned int x_floor = (unsigned int) floor(x);
            unsigned int y_floor = (unsigned int) floor(y);
            unsigned int x_ceil = (unsigned int) ceil(x);
            unsigned int y_ceil = (unsigned int) ceil(y);
            StorageType ul =
                ida[y_floor][x_floor * GetChannelCount() + channel];
            StorageType ur = ida[y_floor][x_ceil * GetChannelCount() + channel];
            StorageType ll = ida[y_ceil][x_floor * GetChannelCount() + channel];
            StorageType lr = ida[y_ceil][x_ceil * GetChannelCount() + channel];
            double dy = y - y_floor;
            double dx = x - x_floor;

            return ul + dy * (ll - ul) + dx * (ur - ul) + dy * dx * (ul + lr
                - ll - ur);
          }
        else
          {
              BIASERR("BilinearInterpolation() not implemented for planar "
                      "non-RGB images");
            break;
          }
        }
      return res;
    }

  template<class StorageType>
    double
    Image<StorageType>::BicubicInterpolation(const double& x, const double& y,
        const unsigned short int channel) const
    {


      // always use smaller int to find "next" grid position
      const int x_int = floor(x);
      const int y_int = floor(y);

      // compute offset from regular grid
      const double offsetx = x - x_int;
      const double offsety = y - y_int;

      // y weights
      const double wy0 = 0.25 * (((2.0 - offsety) * offsety - 1.0) * offsety);
      const double wy1 = 0.25 * ((3.0 * offsety - 5.0) * offsety * offsety
          + 2.0);
      const double wy2 = 0.25 * (((4.0 - 3.0 * offsety) * offsety + 1.0)
          * offsety);
      const double wy3 = 0.25 * ((offsety - 1.0) * offsety * offsety);

      // x weights
      const double wx0 = ((2.0 - offsetx) * offsetx - 1.0) * offsetx;
      const double wx1 = (3.0 * offsetx - 5.0) * offsetx * offsetx + 2.0;
      const double wx2 = ((4.0 - 3.0 * offsetx) * offsetx + 1.0) * offsetx;
      const double wx3 = (offsetx - 1.0) * offsetx * offsetx;

      const StorageType** pD = (const StorageType**) GetImageDataArray();
      return ((wx0 * pD[y_int - 1][(x_int - 1) * ChannelCount_ + channel] + wx1
          * pD[y_int - 1][x_int * ChannelCount_ + channel] + wx2
          * pD[y_int - 1][(x_int + 1) * ChannelCount_ + channel] + wx3
          * pD[y_int - 1][(x_int + 2) * ChannelCount_ + channel]) * wy0 + (wx0
          * pD[y_int][(x_int - 1) * ChannelCount_ + channel] + wx1
          * pD[y_int][x_int * ChannelCount_ + channel] + wx2 * pD[y_int][(x_int
          + 1) * ChannelCount_ + channel] + wx3 * pD[y_int][(x_int + 2)
          * ChannelCount_ + channel]) * wy1 + (wx0 * pD[y_int + 1][(x_int - 1)
          * ChannelCount_ + channel] + wx1 * pD[y_int + 1][x_int
          * ChannelCount_ + channel] + wx2 * pD[y_int + 1][(x_int + 1)
          * ChannelCount_ + channel] + wx3 * pD[y_int + 1][(x_int + 2)
          * ChannelCount_ + channel]) * wy2 + (wx0 * pD[y_int + 2][(x_int - 1)
          * ChannelCount_ + channel] + wx1 * pD[y_int + 2][x_int
          * ChannelCount_ + channel] + wx2 * pD[y_int + 2][(x_int + 1)
          * ChannelCount_ + channel] + wx3 * pD[y_int + 2][(x_int + 2)
          * ChannelCount_ + channel]) * wy3);
    }

  template<class StorageType>
    int
    Image<StorageType>::AppendChannel(Image<StorageType> &img)
    {
      StorageType *src1, *src2, *dst, *write;

      if (GetWidth() != img.GetWidth() || GetHeight() != img.GetHeight()
          || img.GetChannelCount() > 1)
        {
            BIASERR("Image::AppendChannel(): second image has different "
                <<"dimensions");
          return -1;
        }
      if (img.IsEmpty() || IsEmpty())
        {
            BIASERR("Image::Append(): one or both images are empty");
          return -1;
        }

      src1 = GetImageData();
      src2 = img.GetImageData();

      dst = new StorageType[GetWidth() * GetHeight() * (GetChannelCount() + 1)];
      write = dst;

      if (InterleavedDataOrder_)      	
        {  
          for (unsigned long int p = 0; p < GetPixelCount(); p++)
            {
              for (unsigned long int c = 0; c < GetChannelCount(); c++)
                *write++ = *src1++;
              *write++ = *src2++;
            }
        }
      else
        {
          memcpy(write, src1, GetSizeByte());
          write += GetPixelCount() * GetChannelCount();
          memcpy(write, src2, img.GetSizeByte());
        }

      //         if (GetChannelCount() == 4)
      //             ColorModel_ = CM_RGBA;
      //         if (GetChannelCount() == 2)
      //             ColorModel_ = CM_GreyA;

      ReleaseImageDataPointer();
      ChannelCount_ += 1;
      if (InterleavedDataOrder_)
        WidthStep_ = Width_ * Depth_ * ChannelCount_;
      else
        WidthStep_ = Width_ * Depth_;

      RedirectImageDataPointer(dst);

      return 0;
    }

  template<class StorageType>
  int Image<StorageType>::DeleteNBorderPixel(int n, StorageType delVal)
  {
  	unsigned cc = GetChannelCount();
  	unsigned w = GetWidth();
  	unsigned h = GetHeight();
  	int count =0;
  	StorageType **src = GetImageDataArray();
  	for (int y = 0; y < (int)h; y++) {
  		for (int x = 0; x < (int)w; x++) {
  			for (unsigned c = 0; c < GetChannelCount(); c++){
  				if(x<n || y<n || x> int(w)-n || y>int(h)-n){
  					src[y][x*cc+c] = delVal;
  					count++;
  				}
      	}
  		}
  	}
  	return count;
  }
  
  template<class StorageType>
  int Image<StorageType>::RemoveChannel(unsigned int channel)
    {
      StorageType *src1,*dst, *write;
      
      if (IsEmpty()){
				BIASERR("Image::RemoveChannel(): image is empty");
        return -1;
      }

      src1 = GetImageData();      
      dst = new StorageType[GetWidth() * GetHeight() * (GetChannelCount() -1)];
      write = dst;

      if (InterleavedDataOrder_) {
      	for (unsigned long int p = 0; p < GetPixelCount(); p++) {
      		for (unsigned long int c = 0; c < GetChannelCount(); c++){
      			if(c != channel) *write++ = *src1++;
      			else{src1++;}
      		}  
      	}
      }
      else {
      	for(unsigned i=0; i< GetChannelCount();i++){
      		if(i != channel) 
      			memcpy(write, src1, GetPixelCount()); // copy first channel
      		write += GetPixelCount(); //increase write position by one channel
      	}      	
      }
      
      ReleaseImageDataPointer();
      ChannelCount_ -= 1;
      if (InterleavedDataOrder_)
        WidthStep_ = Width_ * Depth_ * ChannelCount_;
      else
        WidthStep_ = Width_ * Depth_;

      RedirectImageDataPointer(dst);

      return 0;
    }
  
  
  template<class StorageType>
    void
    Image<StorageType>::Release()
    {
      delete[] (char**) ImageDataArray_;
      ImageDataArray_ = NULL;
      delete[] (char*) ImageData_;
      ImageData_ = NULL;
      ChannelCount_ = 0;
      Width_ = Height_ = 0;
      // do not set depth to zero, it is fixed in the templated image case
      BitDepth_ = 8 * Depth_;
      WidthStep_ = 0;
      ColorModel_ = CM_invalid;

      GetROI()->Release();
      _MetaData.clear();
    }

  /// Jan Woetzel 01/2003
  template<class StorageType>
    Image<float>
    Image<StorageType>::Reciprocal(const float factor,
        const unsigned short int channel)
    {



  // OK, work...
  Image<float>
    res(GetWidth
        (), GetHeight
        ()); // result image
  StorageType *p_src; // pointer to data in src image
  float *p_dest; // pointer to data in dest image

  p_src = GetImageData() + channel * GetPixelCount(); // pointer to start of data in src image
  p_dest = res.GetImageData(); // pointer to start of data in dest image
  for (unsigned int i = 0; i < GetPixelCount(); i++)
    {
      // set value
      if (p_src[i] == 0)
        {
          // divison by zero
          p_dest[i] = 0.0f; // 0, +inf or -inf. or...
        }
      else
        {
          p_dest[i] = float(factor / p_src[i]);
        };
    };

  return res;
}


template <class StorageType>
void Image<StorageType>::
Invert()
{
    StorageType *p=GetImageData();
    unsigned int count = GetPixelCount() * GetChannelCount();
    for (register int i=count; i>0 ; i--){
        *p = MaxSTValue() - *p;
        p++;
    }
}


template<class StorageType>
int Image<StorageType>::
InitWithForeignData(unsigned int width, unsigned int height, 
                    unsigned int channels, void *data, const bool interleaved)
{
  //BIASERR("this function does not work completly, the destructor core dumps after using InitWithForeignData(). First guess is that is messes up the ROI");
  // first implementation uses standard init, then releses allocated data
  // and adjusts data_ to data
  // this is quite inefficient and will be changed in the near future
  ImageBase::Init(width, height, channels, StorageType_, interleaved);
  ReleaseImageDataPointer();
  RedirectImageDataPointer(data);
  return 0;
}

} // namespace BIAS


// implementation in extra file:
#include "ImageOperators.cpp"
//
// solve explicit instantiation
//

#define INSTANCE_Image(type)\
template class BIASImageBase_EXPORT Image<type>;

// create instances
INSTANCE_Image(unsigned char)
INSTANCE_Image(float)
#ifdef BUILD_IMAGE_INT
INSTANCE_Image(int)
#endif
#ifdef BUILD_IMAGE_CHAR
INSTANCE_Image(char)
#endif
#ifdef BUILD_IMAGE_SHORT
INSTANCE_Image(short)
#endif
#if defined(BUILD_IMAGE_USHORT)
INSTANCE_Image(unsigned short)
#endif
#ifdef BUILD_IMAGE_DOUBLE
INSTANCE_Image(double)
#endif
#ifdef BUILD_IMAGE_UINT
INSTANCE_Image(unsigned int)
#endif
