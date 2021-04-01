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


#ifndef __Rescale_hh__
#define __Rescale_hh__

#include "FilterBase.hh"
#include "FilterNToN.hh"
#include "Gauss.hh"
#include "Binomial.hh"
#include "Mean.hh"

#define D_RESCALE_INIT       0x00000001
#define D_RESCALE_WRITE_IMG  0x00000002

namespace BIAS 
{
enum InterpolationType { NearestNeighbour, Bilinear, Bicubic };


  /** @class Rescale
   *  @ingroup g_filter
   *  @brief Down-, Upsampling routines and Resize
   *
   *  This class is primarily a loose collection of several functions to
   *  rescale images while avoiding aliasing. If the rescale factor is 
   *  smaller than 1 (magnification), then the upsample functions are used.
   *  Otherwise the image is first smoothed using the active low pass filter
   *  and then subsampled. There are several specializations for colors or for
   *  power of 2 rescales.
   *
   *  Most functions are designed to run fast and not all of these are 
   *  consistent with the general parameters. Special attention must be paid to 
   *  a) the exact rescale factor, because it is sometimes rounded such that
   *     the new image has exact integer dimensions, this can even lead to 
   *     non-isotropic rescale in x and y direction.
   *  b) the offsets between the image coordinate systems: sometimes a 2x2 
   *     patch of pixels is averaged, which implies an offset of 0.5,0.5 
   *     compared to smoothing the image and sampling. See GetPositionOffset()
   *  c) the side effects caused by efficiency: if your sampling factor varies 
   *     from 1.9 to 2.1 then for instance 2.0 is handled differently because
   *     it can be implemented more efficiently. Also grey values are filtered 
   *     in a different way than rgba and so on.
   *  If you are not primarily interested in speed you might want to have a look
   *  at BIAS::ProjectionMapping or BIAS::AffineMapping to have full control.
   *
   *  @author woelk 09/2004 */
  template <class InputStorageType, class OutputStorageType>
  class BIASFilter_EXPORT Rescale : public FilterBase<InputStorageType, 
                                                      OutputStorageType>
  {
  public:
    Rescale();
    Rescale(const Rescale<InputStorageType, OutputStorageType>& other);
    virtual ~Rescale();
    
    Rescale<InputStorageType, OutputStorageType>& 
    operator=(const Rescale<InputStorageType, OutputStorageType>& other);

    /** scales the src to dst using the downsampling factor from SetFactor()
     */
    virtual int Filter(const Image<InputStorageType>& src,
                       Image<OutputStorageType>& dst);

 
    /////////////////////////////////////////////////////////////////////////

    /** @brief generic downsample function.
        @attention, results in coo system offset, when downsampling by 2^n !!
        see DownsampleBy2() for details
        @author woelk 09/2004 */
    int Downsample(const Image<InputStorageType>& src, 
                   Image<OutputStorageType>& dst);

    /** @brief downsamples src to dst of size newwidth x newheight
        @attention, results in coo system offset, when downsampling by 2^n !!
        see DownsampleBy2() for details
        @attention may result in different subsampling factors for x and y
        @author woelk 09/2004 **/
    int Downsample(const Image<InputStorageType>& src, 
                   Image<OutputStorageType>& dst,
                   const unsigned newwidth, const unsigned newheight);

    int Downsample(const Image<InputStorageType>& src, 
                   Image<OutputStorageType>& dst, const double factor);

  //   /** Takes the source image that has to have a defined ROI. Based on this 
//         ROI it will  store a  downsampled version in the destination image. 
//         The pixel values surounding the ROI will be  taken into account while 
//         executing the mean filter. Therefore it is not possible to downsample 
//         the whole image.
//         @param Source Image with a defined ROI
//         @param Destination Image where the rescaled image will be stored. Will 
//         initialized if required.
//         @param newwidth Desired width of the target image
//         @param newheight Desired height of the target image
//         @return -1 In any case of error (e.g. no ROI defined, ROI to close to
//         the edge or desired width/height does no work)
//         @date 26.09.2003
//         @author Ingo Thomsen **/
    // int DownsampleROI(const Image<InputStorageType>& src, 
    //                  Image<OutputStorageType>& dst,
    //                  const unsigned newwidth, const unsigned newheight);

    /////////////////////////////////////////////////////////////////////////
    //              specialized downsampling functions
    /////////////////////////////////////////////////////////////////////////

    /** Calls the appropriate function for the color model.
        Mean low pass filter of size 2x2 is used implicitly,
        resulting in the coordinate conversion equation:

        (2*xs+0.5, 2*ys+0.5) = (xb, yb) or
        (xs, ys) = ((xb-0.5)/2.0, (yb-0.5)/2.0)

        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (0.5, 0.5).

        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.

        @author woelk 09/2004 */
    int DownsampleBy2(const Image<InputStorageType>& src, 
                      Image<OutputStorageType>& dst);

    /** assumes that destination is initialized.
        mean low pass filter is used implicitly,
        resulting in the coordinate conversion equation:
         
        (2*xs+0.5, 2*ys+0.5) = (xb, yb) or
        (xs, ys) = ((xb-0.5)/2.0, (yb-0.5)/2.0)
         
        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (0.5, 0.5).
         
        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.

        Destination width = floor(source.GetWidth()/2)
        Destination height = floor(source.GetHeight()/2)
        Only for one channel images.
        Also make use of ROI in Source image (fast :-)) ).

        Not tested with odd size images.
         
        @author woelk 01/2003 */
    int DownsampleBy2Grey(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);

    /** assumes that destination is initialized. 
        mean low pass filter is used implicitly,
        resulting in the coordinate conversion equation:
         
        (2*xs+0.5, 2*ys+0.5) = (xb, yb) or
        (xs, ys) = ((xb-0.5)/2.0, (yb-0.5)/2.0)
         
        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (0.5, 0.5).
         
        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.

        Destination width = floor(source.GetWidth()/2)
        Destination height = floor(source.GetHeight()/2)
        only for three channel interleaved images
        not tested with odd size images
        @author woelk 01/2003 */
    int DownsampleBy2Color(const Image<InputStorageType>& src, 
                           Image<OutputStorageType>& dst);

    /////////////////////////////////////////////////////////////////////////

    /** calls the appropriate function for the color model
        mean low pass filter is usedimplicitly,
        resulting in the coordinate conversion equation:
         
        (4*xs+1.5, 4*ys+1.5) = (xb, yb) or
        (xs, ys) = ((xb-1.5)/4.0, (yb-1.5)/4.0)
         
        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (1.5, 1.5).
         
        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.

        @author woelk 09/2004 */
    int DownsampleBy4(const Image<InputStorageType>& src, 
                      Image<OutputStorageType>& dst);

    /** Assumes that destination is initialized. 
        The mean low pass filter is used implicitly,
        resulting in the coordinate conversion equation:
         
        (4*xs+1.5, 4*ys+1.5) = (xb, yb) or
        (xs, ys) = ((xb-1.5)/4.0, (yb-1.5)/4.0)
         
        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (1.5, 1.5).
         
        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.

        Destination width = floor(source.GetWidth()/4)
        Destination height = floor(source.GetHeight()/4)
        only for one channel grey images
        also make use of ROI in Source image (fast :-)) )
        @author woelk 09/2004 */
    int DownsampleBy4Grey(const Image<InputStorageType>& src, 
                          Image<OutputStorageType>& dst);


    /** The mean low pass filter is used implicitly,
        resulting in the coordinate conversion equation:
         
        (2^n*xs+2^(n-1)-0.5, 2^n*ys+2^(n-1)-0.5) = (xb, yb) or
        (xs, ys) = ((xb-2^(n-1)+0.5)/2^n, (yb-2^(n-1)+0.5)/2^n)
         
        where (xs, ys) is the pixel position in pixel coo. the smaller  
        destination image and (xb, yb) is the position in pixel coo. in the 
        bigger source image and the image is downsample by a facteo 2^n in 
        width and in height.
        E.g. the ray through position (0,0) in the small image intercepts the 
        bigger image at position (2^(n-1)-0.5, 2^(n-1)-0.5).
         
        This must be taken into account when calculating the corrsponding 
        K matrix of the smaller image. I.e. the principal point in the 
        smaller image cannot be calculated by simply dividing by 2, but 
        instead using the formula above.
        @author evers */
    int DownsampleBPoT(const Image<InputStorageType>& Source,
                       Image<OutputStorageType>& Destination);

    /////////////////////////////////////////////////////////////////////////
    //              upsampling functions
    /////////////////////////////////////////////////////////////////////////

    /** generic upsample function
        @author woelk 09/2004 */
    int Upsample(const Image<InputStorageType>& src, 
                 Image<OutputStorageType>& dst);

    /** Upsamples src to dst of size width by height.
        width > src.GetWidth() && height > src.GetHeight() is asserted.
        ROI not supported.
        @param exactfactor     this is the rescale factor, while new width and 
        newheight just determine the dimensions of the image. If exactfactor=0,
        then it is determined from the width ratio.
        @author ? */
    int Upsample(const Image<InputStorageType>& src, 
                 Image<OutputStorageType>& dst,
                 const unsigned newwidth, const unsigned newheight);

    /** @brief Upsamples src to dst of size width by height.
        Only for one channel grey images.
        ROI not supported.
        width > src.GetWidth() && height > src.GetHeight() is asserted.
        @param src [in], source image
        @param dst [out], destination image
        @param newwidth [in], new image width after upsample
        @param newheight [in], new image height after upsample
        @return 0 on success, < 0 otherwise        
        @author MIP */
    int UpsampleGrey(const Image<InputStorageType>& src, 
                     Image<OutputStorageType>& dst,
                     const unsigned newwidth, const unsigned newheight);

    /////////////////////////////////////////////////////////////////////////

    /** assumes that destination is initialized 
        dst width = 2*src.GetWidth()
        dst height = 2*src.GetHeight()
        only for interleaved images
        for images with arbitrary channel count
        @author woelk 04/2004 */
    int UpsampleBy2(const Image<InputStorageType>& src, 
                    Image<OutputStorageType>& dst);

    /** assumes that destination is initialized 
        dst width = 2*src.GetWidth()
        dst height = 2*src.GetHeight()
        @param InterpolationType which interpolation to use for off-grid pixel
        @param valmin/valmax if bicubic, limit range to these values
        only for one channel images
        @author woelk/koeser */
    int UpsampleBy2Grey(const Image<InputStorageType>& src, 
                        Image<OutputStorageType>&dst,
                        InterpolationType i=NearestNeighbour,
                        const double& valmin = 0.0, 
                        const double& valmax = 255.0);

    /** assumes that destination is initialized 
        dst width = 2*src.GetWidth()
        dst height = 2*src.GetHeight()
        @param InterpolationType which interpolation to use for off-grid pixel
        @param valmin/valmax if bicubic, limit range to these values
        only for three channel images
        @author koeser 06/2005 */
    int UpsampleBy2RGBInterleaved(const Image<InputStorageType>& src, 
                                  Image<OutputStorageType>&dst,
                                  InterpolationType i=NearestNeighbour,
                                  const double& valmin = 0.0, 
                                  const double& valmax = 255.0);
    
    /** updates _lowpass using _RescaleMeanType
        @author woelk 09/2004 */
    void UpdateLowpass(double factor);

    /////////////////////////////////////////////////////////////////////////
    //              get/set functions
    /////////////////////////////////////////////////////////////////////////

    /** the downsampling factor 
     */
    inline void SetFactor(double factor) { _RescaleFactor=factor; }

    inline double GetFactor() const { return _RescaleFactor; }

    /// Sets the lowpass filter. Creates a copy of the given filter.
    void SetLowPassFilter(const FilterNToN<InputStorageType, OutputStorageType>& lowpass);

    /// Sets the lowpass filter by a lowpass filter type.
    void SetLowPassType(int lpt);
    
    /// see DownsampleBy2 for explanation
    inline double GetPositionOffset() const { return _LastPositionOffset; };
    
    inline FilterNToN<InputStorageType, OutputStorageType>* GetLowpass()
    { return _lowpass; }

  protected:
    // the mean filter
    FilterNToN<InputStorageType, OutputStorageType> *_lowpass;
    
    // parameter 
    // _RescaleFactor<1.0 means upsampling
    // _RescaleFactor>1.0 means downsampling
    double _RescaleFactor;
    double _RescaleMeanSigmaFactor;
    
    /// see DownsampleBy2 for explanation
    double _LastPositionOffset;

    /** updates _mean using _RescaleMeanType
        and calls _mean->Filter(src, dst)
        @author woelk 09/2004 */
    int _ApplyMeanFilter(const Image<InputStorageType>& src, 
                         Image<OutputStorageType>& dst, const double factor);

    /** Assumes dst is initialized and ROI is set.
        For every pixel (x, y) in ROI of dst, a bilinear lookup at the 
        coordinates (x*src.Width/dst.Width, y*src.Height/dst.GetHeight)
        is performed.
        Only for one channel grey images
        @author woelk 09/2004 */
    int _FillInterpolatedGrey(const Image<OutputStorageType>& src, 
                              Image<OutputStorageType>& dst);

    /** Same as _FillInterpolatedGrey but for multi channel interleaved 
        images. 
        @author woelk 09/2004 */
    int _FillInterpolatedColor(const Image<OutputStorageType>& src, 
                               Image<OutputStorageType>& dst);

    int _FillInterpolated(const Image<OutputStorageType>& src, 
                          const double &factor, Image<OutputStorageType>& dst);
 
    virtual void GetBordersValid_(int &border_x, int &border_y) const;

  }; // class

#ifdef WIN32
  template <> int Rescale<unsigned char, unsigned char>::
  _FillInterpolatedGrey(const Image<unsigned char>& src, 
                        Image<unsigned char>& dst);

