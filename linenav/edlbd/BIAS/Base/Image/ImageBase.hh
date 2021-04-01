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

#ifndef _IMAGE_BASE_H_INCLUDED_
#define _IMAGE_BASE_H_INCLUDED_

#include "bias_config.h"
#include <exception>
#include <string>


# include <malloc.h>
#include <string.h>
#include <stdio.h>

//#include <Base/Debug/Debug.hh>
//#include <Base/Debug/Error.hh>
#include <Base/Common/CompareFloatingPoint.hh>
#include "MetaData.hh"

#include "ROI.hh"



// Begin namespace BIAS
namespace BIAS
{

#define MIP_DV_WIDTH 500
#define MIP_DV_HEIGHT 288

#define D_COPYIN        1<<1
#define D_CONVERT       1<<2
#define D_CAST_CHECK    1<<3
#define D_MI_TOPLANAR   1<<4
#define D_MI_IOSTREAM   1<<5
#define D_MI_CONVGREY   1<<6

  // global debug level
#define D_IMAGE_TRACE 1<<25

#define VERSION_NUMBER 320

#define IDENTIFIER "MIP"
#define IDENTIFIER_LENGTH 3

  // used only by Display float to Byte range conversion
#define DEFAULT_32to8_scale 255.0f
  // used by Display if waitforkey is specicied
#define DEFAULT_Display_delay 3000
  // used by Display routine
//#define DEFAULT_WrapBias2Ipl_WINNAME "WrapBias2Ipl_0"

  // forward declarations
  class BIASImageBase_EXPORT ImageConvert;
  template<class StorageType>
  class BIASImageBase_EXPORT Image;

  /** @class ImageBase
      @ingroup g_image_base
      \brief This is the base class for images in BIAS.

      It is a non template class, so you can write image type
      independet functions.
      Use this class if you deal with different storage types or
      you don't know the storage type, eg. when loading an image from disk.
  */
  class BIASImageBase_EXPORT ImageBase : public Debug
  {
    friend class ImageConvert;
    friend class ImageIO;
  //  friend class WrapBias2Ipl; //JW

  public:
    enum EStorageType
    {
      ST_invalid = -1, ///< invalid not set image storage type
      ST_unsignedchar, ///< (8bit) unsigned char image storage type
      ST_char, ///< (8bit) signed char image storage type
      ST_unsignedshortint, ///< (16bit) unsigned integer image storage type
      ST_shortint, ///< (16bit) signed integer image storage type
      ST_unsignedint, ///< (32bit) unsigned integer image storage type
      ST_int, ///< (32bit) signed integer image storage type
      ST_float, ///< float image storage type
      ST_double
      ///< double image storage type
    };

