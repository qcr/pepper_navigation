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


#ifndef __Mean_hh__
#define __Mean_hh__

#include "Convolution.hh"

//#include <Base/Common/BIASpragmaStart.hh>

namespace BIAS 
{
  /** @class Mean
      @ingroup g_filter
      @brief average mean filter
      
      @author woelk 09/2004 */
  template <class InputStorageType, class OutputStorageType>
  class BIASFilter_EXPORT Mean : public Convolution<InputStorageType, OutputStorageType>
  {
  public:
    Mean();
    Mean(const Mean<InputStorageType, OutputStorageType>& other);
    ~Mean();
    
    /** @brief averages over a region with constant weights
        @author woelk 09/2004 */
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst);

    /** @brief only calls filter, for consistency of params */
    virtual int FilterInt(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst); 
    
    /** @brief only calls filter, for consistency of params */
    virtual int FilterFloat(const Image<InputStorageType>& src, 
                            Image<OutputStorageType>& dst);

    virtual FilterNToN<InputStorageType, OutputStorageType>* Clone() const
    { return new Mean<InputStorageType, OutputStorageType>(*this); }

    /** deprecated
        @author woelk 09/2004 */
    int FilterMean(const Image<InputStorageType>& src, 
                   Image<OutputStorageType>& dst);

    /** dst width = src width - 1, dst height = src height - 1
        @todo support for ROI is still missing.
        @author woelk  01/2003 */
    int FilterMean2x2(const Image<InputStorageType>& src, 
                      Image<OutputStorageType>& dst);

    /** dst must be initialized:
        dst width = src width - 1, dst height = src height - 1
        Does support ROI. No specialization for integer data types.
        @author woelk 09/2004 */
    int FilterMean2x2Grey(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    /////////////////////////////////////////////////////////////////////////

    inline void SetSize(const int size) { _MeanSize=size; }
    inline int GetSize() const { return _MeanSize; }

  protected:
    int _MeanSize;

    /** sets the vector members of Convolution
        @author woelk 09/2004 */
    void _SetKernel();

    using Convolution<InputStorageType, OutputStorageType>::_fm;

    virtual void GetBordersValid_(int &border_x, int &border_y) const;

  }; // class




} // namespace

//#include <Base/Common/BIASpragmaEnd.hh>

#endif // __Mean_hh__
