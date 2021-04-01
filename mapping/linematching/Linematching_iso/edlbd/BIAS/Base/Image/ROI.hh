/* This file is part of the BIAS library (Basic ImageAlgorithmS).

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA*/
#ifndef __ROI_hh__
#define __ROI_hh__



#include <Base/Debug/Debug.hh>
#include <Base/Debug/Error.hh>
#include <Base/Debug/Exception.hh>
#include <iostream>
#include <vector>
#include <cmath>

#define ROI_VERSION_NUMBER 200

namespace BIAS {

  // forward declaration to avoid mutual inclusion
  class ImageBase;

  enum ERoiType { ROI_Corners, ROI_Mask, ROI_Points, ROI_Rows };

  BIASImageBase_EXPORT std::ostream& operator<<(std::ostream& os, const enum ERoiType& type);

  /** @class Position
      @ingroup g_image_base
      @brief stores valid/nvalid positions in image

      @author woelk 07/2005 */
  class BIASImageBase_EXPORT Position
  {
  public:
    Position() : x(0), y(0) {};
    Position(unsigned xx, unsigned yy) : x(xx), y(yy) {};
    Position(const Position &pos) { *this = pos; }

    ~Position() {};

    /// assignment operator
    Position& operator=(const Position& p) { x=p.x; y=p.y; return *this; }

    /// comparison, needed for sorting
    bool operator<(const Position& p) const
    { return (y==p.y)?(x<p.x):(y<p.y); }
    /// comparison
    bool operator!=(const Position& p) const
    { return (y!=p.y)||(x!=p.x); }

    friend BIASImageBase_EXPORT
      std::ostream& operator<<(std::ostream& os, const Position &pos);

    /// member variables
    unsigned x, y;
  };

  /** @class ROI
      @ingroup g_image_base
      @brief class for handling different region of interest (ROI)
      representations...

      The ROI class usually is member of an ImageBase and is stored together
      with the ImageBase. If an ImageBase is written to disk in the binary mip
      format, the ROI member is also automatically written in the same file.

      Flags indication the validity of the single representations exist within
      the ROI class.

      Right now, three different representations are possible:
      - \b Corner representation:
      The corner representation represents a rectangular ROI. The corner
      representation holds upper left and lower right corners defining a
      rectangular roi region. The upper left corner is included in the roi
      while the lower right corner is excluded from the roi. When ROI belongs
      to image and no roi is set, upper left corner is (0, 0) and lower right
      corner is given by (width, height). The corner representation is always
      valid.
      - \b Map representations:
      If the ROI belongs to an ImageBase, the MapData() and MapDataArray()
      must be of the same size as the Image. This is usually nesured by the
      ImageBase.Init() functions. The MapData() is of Type unsigned char.
      A value of 0 mneans not belonging to ROI, while any other value means
      belonging to ROI.
      - \b Vector representation:
      Every position in the vector belongs to roi, while every position not
      in the vector does not belong to the ROI. The VectorValid() flag
      indicates the validity of the vector entries.
      - \b Row representation:
      A start and an endn point ist stored fro each image row. It is thus
      possible to store a convex ROI very memory efficient

      The ROI class can later on be expanded in a way, that the vector does
      not only hold positions, but maybe somthing like more complex mask
      patches...

      \todo TODO change ROI memebr in ImageBase to pointer to:
      - share a ROI between multiple images

      @author woelk 07/2005 redesign woelk 02/2008 (c) www.vision-n.de */
  class BIASImageBase_EXPORT ROI : public Debug
  {
  public:
    /** @name construction and destruction
        @{ */

    /// destructor
    ~ROI();

    /// str. constructor
    ROI();

    /// @brief calls operator=
    ROI(const ROI& roi);

    /** Deletes internal memory, sets mask and vector invalid.
        @author woelk 07/2005 */
    void Release();

    /** Resizes parent image. Unsets region of interest!
        @author woelk 07/2005 */
    void Resize(unsigned width, unsigned height);