    /** @enum EColorModel
        \brief These are the most often used color models.
        Determines the data storage size per channel per pixel.
    */
    enum EColorModel
    {
      CM_invalid = -1, ///< invalid (not set) image format
      CM_Grey,        ///< gray values, 1 channel
      CM_RGB,         ///< color values, 3 channels, order: red,green,blue
      CM_BGR,         ///< color values, 3 channels, order: blue,green,red
      CM_YUYV422,     ///< YUYV422, 2 channels, full luminance Y, subsampled half U,V
      CM_UYVY422,     ///< UYVY422, 2 channels, full luminance Y, subsampled half U,V inverse order
      CM_YUV420P,     ///< YUV420P, 2 channels, full luminance Y, 1 U, 1 V. Y, U and V are grouped together for better compression
      CM_YUV444,      ///< CM_YUV444, 3 channels, all channels have full data
      CM_YUV411,      ///< YUV411, 2 channles, full luminance, 1 U, 1 V.
      CM_HSV,         ///< HSV, 3 channels, order: hue, sat , value
      CM_HSI_OBS,     ///< obsolete, HSI is unused and identical to HSL
      CM_DV,          ///< DV, color model used for digital video cameras such as Mini-DV
      CM_RGBA,        ///< RGBA, 4 channels, order: red,green,blue,alpha
      CM_GreyA,       ///< GreyA, 2 channels, grey plus Alpha
      CM_Bayer_RGGB,  ///< Bayer_RGGB, 1 channel RGB image Bayer tile
      CM_Bayer_GBRG,  ///< Bayer_GBRG, 1 channel RGB image Bayer tile
      CM_Bayer_GRBG,  ///< Bayer_GRBG, 1 channel RGB image Bayer tile
      CM_Bayer_BGGR,  ///< Bayer_BGGR, 1 channel RGB image Bayer tile
      CM_HSL,         ///< HSL, similar to HSV but space is a double tipped cone
      CM_hsL,         ///< hsl, similar to HSL but euclidean (h,s) for CNCC
      CM_SymTensor2x2,///< SymTensor2x2 The image contains a 2x2 symmetric tensor
      CM_BGRA, ///< BGRA color values, 4 channels, order: blue,green,red,alpha
      CM_RGBE, ///< RGBE color values, 4 channels, RADIANCE hdr format, four low dynamic channels meaning: 3x mantissa (red,green,blue) +1x (exponent)
      CM_PGR_XB3_F7M3_GBRG, ///< PGR XB3 in format 7 mode 3 delivers an image that consists of 3 channels with 8bbp (overal 24bpp), each channel codes an whole color image in GBRG bayer pattern, ch0 = left image, ch1 = right image, ch3 = center image
      CM_DepthAndVariance, ///< Todo: Unclear, I think one channel float, why isn't grey used for that?
      CM_YUYV,        ///< Todo: Conflict with YUVU model, what does it do?
      CM_LUV,         ///< CIELUV color space, 3 channels, http://en.wikipedia.org/wiki/CIELUV_color_space
      CM_XYZ,         ///< XYZ, 3 channels, http://en.wikipedia.org/wiki/Xyz_color_space
      CM_LAB,         ///< LAB, 3 channels, http://en.wikipedia.org/wiki/Lab_color_space
      CM_Disparity, ///< Disparity images Q: should disp and depth be treated separately, if not what would be a good name to cover both?
      CM_Depth, ///< Depth images A: separated for now.
      CM_I1I2I3, ///< Othe's principle component generalization for RGB based segmentation
      CM_DOES_NOT_EXIST
      //< end of list marker for loops. >= is invalid
    };

    ImageBase();
    virtual
      ~ImageBase();

    ImageBase(const ImageBase& im);

    /** @brief Constructor, initializing image size and channels
        @author Felix Woelk
        @status tested
        @param width  image width  of the new image
        @param height image height of the new image
        @param channels number of color planes
        @note Example: Defining an RGB image.
        @verbatim
        ImageBase im;
        im.init(width, height, 3);
        @endverbatim **/
    ImageBase(unsigned int width, unsigned int height, unsigned int channels =
              1, enum EStorageType storageType = ST_unsignedchar,
              const bool interleaved = true);

    /** virtual covariant copy constructor which produces the same type of
        object as the template object (Image, Camera, ...).
        Must be implemented in all derived classes and since some people do
        work with ImageBase objects, it is also implemented there.
        The caller is responsible to delete the clone.
        @author koeser 06/2006 */
    virtual ImageBase*
      Clone() const
    {
      return new ImageBase(*this);
    }

    /**  @brief (Re-)Initialize Image data if required.
         Contains logic to re-allocate only if required.
         Keep existing data area if data layout fits.
         The idea is to reuse a previously initialized image
         with no need to check it has been initialized exactly once.
         @author Jan Woetzel */
    void
      ReInit(const unsigned int & width, const unsigned int & height,
             const unsigned int nChannels = 1, const enum EStorageType storageType =
             ST_unsignedchar, const bool interleaved = true,
             const EColorModel colormodel = CM_Grey);

    /**  @brief Initialize image size and channels

         @warning call this only for empty images,
         calls new for the image data array. Overwrites the color model with
         some kind of best guess
         @author Felix Woelk
         @status tested
         @param width  image width  of the new image
         @param height image height of the new image
         @param channels number of color planes **/
    void Init(unsigned int width, unsigned int height, unsigned int nChannels=1,
              enum EStorageType storageType = ST_unsignedchar,
              const bool interleaved=true);

