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

#ifndef __Image_hh__
#define __Image_hh__

#include "BIAS/bias_config.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>
#include <string>
#include <fstream>
// for MaxSTValue and MinSTValue
#include <float.h>
#include <limits.h>

//BIAS
#include "BIAS/Base/Image/ImageBase.hh"

#define MIP_DV_WIDTH 500
#define MIP_DV_HEIGHT 288

#define D_COPYIN        1<<1
#define D_CONVERT       1<<2
#define D_CAST_CHECK    1<<3
#define D_MI_TOPLANAR   1<<4
#define D_MI_IOSTREAM   1<<5
#define D_MI_CONVGREY   1<<6
#define D_IMAGE_MINMAXCOO   1<<7

namespace BIAS
{

  /** @class Image
   @ingroup g_image_base
   @brief The image template class for specific storage types.

   The real image data is stored as an array of the specific storage type.
   You can get the pointer by GetImageData() while
   accessing the image data could also be more efficiently done
   by GetImageDataArray()[y][x],
   which gives the first channel of pixel at position (x,y).
   The second and third channels for interleaved images are at
   [y][channelcount*x+1] and [y][channelcount*x+2].<br>

   There is an additional MetaData array, which holds information like
   the Pmatrix, which are used by derived classes.<br>

   see also ExampleLoad.cpp , ExampleOperator.cpp, ExampleChessboard.cpp

   @internal The explicit instatiation of the template
   is handled in ImageInstantiation.cpp
   **/
  template<class StorageType>
    class BIASImageBase_EXPORT Image : public ImageBase
    {
    public:
      Image();
      virtual
      ~Image();

      /** constructor with size and channels
       allocates image data */
      Image(unsigned int width, unsigned int height, unsigned int channels = 1,
          const bool interleaved = true);

      /** create image from imagebase. this is a dangerous operation and must
       therefore not be used by implicit conversion. That means, you can
       manually convert ImageBase->Image, but you cannot pass an
       Image<float> where Image<char> is expected. */
      explicit
      Image(const ImageBase& im);

      /** virtual covariant copy constructor which produces the same type of
       object as the template object (Image, Camera, ...).
       Must be implemented in all derived classes and since some people do
       work with ImageBase objects, it is also implemented there.
       The caller is responsible to delete the clone.
       @author koeser 06/2006 */
      virtual Image<StorageType>*
      Clone() const
      {
        return new Image<StorageType> (*this);
      }

      /** @brief calls Init from ImageBase
       storageType is ignored, just dummy argument */
      void
      Init(unsigned int Width, unsigned int Height, unsigned int channels = 1,
          enum EStorageType storageType = ST_unsignedchar,
          const bool interleaved = true);

      /** @brief reimplemented from ImageBase */
      void
      Release();

      /**
       This is used to construct a BIAS::Image hull around
       existing image data. Be sure to ClearDataPointer() _before_
       destructing the Image object to avoid deletion of the data array
       @author evers
       */
      int
      InitWithForeignData(unsigned int width, unsigned int height,
          unsigned int channels, void *data, const bool interleaved = true);

      /** @name Get Functions
       @{
       */
      /** overloaded GetImageData() from ImageBase
       @author Felix Woelkt */
      inline const StorageType*
      GetImageData() const
      {
        return (const StorageType *) ImageData_;
      }

      /** overloaded GetImageData() from ImageBase
       @author Daniel Grest */
      inline StorageType*
      GetImageData()
      {
        return (StorageType *) ImageData_;
      }

      /** overloaded GetImageDataArray() from ImageBase
       @author Felix Woelk  */
      inline const StorageType**
      GetImageDataArray() const
      {
        return (const StorageType **) ImageDataArray_;
      }

      /** overloaded GetImageDataArray() from ImageBase
       @author Daniel Grest  */
      inline StorageType**
      GetImageDataArray()
      {
        return (StorageType **) ImageDataArray_;
      }

      /** Returns value of pixel at specific position,
       using specific channel as offset
       @attention Checks boundaries in debug build.
       @author Ingo Thomsen, JW
       @date 03/27/2002 */
      inline StorageType
      PixelValue(const unsigned int x, const unsigned int y,
          const unsigned short int channel = 0) const;

      /** Returns value of pixel at specific position, using specific channel
       as offset, in interleaved images.
       @attention Checks boundaries and interleaved flag in debug build.
       @author Johan Skoglund
       @date 06/10/2004 */
      inline StorageType
      PixelValueInterleaved(const int x, const int y, const int channel = 0) const;