    /** @brief Delete region of interest.
        @author woelk 07/2005 **/
    void UnsetROI();

    /// assignment operator
    ROI& operator=(const ROI& roi);

    /// alloc and free of internal memory
    void SetROIType(const enum ERoiType& type);

    /// comparison
    bool operator==(const ROI& roi) const;
    inline bool operator!=(const ROI& roi) const
    { return !operator==(roi); }

    //@}

    /** @name access function
        @{ */

    /** @brief Sets a rectangular region of interest
     *  @param [in] UpperLeftX,UpperLeftY: first corner (x,y) of ROI (start), included in ROI
     *  @param [in] LowerRightX,LowerRightY: lower right corner (x,y) of ROI (end), NOT included in ROI   
     *  @attention LowerRightX and LowerRightY are not included in the ROI, this is correct per definition!        
     *  @author woelk 07/2005 */
    int SetCorners(unsigned UpperLeftX, unsigned UpperLeftY,
                   unsigned LowerRightX, unsigned LowerRightY);

    /** @brief Return the region of interest, by saving the
     *  coordinates within the variables defined by the parameters
     *  @param [out] UpperLeftX,UpperLeftY: first corner (x,y) of ROI (start), included in ROI
     *  @param [out] LowerRightX,LowerRightY: lower right corner (x,y) of ROI (end), NOT included in ROI   
     *  @author woelk 07/2005 */
    inline void GetCorners(unsigned &UpperLeftX, unsigned &UpperLeftY,
                           unsigned &LowerRightX,
                           unsigned &LowerRightY) const;
    /** @brief Return the region of interest, by saving the
     *  coordinates within the variables defined by the parameters
     *  @param [out] UpperLeftX,UpperLeftY: first corner (x,y) of ROI (start), included in ROI
     *  @param [out] LowerRightX,LowerRightY: lower right corner (x,y) of ROI (end), NOT included in ROI        
     *  @author woelk 07/2005 */
    inline void GetCorners(int& UpperLeftX, int& UpperLeftY,
                           int& LowerRightX, int& LowerRightY) const;

    /** @brief fetch individual ROI corner coordinate

        efficient inline access, used by BackwardMapping in most inner loop
        @author koeser 08/2007 */
    inline unsigned GetCornerUpperLeftX() const { return UpperLeftX_; }

    /** @brief fetch individual ROI corner coordinate

        efficient inline access, used by BackwardMapping in most inner loop
        @author koeser 08/2007 */
    inline unsigned GetCornerUpperLeftY() const { return  UpperLeftY_; }

    /** @brief fetch individual ROI corner coordinate

        efficient inline access, used by BackwardMapping in most inner loop
        @author koeser 08/2007 */
    inline unsigned GetCornerLowerRightX() const { return LowerRightX_; }

    /** @brief fetch individual ROI corner coordinate

        efficient inline access, used by BackwardMapping in most inner loop
        @author koeser 08/2007 */
    inline unsigned GetCornerLowerRightY() const { return LowerRightY_; }

    /** @brief ROI check if pixel position is inside the ROI */
    inline bool IsInROI(const double& x, const double& y) const;

    /** @brief ROI check if rectangular window is inside the ROI
     @attention Tests if right bottom corner is also included in the ROI! */
    inline bool IsInROI(const double& left, const double& top,
                        const double& right, const double& bottom) const;

    /** returns an image of StorageType unsigned char, where every
        pixel not in the ROI is set to UCHAR_MAX and every other pixel to 0
        @author woelk 07/2005 */
    int GetMaskImage(ImageBase& im) const;

    /** every pixel with value==0.0 is in the ROI, every pixel with
        value!=0.0 is not in the ROI
        @author woelk 07/2005 */
    int SetMaskImage(const ImageBase& im);

    /** Direct access to the mask data
        @author woelk 07/2005 */
    void SetMask(const unsigned& x, const unsigned& y, const bool val);

    /** Direct access to the mask data, const version
        @author woelk 07/2005 */
    bool Mask(const unsigned& x, const unsigned& y) const;

