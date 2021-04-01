/*
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003-2009    (see file CONTACT for details)
  Multimediale Systeme der Informationsverarbeitung
  Institut fuer Informatik
  Christian-Albrechts-Universitaet Kiel

  Johan Skoglund
  skoglund@isy.liu.se
  CVL/isy
  university of Linkoeping
  sweden

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


#ifndef __Convolution_hh__
#define __Convolution_hh__

#include <typeinfo>

#include "FilterNToN.hh"
#include "FilterMask.hh"

#define D_GRADGAUSS_KERNEL 0x00000001
#define D_CONV_KERNEL      0x00000002
#define D_GRAD_DEB         0x00000004
#define D_WRITE_IMAGES     0x00000008
#define D_CONV_TYPES       0x00000010
#define D_HESSGAUSS_KERNEL 0x00000012

// data types, in which intermediate values are computed
#define CONV_FLOAT float
#define CONV_INT   int

namespace BIAS {

  /** @class Convolution
      @ingroup g_filter
      @brief generic convolution class.

      Use as follows:
      - SetKernel(FilterMask) 
      - Filter(source, destination)
     
      Center pixel is determined by (size-1)>>1.

      @author woelk/koeser/skoglund
      @date 11/2004 */

  template <class InputStorageType, class OutputStorageType> 
  class BIASFilter_EXPORT Convolution 
    : public FilterNToN<InputStorageType,  OutputStorageType>
  {
  public:
    Convolution();
    Convolution(const Convolution<InputStorageType, OutputStorageType>& other);
    virtual ~Convolution();

    /** @brief decides on the image types which
        FilterFunction should be called */
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst);


    /** @brief do the convolution using integer arithmetics and shifts */
    virtual int FilterInt(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    /** @brief do the convolution using floating point calculations */
    virtual int FilterFloat(const Image<InputStorageType>& src, 
                            Image<OutputStorageType>& dst);

    virtual FilterNToN<InputStorageType, OutputStorageType>* Clone() const
    { return new Convolution<InputStorageType, OutputStorageType>(*this); }

    /** @brief get the current filter mask */
    inline void GetKernel(FilterMask& fm) const
    { fm = _fm; }

    /** @brief set a new filter mask */
    inline void SetKernel(FilterMask& fm)
    { _fm = fm;}

    /** @brief prints the used filter mask, for debugging */
    void PrintKernel() const;

    /** @brief decides if calculation is done in integer or floating point */
    inline bool CalculationInFloat() const
    { return ((typeid(OutputStorageType)==typeid(float)) ||
              (typeid(OutputStorageType)==typeid(double))||
              (typeid(InputStorageType)==typeid(float)) ||
              (typeid(InputStorageType)==typeid(double))); };

  protected:
    /// the kernel data used for convolution
    FilterMask _fm;

    /// temporary memory, needed for separable filters when float filtering
    Image<CONV_FLOAT> _tmpFloat;

    /// temporary memory, needed for separable filters when integer filtering
    Image<CONV_INT> _tmpInt;

    /// @brief worker function for int matrix convolution
    int ConvIntMat_(const Image<InputStorageType> &src, 
                    Image<OutputStorageType> &dst);

    /// @brief worker function for int separated (horizontal) convolution
    int ConvIntHori_(const Image<InputStorageType> &src,
                     Image<CONV_INT> &dst);

    /// @brief worker function for int separated (vertical) convolution
    int ConvIntVert_(const Image<CONV_INT> &src,
                     Image<OutputStorageType> &dst);
    
    /// @brief worker function for float matrix convolution
    int ConvFloatMat_(const Image<InputStorageType> &src,
                      Image<OutputStorageType> &dst);

    /// @brief worker function for float separated (horizontal) convolution
    int ConvFloatHori_(const Image<InputStorageType> &src,
                       Image<CONV_FLOAT> &dst);
    
    /// @brief worker function for float separated (vertical) convolution
    int ConvFloatVert_(const Image<CONV_FLOAT> &src,
                       Image<OutputStorageType> &dst);

    inline void GetBordersValid_(int &border_x, int &border_y) const
    { _fm.GetSize(border_x, border_y); }

  }; // class 


} // namespace

#endif // __Convolution_hh__
