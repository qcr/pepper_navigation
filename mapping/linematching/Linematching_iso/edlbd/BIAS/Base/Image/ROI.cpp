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
#include "ROI.hh"

#include <climits>




#include <Base/Debug/Error.hh>
#include "ImageBase.hh"

#include <algorithm>

using namespace BIAS;
using namespace std;

#define ROI_IDENTIFIER "MIP-ROI"
#define ROI_IDENTIFIER_LENGTH 7


//////////////////////////////////////////////////////////////////////
// operator<<
/////////////////////////////////////////////////////////////////////

ostream&
BIAS::operator<<(ostream& os, const enum ERoiType& type)
{
  switch (type)
    {
  case ROI_Corners:
    os << "ROI_Corners";
    break;
  case ROI_Mask:
    os << "ROI_Mask";
    break;
  case ROI_Points:
    os << "ROI_Points";
    break;
  case ROI_Rows:
    os << "ROI_Rows";
    break;
  default:
    os << "Unknown ROI type" << type;
    break;
    }
  return os;
}

ostream&
BIAS::operator<<(ostream& os, const Position& pos)
{
  os << "[" << pos.x << ", " << pos.y << "]";
  return os;
}

ostream&
BIAS::operator<<(ostream& os, const ROI& roi)
{
  os << "ROI (" << roi.Width_ << "x" << roi.Height_ << ") type: "
      << roi.RoiType_ << " : ";
  switch (roi.RoiType_){
  case ROI_Corners:
    os << "(" << roi.UpperLeftX_ << ", " << roi.UpperLeftY_ << ")  <-->  ("
        << roi.LowerRightX_ << ", " << roi.LowerRightY_ << ")";
    break;
  case ROI_Points:
    {
      vector<Position>::const_iterator it;
      for (it = roi.Vector_.begin(); it != roi.Vector_.end(); it++)
        os << *it << " ";
    }
    break;
  case ROI_Rows:
    for (unsigned y = 0; y < roi.Height_; y++)
      os << y << ":[" << roi.RowStart_[y] << ", " << roi.RowEnd_[y] << ")  ";
    break;
  case ROI_Mask:
    break;
  default:
    BEXCEPTION("unknonw roi type "<<roi.RoiType_);
    break;
  }
  return os;
}

//////////////////////////////////////////////////////////////////////
// class
/////////////////////////////////////////////////////////////////////

const int ROI::Version_ = ROI_VERSION_NUMBER;

ROI::~ROI()
{
  Release();
}

ROI::ROI() :
  RoiType_(ROI_Corners), Width_(0), Height_(0), UpperLeftX_(0), UpperLeftY_(0),
      LowerRightX_(0), LowerRightY_(0), Mask_(), Vector_(), RowStart_(),
      RowEnd_()
{
  NewDebugLevel("D_ROI_IO");
  UnsetROI();
}

ROI::ROI(const ROI& roi) :
  RoiType_(ROI_Corners), Width_(0), Height_(0), UpperLeftX_(0), UpperLeftY_(0),
      LowerRightX_(0), LowerRightY_(0), Mask_(), Vector_(), RowStart_(),
      RowEnd_()
{
  NewDebugLevel("D_ROI_IO");
  operator=(roi);
}

void
ROI::Release()
{
  RoiType_ = ROI_Corners;
  Width_ = 0;
  Height_ = 0;
  UpperLeftX_ = UpperLeftY_ = 0;
  LowerRightX_ = LowerRightY_ = 0;
  Mask_.clear();
  Vector_.clear();
  RowStart_.clear();
  RowEnd_.clear();
}

ROI&
ROI::operator=(const ROI& roi)
{
  RoiType_ = roi.RoiType_;
  Width_ = roi.Width_;
  Height_ = roi.Height_;
  UpperLeftX_ = roi.UpperLeftX_;
  UpperLeftY_ = roi.UpperLeftY_;
  LowerRightX_ = roi.LowerRightX_;
  LowerRightY_ = roi.LowerRightY_;
  Mask_ = roi.Mask_;
  Vector_ = roi.Vector_;
  RowStart_ = roi.RowStart_;
  RowEnd_ = roi.RowEnd_;

  return *this;
}