    /** Sets MaskValid_=false and VectorValid_=true.
        Copies pos -> Vector_.
        Sorts Vector_.
        @author woelk 07/2005 */
    void SetVector(std::vector<Position>& pos);

    /// returns a pointer to the vector representation of the ROI
    inline std::vector<Position>* GetVector()
    { if (RoiType_!=ROI_Points) return NULL; return &Vector_; }

    /** get a copy of mask vector. Returns true if vector is valid
        @author woelk 07/2005 */
    bool GetVector(std::vector<Position>& vec) const;

    /// width capacity of roi (image width)
    inline unsigned GetWidth() const
    { return Width_; }

    /// height capacity of roi (image height)
    inline unsigned GetHeight() const
    { return Height_; }

    /** Horizontal start and end position per image row, the length of the
        vectors always corresponds to the image height. When no pixels in a
        certain row y are located inside the roi, both start[y] and end[y]
        are zero. */
    inline bool GetRows(std::vector<unsigned>& start,
                        std::vector<unsigned>& end) const
    { start = RowStart_; end = RowEnd_; return (RoiType_==ROI_Rows); }

    /** Horizontal start and end position per image row, the length of the
        vectors always corresponds to the image height. When no pixels in a
        certain row y are located inside the roi, both start[y] and end[y]
        are zero. */
    void SetRows(const std::vector<unsigned>& start,
                 const std::vector<unsigned>& end);

    //@}

    /** @name interpolation
        @{ */

    /** @brief Check if subpixel position (x,y) in ROI is valid for bilinear
        interpolation (i.e. has 4 neighbors inside the ROI).
        @author esquivel */
    inline bool CheckBilinearInterpolation(const double x,
                                           const double y) const;

    /** @brief Check if subpixel position (x,y) in ROI is valid for bicubic
        interpolation (i.e. 4x4 window centered at (x,y) is inside the ROI).
        @author esquivel */
    inline bool CheckBicubicInterpolation(const double x,
                                           const double y) const;

    /** @brief Get valid neighbor pixels in ROI for bilinear interpolation
        at subpixel position (x,y).
        @return Returns number of valid neighbors found in ROI (0 to 4).
        @author esquivel */
    inline int GetBilinearInterpolationNeighbors(const double x,
                                                 const double y,
                                                 std::vector<int> &nx,
                                                 std::vector<int> &ny) const;
    //@}

    /** @name checking
        @{ */

    /// is the mask image valid?
    inline enum ERoiType GetROIType() const
    { return RoiType_; }

    /// is the position vector valid?
    inline bool MaskValid() const
    { return (RoiType_==ROI_Mask); }

    /// is the position vector valid?
    inline bool VectorValid() const
    { return (RoiType_==ROI_Points); }

    /** Returns true if both ROIs have the same size.
        Only the rectangular ROI as defined by the corners is validated.
        @author woelk 07/2005 */
    bool SameSize(const ROI& roi) const;

    //@}

    /** @name io functions
        @{ */

    ///
    friend BIASImageBase_EXPORT std::ostream&
      operator<<(std::ostream& os, const ROI& roi);

    /** binary output to stream
        @author woelk 07/2005 */
    int WriteBinary(std::ostream& os) const;

    /** binary input from stream
        @author woelk 07/2005 */
    int ReadBinary(std::istream& is);

    //@}

    /** @name conversion functions
        @{ */

    /** Generic conversion function from current ROI representation to the
        given ROI representation.
        @return Returns 0 if the conversion was successful, < 0 otherwise.
        @author esquivel 01/2010
    */
    int ConvertROIType(const enum ERoiType& type);

    //@}

    void Erode(const ROI &src, const unsigned half_mask_size);

  protected:
    /// which method is used to store the ROI
    enum ERoiType RoiType_;
    /// maximum dimension of roi data
    unsigned Width_, Height_;

    /// internal data for ROI of type ROI_Corners
    /// rectangular region of interest defined by upper left and lower right
    /// corners. The upperleft corner is part of the rectangle, while the
    /// lower is just outside
    unsigned UpperLeftX_, UpperLeftY_;
    unsigned LowerRightX_, LowerRightY_;