    /** @brief check if ImageData_ points to allocated image buffer or not */
    inline bool IsEmpty() const
    { return (ImageData_ == NULL); }

    /** @brief Take some data and fill it into the Image
        @warning Image must be initialized already !
        @author Evers  **/
    void CopyIn_NoInit(void *data);

    /** @brief Free the allocated data structures 
        Hands off: Do !!NOT!! change the default of reset_storage_type:
        ImageBase::Release() is also implicitly called for templated images
        when releasing the storage type here, it becomes possible to assign
        a float image to an unsigned char image via ImageBase:
          Image<float> f;
          Image<unsigned char> uc;
          ImageBase *ibf = dynamic_cast<ImageBase *>(f);
          ImageBase *ibuc = dynamic_cast<ImageBase *>(uc);
          (*ibf) = (*ibuc); */
    void Release(const bool reset_storage_type = false);

    /** @brief steals the image data array from source,
        after releasing the actual image data
        and sets source image data to NULL

        @warning Important for template: Steal images only, if u know the
        storageTypes are euqal!
        @author Daniel Grest
        @status tested */
    int StealImage(ImageBase &source);

    /////////////////////Get Functions ///////////////////////////////////////
    /** @name Get functions
        @{
    */
    inline const void*
      GetImageData() const
    {
      return ImageData_;
    }
    ;

    inline void*
      GetImageData()
    {
      return ImageData_;
    }
    ;

    /** @brief Get an array of pointers to image data.

        A pixel can be accessed  by
        GetImageDataArray()[row][col] or GetImageDataArray()[y][x] <br>
        For multi channel images with interleaved data order, access with
        GetImageDataArray()[row][col * GetChannelCount() + Channel] <br>
        For multi channel images with planar data order, access with
        GetImageDataArray()[row + GetHeight * Channel][col] <br>

        @author Felix Woelk
        @return array with pointer to the first pixels in every row. **/
    inline void**
      GetImageDataArray() const
    {
      return ImageDataArray_;
    }
    ;

    inline unsigned int
      GetWidth() const
    {
      return Width_;
    }
    ;

    inline unsigned int
      GetHeight() const
    {
      return Height_;
    }
    ;

    /** returns the bytes per channel, which is the sizeof(StorageType)
        Should match GetSizeDepth(GetStorageType()). */
    inline unsigned int
      GetDepth() const
    {
      return Depth_;
    }
    ;
    inline unsigned int
      GetByteDepth() const
    {
      return GetDepth();
    }
    ;

    /** returns the bits per channel
        Is not necessairily  8*sizeof(StorageType), could be fewer bits, eg. 12/16.
        @author woelk 12/2004 */
    inline unsigned int
      GetBitDepth() const
    {
      return BitDepth_;
    }
    ;

    /** returns the nr. of image data Bytes - NOT the dimension */
    inline unsigned int
      GetSizeByte() const
    {
      return GetPixelCount() * Depth_ * ChannelCount_;
    }
    ;

    /** returns the image size in bytes = count - NOT the dimension
        DEPRECATED Please use GetSizeByte instead to avoid storage type confusion on pointers and char/wideChar unicode */
    inline unsigned int
      GetSize() const
    {
      return GetSizeByte();
    }
    ;

    /** @return true if the image dimension w x h of this and other
        match = are equal, values of descriptions Fit.
        @author JW */
    bool
      DimensionMatch(const BIAS::ImageBase & other) const;

    /** @return true if (all) format descriptions between this and dest image match/fit are equal.
        @author JW */
    bool
      FormatMatch(const BIAS::ImageBase & d) const;

    /** @brief returns the number of Color channels, e.g. RGB 3

        the value returned is independent of the StorageType */
    inline unsigned int
      GetChannelCount() const
    {
      return ChannelCount_;
    }
    ;

    /** @brief returns the number of bytes per line

        returns the number of bytes per line, not the number of StorageTypes

        interleaved data order: = Width * Depth * ChannelCount
        planar data order: = Width * Depth

        Useful for memory aligned images (e.g. 4/8 Byte)
        and packing of textures.
        JW
    */
    inline unsigned int
      GetWidthStep() const
    {
      return WidthStep_;
    }
    ;