bool 
ROI::operator==(const ROI& roi) const
{
  if (RoiType_!=roi.RoiType_) return false;
  switch (RoiType_){
  case ROI_Corners:
    return (UpperLeftX_==roi.UpperLeftX_ && UpperLeftY_==roi.UpperLeftY_ && 
            LowerRightX_==roi.LowerRightX_ && LowerRightY_==roi.LowerRightY_);
    break;
  case ROI_Mask:
    if (Mask_.size()!=roi.Mask_.size()) return false;
    for (unsigned i=0; i<Mask_.size(); i++){
      if (Mask_[i]!=roi.Mask_[i]) return false;
    }
    return true;
    break;
  case ROI_Points:
    if (Vector_.size()!=roi.Vector_.size()) return false;
    for (unsigned i=0; i<Vector_.size(); i++){
      if (Vector_[i]!=roi.Vector_[i]) return false;
    }
    return true;
    break;
  case ROI_Rows:
    BIASASSERT(RowStart_.size()==RowEnd_.size());
    if (RowStart_.size()!=roi.RowStart_.size() ||
        RowEnd_.size()!=roi.RowEnd_.size() ) return false;
    for (unsigned i=0; i<Vector_.size(); i++){
      if (RowStart_[i]!=roi.RowStart_[i] || 
          RowEnd_[i]!=roi.RowEnd_[i]) return false;
    }
    return true;
    break;
  default:
    BIASERR("ROI::operator==() unfinished code");
    BIASABORT;
    break;
  }
  return false;
}

void
ROI::UnsetROI()
{
  RoiType_ = ROI_Corners;
  UpperLeftX_ = UpperLeftY_ = 0;
  LowerRightX_ = Width_;
  LowerRightY_ = Height_;
  Mask_.clear();
  Vector_.clear();
  RowStart_.clear();
  RowEnd_.clear();
}

void
ROI::Resize(unsigned w, unsigned h)
{
  if (Width_ != w || Height_ != h)
    {
      Release();
      Width_ = w;
      Height_ = h;
    }
}

void
ROI::SetROIType(const enum ERoiType& type)
{
  if (type == RoiType_)
    return;
  // release memory
  switch (RoiType_)
    {
  case ROI_Corners:
    break;
  case ROI_Mask:
    Mask_.clear();
    break;
  case ROI_Points:
    break;
  case ROI_Rows:
    RowStart_.clear();
    RowEnd_.clear();
    break;
  default:
    BEXCEPTION("Unknown ROI type "<<RoiType_<<" given");
    break;
    }
  // allocate memory
  switch (type)
    {
  case ROI_Corners:
    break;
  case ROI_Mask:
    Mask_.resize(Width_ * Height_, true);
    break;
  case ROI_Points:
    break;
  case ROI_Rows:
    RowStart_.resize(Height_, 0);
    RowEnd_.resize(Height_, Width_);
    break;
  default:
    BEXCEPTION("Unknown ROI type "<<RoiType_<<" given");
    break;
    }
  RoiType_ = type;
}

///////////////////////////////////////////////////////////////////////
// corners
//////////////////////////////////////////////////////////////////////


int
ROI::SetCorners(unsigned UpperLeftX, unsigned UpperLeftY,
                unsigned LowerRightX, unsigned LowerRightY)
{
  //cout<<"ROI::SetCorners() : "<<UpperLeftX<<","<<UpperLeftY
  //<<" - "<<LowerRightX<<","<<LowerRightY<<endl;
  SetROIType(ROI_Corners);
  int res = 0;
#ifdef BIAS_DEBUG
  if (UpperLeftX < 0)
    {
      BIASWARN( "UpperLeftX "<<UpperLeftX<<" is <0");
      UpperLeftX =0;
    }
  else if( UpperLeftX > Width_)
    {
      BIASWARN( "UpperLeftX "<<UpperLeftX<<" is out of image range "<<Width_);
      UpperLeftX = Width_;
    }
  if (UpperLeftY < 0)
    {
      BIASWARN("UpperLeftY "<<UpperLeftY<<" is <0");
      UpperLeftY=0;
    }
  else if(UpperLeftY > Height_)
    {
      BIASWARN("UpperLeftY "<<UpperLeftY<<" is out of image range "<<Height_);
      UpperLeftY=Height_;
    }
  if (LowerRightX < 0 )
    {
      BIASWARN("LowerRightX "<<LowerRightX<<" is < 0");
      LowerRightX = 0;
    }
  else if( LowerRightX > Width_ )
    {
      BIASWARN("LowerRightX "<<LowerRightX<<" is out of image range "<<Width_);
      LowerRightX = Width_;
    }

  if (LowerRightY < 0)
    {
      BIASWARN("LowerRightY "<<LowerRightY<<" is <0");
      LowerRightY = 0;
    }
  else if (LowerRightY > Height_ )
    {
      BIASWARN("LowerRightY "<<LowerRightY<<" is out of image range "<<Height_);     
      LowerRightY = Height_;
    }
  if (LowerRightX < UpperLeftX)
    {

      BIASWARN("WARNING: You swapped the x coordinates of the upper left:"<<UpperLeftX<<" and the"
          <<" lower right "<<LowerRightX<<" corner. It's been re-swapped!");
      unsigned int temp = UpperLeftX;
      UpperLeftX = LowerRightX;
      LowerRightX = temp;
    }

  if (LowerRightY < UpperLeftY)
    {
      BIASWARN("WARNING: You swapped the y coordinates of the upper left:"<<UpperLeftY<<" and the"
          <<" lower right "<<LowerRightY<<"corner. It's been re-swapped!");
      unsigned int temp = UpperLeftY;
      UpperLeftY = LowerRightY;
      LowerRightY = temp;
    }
#endif

  UpperLeftX_ = UpperLeftX;
  LowerRightX_ = LowerRightX;
  UpperLeftY_ = UpperLeftY;
  LowerRightY_ = LowerRightY;

  return res;
}

