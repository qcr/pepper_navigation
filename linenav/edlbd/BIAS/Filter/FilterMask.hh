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


#ifndef __FILTERMASK_hh__
#define __FILTERMASK_hh__

#include <Base/Image/Image.hh>
#include <Base/Math/Matrix.hh>
#include <Base/Math/Vector.hh>

/// define the type used in the floating point filter mask
#define FM_FLOAT float
/// define the type used in the integer filter mask
#define FM_INT  short
/// set other filter by default to zero ?
#define FM_RESET_OTHER true

#define D_FM_PRECISION (1<<0)
#define D_FM_INTAPPROX (1<<1)

namespace BIAS {
  /** @class FilterMask
      @ingroup g_filter
      @brief A filter mask (or a kernel) used for convolution

      When convolving an image with a FilterMask f, that mask is at first
      reflected at its symmetry center. Afterwards for each pixel x a
      weighted (by the entries of f) sum of the pixels around x is computed
      and written into the output image. In case of integer filtering,
      the result is shifted by the appropriate right shift.

      You can define a filter mask by specifying either the float values or
      the int values and computing the other type. You may also set both
      manually, set the initalltypes flag to false for that.
      If your filtermask is separable you have to call the vector-init to
      define that.
      @author skoglund, koeser, woelk 11/2004  */
  
