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


#ifndef _Matrix_hh_
#define _Matrix_hh_

#include "bias_config.h"

#define _USE_MATH_DEFINES
#include <math.h>

//#include <Base/Debug/Error.hh>

#include <Base/Math/tnt/tnt.h>
#include <Base/Math/tnt/cmat.h>
#include <Base/Math/tnt/fmat.h>
#include <iostream>
#include <string>

#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
#include "Base/Math/Polynom.hh"
#endif

namespace BIAS {

#ifndef DOUBLE_EPSILON
#  define DOUBLE_EPSILON 1E-12
#endif

  template <class T> class Vector;
  template <class T> class Vector3;
  template <class T> class Matrix3x3;

  /** @enum MatrixInitType
      @brief can be passed to matrix constructors to init the matrix with
      the most often used values */
  typedef enum {MatrixZero, MatrixIdentity} MatrixInitType;

  /**    @class Matrix
         @ingroup g_math
         @brief  matrix class with arbitrary size, indexing is row major. <br>
         Access the data by M[row][col].

         class Matrix is the matrix class that should be used by common BIAS
         algorithms and data structures with arbitrary size <br>

         Access the data simply by M[row][col]. <br>

         It is derived from TNT::Matrix to inherit the basic operations and
         algorithm.
         special implementations should be done here and NOT in TNT:Matrix
         because the true 'base' Matrix class "TNT::Matrix"
         should be interchangeable and aside from this maintained by NIST.

         Note the specialized fixed size classes Matrix3x3, Matrix3x4, ...
         with more efficient memory layout.

         moved from MIP to BIAS (11/05/2002)
         operators are no longer class members, they are defined outside the
         class.

         @author Jan Woetzel, woelk
  */
  template <class T=double>
  class BIASMathBase_EXPORT Matrix : public TNT::Matrix<T>
  {
  public:
    virtual ~Matrix();

    Matrix() : TNT::Matrix<T>() {};

    explicit Matrix(const Matrix3x3<T>& mat);

    /** @author Jan Woetzel
     **/
    Matrix(int rows, int cols) :TNT::Matrix<T>(rows, cols) {};

    /** assignment with a constant values for all elements (=set)
        @author Jan Woetzel **/
    Matrix(int rows, int cols, const T value)
      : TNT::Matrix<T>(rows, cols, value) {};

    /** init with standard form
        @author koeser **/
    Matrix(int rows, int cols, const MatrixInitType& i)
      : TNT::Matrix<T>(rows, cols, T(0)) {
      switch (i) {
      case MatrixZero:return;
      case MatrixIdentity:{
        for (int r=0; r<rows; r++) {
          if (r==cols) return;
          (*this)[r][r] = T(1);
        }
        return;
      }
      default: BIASERR("undefined MatrixInitType");
        BIASABORT;
      }
    };


    /** DEPRECATED due to instantiation problem @author Jan Woetzel **/
    //Matrix(int rows, int cols, const char *s) : TNT::Matrix<T>(rows, cols, s){};

    /// replacement for the above to be called as Matrix<>(2,3, "1 2 3 4 5 6") @author Jan Woetzel
    explicit Matrix(const int rows, const int cols, const std::string & s);

    /** @author Jan Woetzel **/
    Matrix(const Matrix<T> &A) : TNT::Matrix<T>(A) {};

    /** @author Jan Woetzel **/
    Matrix(const TNT::Matrix<T> &A) : TNT::Matrix<T>(A) {};

    /** @brief constructs Nx1 Matrix from N-Vector */
    Matrix(const Vector<T> &v) : TNT::Matrix<T>(v.size(), 1) {
      for (int i=0; i<v.size(); i++)
        (*this)[i][0] = v[i];
    }

    /** transpose function, storing data destination matrix
        @author JMF   **/
    inline Matrix<T> Transpose() const;

    /** returns an matrix the same size as the matrix n and m with the
     *  largest elements taken from n or m
     * @author bangerer 01/2009
     */
    Matrix<T> GetMax(Matrix<T>& m);

    /** returns an matrix the same size as the matrix n and m with the
         *  smallest elements taken from n or m
         * @author bangerer 01/2009
         */
    Matrix<T> GetMin(Matrix<T>& m);