    /// internal data for ROI of type ROI_Mask
    /// the mask data: Mask_[y*Width_+x] corresponds to pixel at (x,y)
    /// a bool vector is used, because the bool std::vector is internally
    /// stored as a bitfield
    std::vector<bool> Mask_;

    /// internal data for ROI of type ROI_Points
    /// Vector instance of ROI, always sorted from top left to bottom right
    std::vector<Position> Vector_;

    /// internal data for ROI of type ROI_Rows
    /// the ROI is specified by a min and a max value for each row in the image
    std::vector<unsigned> RowStart_, RowEnd_;

    /// version number of ROI class
    static const int Version_;

    /** @name internal conversion functions
        @{ */

    /** Fills mask representation of ROI from corner representation */
    int Corners2Mask_();

    /** Fills vector representation of ROI from corner representation */
    int Corners2Vector_();

    /** Fills rows representation of ROI from corner representation */
    int Corners2Rows_();

    /** Fills corner/bounding box representation of ROI from mask
        representation. Returns 0 if mask is valid.  */
    int Mask2Corners_();

    /** Fills vector representation of ROI from mask representation.
        Returns 0 if mask is valid. Sets VectorValid flag if maks is valid.
        @author woelk 07/2005 */
    int Mask2Vector_();

    /** Fills rows representation of ROI from mask representation. */
    int Mask2Rows_();

    /** Fills corner/bounding box representation of ROI from vector
        representation. Returns 0 if vector is valid.
        @author woelk 07/2005 */
    int Vector2Corners_();

    /** Fills vector representation of ROI from mask representation.
        Returns 0 if vector is valid. Sets MaskValid flag if vector is valid.
        @author woelk 07/2005  */
    int Vector2Mask_();

    /** Fills rows representation of ROI from vector representation. */
    int Vector2Rows_();

    /** Fills corner representation of ROI from rows representation. */
    int Rows2Corners_();

    /** Fills mask representation of ROI from rows representation. */
    int Rows2Mask_();

    /** Fills vector representation of ROI from rows representation. */
    int Rows2Vector_();

    //@}

  }; // class

  /** human readable output operator for ROI, mainly for debugging
      @author woelk 07/2005 */
  BIASImageBase_EXPORT std::ostream&
  operator<<(std::ostream& os, const ROI& roi);


  ///////////////////////////////////////////////////////////////////////
  // inline functions
  ///////////////////////////////////////////////////////////////////////

  inline
  void ROI::GetCorners(unsigned &UpperLeftX, unsigned &UpperLeftY,
                       unsigned &LowerRightX,unsigned &LowerRightY) const
  {
    UpperLeftX  = UpperLeftX_;
    UpperLeftY  = UpperLeftY_;
    LowerRightX = LowerRightX_;
    LowerRightY = LowerRightY_;
  }

  inline
  void ROI::GetCorners(int& UpperLeftX, int& UpperLeftY,
                       int& LowerRightX,  int& LowerRightY) const
  {
    UpperLeftX  = (int)UpperLeftX_;
    UpperLeftY  = (int)UpperLeftY_;
    LowerRightX = (int)LowerRightX_;
    LowerRightY = (int)LowerRightY_;
  }
#  define rint(x) (floor(x+0.5))