    inline enum EColorModel
      GetColorModel() const
    {
      return ColorModel_;
    }
    ;

    inline enum EStorageType
      GetStorageType() const
    {
      return StorageType_;
    }
    ;

    /** @brief returns number of pixels in image */
    inline unsigned long int
      GetPixelCount() const
    {
      return Width_ * Height_;
    }
    ;

    /**
       returns the Position (index from array start) in ImageData
       Warning: until 2005-01-04 the position inbyte was returned
       but used as position in <StorageType> (ImageDraw).
       Changed this method to return position in StorageType
       Jan-Friso Evers
    */
    inline unsigned int
      GetPixelPosition(unsigned int x, unsigned int y,
                       unsigned short int channel = 0) const;

    /** copies one specific channel from source to Image
        can only be called from an planar image, so call ToPlanar() before
        so far only implemented for CM_YUYV422, BGR  and RGB
        @author Felix Woelk
        @status untested (17/04/2002) */
    int
      GetChannel(const ImageBase & source, const unsigned int channel);

    /** @brief returns the UUID of the image */
    /*inline const BIAS::UUID &
      GetUID() const
    {
      return _UID;
    }
    ;*/

    inline MetaData *
      GetMetaData()
    {
      return &_MetaData;
    }
    ;

    inline const MetaData *
      GetMetaData() const
    {
      return &_MetaData;
    }
    ;

    inline void
      SetMetaData(const MetaData& m)
    {
      _MetaData = m;
    }
    ;

    inline int
      GetVersionNumber() const
    {
      return Version_;
    }
    ;

    inline bool
      IsPlanar() const
    {
      return (!InterleavedDataOrder_);
    }
    ;

    inline bool
      IsInterleaved() const
    {
      return (InterleavedDataOrder_);
    }
    ;

    friend BIASImageBase_EXPORT
      std::ostream&
      operator<<(std::ostream& os, const ImageBase &img);
    friend BIASImageBase_EXPORT
      std::istream&
      operator>>(std::istream& is, ImageBase& img);

    /** @brief print the (typed) pixel value to stream.
       Implemented using a big switch with casting to BIAS::Image internally
       to work with all image formats w.r.t StorageTypes, planar, interleaved etc.
       Jan Woetzel*/
    static void
      PrintPixelValue(const ImageBase & im, const unsigned int x,
                      const unsigned int y, const unsigned short channel = 0,
                      std::ostream & os = std::cout);

    //@}

    /** Determines the internale ImageBase type and casts it to the output type.
     * \attention im and input coordinates are not checked for validity!
     * Only for interleaved images.
     **/
    template<class castType>
      static castType GetValue(const ImageBase & im, const unsigned int x,
                               const unsigned int y, const unsigned int channel = 0);

    /** Determines the internal ImageBase type and casts the input type to the type foreseen in im.
     * \attention im and input coordinates are not checked for validity!
     * Only for interleaved images.
     **/
    template<class inputType>
      static void SetValue(const ImageBase & im,
                           const unsigned int x,
                           const unsigned int y,
                           const unsigned int channel,
                           const inputType val);
      
    /** Copy channel, determines the internal ImageBase type and casts the input type to the type foreseen in im.
     * \attention im and channel both have to be initialized and of compatible size!
     * Only for interleaved images.
     **/
    template<class inputType>
      static void SetChannel(const ImageBase & im,                           
                             const unsigned int channelId,
                             const inputType* channelIn);

    /** Copy channel, determines the internal ImageBase type and casts it to the output type.
     * \attention im and channel both have to be initialized and of compatible size!
     * Only for interleaved images.
     **/
    template<class outputType>
      static void GetChannel(const ImageBase & im,                           
                             const unsigned int channelId,
                             outputType* channelOut);
#ifdef BIAS_HAVE_OPENEXR
    static void GetChannel(const ImageBase & im,
                           const unsigned int channelId,
                           half* channelOut);
#endif
        
    /** @name Set functions
        @{
    */
    inline void
      SetColorModel(EColorModel Model)
    {
      ColorModel_ = Model;
    }
    ;