      /** @brief Return pixel value at specified position as reference
       for read+write access in interleaved images.

       Useful to iterate with read+write acess over pixels for shorter code.
       Not faster - but easier to debug.
       Useful for rapid prototyping of i.e. Cg fragment shaders in Software.
       Intentionaly NOT const and reference return instead of value.
       Assume interleaved (color) images of typically 3/4 channels (RGB/RGBA).
       @attention Checks boundaries and interleaved flag in debug build.
       @author Jan Woetzel */
      inline StorageType &
      PixelValueRefInterleaved(const int x, const int y, const int channel = 0);

      /** Get the minimal pixel value
       if coo!=NULL the coo[0]=x of min and coo[1]=y of min
       @author Felix Woelk **/
      StorageType
          GetMinPixelValue(unsigned short int channel = 0, unsigned int *coo =
              NULL) const;

      /** Get the maximal pixel value
       if coo!=NULL the coo[0]=x of max and coo[1]=y of max
       @author Felix Woelk **/
      StorageType
          GetMaxPixelValue(unsigned short int channel = 0, unsigned int *coo =
              NULL) const;

      /** @brief returns the minimal and maximal pixel value in channel only
       Finds minimum and maximum pixel value in image in channel only, i.e.
       all other channels are ignored during the search. Returns the first
       occurance of min/max pixelvalue, if they occure multiple times.
       @param min The variable where to store the minimun value
       @param max The variable where to store the maximum value
       if mincoo or maxcoo !=NULL, also stores the location of
       min resp. max, works slow on
       @tested woelk 10/2004 ( Examples/ExampleMinMax.cpp )
       @author Jan Frahm, woelk 01/2003, woelk 10/2004 **/
      void
      GetMinMaxPixelValue(StorageType& min, StorageType& max,
          unsigned short int channel = 0, unsigned int *mincoo = NULL,
          unsigned int *maxcoo = NULL) const;
      
      /** @author woelk 08/2004 
       *  @brief calculates mean of pixel
       *  @param mean (output) mean image value with as many entries as image has channels, init before!
       */
      void
      GetMeanPixelValue(StorageType mean[]);
      
      /** @author ischiller 12/09
       *  @brief calculates mean of pixel ignoring values between  ignoreStart and ignoreEnd
       *  @param mean (output) mean image value with as many entries as image has channels, init before!
       *  @param ignoreStart (input) start of interval of values to ignore (default=0)
       *  @param ignoreEnd (input) end of interval of values to ignore (default=0)
       */
      void
      GetMeanPixelValueIgnoreRange(StorageType mean[],
      		const StorageType ignoreStart = 0, const StorageType ignoreEnd = (StorageType)0) const;
      
      /** @brief Get both, minimal and maximal pixel value
       --actual implementation only for planar images--
       @param min The variable where to store the minimum value
       @param max The variable where to store the maximum value
       @param ignoreStart, ignoreEnd define the range of values to be ignored
       @param channel to be searched from multi-channel images
       @return true if a value within range was found, else false
       @author Jan Woetzel 01/2003 **/
      bool
      GetMinMaxPixelValueIgnoreRange(StorageType& min, StorageType& max,
          const StorageType ignoreStart = 0, const StorageType ignoreEnd = 0,
          const unsigned short int channel = 0) const;

      /** @brief Get the value of a specific channel offset of specific pixel
       nearest to given double coordinates

       @author Ingo Thomsen
       @date 03/27/2002
       @attention Checks boundaries in debug build.
       @status tested (only with interleaved grey images) **/
      inline const StorageType&
      GetPixelNearestNeighbor(const double x, const double y,
          const unsigned short int channel = 0) const;

      /** @brief Get the value of a specific channel offset of specific pixel
       nearest to given float coordinates, using the double version.
       @author Ingo Thomsen
       @date 03/28/2002
       @attention Checks boundaries in debug build.
       @status tested (only with interleaved gray images) **/
      inline const StorageType&
      GetPixelNearestNeighbor(const float x, const float y,
          const unsigned short int channel = 0) const;

      // @}
      ////////end of Get Functions////////////////////////////////////////////

      /** @name Set Functions
       @{
       */
      /**
       @brief sets all pixels to zero/value

       clear all pixels elements and set them to a 'default' value
       JW 04/2003
       */
      inline
      void Clear(const StorageType value = 0)
      {
        FillImageWithConstValue(value);
      }

      /** Sets all pixel values in the declared channel to the specified value.        
        * \author bartczak 05/2010
       **/
      void ClearChannel(const unsigned int channel, const StorageType value);

      /// @brief fill grey images
      void
      FillImageWithConstValue(StorageType Value);
      /// @brief fill color images
      void
      FillImageWithConstValue(StorageType Value[]);

