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
 ut WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with BIAS; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef _BIAS_IMAGE_CONVERSION_H_
#define _BIAS_IMAGE_CONVERSION_H_

#include "bias_config.h"

#include "Image.hh"
#include "ImageBase.hh"

#  include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>




#define D_IMCONV_IPL  0x00000001

// PointGrey XB3 special conversion (JW)
// format for acquired raw, packed image
#ifndef vidap_DEFAULT_PACKED_COLORMODEL
#  define vidap_DEFAULT_PACKED_COLORMODEL  BIAS::ImageBase::CM_PGR_XB3_F7M3_GBRG
#endif
#ifndef vidap_DEFAULT_BAYER_COLORMODEL
// format for deinterleaved single camera Bayer pattern images
#  define vidap_DEFAULT_BAYER_COLORMODEL  BIAS::ImageBase::CM_Bayer_GBRG
#endif

namespace BIAS
{

  /**
   * @brief methods for conversion from bayer to rgb
   */
  typedef enum {
    BAYER_DEMOSAIC_METHOD_NEAREST,
    BAYER_DEMOSAIC_METHOD_SIMPLE,
    BAYER_DEMOSAIC_METHOD_BILINEAR,
    BAYER_DEMOSAIC_METHOD_HQLINEAR,
    BAYER_DEMOSAIC_METHOD_VNG,
    BAYER_DEMOSAIC_METHOD_AHD
  } BayerDemosaicMethod;

  /** @class ImageConvert
   @ingroup g_image_io
   @brief class with static functions
   for converting all kinds of color formats and storage types

   As all fucntions are static they have to be called by
   eg. ImageConvert::Convert() and not with an object.

   @author Daniel Grest, and others Nov. 2002
   */
  class BIASImageBase_EXPORT ImageConvert : Debug
  {
    friend class ImageBase;
  public:
    /** \brief main general conversion function, calls desired specialized functions,
     *  always initializes the destImage(if not source==dest)
     *  \author Daniel Grest
     *  \date Nov. 2002 */
    static int
    Convert(BIAS::ImageBase &source, BIAS::ImageBase &dest,
        enum BIAS::ImageBase::EColorModel targetColorModel, bool bPlanar =
            false);

    /** \brief Function to convert the storage type of images
     *  e.g. from unsigned char to float or any other
     *  \param source[in]: the source image
     *  \param dest[out]: the target image
     *  \param targetST[in]: the storage type of the new target image
     *  \author Friso Evers */
    static int
    ConvertST(const BIAS::ImageBase &source, BIAS::ImageBase &dest,
        ImageBase::EStorageType targetST);

    /** \brief In place conversion to gray image. No further memory is allocated,
     *  because only the the first part of the existing image is used.
     *  \author Felix Woelk **/
    template<class StorageType>
      static int
      IP_ToGrey(Image<StorageType> &img);

    /**
     * \brief wrapper for the templated function ToGrey
     * \author woelk 02/2006 */
    static int
    ToGrey(const ImageBase& source, ImageBase& dest);