    inline void
      SetInterleaved(bool interleaved)
    {
      InterleavedDataOrder_ = interleaved;
      if (interleaved)
        WidthStep_ = Width_ * Depth_ * ChannelCount_;
      else
        WidthStep_ = Width_ * Depth_;
      UpdateImageDataArrayFromImageData_();
    }
    ;

    /// needed by ImageIO
    inline void
      SetBitDepth(unsigned bitdepth)
    {
      BIASASSERT(bitdepth<=(8*Depth_));
      BitDepth_ = bitdepth;
    }
    ;

  /*  inline void
      SetUID(const BIAS::UUID& id)
    {
      _UID = id;
    }
    ;*/

    /** @brief sets the image's uid to invalid */
   /* inline void
      InvalidateUID()
    {
      _UID.Invalidate();
    }
    ;*/

    //@}


    /** @name Region of Interest Functions
        Important: the ROI has a size of (lrx-ulx)*(lry-uly) that means
        the lower right point is not part of the ROI!
        @{
    */

    /** Returns a pointer to the roi object.
        @author woelk 07/2005 */
    inline ROI*
      GetROI()
    {
      return &Roi_;
    }

    inline const ROI*
      GetROI() const
    {
      return &Roi_;
    }

    /** @brief deprecated, use SetROICorners()  */
    int
      SetROI(unsigned int UpperLeftX, unsigned int UpperLeftY,
             unsigned int LowerRightX, unsigned int LowerRightY);

    int
    SetROI(const ROI& roi);

    
    int
      SetROICorners(unsigned int UpperLeftX, unsigned int UpperLeftY,
                    unsigned int LowerRightX, unsigned int LowerRightY);

    /** @brief deprecated, use GetROI()->UnsetROI() */
    void
      UnsetROI();

    /** @brief deprecated, use GetROICorners() */
    void
      GetROI(unsigned int& UpperLeftX, unsigned int& UpperLeftY,
             unsigned int& LowerRightX, unsigned int& LowerRightY) const;
    /** @brief deprecated, use GetROICorners() */
    void
      GetROI(int& UpperLeftX, int& UpperLeftY, int& LowerRightX,
             int& LowerRightY) const;

    /** access region of interest rectangle JW */
    void
      GetROICorners(unsigned int& UpperLeftX, unsigned int& UpperLeftY,
                    unsigned int& LowerRightX, unsigned int& LowerRightY) const;
    /** access region of interest rectangle JW */
    void
      GetROICorners(int& UpperLeftX, int& UpperLeftY, int& LowerRightX,
                    int& LowerRightY) const;

    /** @brief deprecated, use 'os << *GetROI()' */
    void
      PrintROI(std::ostream& os = std::cout) const;

    /** @brief returns a copy of ROI (a new image) lower right point excluded,
        only interleaved images !
        @author Felix Woelk, grest
        @status untested **/
    int
      GetCopyOfROI(ImageBase& copy) const;

    /** @brief like GetCopyOfROI, but with lower right point included
        only interleaved images !
        @note GetCopyOfROI returns an image where the lower right ROI point
        is not included. This version does it. Correction are not made in
        the original method, because extensive corrections would become
        necessary in other methods.
        @author Ingo Thomsen  **/
    int
      GetCopyOfROI2(ImageBase& copy) const;

    /** @brief paste Image to current ROI
        @author Felix Woelk 02 2003 **/
    int
      Paste2ROI(const ImageBase& Image);

    /** @brief reduces image to current ROI,
        !!! image size changes !!!
        @author Felix Woelk
        @status untested
        @todo Make it work on colored Images **/
    int
      Cut2ROI();

    /** @brief sets all pixel not in ROI to zero
        @author woelk 10/2004 */
    void
      SetOutsideROIZero();

    inline const bool
      IsInROI(const double &x, const double& y) const
    {
      return Roi_.IsInROI(x, y);
    }

    /// deprecated, use GetROI()->GetCorners()
    const unsigned int
      GetROIUpperLeftX() const;

    /// deprecated, use GetROI()->GetCorners()
    const unsigned int
      GetROIUpperLeftY() const;

    /// deprecated, use GetROI()->GetCorners()
    const unsigned int
      GetROILowerRightX() const;