      /// @brief fills image with value depending on x coordinate
      void
      FillImageWithXValue();

      /** Set the value of a given pixel (x,y) in channel to value.
       @warning This function is slower than direct pointer access if you
       want to access ALL pixels of an image!
       @attention Checks boundaries in debug build.
       @author Jan Woetzel 08/2003 */
      inline void
      SetPixel(const StorageType & value, const unsigned int & x,
          const unsigned int & y, const unsigned short int channel = 0);

      /** Set the values of the first three channels of a given pixel (x,y)
       in channel to value[0..2].
       @warning This function is slower than direct pointer access if you
       want to access ALL pixels of an image!
       @attention Checks boundaries in debug build.
       @author Jan Woetzel 08/2003 */
      inline void
      SetPixel(const StorageType & value0, const StorageType & value1,
          const StorageType & value2, const unsigned int & x,
          const unsigned int & y);

      // @}
      //////end of Set Functions /////////////////////////////////////////////

      /** @name Thresholding and Scaling
       @{
       */

      /** @brief scales and shifts image (all channels simultanously)

       does scale and shift every channel with same factor so that
       (max of all channels) = Max and (min of all channels) = Min.
       First shifts, then scales.
       @author Felix Woelk **/
      int
      ScaleShift(double Scale, double Shift);

      /** @brief similiar to ScaleShift, but only scales and shifts one
       image channel
       @author Patrick Fittkau **/
      int
      ScaleShiftChannel(double Scale, double Shift, unsigned int channel);

      /** @brief scales and shifts image so afterwards every pixel has a value
       between Min and Max

       scales and shifts every channel with same factor
       @author Felix Woelk **/
      int
      ScaleShiftBetween(double Min, double Max);

      /**
       * @brief Transposes an image on diagonal, rows become columns, 
       * columns become rows
       */
      int
      Transpose(BIAS::Image<StorageType>& result);

      /**
       * @brief inverts values of image, new values are StorageType_MAX-oldvalue
       * or  newMax-oldvalue if newMax != NULL
       * @param inverted (out) the inverted image
       * @param newMax (in) the new maximum of image
       */
      int 
      InvertValues(BIAS::Image<StorageType>& inverted, 
      		StorageType* newMax = NULL);
      
      /** @brief calculate the values needed by ScaleShiftBetween */
      int
      CalcScaleShift(double Min, double Max, double& Scale, double &Shift) const;

      /// @brief sets alls pixels with values below Threshold to Value
      int
      BelowThresholdToValue(StorageType Threshold, StorageType Value);

      /// @brief sets alls pixels with values above Threshold to Value
      int
      AboveThresholdToValue(StorageType Threshold, StorageType Value);

      /** @brief sets all pixels >= Threshold to MaxPixelValue
       and all others to MinPixelValue
       only for one channel / grey images
       @author woelk
       @date 01/2003 */
      void
      Binarize(StorageType Threshold);

      /** @brief Wrapper for Binarize()
       *  @author woelk
       *  @date 01/2003 */
      void
      Binarise(StorageType Threshold);

      /** @brief Test for every pixel wether it is >= than the threshold. If so
       *  set to a MaxValue, otherwise to an MinValue. (status tested)
       *  @author Ingo Thomsen
       *  @date 03/28/2002 */
      int
      BinaryThreshold(StorageType Threshold, StorageType MinValue,
          StorageType MaxValue);
      /**
       * \brief Calculates a mask image from given image depending on threshold.
       * If the value of one or more channels is above a pixel, the binary image
       * is set to 255, if none of the image channels of above the threshold
       * the binary image is set to 0.
       * \param Threshold[in]: threshold to check.
       * \param binaryImage[out]: resulting 1 channel binary image */
      void
      MaskValues(StorageType Threshold, Image<unsigned char>& binaryImage);
      //@}
      /////// end of thresholding //////////////////////////////////////////////
      /////////////////////////////////////////////////////////////////////////
      /**
       @name Operators

       Operators -,+,*,/,-=,+=,*=,/= for scalar and image arguments <br>
       Operators do only work for argument of the same type <br>

       @{
       */
      /** copies ImageBase to an desired Image<>,
       be careful with this, because u can do
       something like: Image<char> = Image<float>,
       what usualluy not makes sense
       @author Friso Evers, Nov. 2002 */
      Image<StorageType>&
      operator=(const ImageBase& Source);

      /// Operator /= for scalar value
      Image<StorageType>&
      operator/=(const StorageType& argimage);

      /// Operator / for scalar value, returning a new image
      Image<StorageType>
      operator/(const StorageType& argimage) const;