  class BIASFilter_EXPORT FilterMask:
    public Debug
  {
  public:
    FilterMask();

    /** @brief calls operator= */
    FilterMask(const FilterMask& mask) 
    { (*this) = mask; }

    /** @brief calls Init 
        @param ResetOtherType if true, sets other filter entries to zero */
    FilterMask(const Matrix<FM_INT> &kernel, int rs,
               bool ResetOtherType=FM_RESET_OTHER) 
    { Init(kernel, rs, ResetOtherType); }
    
    /** @brief calls Init */
    FilterMask(const Matrix<FM_FLOAT> &kernel, 
               bool ResetOtherType=FM_RESET_OTHER) 
    { Init(kernel, ResetOtherType); }

    /** @brief calls Init */
    FilterMask(const Vector<FM_INT> &horz, const Vector<FM_INT> &vert,
               int hrs, int vrs, bool ResetOtherType=FM_RESET_OTHER) 
    { Init(horz, vert, hrs, vrs, ResetOtherType); }

    /** @brief calls Init */
    FilterMask(const Vector<FM_FLOAT> &horz, const Vector<FM_FLOAT> &vert, 
               bool ResetOtherType=FM_RESET_OTHER) 
    { Init(horz, vert, ResetOtherType); }

    /** @brief sets _sKernel and also _fKernel. _Separable is set to false */
    void Init(const Matrix<FM_INT> &kernel, int rs, 
              bool ResetOtherType=FM_RESET_OTHER);

    /** @brief sets _fKernel and also calculates the best int approximation 
        int _sKernel. _Separable is set to false. */
    void Init(const Matrix<FM_FLOAT> &kernel,
              bool ResetOtherType=FM_RESET_OTHER);
    
    /** @brief sets horz and vert vectors and their right shifts, 
        _Separable is true, FM_FLOAT vectors are updated also */
    void Init(const Vector<FM_INT> &horz, const Vector<FM_INT> &vert, 
              int hrs, int vrs, bool ResetOtherType=FM_RESET_OTHER);

    /** @brief sets horz and vert vectors, 
        _Separable is true, integer vectors are updated also */
    void Init(const Vector<FM_FLOAT> &horz, const Vector<FM_FLOAT> &vert,
              bool ResetOtherType=FM_RESET_OTHER);

    /** @brief returns pointer to int filter mask */
    inline const Matrix<FM_INT>* GetKerneli() const
    { return &_sKernel; }

    /** @brief returns pointer to float filter mask */
    inline const Matrix<FM_FLOAT>* GetKernelf() const
    { return &_fKernel; }

    /** @brief returns pointer to horizontal int vector for sep. filtering */
    inline const Vector<FM_INT>* GetSepih() const
    { return &_sHoriz; }

    /** @brief returns pointer to vertical int vector for sep. filtering */
    inline const Vector<FM_INT>* GetSepiv() const
    { return &_sVert; }

    /** @brief returns pointer to horiz float vector for sep. filtering */
    inline const Vector<FM_FLOAT>* GetSepfh() const
    { return &_fHoriz; }

    /** @brief returns pointer to vertical float vector for sep. filtering */
    inline const Vector<FM_FLOAT>* GetSepfv() const
    { return &_fVert; }

    /** @brief checks if the kernel is separable */
    inline bool IsSeparable() const 
    { return _Separable; }

    /** @brief copy operator, copies ALL components */
    FilterMask &operator=(const FilterMask& src);

    /** @brief returns right shift applied to data after int mask filtering */
    inline int GetKernelRightShift() const 
    { return _KernelRightShift; }

    /** @brief if separable, returns right shift after separated horizontal 
        int filtering */
    inline int GetHorizRightShift() const 
    { return _HorizRightShift; }
    
    /** @brief if separable, returns right shift after separated vertical 
        int filtering */
    inline int GetVertRightShift() const 
    { return _VertRightShift; }
    
    /** @brief create the float filter from the int filter */
    void CreateFloatFilter();

    /** @brief create the int filter from the float filter */
    void CreateIntFilter(int rshift, int rshifth, int rshiftv);

    /** @brief fill float filter with zeros, same dim as int filter */
    void ResetFloatFilter();

    /** @brief fill int filter with zeros, same dim as float filter */
    void ResetIntFilter();


    /** @brief compute the number of shiftable bits for int-from-float mask

        When you compute an int approximation of the float mask, you must
        avoid overflow. The weighted sum of the convolution is stored in 
        the type CONV_INT, e.g. int, which means 31 bit range.
        The input data has typically 8 bit, so depending on the norm of the
        float filter mask, say 1.0 in this example, we can approximate
        a 0.3 entry in the float mask by and int entry = 0.3*(1<<(31-8))).
        This function computes the maximum possible shift.

        @param NumberOfBitsInInputData typically sizeof(InputImageType)*8
        @param NumberOfBitsInTempData typically sizeof(CONV_INT)*8-1
        @return number of possible shifts in int mask approximation
    */
    int ComputeIntPrecisionBits(int NumberOfBitsInInputData,
                                int NumberOfBitsInTempData);

    /** @brief reset data, all kernel to zero, separable to false */
    inline void SetZero() {
      if (_sKernel.num_rows()>0) _sKernel.SetZero(); 
      if (_fKernel.num_rows()>0) _fKernel.SetZero(); _Separable=false;}

    /** @brief transposes the kernel (and adapts all dependent data) */
    void Transpose() ;
  
    /** returns true if size is 1 by one 
        @author woelk 05/2005 */
    inline bool IsSize1x1()
    { int h, v; GetSize(h, v); return (h==1 && v==1); }

    /** returns the size of the Kernel 
        @author woelk 05/2005 */
    inline void GetSize(int& hor, int& vert) const
    { if (_Separable) { hor=_fHoriz.size(); vert=_fVert.Size(); } 
      else { hor=_fKernel.num_cols(); vert=_fKernel.num_rows(); } }

    friend BIASFilter_EXPORT std::ostream& operator<<(std::ostream& os, const FilterMask& fm);
  protected:
    /// the filter mask if we have a integer type convolution
    Matrix<FM_INT> _sKernel;
    /// the filter mask if we have a floating type convolution
    Matrix<FM_FLOAT> _fKernel;
    /// the filter mask in separated representation (integer)
    Vector<FM_INT> _sHoriz, _sVert;
    /// the filter mask in separated representation (floating point)
    Vector<FM_FLOAT> _fHoriz, _fVert;
    /// flag indicating if the 2 vectors or the matrix should be used as 
    /// kernel representation
    bool _Separable;
    /// the shifts used for the integer filter masks
    int _KernelRightShift, _HorizRightShift, _VertRightShift;

  
  }; // class

} // namespace

#endif // __FilterMask_hh__
