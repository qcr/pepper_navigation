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


#ifndef __FilterBase_hh__
#define __FilterBase_hh__

#include <Base/Image/Image.hh>

#include <string>
#include <vector>

#define D_FILTERBASE_CALLSTACK 0x80000000

namespace BIAS {

  /** @enum EGradType
      @brief different gradients to be used by hiogher level classes
      - GT_Sobel3x3 gradient using 3x3 sobelx and sobely filters
      - GT_Gauss gradient using derivatives of gauss with square kernel
      - GT_GaussAsymmetric gradient using derivatives of gauss */
  enum EGradType { GT_Simple, GT_Sobel3x3, GT_Gauss, GT_GaussAsymmetric };

  /** @enum LowPassType
      @brief determines which lowpass filter is used before subsampling
      - LPT_Mean use non-weighted mean across a window
      - LPT_Gauss weight (in a window) with gaussian function
      - LPT_GaussThreshold same as Gauss but does not concerne values
        under a specified threshold */
  enum LowPassType { LPT_Mean=0, LPT_Gauss, LPT_Binomial,
                     LPT_GaussThreshold};

  /** @class FilterBase 
   *  @test TestFilterBase.hh 
      @ingroup g_filter
      @brief virtual parent class for API definition of all (future) filters 
      
      We would like to have a general (abstract) class FilterBase, 
      which should be located in BIAS/Filter and which has the main function 
      Filter(inputimage, outputimage).
      This enforces all our filters to have a common interface.
      Derived from that, we have Convolution, Gauss, Median, ...
      To keep the interface clean and exchangable, all Filters should be 
      derived from FilterBase and be usable with the standard interface.
      The standard function checks the desired properties of the Kernel and
      behaves accordingly,
      i.e. may call the optimized subroutines, e.g. FastSobel3x3.
      These subroutines may also be directly called if the user knows 
      exactly which implementation he wants.
      FilterBase also holds an enum for the output size, 
      such as "full", "same" or "valid". 

      @author skoglund, koeser, woelk 11/2004  */ 

  template <class InputStorageType, class OutputStorageType> 
  class BIASFilter_EXPORT FilterBase : public Debug
  {
        
  public:
    /** enum for border calculation, same meanings as in matlab:
        - TBH_valid: returns only the pixels whose values can be computed 
                     without using zero padding of the input image. The 
                     resulting output image is smaller than the input image. 
                     The ROI of the output image is set accordingly.
        - TBH_same:  returns the set of pixels that can be computed by applying
                     the filter to all pixels that are actually part of the 
                     input image. Border pixels are computed using zero 
                     padding, but the center pixel of the computational kernel 
                     is applied only to pixels in the image. This results in an
                     output image that is the same size as the input image.
        - TBH_full:  returns the full convolution. This means conv2 returns all
                     pixels for which any of the pixels in the computational 
                     molecule overlap pixels in the image, even when the center
                     pixel is outside the input image. The resulting output 
                     image is larger than the input image. This is not 
                     implemented at the moment. */
    enum TBorderHandling { TBH_full, TBH_same, TBH_valid };

    FilterBase();
    FilterBase(const FilterBase<InputStorageType, OutputStorageType>& other);
    virtual ~FilterBase();

    /// prototyp for filter computation function
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst) = 0;
    
    void GetBorders(int& border_x, int& border_y) const;


    /** @brief run over all pixel which do not belong to the roi and set them 
        to some value (typically black)
        @author koeser 12/2008 */
    void SetNonROIToValue(Image<OutputStorageType>& dst,
                          const std::vector<OutputStorageType>& t) const;

    /** @brief run over all pixel which do not belong to the roi and set them 
        to the value in the source image regardless of the roi of the source.
        Both images must have same size.
        @author koeser 12/2008 */
    void CopyNonROIFromSource(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst) const;
    

    /////////////////////////////////////////////////////////////////////////
    
    inline void SetBorderHandling(const int bh) { _FilterBorderHandling=bh; }
    
    inline int GetBorderHandling() const { return _FilterBorderHandling; } 

  protected:
    int _FilterBorderHandling; // defaults to valid

    virtual void GetBordersValid_(int& border_x, int& border_y) const = 0;

  }; // class




} // namespace


                            
#endif // __FilterBase_hh__