      /// Operator *= for scalar value
      Image<StorageType>&
      operator*=(const StorageType& argimage);

      /// Operator * for scalar value, returning a new image
      Image<StorageType>
      operator*(const StorageType& argimage) const;

      /// Operator += for scalar value
      Image<StorageType>&
      operator+=(const StorageType& argimage);

      /// Operator + for scalar value, returning a new image
      Image<StorageType>
      operator+(const StorageType& argimage) const;

      /// Operator -= for scalar value
      Image<StorageType>&
      operator-=(const StorageType& argimage);

      /// Operator - for scalar value, returning a new image
      Image<StorageType>
      operator-(const StorageType& argimage) const;

      /// Operator /= for another image as argument
      Image<StorageType>&
      operator/=(const Image<StorageType>& argimage);

      /// Operator / for another image as argument, returning a new image
      Image<StorageType>
      operator/(const Image<StorageType>& argimage)const ;

      /// Operator *= for another image as argument
      Image<StorageType>&
      operator*=(const Image<StorageType>& argimage);

      /// Operator * for another image as argument, returning a new image
      Image<StorageType>
      operator*(const Image<StorageType>& argimage) const;

      /// Operator += for another image as argument
      Image<StorageType>&
      operator+=(const Image<StorageType>& argimage);

      /// Operator + for another image as argument, returning a new image
      Image<StorageType>
      operator+(const Image<StorageType>& argimage) const;

      /// Operator -= for another image as argument
      Image<StorageType>&
      operator-=(const Image<StorageType>& argimage);

      /** subtracts every from every pixel (in ROI if defined)
       the pixel value from argimage
       !!! negative results are clipped to zero !!!
       @author Ingo Thomsen
       @status tested (18/04/2002)
       */
      Image<StorageType>
      operator-(const Image<StorageType>& argimage) const;

      Image<StorageType>&
      operator|(const Image<StorageType>& argimage);

      /** @} */ //doxygen end operators

      /////////// end of Operators ////////////////////////////////////////////

      /** @name Interpolating Functions
       @{
       */
      /** @brief Linear interpolation in image rows.
       @attention Uses boundary check in debug build, but ignores ROI. */
      inline double
      LinearInterpolation(const unsigned int x, const double y) const;

      /** @brief Linear interpolation in image columns.
       @attention Uses boundary check in debug build, but ignores ROI. */
      inline double
      LinearInterpolation(const double x, const unsigned int y) const;

      /** @brief Fast bilinear interpolation for grey-value images.
       Specialisation for storage tupe unsigned char exists which is 20% faster
       than the generic function!
       @attention No boundary check! Returns actual storage type rather
       than double.
       @author woelk 01/2003 */
      inline StorageType
      FastBilinearInterpolationGrey(const double x, const double y) const;

      /** @brief Generic bilinear interpolation.
       Returns the bilinear interpolated pixel value (using channel offset)
       at subpixel position defined by double coordinates.
       @attention Uses boundary check in debug build, but ignores ROI.
                  Does not work for planar non-RGB images!
       @date 04.03.2002
       @status tested (only with gray images yet)
       @author Ingo Thomsen */
      double
      BilinearInterpolation(const double x, const double y,
                            const unsigned short int channel = 0) const;

      /** @brief Check if subpixel position (x,y) in image is valid for
          bilinear interpolation (i.e. has 4 neighbors inside the image).
          @attention Ignores ROI!
          @author esquivel */
      inline bool CheckBilinearInterpolation(const double x,
                                             const double y) const;

      /** @brief Check if subpixel position (x,y) in image is valid for bicubic
          interpolation (i.e. 4x4 window centered at (x,y) is inside the image).
          @attention Ignores ROI!
          @author esquivel */
      inline bool CheckBicubicInterpolation(const double x,
                                            const double y) const;

      /** @brief Bilinear interpolation for grey-value images.
       @attention No boundary check, ignores ROI!
       @author Felix Woelk */
      inline double
      BilinearInterpolationGrey(const double x, const double y) const;

      /** @brief Bilinear interpolation for planar RGB images.
       @attention No boundary check, ignores ROI!
       @author Felix Woelk */
      inline double
      BilinearInterpolationRGBPlanar(const double x, const double y,
          unsigned int channel) const;

      /** @brief Bilinear interpolation for interleaved RGB images.
       @attention No boundary check, ignores ROI!
       @author Felix Woelk */
      inline double
      BilinearInterpolationRGBInterleaved(const double x, const double y,
          unsigned int channel) const;

      /** @brief Interpolation for grey-value images with weights according
       to distance of (x,y) to pixel center.
       @attention No boundary check, ignores ROI! */
      inline double
      DistanceWeightedInterpolationGrey(const double x, const double y) const;