int
ROI::GetMaskImage(ImageBase& im) const
{
  if (RoiType_ != ROI_Mask)
    {
      return -1;
    }

  BIASASSERT(im.GetStorageType()==ImageBase::ST_invalid ||
      im.GetStorageType()==ImageBase::ST_unsignedchar);

  if (im.GetWidth() != Width_ || im.GetHeight() != Height_ ||
      im.GetChannelCount() != 1)
    im.Release();

  if (im.IsEmpty())
    im.Init(Width_, Height_, 1, ImageBase::ST_unsignedchar);

  unsigned char *dst = (unsigned char *) im.GetImageData();
  for (unsigned y = 0; y < Height_; y++)
    {
      for (unsigned x = 0; x < Width_; x++)
        {
          dst[y * Width_ + x] = (Mask(x, y)) ? (0) : (numeric_limits<
              unsigned char>::max());
        }
    }

  return 0;
}

int
ROI::SetMaskImage(const ImageBase& im)
{
  SetROIType(ROI_Mask);
  int res = 0;
  if (im.GetWidth() == Width_ && im.GetHeight() == Height_)
  {
    if (im.GetStorageType() == ImageBase::ST_unsignedchar &&
        im.GetChannelCount() == 1)
    {
      const unsigned char *ida =
        static_cast<const unsigned char *> (im.GetImageData());
      for (unsigned y = 0; y < Height_; y++)
      {
        for (unsigned x = 0; x < Width_; x++)
        {
          SetMask(x, y, (ida[y * Width_ + x] == 0));
        }
      }
    }
    else
    {
      BIASERR("Wrong mask image given, single channel unsigned "
              "character image expected!");
      res = -1;
    }
  }
  else
  {
    BIASERR("Wrong mask image size "<<im.GetWidth()<<"x"<<im.GetHeight()
            <<" given, expected "<<Width_<<"x"<<Height_<<"!"<<endl);
    res = -2;
  }
  return res;
}

void
ROI::SetRows(const std::vector<unsigned>& start,
    const std::vector<unsigned>& end)
{
  SetROIType(ROI_Rows);
  if (start.size() != Height_ || end.size() != Height_)
    {
        BEXCEPTION("invalid argument vector size ("<<Height_<<"): "
            <<start.size()<<" "<<end.size());
    }
  for (unsigned y = 0; y < Height_; y++)
    {
      if (start[y] > end[y])
        {
            BEXCEPTION("invalid argument vector content: start>end "<<start[y]
                <<" > "<<end[y]);
        }
      if (end[y] > Width_)
        {
            BEXCEPTION("invalid argument vector content: end > width "<<end[y]
                <<" > "<<Width_);
        }
    }
  RowStart_ = start;
  RowEnd_ = end;
}

void
ROI::SetVector(std::vector<Position>& pos)
{
  SetROIType(ROI_Points);
  Vector_ = pos;
  sort(Vector_.begin(), Vector_.end());
}