    /** absolute values of all elements of the matrix (in place)
     * @author bangerer 01/2009
     */
    void AbsIP();

    /** @brief computes the adjoint matrix
        @author Christian Beder */
    inline Matrix<T> Adjoint() const;

    /////////////////////// Get Functions ////////////////////////////////
    /** @name Get Functions
        @{
    */
    /** get the pointer to the data array of the matrix
        (for faster direct memeory access)

        the order of the data is linewise, which means an elemnt
        sequence of e.g.
        [0] = M(0,0)
        [1] = M(0,1)
        [2] = M(0,2)
        [3] = M(1,0) // next line
        [4] = M(1,1) ...
        @author Jan Woetzel
        @return a pointer to the data array (beginning with GetData[0])
        @status alpha (02/27/2002)
        -added const (jw) 03/12/2002 **/
    inline  T *GetData()
    { return this->v_; };

    inline  const T *GetData() const
    { return this->v_; };

    /** returns zero based arry for data access */
    inline const T **GetDataArray() const
    { return (const T **)this->row_; }

    inline T **GetDataArray()
    { return this->row_; }

    /** returns 1 based array to data access */
    inline T **GetDataArray1() const
    { return this->rowm1_; };

    inline unsigned int GetRows() const { return this->m_; }

    inline unsigned int GetCols() const { return this->n_; }

    /** Get a pointer to the last data element
        Do not use this on unitilized matrices with <= on pointers
        because on an unitilized matrix
        GetData() will be NULL and last Element < (typically FFFFF8)
        We don't want an if here for performance reasons of the innerst loop.
        @author Ingo Thomsen, Jan Woetzel
        @date 04/11/2002 **/
    inline const T* GetDataLast() const
    {
      BIASASSERT( GetNumElements()>0 ); // check NULL matrix outside
      return GetData() + GetNumElements() - 1;
    };

    inline T* GetDataLast()
    {
      BIASASSERT( GetNumElements()>0 ); // check NULL matrix outside
      return GetData() + GetNumElements() - 1;
    };

    /** @brief return a copy of row "row" of this matrix, zero based counting
        @author Jan Woetzel **/
    Vector<T> GetRow(const int& row) const;

    /** @brief return a copy of column "col", zero based counting
        @author Jan Woetzel **/
    Vector<T> GetCol(const int& col) const;

    /** @brief return a submatrix from this. Start index is [startRow][startCol]
               end index is [startRow+numRows-1][startCol+numCols-1].
               submatrix is resized to numRows/numCols if necessary.
        @author apetersen 12/2010
     */
    void GetSubMatrix(const size_t startRow, const size_t startCol,
                      const size_t numRows, const size_t numCols,
                      Matrix<T> &submatrix) const;

    /** @brief return a submatrix from this. Similar to matlab notation (except
               indexing, starts with 0 of course!). Vectors rows = '1 3' and
               cols = '1 2' for matrix:
                 / 1 2 3 \
                 | 4 5 6 |
                 \ 7 8 9 /
               results in:
                 / 1 2 \
                 \ 7 8 /
               Matrix has to be initialized!!!
        @author apetersen 7/2011
     */
    void GetSubMatrix(const Vector<int> &rows, const Vector<int> &cols,
                      Matrix<T> &submatrix) const;

    /** @brief return a submatrix from this. Similar to matlab notation (except
               indexing, starts with 0 of course!), see
               GetSubMatrix(const Vector<int> &rows, const Vector<int> &cols,
                            Matrix<T> &submatrix)
        @author apetersen 7/2011
     */
    Matrix<T> GetSubMatrix(const Vector<int> &rows,
                           const Vector<int> &cols) const;

    /** @brief sets a submatrix in this. Start index in this is [startRowInThis][startColInThis]
               end index is [startRowInThis+numRows-1][startColInThis+numCols-1]. Entries copyed
               from submatrix beginning at [startRowInSub][startColInSub] and stop at
               [startRowInSub+numRows-1][startColInSub+numCols-1]
        @author apetersen 12/2010
     */
    void SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                      const Matrix<T> &submatrix,
                      const size_t startRowInSub, const size_t startColInSub,
                      const size_t numRows, const size_t numCols);

