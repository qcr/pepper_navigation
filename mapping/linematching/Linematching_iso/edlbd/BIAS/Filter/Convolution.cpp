/*
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2004 (see file CONTACT for details)
  Johan Skoglund
  skoglund@isy.liu.se
  CVL/isy
  university of Linkoeping
  sweden

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

/// \todo This file contains no kind of optimization at all for the moment.
/// To be added later on

#include "Convolution.hh"




using namespace BIAS;
using namespace std;

//calculates the center pixel in the convolution kernel.
#define KERNCENTER(thesize) ((thesize-1)>>1)


template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvIntMat_(const Image<InputStorageType> &src, Image<OutputStorageType> &dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvIntMat_\n");
  const InputStorageType **srcstart=src.GetImageDataArray();
  OutputStorageType **dststart=dst.GetImageDataArray();
  const Matrix<FM_INT> *kernel=_fm.GetKerneli();
  int kern_width=kernel->GetCols();
  int kern_height=kernel->GetRows();
  int img_width=src.GetWidth();
  int img_height=src.GetHeight();
  int channelcount=src.GetChannelCount();

  int kern_center_x=KERNCENTER(kern_width);
  int kern_center_y=KERNCENTER(kern_height);

  signed char rightshift=_fm.GetKernelRightShift();
  const InputStorageType **srcida = NULL;
  OutputStorageType **dstida = NULL;
  
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;

  int offset = 0;
  for(int chloop=0;chloop<channelcount;chloop++) {
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }
    for(int img_x=0;img_x<img_width;img_x++)
      for(int img_y=0;img_y<img_height;img_y++) {
        register CONV_INT sum=0;
        for(int kern_x=0;kern_x<kern_width;kern_x++)
          for(int kern_y=0;kern_y<kern_height;kern_y++) {
            //kern_center_x-kern_x -> img_offset
            int img_offset_x=kern_center_x-kern_x;
            int img_offset_y=kern_center_y-kern_y;
            if(img_offset_x+img_x<0)
              continue;
            if(img_offset_y+img_y<0)
              continue;
            if(img_x+img_offset_x>=img_width)
              continue;
            if(img_y+img_offset_y>=img_height)
              continue;
            sum +=(CONV_INT) (srcida[img_y+img_offset_y][(img_x+img_offset_x)
                                                         *PixelSpacing+offset]*
                              ((*kernel)[kern_y][kern_x]));
          }
        dstida[img_y][img_x*PixelSpacing+offset]=
          (OutputStorageType)((sum)>>rightshift);
      }
  }

  return 0;
}

template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvIntHori_(const Image<InputStorageType> &src, Image<CONV_INT> &dst) 
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvIntHori_\n");
  const InputStorageType **srcstart=src.GetImageDataArray();
  CONV_INT **dststart=dst.GetImageDataArray();
  const Vector<FM_INT> *kernel=_fm.GetSepih();
  int kern_width=kernel->Size();
  int img_width=src.GetWidth();
  int img_height=src.GetHeight();
  int channelcount=src.GetChannelCount();

  int kern_center_x=KERNCENTER(kern_width);

  signed char rightshift=_fm.GetHorizRightShift();
  const InputStorageType **srcida = NULL;
  CONV_INT **dstida = NULL;
  
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;
  int offset = 0;
  for(int chloop=0;chloop<channelcount;chloop++) {
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }
    
    for(int img_y=0;img_y<img_height;img_y++) {
      for(int img_x=0;img_x<img_width;img_x++) {
        int kern_xmax=min((kern_center_x+img_x+1),kern_width);
        int kern_xmin=max(0,(img_x+kern_center_x-img_width+1));
        register CONV_INT sum=0;

        for(int kern_x=kern_xmin;kern_x<kern_xmax;kern_x++)	{
          int img_offset_x=kern_center_x-kern_x;
          sum += 
            (CONV_INT)(srcida[img_y][(img_x+img_offset_x)*PixelSpacing+offset])
            *((*kernel)[kern_x]);
        }
        dstida[img_y][img_x*PixelSpacing+offset] = 
          (CONV_INT)((sum)>>rightshift);
      }
    }
  }

  return 0;
}

template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvIntVert_(const Image<CONV_INT> &src, Image<OutputStorageType> &dst) 
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvIntVert_\n");
  const CONV_INT **srcstart=src.GetImageDataArray();
  OutputStorageType **dststart=dst.GetImageDataArray();
  const Vector<FM_INT> *kernel=_fm.GetSepiv();
  int kern_height=kernel->Size();
  int img_width=src.GetWidth();
  int img_height=src.GetHeight();
  int channelcount=src.GetChannelCount();

  int kern_center_y=KERNCENTER(kern_height);

  signed char rightshift=_fm.GetVertRightShift();
  const CONV_INT **srcida = NULL;
  OutputStorageType **dstida = NULL;
  
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;

  int offset = 0;

  for(int chloop=0;chloop<channelcount;chloop++) {
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }
     
    for(int img_y=0;img_y<img_height;img_y++) {
      int kern_ymax=min((kern_center_y+img_y+1),kern_height);
      int kern_ymin=max(0,(img_y+kern_center_y-img_height+1));
      
      for(int img_x=0;img_x<img_width;img_x++) {
        register CONV_INT sum=0;
        for(int kern_y=kern_ymin;kern_y<kern_ymax;kern_y++)  {
          int img_offset_y=kern_center_y-kern_y;
          sum += 
            (CONV_INT)(srcida[img_y+img_offset_y][img_x*PixelSpacing+offset])*
            ((*kernel)[kern_y]);
        }
        dstida[img_y][img_x*PixelSpacing+offset]=
          (OutputStorageType)((sum)>>rightshift);
      }
    }
  }
  return 0;
}

template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvFloatMat_(const Image<InputStorageType> &src,
              Image<OutputStorageType> &dst) { 
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvFloatMat_\n");
  const InputStorageType **srcstart  = src.GetImageDataArray();
  OutputStorageType **dststart = dst.GetImageDataArray();
  const Matrix<FM_FLOAT> *kernel=_fm.GetKernelf();
  const int kern_width   = kernel->GetCols();
  const int kern_height  = kernel->GetRows();
  const int img_width    = src.GetWidth();
  const int img_height   = src.GetHeight();
  const int channelcount = src.GetChannelCount();
  const int kern_center_x = KERNCENTER(kern_width);
  const int kern_center_y = KERNCENTER(kern_height);

  const InputStorageType **srcida = NULL;
  OutputStorageType **dstida = NULL;
  
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;

  int offset = 0;
  for(int chloop=0;chloop<channelcount;chloop++) {
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }
    for(int img_x=0;img_x<img_width;img_x++)
      for(int img_y=0;img_y<img_height;img_y++) {
        register CONV_FLOAT sum=0;
        for(int kern_x=0;kern_x<kern_width;kern_x++)
          for(int kern_y=0;kern_y<kern_height;kern_y++) {
            //kern_center_x-kern_x -> img_offset
            int img_offset_x=kern_center_x-kern_x*PixelSpacing;
            int img_offset_y=kern_center_y-kern_y;
            if(img_offset_x+img_x<0)
              continue;
            if(img_offset_y+img_y<0)
              continue;
            if(img_x+img_offset_x>=img_width)
              continue;
            if(img_y+img_offset_y>=img_height)
              continue;
            sum += (CONV_FLOAT)srcida[img_y+img_offset_y][(img_x+img_offset_x)*
                                                          PixelSpacing+offset] 
              * ((*kernel)[kern_y][kern_x]);
          }
        dstida[img_y][img_x*PixelSpacing+offset] = (OutputStorageType)sum;
      }
  }

  return 0;
}

template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvFloatHori_(const Image<InputStorageType> &src, Image<CONV_FLOAT> &dst)
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvFloatHori_\n");
  const InputStorageType **srcstart=src.GetImageDataArray();
  CONV_FLOAT **dststart=dst.GetImageDataArray();
  const Vector<FM_FLOAT> *kernel=_fm.GetSepfh();
  int kern_width=kernel->Size();
  int img_width=src.GetWidth();
  int img_height=src.GetHeight();
  int channelcount=src.GetChannelCount();

  int kern_center_x=KERNCENTER(kern_width);
  const InputStorageType **srcida = NULL;
  CONV_FLOAT **dstida = NULL;
  int offset = 0;
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;
  

  for(int chloop=0;chloop<channelcount;chloop++)  {
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }

    
    for(int img_y=0;img_y<img_height;img_y++){
      for(int img_x=0;img_x<img_width;img_x++) {
        int kern_xmax=min((kern_center_x+img_x+1),kern_width);
        int kern_xmin=max(0,(img_x+kern_center_x-img_width+1));
        register CONV_FLOAT sum=0;
        for(int kern_x=kern_xmin;kern_x<kern_xmax;kern_x++)	      {
          int img_offset_x=kern_center_x-kern_x;
          sum+=CONV_FLOAT(srcida[img_y]
                          [(img_x+img_offset_x)*PixelSpacing+offset])*
            ((*kernel)[kern_x]);
        }
        dstida[img_y][img_x*PixelSpacing+offset]=(CONV_FLOAT)sum;
      }
    }
  }

  return 0;
}

template <class InputStorageType,class OutputStorageType>
int Convolution<InputStorageType,OutputStorageType>::
ConvFloatVert_(const Image<CONV_FLOAT> &src, Image<OutputStorageType> &dst) 
{ 
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::ConvFloatVert_\n");
  const CONV_FLOAT **srcstart=src.GetImageDataArray();
  OutputStorageType **dststart=dst.GetImageDataArray();
  const Vector<FM_FLOAT> *kernel=_fm.GetSepfv();
  int kern_height=kernel->Size();
  int img_width=src.GetWidth();
  int img_height=src.GetHeight();
  int channelcount=src.GetChannelCount();

  int kern_center_y=KERNCENTER(kern_height);
  const CONV_FLOAT **srcida = NULL;
  OutputStorageType **dstida = NULL;
  int offset = 0;
  // distance between neighboring pixels in storagetypes
  const int PixelSpacing = (src.IsInterleaved())? channelcount : 1;
  for(int chloop=0;chloop<channelcount;chloop++)    { 
    if (src.IsInterleaved()) {
      srcida = srcstart;
      dstida = dststart;
      // ida[y][PixelSpacing*x] gives first channel, need an offset
      offset = chloop;
    } else {
      // get start of plane:
      srcida = srcstart + chloop*img_height;
      dstida = dststart + chloop*img_height;
      offset = 0;
    }
    for(int img_y=0;img_y<img_height;img_y++) {
      int kern_ymax=min((kern_center_y+img_y+1),kern_height);
      int kern_ymin=max(0,(img_y+kern_center_y-img_height+1));
      for(int img_x=0;img_x<img_width;img_x++) {
        register CONV_FLOAT sum=0;
 	for(int kern_y=kern_ymin;kern_y<kern_ymax;kern_y++)  {
	  //         for(int kern_y=0;kern_y<kern_height;kern_y++)	      {
          int img_offset_y=kern_center_y-kern_y;
         
          sum += 
            (CONV_FLOAT)(srcida[img_y+img_offset_y][img_x*PixelSpacing+offset])
            * ((*kernel)[kern_y]);
  
	}
        dstida[img_y][img_x*PixelSpacing+offset] = (OutputStorageType)sum;
      }
    }
  }
  return 0;
}

template <class InputStorageType, class OutputStorageType> 
Convolution<InputStorageType,OutputStorageType>::
Convolution()
  : FilterNToN<InputStorageType,OutputStorageType>()
{
  _fm.SetZero();
}

template <class InputStorageType, class OutputStorageType> 
Convolution<InputStorageType,OutputStorageType>::
Convolution(const Convolution<InputStorageType, OutputStorageType>& other)
  : FilterNToN<InputStorageType, OutputStorageType>(other), _fm(other._fm)
{
}

template <class InputStorageType, class OutputStorageType> 
Convolution<InputStorageType,OutputStorageType>::~Convolution()
{
 _fm.SetZero();
}

template <class InputStorageType, class OutputStorageType> 
int Convolution<InputStorageType,OutputStorageType>::
Filter(const Image<InputStorageType>& src, Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::Filter\n");

  // check if output image is floating point, if so, do float convolution:
  // and also if the input image is float, we must use this, because the
  // int mask approximation comes into trouble with float input images
  if (CalculationInFloat()) {
    BIASDOUT(D_CONV_TYPES,"Convolution is done with floating point numbers.");
    return Convolution<InputStorageType,OutputStorageType>::FilterFloat(src,dst);
  }
  // no float => integer type, do integer arithmetic convolution
  BIASDOUT(D_CONV_TYPES, "Convolution is done with integer arithmetic.");
  return Convolution<InputStorageType,OutputStorageType>::FilterInt(src,dst);
}

template <class InputStorageType, class OutputStorageType> 
int Convolution<InputStorageType,OutputStorageType>::
FilterInt(const Image<InputStorageType>& src, Image<OutputStorageType>& dst) 
{
  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::FilterInt\n");
  BIASDOUT(D_CONV_KERNEL,"Kernel is "<<_fm);

  if(!dst.SamePixelAndChannelCount(src)) {
    dst.Release();
    dst.Init(src.GetWidth(),src.GetHeight(),src.GetChannelCount(),
             src.GetStorageType(),src.IsInterleaved());
  }
  if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_full) {
    BIASERR("BorderHandling TBH_full not implemented");
    return -1;
  }

  if(_fm.IsSeparable()) {
    if(!_tmpInt.SamePixelAndChannelCount(src)) {
      _tmpInt.Release();
      _tmpInt.Init(src.GetWidth(),src.GetHeight(),src.GetChannelCount(),
                   src.GetStorageType(),src.IsInterleaved());
    }
    ConvIntHori_(src, _tmpInt);
    ConvIntVert_(_tmpInt, dst);

  } else ConvIntMat_(src,dst);

  unsigned int ulx,uly,lrx,lry;
  src.GetROICorners(ulx,uly,lrx,lry);
  if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_same) {
    dst.GetROI()->SetCorners(ulx,uly,lrx,lry);
  } else if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_valid) {
    int kwidth,kheight;
    if(_fm.IsSeparable()) {
      kwidth  = _fm.GetSepih()->Size();
      kheight = _fm.GetSepiv()->Size();
    } else {
      kwidth  = _fm.GetKerneli()->GetCols();
      kheight = _fm.GetKerneli()->GetRows();
    }
    dst.SetROICorners(ulx+KERNCENTER(kwidth),uly+KERNCENTER(kheight),
               lrx-KERNCENTER(kwidth),lry-KERNCENTER(kheight));
  }

  return 0;
}

template <class InputStorageType, class OutputStorageType> 
int Convolution<InputStorageType,OutputStorageType>::
FilterFloat(const Image<InputStorageType>& src, Image<OutputStorageType>& dst) 
{

  BIASCDOUT(D_FILTERBASE_CALLSTACK,"Convolution::FilterFloat\n");
  BIASDOUT(D_CONV_KERNEL,"Kernel is "<<_fm);
  if(!dst.SamePixelAndChannelCount(src)) {
    dst.Release();
    dst.Init(src.GetWidth(),src.GetHeight(),src.GetChannelCount(),
             dst.GetStorageType(),src.IsInterleaved());
  }
  if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_full) {
    BIASERR("BorderHandling TBH_full not implemented");
    return -1;
  }

  if(_fm.IsSeparable()) {
    if(!_tmpFloat.SamePixelAndChannelCount(src)) {
      _tmpFloat.Release();
      _tmpFloat.Init(src.GetWidth(),src.GetHeight(),src.GetChannelCount(),
                     ImageBase::ST_float, src.IsInterleaved());
    }
    ConvFloatHori_(src, _tmpFloat);

    ConvFloatVert_(_tmpFloat, dst);

  } else {
    ConvFloatMat_(src, dst);

  }

  unsigned int ulx,uly,lrx,lry;
  src.GetROICorners(ulx,uly,lrx,lry);
  
  if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_same) {
    dst.GetROI()->SetCorners(ulx,uly,lrx,lry);
  } else if(this->GetBorderHandling()==Convolution<InputStorageType,OutputStorageType>::TBH_valid)  {
    int kwidth,kheight;
    if(_fm.IsSeparable())  {
      kwidth=_fm.GetSepfh()->Size();
      kheight=_fm.GetSepfv()->Size();
    } else {
      kwidth=_fm.GetKernelf()->GetCols();
      kheight=_fm.GetKernelf()->GetRows();
    }
    dst.GetROI()->SetCorners(ulx+KERNCENTER(kwidth),uly+KERNCENTER(kheight),
               lrx-KERNCENTER(kwidth),lry-KERNCENTER(kheight));
  }
  return 0;
}

template <class InputStorageType, class OutputStorageType> 
void Convolution<InputStorageType,OutputStorageType>:: 
PrintKernel() const
{
  if (CalculationInFloat()) { // float
    cout << "float: ";
    if (_fm.IsSeparable()){
      cout << "sep horiz: "<< *_fm.GetSepfh()<<"  vert: "<<*_fm.GetSepfv();
    } else {
      cout << "kernel: "<<*_fm.GetKernelf();
    }
  } else { // int
    cout << "int: ";
    if (_fm.IsSeparable()){
      cout << "sep horiz: "<< *_fm.GetSepih()<<"  vert: "<<*_fm.GetSepiv();
    } else {
      cout << "kernel: "<<*_fm.GetKerneli();
    }
  }
  cout << endl;
}

#define FILTER_INSTANTIATION_CLASS Convolution
#include "Filterinst.hh"