bool
ROI::GetVector(std::vector<Position>& vec) const
{
  vec = Vector_;
  return (RoiType_ == ROI_Points);
}

void
ROI::SetMask(const unsigned& x, const unsigned& y, const bool val)
{
  SetROIType(ROI_Mask);
  if (x >= Width_ || y >= Height_)
    BEXCEPTION("Out of bounds of mask image ("<<x<<", "<<y<<")");
  BIASASSERT(Height_ * Width_ == Mask_.size());
  Mask_[y * Width_ + x] = val;
}

bool
ROI::Mask(const unsigned& x, const unsigned& y) const
{
  BIASASSERT(RoiType_==ROI_Mask);
  if (x >= Width_ || y >= Height_)
    BEXCEPTION("Out of bounds of mask image ("<<x<<", "<<y<<")");
  BIASASSERT(Height_ * Width_ == Mask_.size());
  return Mask_[y * Width_ + x];
}

bool
ROI::SameSize(const ROI& roi) const
{
  BIASASSERT(RoiType_==ROI_Corners);
  return
    ((LowerRightX_-UpperLeftX_) - (roi.LowerRightX_-roi.UpperLeftX_)) == 0 &&
    ((LowerRightY_-UpperLeftY_) - (roi.LowerRightY_-roi.UpperLeftY_)) == 0;
}

int
ROI::WriteBinary(std::ostream& os) const
{
  if (!os)
    return -1;

  // 'header'
  os.write(ROI_IDENTIFIER, ROI_IDENTIFIER_LENGTH);
  os.write(reinterpret_cast<const char*> (&Version_), sizeof(int));
  // size of mask image
  os.write(reinterpret_cast<const char*> (&Width_), sizeof(unsigned));
  os.write(reinterpret_cast<const char*> (&Height_), sizeof(unsigned));
  // type
  os.write(reinterpret_cast<const char*> (&RoiType_), sizeof(enum ERoiType));
  BCDOUT(D_ROI_IO, "wrote roi type: "<<RoiType_<<endl);
  switch (RoiType_)
    {
  case ROI_Corners:
    // rectangular roi
    os.write(reinterpret_cast<const char*> (&UpperLeftX_), sizeof(unsigned));
    os.write(reinterpret_cast<const char*> (&UpperLeftY_), sizeof(unsigned));
    os.write(reinterpret_cast<const char*> (&LowerRightX_), sizeof(unsigned));
    os.write(reinterpret_cast<const char*> (&LowerRightY_), sizeof(unsigned));
    break;
  case ROI_Points:
    {
      unsigned length = (int)Vector_.size();
      os.write(reinterpret_cast<const char*> (&length), sizeof(unsigned));
      vector<Position>::const_iterator it;
      for (it = Vector_.begin(); it != Vector_.end(); it++)
        {
          BCDOUT(D_ROI_IO, "writing " << *it << endl);
          os.write(reinterpret_cast<const char*> (&(it->x)), sizeof(unsigned));
          os.write(reinterpret_cast<const char*> (&(it->y)), sizeof(unsigned));
        }
    }
    break;
  case ROI_Mask:
    bool tmp;
    for (unsigned y = 0; y < Height_; y++)
      {
        for (unsigned x = 0; x < Width_; x++)
          {
            BIASASSERT(Mask_.size() == Width_*Height_);
            tmp = Mask_[y * Width_ + x];
            os.write(reinterpret_cast<const char*> (&(tmp)), sizeof(bool));
          }
      }
    break;
  case ROI_Rows:
    for (unsigned y = 0; y < Height_; y++)
      {
        os.write(reinterpret_cast<const char*> (&(RowStart_[y])),
            sizeof(unsigned));
        os.write(reinterpret_cast<const char*> (&(RowEnd_[y])),
            sizeof(unsigned));
      }
    break;
  default:
    BEXCEPTION("Unknown ROI type "<<RoiType_<<" given");
    break;
    }

  return (os.good()) ? 0 : -2;
}