    /** @brief sets a 3x3 submatrix in this. Start index in this is [startRowInThis][startColInThis]
               end index is [startRowInThis+2][startColInThis+2].
        @author apetersen 12/2010
     */
    void SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                      const Matrix3x3<T> &submatrix);

    /** @brief sets a 3x1 submatrix in this. Start index in this is [startRowInThis][startColInThis]
               end index is [startRowInThis+2][startColInThis].
        @author apetersen 12/2010
     */
    void SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                      const Vector3<T> &subvector);

    /** @brief set a row of matrix from vector
        @author woelk 08/2004 */
    void SetRow(const int row, const Vector<T>& data);

    /** @brief set a col of matrix from vector
        @author woelk 08/2004 */
    void SetCol(const int row, const Vector<T>& data);

    /** Copies the contents of data into this at the position indicated by
        row and col. The size of *this must be big enought to carry data
        @author woelk 05/2008 (c) www.vision-n.de */
    void Set(const int row, const int col, const Matrix<T> &data);

    /** Copies the contents of data into this at the position indicated by
        row and col. The size of *this must be big enought to carry data.
        Data i interpreted as column vector
        @author woelk 05/2008 (c) www.vision-n.de */
    void Set(const int row, const int col, const Vector<T> &data);

    /** Copies the contents of data into this at the position indicated by
        row and col. The size of *this must be big enought to carry data.
        Data is interpreted as row vector.
        @author woelk 05/2008 (c) www.vision-n.de */
    void SetTranspose(const int row, const int col, const Vector<T> &data);


    /** Returns the total number of elements
        @author Ingo Thomsen
        @date 04/11/2002
        @status tested    **/
    inline int GetNumElements() const
    { return this->num_cols() * this->num_rows(); };

    /** Returns the minimum value of the matrix elements
        @author Ingo Thomsen
        @date 04/11/2002
        @status tested    **/
    T GetMin() const;

    /** Returns the maximum value of the matrix elements
        @author Ingo Thomsen
        @date 04/11/2002
        @status tested    **/
    T GetMax() const;

    /// return biggest and smallest entry
    void GetMaxMin(T& max, T& min) const;

    /// return biggest and smallest absolute values
    void GetAbsMaxMin(T& max, T& min) const;


    /** Returns the mean value of the matrix elements
        @author Ingo Thomsen
        @date 04/11/2002
        @status tested    **/
    T GetMean() const;

    /** Return the L1 norm: |a| + |b| + |c| + ...
        @author Ingo Thomsen
        @date 04/11/2002
        @status untested    **/
    inline T NormL1() const;

    /** Return the L2 norm: a^2 + b^2 + c^2 + ...
        @author woelk 07/2004 */
    inline double NormL2() const;

    /** @brief Return Frobenius norm = sqrt(trace(A^t * A)). This function is deprecated, because it is equivalent to NormL2.
        @author koeser 02/2004 */
    inline double NormFrobenius() const;

    /** @author koeser */
    inline T Trace() const;

    /** @author koeser
        @warning very slow, generic implementation (order "n!"), better to use
        matrix decomposition (see BIAS/MathAlgo/Lapack.hh) */
    inline T DetSquare() const {
      BIASASSERT((*this).num_cols()==(*this).num_rows());
      const unsigned int dim = (*this).num_rows();
      const unsigned int subdim = dim-1;
      // end of recursion in 1x1:
      if (subdim==0)
        return ((*this)[0][0]);
      // end of recursion in 2x2:
      else if (subdim==1) return ((*this)[0][0]*(*this)[1][1]-
                                  (*this)[1][0]*(*this)[0][1]);
      T d = 0;
      Matrix<T> SubMatrix(subdim, subdim);
      for (register unsigned int sub=0; sub<dim; sub++) {
        // first column is used,
        // set up the n-1xn-1 submatrices from the right matrix part
        // this is done n times (loop counter="sub")
        for (register unsigned int i=0; i<subdim; i++) {
          // construct the sub matrix under inspection,
          // skip the first column
          for (register unsigned int j=0; j<subdim; j++) {
            SubMatrix[j][i] = (*this)[(sub+j+1)%dim][i+1];
          }
        }
        // add value of subdeterminant to overall sum
        d += SubMatrix.DetSquare() * (*this)[sub][0];
      }
      return d;
    }


    //@}

    //////////////////////////// Arithmetic ////////////////////
    /** @name Arithmetic
        @{
    */
    /** \brief assignment operators
        calling corresponding operator from base class if appropriate
        @author Jan Woetzel
        @status alpha (02/25/2002) */
    inline Matrix<T>& operator=(const TNT::Matrix<T> &mat);
    inline Matrix<T>& operator=(const Matrix<T> &mat);

    /** in place addition function
        @author Ingo Thomsen
        @status tested **/
    inline void AddIP(const T& scalar)
    { Add(scalar, *this); };

    /** Adds arg to this
        @author grest,2004
    */
    inline void AddIP(const Matrix<T> &arg);

    /** Subtracts arg from this <br>
        this -= arg
        @author grest, 2004
    */
    inline void SubIP(const Matrix<T> &arg);

    /** addition function, storing data destination matrix
        @author Ingo Thomsen
        @status tested **/
    inline void Add(const T& scalar,  Matrix<T> &dest) const;

    /** in place subtraction function
        @author Ingo Thomsen
        @status tested **/
    inline void SubIP(const T& scalar)
    { Sub(scalar, *this); };

    /** substraction function, storing data destination matrix
        @author Ingo Thomsen
        @status tested    **/
    inline void Sub(const T& scalar,  Matrix<T> &dest) const;

    /** in place multiplication function
        @author Ingo Thomsen
        @status tested **/
    inline void MultiplyIP(const T& scalar)
    { Multiply(scalar, *this); };

    /** multiplication function, storing data destination matrix
        @author Ingo Thomsen
        @status tested    **/
    inline void Multiply(const T& scalar,  Matrix<T> &dest) const;

    /** matrix multiplication, result is not allocated
        @author Felix Woelk */
    inline void Mult(const Matrix<T>& arg, Matrix<T>& result) const;

    /** in Place matrix multiplication
        this is equal to M = M * arg, but faster
        @author Daniel Grest */
    inline void Mult(const Matrix<T>& arg);

    /** in Place matrix multiplication
        this is equal to M = arg*M, but faster
        @author Daniel Grest */
    inline void MultLeft(const Matrix<T>& arg);

    /** matrix vector multiplication, result is not allocated
        @author Felix Woelk */
    inline void Mult(const Vector<T>& arg, Vector<T>& result) const;

    /** vector matrix multiplication result=arg*this.
        @author Marcel Lilienthal */
    inline void MultLeft(const Vector<T>& arg, Vector<T>& result) const;

    /** matrix matrix multiplication for multiplication with the transpose of
        the given matrix, result=this*arg^T.
        @author Arne Petersen */
    inline void MultiplyWithTransposeOf(const Matrix<T>& arg, Matrix<T>& result) const;

    /** in place division function
        @author Ingo Thomsen
        @status tested    **/
    inline void DivideIP(const T& scalar)
    { Divide(scalar, *this); };

    /** division function, storing data destination matrix
        @author Ingo Thomsen
        @status tested    **/
    inline void Divide(const T& scalar,  Matrix<T> &dest) const;

    /** elementwise division function in place
        @author Ingo Schiller
        @status tested    **/
    inline void DivideElementwise(const Matrix<T> &arg);

    /** @brief compute square system matrix dest = A^T * A
        @param dest holds result of this->Transpose * this

        If you want to solve A * x = b, where A has more rows than columns,
        a common technique is to solve x = (A^T * A)^-1 * A^T * b.
        This function provides a fast way to compute A^T*A from A.
        @author grest/koeser */
    inline void GetSystemMatrix(Matrix<T> &dest) const;

    //@}
    ///////////////  Set Functions //////////////////////////////////////////
    /** @name Set Functions
        @{
    */
    /** Sets all values to zero
        @author Ingo Thomsen, JW
        @date 04/11/2002
        @status tested    **/
    inline void SetZero();

    /** @brief componentwise: this = 0.5(this + this^T) yields symmetric matrix
        only allowed for square shaped matrices
        @author koeser 01/2007 */
    inline void MakeSymmetric(){
      BIASASSERT(GetCols()==GetRows());
      const int num = GetCols();
      for (int r=0; r<num; r++){
        for (int c=r; c<num; c++){
          (*this)[c][r] = (*this)[r][c] =
            T(((*this)[c][r] + (*this)[r][c]) / 2.0);
        }
      }
    }

    /// stl conform interface destroys Matrix JW
    virtual inline void clear() {
      this->destroy();
    };

    /** @brief Converts matrix to identity matrix

    Even if the matrix is not quadratic, the diagonal values
    starting with the upper left element are set to 1. Example:

    1 0 0       1 0 0 0
    0 1 0   or  0 1 0 0
    0 0 1       0 0 1 0
    0 0 0

    @author Ingo Thomsen
    **/
    void SetIdentity();

    /** Takes the elements of a Vector and put them as diagonal elements into
        a matrix. The size of this Vector must be exactly
        min( matrix.width, matrix.height). Optionally all other elements may
        be set to zero
        @author Ingo Thomsen
        @param zero_others Must be true, if all non-diagonal elements shall be
        set to zero    **/
    //void set_diagonal_elements(const Vector<T>& argV, const bool zero_others);

    void Fill(const T& scalar);

    // @}

    //////////////////// Scaling and Normalization //////////////////////////
    /** @name  Scaling and Normalization
        @{
    */
    /** divides each column of the matrix through the element norm_row_index.
        index runs [0..num_rows-1]
        for example:
        2 3
        4 5
        normalized with row '1' will be
        2/4  3/5
        1    1     **/
    void NormalizeColsToOne(const int norm_row_index);

    /** divides each row of the matrix through the element norm_col_index.
        index runs [0..num_cols-1]
        for example:
        2 3
        4 5
        normalized with row '1' will be
        2/3  1
        4/5  1      **/
    void NormalizeRowsToOne(const int norm_col_index);

    /** Normalizes each row to L2 norm one. Attention each row is normaized
        by its own scale! */
    void NormalizeRows();

    /** Normalizes each coloumn to L_2 norm one. Attention each column is
        normaized by its own scale!   */
    void NormalizeCols();

    /** Normalizes the matrix by the entry with the biggest absolute value
        by dividing all elements with this one
        @return value of the element the matrix is normalized with
        @author mdunda 09 2003 */
    T Normalize();

    /** Scales row NoRow with scale.      */
    void ScaleRow(int NoRow,T scale);

    /** Scales column NoCol with scale.     */
    void ScaleCol(int NoCol,T scale);
    // @}

    /////////////////// Input / Output ////////////////////////////////////
    /** @name Input / Output
        @{
    */
    /** method to load directly from a given filename.
        internally using stream operator
        @author Jan Woetzel 05/2003
        @return false in case of error, true in case of success
    */
    bool Load(const std::string & filename);

    /** method to save directly to a given filename.
        internally using stream operator
        @author Jan Woetzel 05/2003
        @return false in case of error, true in case of success
    */
    bool Save(const std::string & filename) const;

    /** Write the matrix in Matlab format to the given stream
        @param name Is the name of the Matlab variable
        @author streckel 08/2006
        @return negativev if error
    */
    int WriteMatlab(std::ostream& ostr, const std::string& name) const;

    /** This method reads a matrix from a given file in binary format.
        The file is not human read-/editable but provides full precision.
        A dimension check is performed.
        @return true on success
        @author mdunda 04 2004 */
    bool BinRead(const char * filename) const;

    /** This method writes a matrix to a given file in binary format.
        The file is not human read-/editable but provides full precision.
        @return true on success
        @author mdunda 04 2004 */
    bool BinWrite(const char * filename) const;

    // @}

    /** Checks if the matrix is a null matrix.
        @author mdunda 12 2003
        @return true if all elements are zero */
    bool IsZero( double eps=0.0 ) const;

    /** Checks if the matrix a an identity. I.e. all elements with index
        i==j are equal 1 and all others are zero.
        @author mdunda 12 2003
        @return true if matrix is identity */
    bool IsIdentity( double eps=0.0 ) const;

    /** vec-operator returns the elements of the matrix columwise as vector */
    void Vec(Vector<T> &dest) const;

    /** Kronecker-product with matrix, result in dest */
    void Kronecker(Matrix<T> const B, Matrix<T> &dest) const;

    /** @brief swaps two rows
        @author woelk 05/2008 www.vision-n.de */
    void SwapRows(const int i, const int r);

    /** @brief use the Gauss Jordan Algrithm to transform the matrix to
        reduced row echelon form.
        @author woelk 05/2008 www.vision-n.de */
    void GaussJordan();



  }; // class


  //////////////////////////////////////////////////////////////////
  ///  operators
  ///////////////////////////////////////////////////////////////////



  /**  @relates Matrix
       addition operator
       @author Ingo Thomsen **/
  template<class T>
  inline Matrix<T>& operator+=(Matrix<T>& mat, const T& scalar)
  { mat.AddIP(scalar); return mat; }

  /**  @relates Matrix
       addition in place operator
       @author grest **/
  template<class T>
  inline Matrix<T>& operator+=(Matrix<T>& mat, const Matrix<T>& arg)
  { mat.AddIP(arg); return mat; }

  /**  @relates Matrix
       subtracts arg from mat, mat = mat - arg
       @author grest **/
  template<class T>
  inline Matrix<T>& operator-=(Matrix<T>& mat, const Matrix<T>& arg)
  { mat.SubIP(arg); return mat; }

  /** @relates Matrix
      substraction operator
      @author Ingo Thomsen */
  template<class T>
  inline Matrix<T>& operator-=(Matrix<T>& mat, const T& scalar)
  { mat.AddIP(scalar); return mat; }

  /** @relates Matrix
      multiplication operator
      @author Ingo Thomsen */
  template<class T>
  inline Matrix<T>& operator*=(Matrix<T>& mat, const T& scalar)
  { mat.MultiplyIP(scalar); return mat; }




  /** @relates Matrix
      operator for equal
      @author Ingo Thomsen (07/02/2002)
      @status tested  */
  template<class T>
  inline bool operator==(const Matrix<T>& argl, const Matrix<T>& argr)
  {
    BIASASSERT( argl.size() == argr.size() );
    register const T* argP = argl.GetData();
    register const T* matP = argr.GetData();
    for (; matP < argr.GetDataLast() ; matP++, argP++)
      if (*matP != *argP)
        return false;
    return true;
  }

  /** @relates Matrix
      operator for not equal
      @author Ingo Thomsen (07/02/2002) **/
  template<class T>
  inline bool operator!=(const Matrix<T>& argl, const Matrix<T>& argr)
  { return !(argl==argr); }

  ////////////////////////////////////////////////////////////////////
  /// implementation
  ///////////////////////////////////////////////////////////////////

  template<class T>
  inline void Matrix<T>::Add(const T& scalar,  Matrix<T> &dest) const
  {
    if (this->num_rows() != dest.num_rows() ||
        this->num_cols() != dest.num_cols())
      dest.newsize(this->num_rows(), this->num_cols());
    register T* destP = dest.GetData();
    register const T* matP = GetData();
    for(;matP <= GetDataLast(); matP++, destP++) {
      *destP = *matP + scalar;
    }
  }

  template<class T>
  inline void Matrix<T>::Sub(const T& scalar,  Matrix<T> &dest) const
  {
    if (this->num_rows() != dest.num_rows() ||
        this->num_cols() != dest.num_cols())
      dest.newsize(this->num_rows(), this->num_cols());
    register T* destP = dest.GetData();
    register const T* matP = GetData();
    for(;matP <= GetDataLast(); matP++, destP++) {
      *destP = *matP - scalar;
    }
  }

  template<class T>
  inline void Matrix<T>::Multiply(const T& scalar,  Matrix<T> &dest) const
  {
    if (this->num_rows() != dest.num_rows() || this->num_cols() != dest.num_cols())
      dest.newsize(this->num_rows(), this->num_cols());
    register T* destP = dest.GetData();
    register const T* matP = GetData();
    for(;matP <= GetDataLast(); matP++, destP++) {
      *destP = *matP * scalar;
    }
  }

  template<class T>
  inline void Matrix<T>::Divide(const T& scalar,  Matrix<T> &dest) const
  {
    if (this->num_rows() != dest.num_rows() || this->num_cols() != dest.num_cols())
      dest.newsize(this->num_rows(), this->num_cols());
    if (scalar == 0) {
      BIASERR("Division by Zero");
      //BIASASSERT(false);
      BIASABORT;
    } else {
      register T* destP = dest.GetData();
      register const T* matP = GetData();
      for(;matP <= GetDataLast(); matP++, destP++) {
        *destP = *matP / scalar;
      }
    }
  }
  
  template<class T>
  inline void Matrix<T>::DivideElementwise(const Matrix<T> &arg) 
  {
    if (this->num_rows() != arg.num_rows() || this->num_cols() != arg.num_cols()){
      BIASERR("Unequal matrix dimensions");
      return;
    }
    register const T* argP = arg.GetData();
    register T* matP = GetData();
    for(;matP <= GetDataLast(); matP++, argP++) {
      *matP = *matP / *argP;
    }    
  }

  template<class T>
  inline Matrix<T> Matrix<T>::Transpose() const
  {
    Matrix<T> Out(this->num_cols(),this->num_rows());
    for (int i=0; i<this->num_rows(); i++)
      for (int j=0; j<this->num_cols(); j++)
        Out[j][i]= (*this)[i][j];
    return Out;
  }

  template<class T>
  inline Matrix<T> Matrix<T>::Adjoint() const
  {
    unsigned int num_rows = this->num_rows();
    unsigned int num_cols = this->num_cols();
    BIASASSERT (num_rows==num_cols);
    Matrix<T> Out(num_cols,num_rows);
    Matrix<T> SubMatrix(num_rows-1,num_cols-1);
    for (unsigned int i=0; i<num_rows; i++) {
      for (unsigned int j=0; j<num_cols; j++) {
        for (unsigned int k=0; k<num_rows-1; k++) {
          unsigned int kk = k<i?k:k+1;
          for (unsigned int r=0; r<num_rows-1; r++) {
            unsigned int rr = r<j?r:r+1;
            SubMatrix[k][r] = (*this)[kk][rr];
          }
        }
        Out[j][i]= (((i+j)%2==0)?1:-1) * SubMatrix.DetSquare();
      }
    }
    return Out;
  }

  template<class T>
  inline void Matrix<T>::SetZero()
  {
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
    for(register T* dataP = GetData(); dataP <= GetDataLast(); dataP++)
      *dataP = (T)(0);
#else
    memset(GetData(), 0, GetNumElements()*sizeof(T)); // JW
#endif
  }

  template<class T>
  inline T Matrix<T>::NormL1() const
  {
    T result = 0;
    for(register const T* dataP = GetDataLast(); dataP >= GetData(); dataP--) {
      result += *dataP > 0 ? *dataP : *dataP * -1;
    }
    return result;
  }


  template<class T>
  inline double Matrix<T>::NormL2() const
  {
    double result = 0;
    for (register const T* dataP = GetDataLast(); dataP >= GetData(); dataP--){
      result += (double)((*dataP) * (*dataP));
    }
    return sqrt(result);
  }
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
  template<>
  inline double Matrix<BIAS::Polynom>::NormL2() const {
    BIASERR("NormL2 is only pollsible for integral types");
    BIASABORT;
    return 0;
  }
