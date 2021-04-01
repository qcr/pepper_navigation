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


#ifndef __Gauss_hh__
#define __Gauss_hh__

#include "Convolution.hh"

#define D_GAUSS_FAST_GREY 0x01

namespace BIAS {
  /** @class Gauss
      @ingroup g_filter
      @brief smoothing with gaussian kernel

      Parameters are sigma and ratio, optionally you can set half window size,
      which is computed from the other parameters otherwise.
      The gaussian kernel is separabel, using vector convolution
      The Gaussian function is truncated, when the ratio between the maximum 
      kernel element and the actual kernel element falls below ratio.

      If you have a one-channel (typically grey) image, the fast FilterGrey
      method comes into play, which calls the appropriate fast FilterNxNGrey
      function according to your sigma and cutoffratio. The fast functions work
      on the whole image and dont care about the roi in the src image.

      @author woelk 09/2004, koeser 06/2005 */
  template <class InputStorageType, class OutputStorageType> 
  class BIASFilter_EXPORT Gauss 
    : public Convolution<InputStorageType, OutputStorageType>
  {
  public:
    Gauss();
    Gauss(const Gauss<InputStorageType, OutputStorageType>& other);
    ~Gauss();
    
    /** @brief sets gauss kernel if params changed and calls convolution or
        fast grey implementation if possible */
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst);
    
    /** @brief sets gauss kernel if params changed and calls convolution */
    virtual int FilterFloat(const Image<InputStorageType>& src, 
                            Image<OutputStorageType>& dst);

    /** @brief sets gauss kernel if params changed and calls convolution */
    virtual int FilterInt(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    /** @brief wrapper around FilterNxNGrey, which decides on sigma and
        cutoffratio which filter to apply and how often 
        @param ResetROI reset the roi during filtering
        @author koeser 06/2005 */
    virtual int FilterGrey(const Image<InputStorageType>& src, 
                           Image<OutputStorageType>& dst,
                           bool ResetROI = false);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        @author koeser 06/2005 */
    virtual int Filter3x3Grey(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        @author koeser 06/2005 */
    virtual int Filter5x5Grey(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        Keep src value of test point if any value under filtermask is below
        threshold.
    */
    virtual int Filter3x3GreyThreshold(const Image<InputStorageType>& src, 
                                       Image<OutputStorageType>& dst,
                                       float threshold = 0.0);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored.
        Keep src value of test point if any value under filtermask is below
        threshold.
    */
    virtual int Filter5x5GreyThreshold(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst,
                              float threshold = 0.0);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored 
        @author koeser 06/2005 */
    virtual int Filter7x7Grey(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst);




    /** @brief 7x7 gauss filtering, values below threshold are ignored
        useful for depth map filtering
        @author koeser 04/2007 */
    int
    Filter7x7GreyIgnoreBelowThreshold(const Image<InputStorageType>& src, 
                                      Image<OutputStorageType>& dst,
                                      const InputStorageType& thresh);


    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        @author koeser 06/2005 */
    virtual int Filter9x9Grey(const Image<InputStorageType>& src, 
                              Image<OutputStorageType>& dst);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        @author koeser 06/2005 */
    virtual int Filter11x11Grey(const Image<InputStorageType>& src, 
                                Image<OutputStorageType>& dst);

    /** @brief fast, approximate, direct implementation, ignoring wrap-arounds,
        roi is updated but ignored
        @author koeser 06/2005 */
    virtual int Filter13x13Grey(const Image<InputStorageType>& src, 
                                Image<OutputStorageType>& dst);

    virtual Gauss<InputStorageType, OutputStorageType>* Clone() const
    { return new Gauss<InputStorageType, OutputStorageType>(*this); }
    
    /////////////////////////////////////////////////////////////////////////
    
    inline void SetSigma(const double si) { _GaussSigma=si; }

    inline double GetSigma() const  { return _GaussSigma; }

    inline void SetRatio(const double ratio) { _GaussRatio=ratio; }

    inline double GetRatio() const { return _GaussRatio; } 

    /** define the half win size of the kernel, if AdjustSigma is true
        sigma is computed according to the cut-off ratio, otherwise the ratio
        is adapted to the sigma */
    inline void SetHalfWinSize(const int hws, bool AdjustSigma=true)
    {
      if (AdjustSigma) {
        _GaussSigma = sqrt((double)hws*(double)hws/(-2.0*log(_GaussRatio))); 
      } else {
        _GaussRatio = 0.5*(exp(((double)hws*(double)hws)
                               /(-2.0*(_GaussSigma)*(_GaussSigma)))+
                            exp(((double)(hws+1.0)*(double)(hws+1.0))
                               /(-2.0*(_GaussSigma)*(_GaussSigma)))) ;
      }
    }


    // these are interfaces for class Tracker is Track is instantiated
    // with out param object
    double GetSigma(){ return _GaussSigma;};
    double GetRatio(){ return _GaussRatio;};
    
  protected:
    
    /// the parameter
    /// sigma of gaussian kernel
    double _GaussSigma;
    /// minimum ratio of 1D kernel center and border, ignore smaller entries
    double _GaussRatio;
    // the parameter at the time of the last call of _CalculateKernels 
    // (for caching)
    double _LastSigma, _LastRatio;

    /// calculates the kernel
    void _CalculateKernels(double Sigma, double Ratio);

    using Convolution<InputStorageType, OutputStorageType>::_fm;
    using Convolution<InputStorageType, OutputStorageType>::_tmpFloat;
    using Convolution<InputStorageType, OutputStorageType>::TBH_valid;

    /// to allow for iterated gauss convolution saving the intermediate
    /// image we need a different instance, this is caused by template concept
    Gauss<OutputStorageType, OutputStorageType> *_GaussOO;
    Gauss<InputStorageType, OutputStorageType> *_GaussIO;

  }; // class

} // namespace



#endif // __Gauss_hh__
