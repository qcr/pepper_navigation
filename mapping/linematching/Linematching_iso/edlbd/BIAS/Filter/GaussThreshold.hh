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


#ifndef __GaussThreshold_hh__
#define __GaussThreshold_hh__

#include "Convolution.hh"
#include "Gauss.hh"

namespace BIAS {
  /** @class GaussThreshold
      @ingroup g_filter
      @brief smoothing with gaussian kernel using a threshold

      All pixel values below the trehshold are not concerned for
      smoothing the image.
      
      @author cmenk 08/2007 */
  template <class InputStorageType, class OutputStorageType> 
  class BIASFilter_EXPORT GaussThreshold 
    : public Convolution<InputStorageType, OutputStorageType>
  {
  public:
    GaussThreshold();
    GaussThreshold(const GaussThreshold<InputStorageType, OutputStorageType>& other); 
    ~GaussThreshold();
    
    /** @brief sets gauss kernel if params changed and calls convolution or
        fast grey implementation if possible */
    virtual int Filter(const Image<InputStorageType>& src, 
                       Image<OutputStorageType>& dst);

    /** @brief 7x7 gauss filtering, values below threshold are ignored
        useful for depth map filtering
        @author koeser 04/2007 */
    int
    Filter7x7GreyIgnoreBelowThreshold(const Image<InputStorageType>& src, 
                                      Image<OutputStorageType>& dst,
                                      const double thresh);

    /** @brief 7x7 gauss filtering, values below threshold are ignored
        and only pixels below threshold are filed
        @author cmenk 08/2007 */
    int
    Filter7x7GreyOnlyBelowThreshold(const Image<InputStorageType>& src, 
                                    Image<OutputStorageType>& dst,
                                    const double thresh);
    
    /////////////////////////////////////////////////////////////////////////

    inline void SetTreshold(const double th) { _Threshold = th; }

    inline double GetTreshold() const { return _Threshold; }
    
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
    // all values below threshold are ignored by the filter
    double _Threshold;
    
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



#endif // __GaussThreshold_hh__