    /// deprecated, use GetROI()->GetCorners()
    const unsigned int
      GetROILowerRightY() const;

    //@}

    /** @name Operators
        @{
    */
    /** @brief assignment operator,
        allocates memory structure via Init only if necessary
        @author Felix Woelk **/
    ImageBase&
      operator=(const ImageBase& Source);

    /** @brief comparison operator,
        based upon the adresses of the actual image data
        @author Ingo Thomsen
        @status tested **/
    inline bool
      operator==(const ImageBase &img) const
    {
      return (GetImageData() == img.GetImageData());
    }

    ///@}


    /** @name Padding and Appending
        @{
    */

    /** @brief flips the image vertically (row order is inverted)
        In place function
        return 0 in case of success, -1 otherwise
        @author Daniel Grest
        @date Jun 2002
        @status Interleaved tested, planar untested
        Aug 2002: Jan-Friso Evers: fixed interleaved for float and other */
    int
      Flip();

    /** @brief flips the image horizontal (column order is inverted)
        In place function
        return 0 in case of success, -1 otherwise
        @author woelk 10/2004 */
    int
      FlipHorizontal();

    /// interface for the above JW
    inline int
      Mirror()
    {
      return FlipHorizontal();
    }
    ;

    /* enlarge image size keeping old pixel data 1:1
       Fill new pixel with padVal which may be zero (for black).
       @author Jan Woetzel */
    int
      Pad(BIAS::ImageBase & dest, const unsigned int & newwidth,
          const unsigned int & newheight, const int & padVal = 0) const;

    /* in place Padding @author Jan Woetzel */
    int
      Pad(const unsigned int & newwidth, const unsigned int & newheight,
          const int & padVal = 0);

    /** backward compatibility interface for Pad.
        DEPRECATED
        @author Jan Woetzel 2005 */
    int
      ZeroPad(const unsigned int newwidth, const unsigned int newheight,
              unsigned char bgcolor = 0);

    /** increase the size of this image to next power of two
        (e.g. 384x100>512x128)
        the original image S is set to upper left rectangle
        @param padVal used by memset to pad the right and lower added pixels
        No scale, just padding!
        \verbatim
        orig xx
        xxxxxxx
        \endverbatim
        @author Jan Woetzel 09/2003 - 11/2005 */
    int
      PadToPowerOfTwo(BIAS::ImageBase & dest, const int & padVal = 0) const;

    /// in place version of the above  @author JW
    int
      PadToPowerOfTwo(const int & padVal = 0);

    /** first pad, then flip.
        useful for OpenGL texture image which must be pow2 size and
        Y-flipped ics.
        example:
        \verbatim
        0 1 2
        3 4 5
        -->
        x x x x
        x x x x
        3 4 5 x
        0 1 2 x
        \endverbatim
        \todo  more efficient in place with memmove
        @author Jan Woetzel */
    int
      PadToPowerOfTwoAndFlip(const int & padVal = 0);

    /// @}


    /** @name Misc functions
        @{ */

    /** @brief checks if data area has same "size" as Image of other type
     */
    inline bool
      SamePixelAndChannelCount(const ImageBase& Image) const;

    /** @brief checks if data area has bigger or the same "size" as Image
        of other type
    */
    inline bool
      NotBiggerPixelAndSameChannelCount(const ImageBase& Image) const;

    /** @brief Releases ImageData_
        (to be used together with RedirectImageDataPointer)
        @author Felix woelk */
    inline void
      ReleaseImageDataPointer();

    /**  @brief This method takes data and set the internal
         image data pointer to this.

         You should really know what you are doing using this method.
         Don't say you haven't been warned.
         @author Friso Evers **/
    inline void
      RedirectImageDataPointer(void *data)
    {
      ImageData_ = data;
      UpdateImageDataArrayFromImageData_();
    }
    ;

    /** @brief set data pointer to NULL, such that image IsEmpty
        _without_ deletion of data.
        @author evers
    */
    inline void
      ClearDataPointer()
    {
      ImageData_ = NULL;
      delete[] ImageDataArray_;
      ImageDataArray_ = NULL;
    }