int
ROI::ReadBinary(std::istream& is)
{
  //SetDebugLevel(D_ROI_IO);
  int res = 0;
  char identifier[ROI_IDENTIFIER_LENGTH];
  int Version;
  unsigned length;
  vector<Position>::iterator it;

  // 'header'
  is.read(identifier, ROI_IDENTIFIER_LENGTH);
  if (strncmp(identifier, ROI_IDENTIFIER, ROI_IDENTIFIER_LENGTH) != 0)
    {
        BIASERR("invalid ROI identifier "<<identifier<<endl);
      return -1;
    }
  is.read(reinterpret_cast<char*> (&Version), sizeof(int));

  switch (Version)
    {
  case 100:
    {
      // rectangular roi
      is.read(reinterpret_cast<char*> (&UpperLeftX_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&UpperLeftY_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&LowerRightX_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&LowerRightY_), sizeof(unsigned));
      // size of mask image
      unsigned w, h;
      is.read(reinterpret_cast<char*> (&w), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&h), sizeof(unsigned));
      Resize(w, h);
      // size of vector
      is.read(reinterpret_cast<char*> (&length), sizeof(unsigned));
      // validity flags

      bool MaskValid, VectorValid;
      is.read(reinterpret_cast<char*> (&MaskValid), sizeof(bool));
      is.read(reinterpret_cast<char*> (&VectorValid), sizeof(bool));
      BCDOUT(D_ROI_IO, "read MaskValid "<<boolalpha<<MaskValid
          << "  VectorValid "<<boolalpha<<VectorValid<<endl);
      // mask image
      if (MaskValid && VectorValid)
        {
            BEXCEPTION("roi can no longer hold mask and vectors concurrently");
        }
      if (MaskValid)
        {
          unsigned char *mask = new unsigned char[w * h];
          BCDOUT(D_ROI_IO, "reading mask image of size "
              <<Width_<<"x"<<Height_<<endl);
          is.read(reinterpret_cast<char*> (mask), Width_ * Height_
              * sizeof(unsigned char));
          SetROIType(ROI_Mask);
          for (unsigned y = 0; y < Height_; y++)
            {
              for (unsigned x = 0; x < Width_; x++)
                {
                  SetMask(x, y, (mask[y * Width_ + x] == 0));
                }
            }
          delete[] mask;
        }
      else if (VectorValid)
        {
          SetROIType(ROI_Points);
          Vector_.resize(length);
          BCDOUT(D_ROI_IO, "reading vector of length "<<Vector_.size()<<endl);
          for (it = Vector_.begin(); it != Vector_.end(); it++)
            {
              is.read(reinterpret_cast<char*> (&(it->x)), sizeof(unsigned));
              is.read(reinterpret_cast<char*> (&(it->y)), sizeof(unsigned));
              BCDOUT(D_ROI_IO, "read "<<*it<<endl);
            }
        }
      res = (is.good()) ? 0 : -3;
    }
    break;
  case 200:
    // size
    unsigned w, h;
    is.read(reinterpret_cast<char*> (&w), sizeof(unsigned));
    is.read(reinterpret_cast<char*> (&h), sizeof(unsigned));
    Resize(w, h);
    // type
    enum ERoiType type;
    is.read(reinterpret_cast<char*> (&type), sizeof(enum ERoiType));
    SetROIType(type);
    switch (RoiType_)
      {
    case ROI_Corners:
      // rectangular roi
      is.read(reinterpret_cast<char*> (&UpperLeftX_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&UpperLeftY_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&LowerRightX_), sizeof(unsigned));
      is.read(reinterpret_cast<char*> (&LowerRightY_), sizeof(unsigned));
      break;
    case ROI_Points:
      unsigned length;
      is.read(reinterpret_cast<char*> (&length), sizeof(unsigned));
      Vector_.resize(length);
      BCDOUT(D_ROI_IO, "reading vector of length "<<Vector_.size()<<endl)
      ;
      for (it = Vector_.begin(); it != Vector_.end(); it++)
        {
          is.read(reinterpret_cast<char*> (&(it->x)), sizeof(unsigned));
          is.read(reinterpret_cast<char*> (&(it->y)), sizeof(unsigned));
          BCDOUT(D_ROI_IO, "read "<<*it<<endl);
        }
      break;
    case ROI_Mask:
      bool tmp;
      for (unsigned y = 0; y < Height_; y++)
        {
          for (unsigned x = 0; x < Width_; x++)
            {
              is.read(reinterpret_cast<char*> (&tmp), sizeof(bool));
              SetMask(x, y, tmp);
            }
        }
      break;
    case ROI_Rows:
      for (unsigned y = 0; y < Height_; y++)
        {
          is.read(reinterpret_cast<char*> (&(RowStart_[y])), sizeof(unsigned));
          is.read(reinterpret_cast<char*> (&(RowEnd_[y])), sizeof(unsigned));
        }
      break;
    default:
      BEXCEPTION("Unknown ROI type "<<RoiType_<<" given");
      break;
      }
    break;
  default:
    BIASERR("Unknown ROI version "<<Version<<"!");
    res = -2;
    break;
    }
  return res;
}