      /** @brief Interpolate shifted rectangular region centered at given
       subpixel position.
       Sample the continuous (bilinear interpolation of (*this))
       image using a grid with one pixel spacing at subpixel positions
       (x-hws, y-hws) , ... (x,y), ... (x-hws, y-hws).
       You get a window of size (2*hws_x+1)*(2*hws_y+1) which is exactly
       centered at (x,y) (a subpixel position) of the original image
       @param buffer must have size (2*hws_x+1)*(2*hws_y+1)
       @param hws_x half window size in x direction
       @param hws_y half window size in y direction
       @param x x-center of window
       @param y y-center of window
       @attention No boundary check, ignores ROI! Image must be one-channel!
       Only unsigned char is implemented!
       @author koeser 01/2004 */
      inline void
      BilinearInterpolationShiftRegion(const double& x, const double& y,
          unsigned int hws_x, unsigned int hws_y, StorageType* buffer);

      /** @brief Bilinear interpolation for grey-value images with pixel
       value check.
       If at least one of the input values is <= minVal, then the unknown
       value minVal is returned, too.
       @param minVal all values <= minVal are invalid/unknown.
       @attention No boundary check, ignores ROI!
       @author Jan Woetzel 12/2004 */
      inline double
      BilinearInterpolationGreyMinVal(const double x, const double y,
                                      const StorageType & minVal) const;

      /** @brief Bilinear interpolation for grey-value images with pixel
       value check.
       If at least one of the input values is >= maxVal, then the unknown
       value maxVal is returned, too.
       @param maxVal all values >= maxVal are invalid/unknown.
       @attention No boundary check, ignores ROI!
       @author bartczak 08/2006 */
      inline double
      BilinearInterpolationGreyMaxVal(const double x, const double y,
                                      const StorageType & maxVal) const;

      /** @brief Generic bicubic interpolation.
       Returns the bicubic interpolated pixel value (using channel offset)
       at subpixel position defined by double coordinates.
       The subpixel value is obtained by computing a weighted average of
       the surrounding 4x4 pixel block. The weights are computed by
       independent x/y direction cubic polynomials which guarantee
       constant first order derivatives at the grid positions.

       @attention Uses boundary check in debug build, but ignores ROI.
       Does not work for planar multi-channel images!

       @author koeser 02/2005
       **/
      double
      BicubicInterpolation(const double& x, const double& y,
                           const unsigned short int channel = 0) const;

      /** @} */ //doxygen end Interpolation

      /**
      * @name Misc Functions
      *  @{
      * \brief Append an image with one channel of same StorageType
      *   Append means: RGB + A => RGBA
      **/
      int
      AppendChannel(Image<StorageType> &img);

      /**
       * \brief remove a channel from an image 
			 * Remove means: RGB, remove channel 2 => RG
			 * CAUTION, channels start a 0
			 * Status, planar case untested 
			 * \param channel, the channel to remove, starting at 0
			 * \author ischiller
			 * \date 04/2010 
			*/
      int 
      RemoveChannel(unsigned int channel);
      
      /**
       * \brief Deletes n pixels at border of image
       * \param n[in]: number of pixels to delete
       * \param delVal[in]: new value for deleted pixel (default=0)
       * \return number of deleted pixel
       * \author ischiller
			 * \date 08/2010
       */
      int
      DeleteNBorderPixel(int n, StorageType delVal=StorageType(0));

      /** \brief returns the maximal possible StorageType */
      inline StorageType
      MaxSTValue();

      /** \brief returns the minimal possible StorageType */
      inline StorageType
      MinSTValue();

      /** \brief writes pointer of IplImage_ to os (ascii) */
      std::ostream&
      PrintPointer(std::ostream &os) const;

      /** \brief writes data of IplImage_ to os (ascii) */
      std::ostream&
      PrintData(std::ostream &os) const;

      /** @brief (*this) = | im1 - im2 |
       sets this as the absolute difference between two arg images
       @author Felix Woelk */
      void
      AbsDiff(const Image<StorageType>& im1, const Image<StorageType>& im2);

      /** @brief Calculates the reciprocal for each pixel: x' = 1/x
       computes for exactly one channel (in case of multi channel images)
       @return resulting reciprocal image
       @author Jan Woetzel 01/2003 */
      Image<float>
          Reciprocal(const float factor = 1.0,
              const unsigned short int channel = 0);

      /// inverts every pixel value, i.e. MaxSTValue() - value
      void Invert();

      /** @} */

    };

// inline functions

#include "ImageInline.hh"

} // namespace BIAS

#endif // __Image_hh__