    /** @return true if image size is a power of two JW */
    bool
      IsPowerOfTwoSize() const;
    static bool
      IsPowerOfTwoSize(const unsigned int w, const unsigned int h);

    /** @return next pow2 >= val  JW */
    static unsigned int
      PowerOfTwoSize(const unsigned int & val);

    /** @return true if both, width and height are already a power of two.
        The next power of two size >= actual size is computed and stored
        in args.
        useful for OpenGL texture enlargement for 'power of two' textures.
        @author Jan Woetzel 09/2003 */
    bool
      PowerOfTwoSize(unsigned int & width2, unsigned int & height2) const;

    void *
      PixelValueBase(unsigned x, unsigned y, unsigned channel = 0);

    void
      PrintHeader(std::ostream& os = std::cout) const;

    /// zeroes the image
    inline void
      SetZero();

    /** @brief check if image contains that pixel position */
    inline bool
      IsPositionInImage(const int& x, const int& y) const;

    /** @brief find closest valid pixel position to x,y
        @author koeser 01/2005
        @return 0=was correct 3,4=y out of range, 1,2 x out of range  */
    inline int
      MoveToClosestPositionInImage(int& x, int& y) const;

    /** @brief transfer BIAS image coordinates [0..w-1] x [0..h-1]
        to GL texture coordinates [0..1[ x [0..1[

        GL coordinates are relative to image size of *this (must be valid) !

        Please note that this is only one of many possible bias to gl coord
        transformations which may be particularly different for pow2, non-pow2,
        rectangle, padded, scaled, bordered, offset etc. OpenGL textures. */
    inline void
      BIASToTextureCoordinates(const double& biasx, const double& biasy,
                               double& gl_x, double& gl_y) const;

    /** @brief transfer GL texture coordinates [0..1[ x [0..1[
        to BIAS image coordinates [0..w-1] x [0..h-1]

        GL coordinates are relative to image size of *this (must be valid) !

        Please note that this is only one of many possible bias to gl coord
        transformations which may be particularly different for pow2, non-pow2,
        rectangle, padded, scaled, bordered, offset etc. OpenGL textures. */
    inline void
      TextureToBIASCoordinates(const double& gl_x, const double& gl_y,
                               double& biasx, double& biasy) const;

    /** get the size in Byte of the type corresponding to a storagetype
        uses sizeof(type) internally.
        Useful for allocation calculations.
        @author Jan Woetzel */
    static int
      GetSizeByte(const enum BIAS::ImageBase::EStorageType & storagetype);

    /** get the number of (packed) data values of color model
        E.g:
        CM_Grey              : 1
        CM_YUYV422           : 2 packs 3 channel YUV into 2 Bytes
        CM_RGB               : 3
        CM_BGRA              : 4
        CM_Bayer_GBRG        : 1 packed Bayer pattern
        CM_PGR_XB3_F7M3_GBRG : 3 packed three Bayer patterns w. 9 values in tri-image
        Useful for allocation calculations.
        Returns the number of data units, not Bytes!
        @author Jan Woetzel */
    static int
      GetSizeUnits(const enum BIAS::ImageBase::EColorModel & colormodel);

    /** get the number of channels corresponding to the enum ColorModel
        Determines the number of (packed) data values.
        Is typically consistent with GetSizeByte.
        E.g.
        CM_Grey              : 1
        CM_YUYV422           :   packs 3 channel YUV into 2 Bytes
        CM_RGB               : 3
        CM_BGRA              : 4
        CM_Bayer_GBRG        : 1 packs 3 channel RGB into 1 Byte Bayer pattern
        @author Jan Woetzel */
    static int
      GetChannelcount(const enum BIAS::ImageBase::EColorModel & colormodel);

    /** @brief computes the storage data size in Byte required for a given video format.
        @author Jan Woetzel */
    static int
      GetStorageSizeByte(const unsigned int & width, const unsigned int & height,
                         const unsigned int nChannels, const enum EStorageType storageType
                         /*,const EColorModel colormodel*/);



    /// OpenCV onscreen popup display, very useful for fast debugging. (JW)