//////////////////////////////////////////////////////////////////////
// conversion functions
/////////////////////////////////////////////////////////////////////

int
ROI::ConvertROIType(const enum ERoiType& type)
{
  switch (RoiType_) {
    case ROI_Corners:
      switch (type) {
        case ROI_Corners:
          return 0;
        case ROI_Mask:
          return Corners2Mask_();
        case ROI_Points:
          return Corners2Vector_();
        case ROI_Rows:
          return Corners2Rows_();        
      }
      break;
    case ROI_Mask:
      switch (type) {
        case ROI_Corners:
          return Mask2Corners_();
        case ROI_Mask:
          return 0;
        case ROI_Points:
          return Mask2Vector_();
        case ROI_Rows:
          return Mask2Rows_();        
      }
      break;
    case ROI_Points:
      switch (type) {
        case ROI_Corners:
          return Vector2Corners_();
        case ROI_Mask:
          return Vector2Mask_();
        case ROI_Points:
          return 0;
        case ROI_Rows:
          return Vector2Rows_();        
      }
      break;
    case ROI_Rows:
      switch (type) {
        case ROI_Corners:
          return Rows2Corners_();
        case ROI_Mask:
          return Rows2Mask_();
        case ROI_Points:
          return Rows2Vector_();
        case ROI_Rows:
          return 0;
      }
      break;
    default:
      BIASERR("Source ROI for conversion has unknown type " << RoiType_ << "!");
      return -1;
  }
  BIASERR("Unknown target ROI type " << type << " for conversion given!");
  return -1;
}

int
ROI::Corners2Mask_()
{
  BEXCEPTION("Unfinished ROI conversion from corners to mask!");
  return (RoiType_ == ROI_Corners ? 0 : -1);
}

int
ROI::Corners2Vector_()
{
  BEXCEPTION("Unfinished ROI conversion from corners to point vector!");
  return (RoiType_ == ROI_Corners ? 0 : -1);
}

int
ROI::Corners2Rows_()
{
  BEXCEPTION("Unfinished ROI conversion from corners to rows!");
  return (RoiType_ == ROI_Corners ? 0 : -1);
}

int
ROI::Mask2Corners_()
{
  BEXCEPTION("Invalid ROI conversion from mask to corners!");
  return -1;
}

int
ROI::Mask2Vector_()
{
  if (RoiType_ != ROI_Mask)
    return -1;
  register unsigned y = 0, x = 0;
  vector<Position> vec;

  for (y = 0; y < Height_; y++)
    {
      for (x = 0; x < Width_; x++)
        {
          if (Mask(x, y))
            vec.push_back(Position(x, y));
        }
    }
  SetVector(vec);
  return 0;
}

int
ROI::Mask2Rows_()
{
  BEXCEPTION("Invalid ROI conversion from mask to rows!");
  return -1;
}

int
ROI::Vector2Corners_()
{
  if (RoiType_ != ROI_Points)
    return -1;
  //@todo Ensure that point vector is sorted from top left to bottom right!
  SetCorners(Vector_.begin()->x, Vector_.begin()->y, Vector_.rbegin()->x,
             Vector_.rbegin()->y);
  return 0;
}

int
ROI::Vector2Mask_()
{
  if (RoiType_ != ROI_Points)
    return -1;
  vector<Position> vec;
  GetVector(vec);
  vector<Position>::iterator it;
  SetROIType(ROI_Mask);
  for (it = vec.begin(); it != vec.end(); it++)
  {
#ifdef BIAS_DEBUG
    if (!(it->y >= 0 && it->x >= 0 &&
          it->y < GetHeight() && it->x < GetWidth() ))
      BIASERR((*it)<<"  "<<Width_<<"x"<<Height_<<endl);
    BIASASSERT(it->y >= 0 && it->x >= 0 &&
               it->y < Height_ && it->x < Width_ );
#endif    
    SetMask(it->x, it->y, true);
  }
  return 0;
}

int
ROI::Vector2Rows_()
{
  BEXCEPTION("Invalid ROI conversion from point vector to rows!");
  return -1;
}