#endif

/** this method is deprecated because it is equivalent to NormL2 */
  template<class T>
  inline double Matrix<T>::NormFrobenius() const
  {
    return NormL2();
  }

  template<class T>
  inline T Matrix<T>::Trace() const
  {
    BIASASSERT(this->num_cols() == this->num_rows())
      T result = 0;
    for (int x=0; x<this->num_rows(); x++)
      result = result + (*this)[x][x];
    return result;
  }

  template<class T>
  inline void Matrix<T>::Mult(const Matrix<T>& arg, Matrix<T>& result) const
  {
    if (result.num_cols() != arg.num_cols() ||
        result.num_rows() != this->num_rows()){
      result.newsize(this->num_rows(), arg.num_cols());
    }

    result.SetZero();
    register int x, y, i;
    for (x=0; x<result.num_cols(); x++)
      for (y=0; y<result.num_rows(); y++)
        for (i=0; i<this->num_cols(); i++)
          result[y][x]+=(*this)[y][i]*arg[i][x];
  }

  template<class T>
  inline void Matrix<T>::Mult(const Matrix<T>& arg)
  {
    Matrix<T> result(this->num_rows(),arg.num_cols());
    Mult(arg,result);
    (*this)=result;
  }

  template<class T>
  inline void Matrix<T>::MultLeft(const Matrix<T>& arg)
  {
    Matrix<T> result(arg.num_rows(),this->num_cols());
    arg.Mult(*this,result);
    (*this)=result;
  }

  template<class T>
  inline void Matrix<T>::Mult(const Vector<T>& arg, Vector<T>& result) const
  {
    if (result.size() != this->num_rows()) {
        result.newsize(this->num_rows());
    }


    result.Fill((T)0);
    register int y, i;
    for (y=0; y<result.size(); y++)
      for (i=0; i<this->num_cols(); i++)
        result[y]+=(*this)[y][i]*arg[i];
  }

  template<class T>
  inline void Matrix<T>::MultLeft(const Vector<T>& arg,
                                  Vector<T>& result) const
  {
    if (result.size() != this->num_cols()) {
        result.newsize(this->num_cols());
    }

    if (result.size() != this->num_cols()) {
      result.newsize(this->num_cols());
    }
    result.SetZero();
    register int y, i;
    for (y=0; y<this->num_cols(); y++)
      for (i=0; i<this->num_rows(); i++)
        result[y]+=(*this)[i][y]*arg[i];

  }

  template<class T>
  inline void Matrix<T>::MultiplyWithTransposeOf(const Matrix<T>& arg, Matrix<T>& result) const
  {
    if (result.num_cols() != arg.num_rows() ||
        result.num_rows() != this->num_rows()){
      result.newsize(this->num_rows(), arg.num_rows());

    }

    result.SetZero();
    register int x, y, i;
    for (x=0; x<result.num_cols(); x++)
      for (y=0; y<result.num_rows(); y++)
        for (i=0; i<this->num_cols(); i++)
          result[y][x]+=(*this)[y][i]*arg[x][i];
  }

  template<class T>
  inline void Matrix<T>::AddIP(const Matrix<T> &arg)
  {

    T* thisData=GetData();
    const T* argData=arg.GetData();
    register unsigned int size=this->num_rows()*this->num_cols();
    for (unsigned int i=0;i<size;i++) {
      *thisData = *thisData + T(*argData);
      thisData++; argData++;
    }
  }

  template<class T>
  inline void Matrix<T>::SubIP(const Matrix<T> &arg)
  {

    T* thisData=GetData();
    const T* argData=arg.GetData();
    register unsigned int size=this->num_rows()*this->num_cols();
    for (unsigned int i=0;i<size;i++) {
      *thisData = *thisData  - T(*argData);
      thisData++; argData++;
    }
  }

  template <class T>
  inline void Matrix<T>::GetSystemMatrix(Matrix<T> &dest) const
  {
    const unsigned int colsJ = this->num_cols(), rowsJ = this->num_rows();
    // resize result to square matrix
    dest.newsize(colsJ, colsJ);
    dest.SetZero();
    /// Hessian is symmetric! so first diagonal then lower left
    // diagonal
    for (unsigned int col = 0; col<colsJ; col++) {
      for (unsigned int k = 0; k<rowsJ; k++) {
        dest[col][col]+=this->row_[k][col]*this->row_[k][col];
      }
    }
    // lower left (which is equal to transposed upper right)
    for (unsigned int r=1; r<colsJ; r++) {
      for (unsigned int c=0; c<r; c++) {
        for (unsigned int k = 0; k<rowsJ; k++) {
          const T* pJ_k_ = this->row_[k];
          dest[r][c] += pJ_k_[r] * pJ_k_[c];
        }
        dest[c][r]=dest[r][c]; // symmetry of Hessian!
      }
    }
  }

  template <class T>
  inline Matrix<T> &Matrix<T>::operator=(const TNT::Matrix<T> &mat)
  {
    TNT::Matrix<T>::operator=(mat);	// call the operator of the base class
    return *this;
  }

  template <class T>
  inline Matrix<T> &Matrix<T>::operator=(const Matrix<T> &mat)
  {
    TNT::Matrix<T>::operator=(mat);	// call the operator of the base class
    return *this;
  }

} // namespace BIAS

#include <Base/Math/Operators.hh>

#endif // _Matrix_hh_