  template <>
  int Rescale<unsigned char, unsigned char>::
  _FillInterpolatedColor(const Image<unsigned char>& src, 
                         Image<unsigned char>& dst);

  template <> 
  int Rescale<unsigned char, unsigned char>::
  DownsampleBy2Color(const Image<unsigned char>& Source, 
                     Image<unsigned char>& Destination);

  template <>
  int Rescale<unsigned char, unsigned char>::
  DownsampleBy2Grey(const Image<unsigned char>& Source, 
                    Image<unsigned char>& Destination);



  template <>
  int Rescale<unsigned char, unsigned char>::
  DownsampleBy4Grey(const Image<unsigned char>& Source, 
                    Image<unsigned char>& Destination);

#ifdef BUILD_IMAGE_USHORT
  template <>
  int Rescale<unsigned short, unsigned short>::
  _FillInterpolatedGrey(const Image<unsigned short>& src,
                        Image<unsigned short>& dst);

  template <> 
  int Rescale<unsigned short, unsigned short>::
  DownsampleBy2Color(const Image<unsigned short>& Source, 
                     Image<unsigned short>& Destination);
  template <>
  int Rescale<unsigned short, unsigned short>::
  DownsampleBy2Grey(const Image<unsigned short>& Source, 
                    Image<unsigned short>& Destination);
  template <>
  int Rescale<unsigned short, unsigned short>::
  DownsampleBy4Grey(const Image<unsigned short>& Source, 
                    Image<unsigned short>& Destination);

  template <>
  int Rescale<unsigned short, unsigned short>::
  _FillInterpolatedColor(const Image<unsigned short>& src, 
                         Image<unsigned short>& dst);

#endif //BUILD_IMAGE_USHORT
  //
  //
  //
#endif // WIN32


} // namespace

#endif // __Rescale_hh__