    /** display image as popup to screnn sing OpenCv IplImage using Highgui functions.
        Very useful to easily display an image, e.g. for debugging.
        Can be used inside MSVS debugger watch to inspect intermediate Images wit a popup.
        Can be used with waiting for user interaction (for delayMsec).
        @return positive nr. of pressed key or negative if no key pressed or error occured.
        @param waitForKey if true delayMsec is waited for keypres. 0=forever.
        @param delayMsec if waitForKey is true wait for key with 0=forever, see cvWaitkey.
        @return pressed key (if any)
        @author Jan Woetzel 2005 */
   /* int
      Display(const std::string & DestWin, const bool & autoresize,
              const bool & moveToTopLeft, const bool & waitForKey,
              const unsigned int & delayMsec = DEFAULT_Display_delay,
              const float & scale = DEFAULT_32to8_scale,
              const bool & allowAlphaWindow = false) const;*/

    /** simpler interface for the above.
        shows image in window named "DestWin" without. Does not wait for key,
        you may want to use WrapBias2Ipl::WaitKey after calling this.
    */
   /* int
      Display(const std::string & DestWin) const;*/

    /** interface for the above  @author Jan Woetzel 2005 */
 /*   int
      Display(const bool & waitForKey, const unsigned int & delayMsec,
              const float &scale) const;*/

    /** @brief display image simple signature interface for Debugger usage
        This function can be used to display an (intermediate) image
        "interactively" with the MS Visual Studio Debugger
        in "Watch+Debug" expert mode.
        This signature is required because MSVS debugger
        does not support default params.
        @author Jan Woetzel */
 /*   int
      Display() const;

*/

    static void StringToColorModel(const std::string& str,
                                   ImageBase::EColorModel &cm);
    static void StringToStorageType(const std::string& str,
                                    ImageBase::EStorageType &st);


    // @}



  protected:
    /** @brief recomputes pointer array returned by GetImageDataArray */
    int
      UpdateImageDataArrayFromImageData_();

    /** changes StorageType data mmeber */
    inline void
      SetStorageType(const EStorageType st)
    {
      StorageType_ = st;
    }

    // ---------------------------------------
    // --- data members:
    // ---------------------------------------

    /// version number of imagebase class
    static const int Version_ = VERSION_NUMBER;
    /// the color model of the pixels
    enum EColorModel ColorModel_;
    /// the storage type in a pixel channel
    enum EStorageType StorageType_;
    /// number of channels per pixel
    unsigned int ChannelCount_;
    /// size of one channel of one pixel in bytes
    unsigned int Depth_;
    /// relevant bits per pixel per channel
    unsigned int BitDepth_;
    /// image width in pixels
    unsigned int Width_;
    /// image height in pixels
    unsigned int Height_;
    /// planar or interleaved: planar means we have several image planes,
    /// e.g. for a 3x3 RGB image data is RRRRRRRRRGGGGGGGGGGBBBBBBBB
    /// while interleaved means RGBRGBRGBRGBRGBRGBRGBRGBRGB
    bool InterleavedDataOrder_;
    /// size of a line in bytes for interleaved, NOT nr . of pixels (but size a line in one plane for planar)
    unsigned int WidthStep_;
    /// array of pointers to the first byte in an image row
    void** ImageDataArray_;
    /// a pointer to the image data
    void* ImageData_;
    /// unique id for every image
  //  BIAS::UUID _UID;
    /// additional data block, handled by derived classes
    MetaData _MetaData;
    /// roi object
    ROI Roi_;

  }; // end of class

  //// fixes for building dlls
  BIASImageBase_EXPORT std::ostream&
  operator<<(std::ostream& os, const ImageBase::EStorageType &st);
  BIASImageBase_EXPORT std::ostream&
  operator<<(std::ostream& os, const ImageBase::EColorModel &st);
  BIASImageBase_EXPORT std::ostream&
  operator<<(std::ostream& os, const ImageBase &img);
  BIASImageBase_EXPORT std::istream&
  operator>>(std::istream& is, ImageBase& img);


#include "ImageBaseInline.hh"


} // end namespace

#endif // _IMAGE_BASE_H_INCLUDED_