  inline bool ROI::IsInROI(const double& x, const double& y) const
  {
    switch (RoiType_)
    {
      case ROI_Corners:
        return (UpperLeftX_ <= x && (LowerRightX_-1) >= x &&
                UpperLeftY_ <= y && (LowerRightY_-1) >= y);
        break;
      case ROI_Mask:
      {
        int iy = (int)rint(y);
        int ix = (int)rint(x);
        if (ix<0 || iy<0 || ix>=(int)Width_ || iy>=(int)Height_) return false;
        return Mask((unsigned)ix, (unsigned)iy);
      }
      break;
      case ROI_Points:
      {
        unsigned iy = (unsigned)rint(y);
        unsigned ix = (unsigned)rint(x);
        std::vector<Position>::const_iterator iter;
        for (iter=Vector_.begin(); iter!=Vector_.end(); iter++){
          if (iter->x==ix && iter->y==iy) return true;
        }
        return false;
      }
      break;
      case ROI_Rows:
      {
        int iy = (int)rint(y);
        if (iy<0 || iy>=(int)Height_) return false;
        int ix = (int)rint(x);
        return (ix>=(int)RowStart_[iy] && ix<(int)RowEnd_[iy]);
      }
      break;
      default:
        BEXCEPTION("Unknown ROI type "<<RoiType_<<" found");
        break;
    }
    return false;
  }

  inline bool ROI::IsInROI(const double& left, const double& top,
                           const double& right, const double& bottom) const
  {
    switch (RoiType_)
    {
      case ROI_Corners:
        return (UpperLeftX_ <= left && (LowerRightX_-1) >= right &&
                UpperLeftY_ <= top && (LowerRightY_-1) >= bottom);
        break;
      case ROI_Mask:
      {
        int il = (int)rint(left);
        int it = (int)rint(top);
        int ir = (int)rint(right);
        int ib = (int)rint(bottom);
        if (il<0 || it<0 || ir>=(int)Width_ || ib>=(int)Height_) return false;
        for (register int iy = it; iy <= ib; iy++)
          for (register int ix = il; ix <= ir; ix++)
            if (!Mask((unsigned)ix, (unsigned)iy)) return false;
        return true;
      }
      break;
      case ROI_Points:
      {
        // TODO Does not work when ROI is allowed to contain points multiply!
        unsigned il = (unsigned)rint(left);
        unsigned it = (unsigned)rint(top);
        unsigned ir = (unsigned)rint(right);
        unsigned ib = (unsigned)rint(bottom);
        int count = 0;
        const int size = (ir-il+1)*(ib-it+1);
        std::vector<Position>::const_iterator iter;
        for (iter=Vector_.begin(); iter!=Vector_.end() && count<size; iter++) {
          if (iter->x >= il && iter->x <= ir && iter->y >= it && iter->y <= ib)
            count++;
        }
        return (count == size);
      }
      break;
      case ROI_Rows:
      {
        int il = (int)rint(left);
        int it = (int)rint(top);
        int ir = (int)rint(right);
        int ib = (int)rint(bottom);
        if (it<0 || ib>=(int)Height_) return false;
        for (register int iy = it; iy <= ib; iy++) {
          if (il < (int)RowStart_[iy] || ir >= (int)RowEnd_[iy]) return false;
        }
        return true;
      }
      break;
      default:
        BEXCEPTION("Unknown ROI type "<<RoiType_<<" found");
        break;
    }
    return false;
  }

  inline bool ROI::
  CheckBilinearInterpolation(const double x, const double y) const
  {
    return IsInROI(floor(x), floor(y), ceil(x), ceil(y));
  }

  inline bool ROI::
  CheckBicubicInterpolation(const double x, const double y) const
  {
    return IsInROI(floor(x)-1, floor(y)-1, floor(x)+2, floor(y)+2);
  }

  inline int ROI::
  GetBilinearInterpolationNeighbors(const double x, const double y,
                                    std::vector<int> &nx,
                                    std::vector<int> &ny) const
  {
    nx.clear();
    ny.clear();
    const int fx = floor(x);
    const int cx = ceil(x);
    const int fy = floor(y);
    const int cy = ceil(y);
    if (IsInROI(fx, fy)) { nx.push_back(fx); ny.push_back(fy); }
    if (IsInROI(cx, fy)) { nx.push_back(cx); ny.push_back(fy); }
    if (IsInROI(fx, cy)) { nx.push_back(fx); ny.push_back(cy); }
    if (IsInROI(cx, cy)) { nx.push_back(cx); ny.push_back(cy); }
    return int(nx.size());
  }

} // namespace


#endif // __ROI_hh__