    /** \brief Create a grey converted copy of source image in this.
     *  Release() and Init() are called if necessary.
     *  \author Felix Woelk **/
    template<class StorageType>
      static int
      ToGrey(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Create a new image dest by cropping one of the source images channels.
     *  Unlike GetChannel(Image<StorageType>& image, int channel), this
     *  one really creates a new single-channel image with the selected channel
     *  of the source. fkellner: Other GetChannels now have "strip" param, doing the
     *  same in-place.
     * \author Patrick Fittkau **/
    template<class StorageType>
      static int
      GetChannel(const Image<StorageType>& source, Image<StorageType>& dest,
          const unsigned int channel);

    /** \brief in place: Crop the selected channel.
     *  (will actually copy values from selected channel to all other channels)
     *  \param image[in,out]: input image which will have only one channel, other channels are thrown away
     *  \author Christoffer Menk, fkellner **/
    template<class StorageType>
      static int
      GetChannel(Image<StorageType>& image, int channel, bool strip = false);

    /** \brief in place: Crop the selected channel.
     *  (will actually copy values from selected channel to all other channels)
     *  \param image[in,out]: input image which will have only one channel, other channels are thrown away
     *  \author Christian Beder, fkellner **/
    static int
    GetChannel(ImageBase& image, int channel, bool strip = false);

    /** \brief Create a RGB converted copy of source image in this.
     *  Release() and Init() are called if necessary.
     *  \author Felix Woelk **/
    template<class StorageType>
      static int
      ToRGB(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Create a RGBA converted copy of source image in dest.
     intermediate rgb image is produced, only interleaved supported
     @author koeser **/
    template<class StorageType>
      static int
      ToRGBA(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Create a HSV converted copy of source image in this
     Release() and Init() are called if necessary.
     HSV space is implemented as a cylinder not as a cone!
     only for float and unsigned char
     @author Felix Woelk  partially tested **/
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      ToHSV(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Create a HSL converted copy of source image in this
     Release() and Init() are called if necessary.
     only for float and unsigned char
     @author Daniel Grest, April 2003
     @status tested for unsigned char interleaved
     */
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      ToHSL(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Create a hsL converted copy of source image in this
     Release() and Init() are called if necessary.
     only for unsigned char
     similar to HSL but in euclidean coordinates for the CNCC
     @author Daniel Grest, June 2003
     @status tested for unsigned char interleaved
     */
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      TohsL(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Takes the source image, and build an interleaved version
     @author Ingo Thomsen
     @date 11.06.2002
     @status tested
     @todo make it work for yuv **/
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      ToInterleaved(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Takes the three channels and composes one interleaved RGB image */
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      ToInterleavedRGB(const Image<StorageType>& red,
          const Image<StorageType>& green, const Image<StorageType>& blue,
          Image<StorageType>& dest);

    /** \brief Take Pointgrey X3B packed interleaved raw
     and store there three contained images side by side
     keeping the Bayer pattern.
     data:  abcABC --> aAbBcC
     @author Jan Woetzel */
    static int
    DeinterleaveHorizontal(const BIAS::ImageBase & raw, BIAS::ImageBase & out);

    /** \brief Converts the existing image to an planar version. If source = dest in place conversion alters the source image!!!
     @author Ingo Thomsen
     @date 03.04.2002
     @status none
     case CM_YUYV422 not yet tested **/
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      ToPlanar(const Image<StorageType>& source, Image<StorageType>& dest);

    /** \brief Extracts color planes to argument images without changing source.
     So far only implemented for CM_YUYV422
     @author Felix Woelk
     @status alpha (18/04/2002) */
    template<class StorageType>
      BIASImageBase_EXPORT
      static int
      FromInterleaved(const Image<StorageType>& source,
          Image<StorageType>& Image1, Image<StorageType>& Image2, Image<
              StorageType>& Image3);

    /**
     @author frick
     converts image from rgb to xyz color space
     rgb values are requiered to be in intervall [0,scale]
     */
      static int
      RGBToXYZ(const BIAS::ImageBase& source, BIAS::Image<float>& dest,float scale = 255.0);

    /**
     @author frick
     */
    template<class StorageType>
      static int
      XYZToRGB(const BIAS::Image<float>& source, BIAS::Image<StorageType>& dest,
          float scale = 255.0);

    /** \brief Convert XYZ image to a LUV color space image
     *  \author africk */
    static int
    XYZToLUV(const BIAS::Image<float>& source, BIAS::Image<float>& dest);

    /** \brief Convert LUV image to a XYZ color space image
     *  \author africk */
    static int
    LUVToXYZ(const BIAS::Image<float>& source, BIAS::Image<float>& dest);

    /**\brief Convert XYZ image to a LAB color space image
     * \author bangerer
     * \date 01/2009 */
    static int
    XYZToLAB(const BIAS::Image<float>& source, BIAS::Image<float>& dest);

    /** \brief Convert RGB image to a LUV color space image
     *  \author frick  */
      static int
      RGBToLUV(const BIAS::ImageBase& source, BIAS::Image<float>& dest,
      		    float scale = 255.0);

      /** \brief Convert RGB image to a LAB color space image
       *  \author frick  */
      static int
      RGBToLAB(const BIAS::ImageBase& source, BIAS::Image<float>& dest,
          float scale = 255.0);

      /** \brief Convert LUV image to a RGB color space image
       *  \author frick  */
      template<class StorageType>
        static int
        LUVToRGB(const BIAS::Image<float>& source, BIAS::Image<StorageType>& dest,
        float scale = 255.0);

      /**
      * Converts RGB image to Othe's principle component generalization for RGB based
      * segmentation.
      * \author bartczak 12/2008
      */
      static int RGBToI1I2I3(const BIAS::ImageBase& source, BIAS::Image<float>& dest);
    protected:
        template<class sourceType>
          static void ConvertRGBToI1I2I3Worker_(const sourceType* soP, float* siP, int size);

    public:
    /////////////////////////////////////////////////////////////////
    /// foreign image formats, ipl, vil, ImageMagick
    /////////////////////////////////////////////////////////////////

#ifdef BIAS_HAVE_DV
    /** Can specify speed/quality trade off
     0 : monochrome , DC coeffs only
     1 : monochrome , AC 1 coeffs
     2 : color ,  DC coeffs only
     3 : color , AC 1 coeffs
     4 : color, AC 2 coeffs (slowest but best quality)
     */
    template <class StorageType>
    static int DVToRGB_(const Image<StorageType>& source,
        Image<StorageType>& dest,unsigned char quality=4);
#endif




    /** Returns a new, separate IplImage for use with OpenCV created rom
     the source BIAS Image.
     The dest has to be a call by reference "*&" because the pointer
     itself is changed due to new.
     The colorChannel order is swapped because OpenCv used BGR -
     which may be inefficient.
     See WrapBias2Ipl class, too.
     @return newly created IplImage
     @todo   ROIs are not yet kept, when converted to IplImages
     @author Ingo Thomsen 04.05.2002, Jan Woetzel 2005
     */
    template<class StorageType>
      static int
      BIAS2ipl(const Image<StorageType>& source, IplImage* &dest);

    /** Takes the image information and data of an existing IplImage
     and builds a Image. The existing data is discarded.

     @author Ingo Thomsen
     @date 04.17.2002
     @return -1 in case of failure
     @param image The IplImage to be transfered
     @status untested

     @todo Make it work for colored images
     @todo Make it work for ROIs **/
    template<class StorageType>
      static int
      Ipl2BIAS(IplImage* source, Image<StorageType>& dest);

  protected:
    template<class StorageType>
      static int
      IP_RGBToGrey_(Image<StorageType> &img);

    template<class StorageType>
      static int
      IP_YUV422ToGrey_(Image<StorageType> &img);

    template<class StorageType>
      static int
      BGRToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      YUV422ToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    template <class StorageType>
     static int 
     HSLToRGB_(const BIAS::Image<StorageType>& source,BIAS::Image<StorageType>& dest);
    
    template<class StorageType>
      static int
      YUV420PToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      YUV420PToGrey_(const Image<StorageType>& source,Image<StorageType>& dest);

    template<class StorageType>
      static int
      YUV411ToGrey_(const Image<StorageType>& source, Image<StorageType>& dest);
    template<class StorageType>
      static int
      GreyToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      RGBToGrey_(const Image<StorageType>& source, Image<StorageType>& dest);

    /**
     not tested - use at own risk - alpha channel will be ignored
     **/
    template<class StorageType>
      static int
      BGRAToGrey_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      YUV422ToGrey_(const Image<StorageType> &source, Image<StorageType>& dest);

  public:
    /** added border calculation, woelk
     @authour evers, woelk*/
    template<class StorageType>
      static int
      BayerToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    /**
    @brief converts an image with bayer pattern to rgb
    @author bangerer 05/2009
    @param source[in]: source image
    @param dest[out]: destination image
    @param method[in]: choose the demosaicing method from several methods drom BayerDemosaicMethod enum. standard is bilinear
    @param flip[in]: flip the image horizontal after converting it to rgb
    */
    template<class StorageType>
    static int BayerToRGB(const Image<StorageType>& source, Image<StorageType>& dest, BayerDemosaicMethod method = BAYER_DEMOSAIC_METHOD_BILINEAR, bool flip = false);

    template<class StorageType>
    static int BayerToRGBSimple(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile);

    /**
     * \brief conversion of one pixel in HSV to RGB colorspace
     * \date 06/2010
     * \return 0 on sucess, < 0 otherwise
     * \param [in] h,s,v : HSV values
     * \param [out] r,g,b : calculated RGB values
     * \author ischiller
     */
    template<class StorageType> BIASImageBase_EXPORT
    static int HSVToRGB(const StorageType h, const StorageType s, const StorageType v,
    		                StorageType &r, StorageType &g, StorageType &b);


  protected:
    /** this is the right way, green interpoaltion is mostly done
     out of two values
     no visible difference to fast calculation as used above
     @author woelk 03 2003 */
    template<class StorageType>
      static int
      BayerToRGBSlow_(const Image<StorageType>& source,
          Image<StorageType>& dest);
    /** interpolates, better than above
     @author woelk 03 2003 */
    template<class StorageType>
      static int
      BayerToGrey_(const Image<StorageType>& source, Image<StorageType>& dest);

  protected:
    // methods for demosaic bayer pattern
    // the methods are taken from libdc1394 - 1394-Based Digital Camera Control Library, and adapted to BIAS
    template<class StorageType>
    static int BayerToRGBSimple_(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile);
    template<class StorageType>
    static int BayerToRGBBilinear_(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile);
    template<class StorageType>
    static int BayerToRGBNearestNeighbour_(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile);
    static int BayerToRGBHQLinear_(const unsigned char* source, unsigned char* dest, int sx, int sy, ImageBase::EColorModel tile);
    static int BayerToRGBHQLinear_(const unsigned short* source, unsigned short* dest, int sx, int sy, ImageBase::EColorModel tile, int bits);
    template<class StorageType>
    static int BayerToRGBAHD_(const StorageType* source, StorageType* dest, int sx, int sy, ImageBase::EColorModel tile, int bits);
    template<class StorageType>
    static void ClearBorders_(StorageType *rgb, int sx, int sy, int w);

    template<class StorageType>
    static void CLIP(StorageType& in, StorageType& out);
    template<class StorageType>
    static void CLIP16(StorageType& in, StorageType& out, StorageType bits);

    /** just copies the L channel
     @author Daniel Grest */
    template<class StorageType>
      static int
      HSLToGrey_(const Image<StorageType>& source, Image<StorageType>& dest);

    // helper function for ToPlanar(Image<StorageType>& source)
    template<class StorageType>
      static int
      ToPlanarYUYV422_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      ToPlanarRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    // helper function for ToInterleaved(Image<StorageType>& source)
    template<class StorageType>
      static int
      ToInterleavedYUYV422_(const Image<StorageType>& source,
          Image<StorageType>& dest);
    template<class StorageType>
      static int
      ToInterleavedRGB_(const Image<StorageType>& source,
          Image<StorageType>& dest);

    template<class StorageType>
      static int
      FromInterleavedYUYV422_(const Image<StorageType>& source, Image<
          StorageType>& Y, Image<StorageType>& U, Image<StorageType>& V);

    template<class StorageType>
      static int
      FromInterleavedRGB_(const Image<StorageType>& source,
          Image<StorageType>& R, Image<StorageType>& G, Image<StorageType>& B);

    /// so far only implemented for StorageType=float
    //template <class StorageType>
    template<class StorageType>
      static int
      RGBToHSV_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      HSVToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);


    template<class StorageType>
      static int
      RGBToHSL_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      BGRToHSL_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      BGRAToHSL_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      RGBTohsL_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      RGBAToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    template<class StorageType>
      static int
      BGRAToRGB_(const Image<StorageType>& source, Image<StorageType>& dest);

    //in WIN32 forward declarations for specialized function templates are needed
#ifdef _WIN32
    template <>
    static int RGBToHSV_(const Image<float>& source,
        Image<float>& dest);

    template <>
    static int RGBToHSV_(const Image<unsigned char>& source,
        Image<unsigned char>& dest);

    template <>
    static int RGBToHSL_(const Image<float>& source,
        Image<float>& dest);

    template <>
    static int RGBToHSL_(const Image<unsigned char>& source,
        Image<unsigned char>& dest);

    template <>
    static int BGRToHSL_(const Image<unsigned char>& source,
        Image<unsigned char>& dest);
#ifdef BUILD_IMAGE_INT
    template <>
    static int RGBTohsL_(const Image<int>& source,
        Image<int>& dest);
#endif
    template <>
    static int RGBTohsL_(const Image<unsigned char>& source,
        Image<unsigned char>& dest);

#endif
    static int
    FloatToUC_(const ImageBase &source, ImageBase &dest);

#ifdef BIAS_HAVE_DV
    static dv_decoder_t *DVDecoder_;
#endif

  };

} // namespace


#endif
