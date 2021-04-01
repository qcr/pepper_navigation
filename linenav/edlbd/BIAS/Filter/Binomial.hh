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

#ifndef __Binomial_hh__
#define __Binomial_hh__

#include "Convolution.hh"

#define D_BINOMIAL_KERNEL   0x00010000
#define D_BINOMIAL_WRITE_IM 0x00020000

namespace BIAS {
	/** 
	 * @class Binomial
	 * @test TestFilterBinomial.cpp
	 * @ingroup g_filter
	 * @brief binomial low pass filter class
	 * @author woelk 12/2004 
   */
  template <class InputStorageType, class OutputStorageType>
  class BIASFilter_EXPORT Binomial 
   : public Convolution<InputStorageType, OutputStorageType>
  {
   
  public:
    Binomial();
    Binomial(const Binomial<InputStorageType, OutputStorageType>& other);
    ~Binomial();

    /** @brief sets kernel if params changed and calls convolution */
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst);

    /** @brief no implementation, calls Filter */
    virtual int FilterFloat(const Image<InputStorageType>& src, 
                            Image<OutputStorageType>& dst);

    /** @brief no implementation, calls Filter */
    virtual int FilterInt(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    virtual FilterNToN<InputStorageType, OutputStorageType>* Clone() const
    { return new Binomial<InputStorageType, OutputStorageType>(*this); }

    /** @author woelk 12/2004 */
    int Filter3x3(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    /** @author woelk 12/2004 */
    int Filter5x5(const Image<InputStorageType>& src,
                  Image<OutputStorageType>& dst);

    /** @brief specialized function for convolution with 3x3 binomial filter.
        dst must be initialized with the right image size. Internal calculation
        is done using floating point numbers. 
        @author woelk 12/2004 */
    int Filter3x3ValidGreyFloat(const Image<InputStorageType>& src, 
                                Image<OutputStorageType>& dst);

    /** @brief specialized function for convolution with 5x5 binomial filter.
        dst must be initialized with the right image size. Internal calculation
        is done using floating point numbers. 
        @author woelk 12/2004 */
    int Filter5x5ValidGreyFloat(const Image<InputStorageType>& src, 
                                Image<OutputStorageType>& dst);

    /** @brief specialized function for convolution with 3x3 binomial filter.
        dst must be initialized with the right image size. Internal calculation
        is done using floating point numbers. 
        Uses "zero padding" of the image to achieve destination image ROIs
        of same size as source image ROIs. Untested !! 
        @author woelk 07/2005 */
    int Filter3x3SameGreyFloat(const Image<InputStorageType>& src, 
                               Image<OutputStorageType>& dst);
    

    //////////////////////////////////////////////////////////////////////
    
    inline int GetHalfWinSize() const
    { return _HalfWinSize; }

    inline void SetHalfWinSize(int hws)
    { _HalfWinSize = hws; }

  protected:
    /// the parameter
    int _HalfWinSize;
    int _LastHalfWinSize;

    /// calculates the kernel
    void _CalculateKernel(int HalfWinSize);

    virtual void GetBordersValid_(int &border_x, int &border_y) const;

    using Convolution<InputStorageType, OutputStorageType>::_FilterBorderHandling;
    using Convolution<InputStorageType, OutputStorageType>::TBH_valid;
    using Convolution<InputStorageType, OutputStorageType>::TBH_same;
    using Convolution<InputStorageType, OutputStorageType>::TBH_full;
    using Convolution<InputStorageType, OutputStorageType>::_tmpFloat;
    using Convolution<InputStorageType, OutputStorageType>::_fm;
    

  }; // class

} // namespace


#endif // __Binomial_hh__