int
ROI::Rows2Corners_()
{
  BEXCEPTION("Unfinished ROI conversion from rows to corners!");
  return (RoiType_ == ROI_Rows ? 0 : -1);
}

int
ROI::Rows2Mask_()
{
  BEXCEPTION("Unfinished ROI conversion from rows to mask!");
  return (RoiType_ == ROI_Rows ? 0 : -1);
}

int
ROI::Rows2Vector_()
{
  BEXCEPTION("Unfinished ROI conversion from rows to point vector!");
  return (RoiType_ == ROI_Rows ? 0 : -1);
}
/*
int
ROI::Vector2Corners()
{
  if (RoiType_ != ROI_Points)
    return -1;
  SetCorners(Vector_.begin()->x, Vector_.begin()->y, Vector_.rbegin()->x,
      Vector_.rbegin()->y);
  return 0;
}

int
ROI::Mask2Corners()
{
    BEXCEPTION("unfinished");
  return (RoiType_ == ROI_Mask);
}

void
ROI::Corners2Mask()
{
    BEXCEPTION("unfinished");
}

void
ROI::Corners2Vector()
{
    BEXCEPTION("unfinished");
}
*/
void 
ROI::Erode(const ROI &src, const unsigned half_mask_size)
{
  RoiType_ = src.RoiType_;
  switch (RoiType_){
  case ROI_Corners:
    UpperLeftX_ += half_mask_size;
    UpperLeftY_ += half_mask_size;
    if (UpperLeftX_>Width_){
      UpperLeftX_ = Width_;
    }
    if (UpperLeftY_>Height_){
      UpperLeftY_=Height_;
    }
    if (LowerRightX_<half_mask_size){
      LowerRightX_ = 0;
    }
    if (LowerRightY_<half_mask_size){
      LowerRightY_ = 0;
    }
    LowerRightX_ -= half_mask_size;
    LowerRightY_ -= half_mask_size;
    if (UpperLeftX_>LowerRightX_ || UpperLeftY_>LowerRightY_){
      BEXCEPTION("empty ROI");
    }
    break;
  case ROI_Rows:
    {
      // check the border of the mask, when the boprder is in the ROI, all
      // pixel in  the mask are also in the roi
      const int hws = half_mask_size;
      const int h = (int)src.RowStart_.size();
      RowStart_.resize(h);
      RowEnd_.resize(h);
      int i, y, x;
      for (y=0; y<h; y++){
        if (src.RowStart_[y]+2*hws+1>=src.RowEnd_[y]){ 
          RowStart_[y] = RowEnd_[y] = 0;
          continue; // next row
        }
        // search for the first pixel to the right from start, where the border
        // of the maske around it resides completely inside the old roi
        bool found_start = false;
        for (x=(int)(src.RowStart_[y])+hws; x<(int)(src.RowEnd_[y])-hws; x++){
          bool all_in = true;
          for (i=-hws; i<=hws; i++){
            if ( ! ( src.IsInROI(x+i, y+hws) && src.IsInROI(x+i, y-hws) &&
                     src.IsInROI(x+hws, y+i) && src.IsInROI(x-hws, y+i) ) ){
              all_in = false;
              break;
            }
          }
          if (all_in){
            found_start = true;
            RowStart_[y] = x;
            break; // found start, search for end
          }
        }
        if (!found_start){
          RowStart_[y] = RowEnd_[y] = 0;
          continue;
        }
        // search for the first pixel to the left from end, where the border
        // of the maske around it resides completely inside the old roi
        bool found_end = false;
        for (x=(int)(src.RowEnd_[y])-hws-1; x>(int)(RowStart_[y]); x--){
          bool all_in = true;
          for (i=-hws; i<=hws; i++){
            if ( ! ( src.IsInROI(x+i, y+hws) && src.IsInROI(x+i, y-hws) &&
                     src.IsInROI(x+hws, y+i) && src.IsInROI(x-hws, y+i) ) ){
              all_in = false;
              break;
            }
          }
          if (all_in){
            found_end = true;
            RowEnd_[y] = x;
            break; // found start, search for end
          }
        }
        if (!found_end){
          RowStart_[y] = RowEnd_[y] = 0;
          continue;
        }
      } 
    }
    break;
  case ROI_Mask:
  case ROI_Points:
  default:
    BIASERR("Erode() is not implemented yet for this ROI type!");
    BIASABORT;
    break;
  }
}
