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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#ifndef _Matrix3x3_hh_
#define _Matrix3x3_hh_
#include "bias_config.h" 

#include <Base/Debug/Error.hh>

#include <Base/Math/Vector3.hh>

#include <iostream>
#include <string>
#include <limits>

namespace BIAS {

  template <class T> class Matrix;
  template <class T> class Matrix3x3;
  template <class T> class Matrix3x4;

  template <class T> 
  std::ostream& operator<<(std::ostream& os, const Matrix3x3<T>& mat);

  template <class T> 
  std::istream& operator>>(std::istream& is, Matrix3x3<T>& mat);

  /** @class Matrix3x3
      @ingroup g_math
      @brief is a 'fixed size' quadratic matrix of dim. 3 x 3 which is 
      templated over the element-type.
      
      complete rewrite of class with fixed internal memory, to avoid 
      calling of allocator new[]

      @author woelk 11/2007 (c) www.vision-n.de */
  template <class T=double>
  class BIASMathBase_EXPORT Matrix3x3 
  {
  public:
    /**	default constructor */
    inline Matrix3x3<T>() {}

    //explicit Matrix3x3<T>(const std::string & s);

    /// constructor from 9 values
    Matrix3x3<T>(const T a0, const T a1, const T a2, 
                 const T a3, const T a4, const T a5,
                 const T a6, const T a7, const T a8);

    /** @brief init matrix with identity or zero 
        @author koeser */
    explicit Matrix3x3<T>(const MatrixInitType& i);

    /** copy constructor **/
    Matrix3x3<T>(const Matrix3x3<T>& A);

    /** cast constructor from Matrix */
    Matrix3x3<T>(const Matrix<T>& A);

    /** cast constructor from TNT::Matrix */
    Matrix3x3<T>(const TNT::Matrix<T>& A);

    /** destructor */
    virtual ~Matrix3x3<T>();
    
    /** assignment operator
        @author woelk 11/2007 (c) www.vision-n.de **/
    Matrix3x3<T>& operator=(const Matrix3x3<T>& mat);

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline T* operator[](const unsigned row)
    { return Data_+3*row; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline const T* operator[](const unsigned row) const
    { return Data_+3*row; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline T* GetData()
    { return Data_; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline const T* GetData() const
    { return Data_; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline unsigned GetNumElements() const
    { return 9u; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline void SetZero();

    /** set the elements of this matrix to the identity matrix
        (possibly overriding the inherited method)
        @author Ingo Thomsen, Jan Woetzel
        @date 04/17/2002
        @status untested **/
    inline void SetIdentity();

    /** matrix - vector multiplicate this matrix with Vector3, 
        storing the result in destvec
        calculates:
        destvec = (this Matrix) * argvec
        @author Ingo Thomsen, Jan Woetzel
        @status untested (04/17/2002)**/
    inline void Mult(const Vector3<T> &argvec, Vector3<T> &destvec) const;

    /** multiplies matrix from left with transposed argvec, resulting
        in transposed destvec
        @author woelk 05/2003 */
    inline 
    void TransposedMult(const Vector3<T> &argvec, Vector3<T> &destvec) const;

    /** matrix-matrix multiplication with other Matrix3x3, 
        storing the result in destmat
        calculates:
        destmat = (this mat) * argmat
        @author Ingo Thomsen, Jan Woetzel
        @status untested (04/17/2002)**/
    inline void Mult(const Matrix3x3<T> & argmat,Matrix3x3<T> & destmat) const;

    /// woelk 11/2007 (c) www.vision-n.de
    inline Matrix3x3<T>& operator*=(const Matrix3x3<T> & arg)
    { Matrix3x3<T> tmp(*this); tmp.Mult(arg, *this); return *this; } 

    /** @author woelk 06 2003 */
    void Mult(const Matrix3x4<T> & argmat,Matrix3x4<T> & destmat) const;

    /** scalar-matrix multiplication */
    inline void Scale(const T &scalar, Matrix3x3<T> & destmat) const;

    /// woelk 11/2007 (c) www.vision-n.de
    Matrix3x3<T>& operator*=(const T& arg);

    /// @author woelk 11/2007 (c) www.vision-n.de
    Matrix3x3<T>& operator/=(const T& arg);

    /// @author woelk 11/2007 (c) www.vision-n.de
    Matrix3x3<T>& operator+=(const Matrix3x3<T>& arg);

    /// @author woelk 11/2007 (c) www.vision-n.de
    Matrix3x3<T>& operator-=(const Matrix3x3<T>& arg);

    /**	tranpose this matrix "in place"
        example: 
        0 1 2  -->  0 3 6
        3 4 5  -->  1 4 7
        6 7 8  -->  2 5 8 */
    inline void TransposeIP();

    /** returns transposed matrix
        tested 12.06.2002
        @author Felix Woelk  */
    Matrix3x3<T> Transpose() const;

    /** sets this as transposed arg,  fw */
    inline void Transpose(const Matrix3x3<T>& arg); 

    /** sets the diagonalelements of this 3x3 Matrix
        rowwise with the values of the 9 (x1) vector
        @author Ingo Thomsen, Jan Woetzel **/
    void SetFromVector(const TNT::Vector<T> & vec);
    
    /** set this matrix from 3 vectors each representating a column*/
    void SetFromColumnVectors(const BIAS::Vector3<T> & v0,
                              const BIAS::Vector3<T> & v1,
                              const BIAS::Vector3<T> & v2);
                              
    /** set this matrix from 3 vectors, each representating a row */  
    void SetFromRowVectors(const BIAS::Vector3<T> & v0,
                           const BIAS::Vector3<T> & v1,
                           const BIAS::Vector3<T> & v2);
                           
    /** extract one row ('Zeile') from ths matrix (for convenience) */
    void GetRow(const unsigned int row, Vector3<T>& r) const;
    inline Vector3<T> GetRow(const unsigned int row) const
    {  BIAS::Vector3<T> m; GetRow(row, m); return m; }

    /** extract one column ('Spalte') from this matrix (for convenience) */
    void GetColumn(const unsigned int col, Vector3<T>& r) const;

    BIAS::Vector3<T> GetColumn(const unsigned int col) const
    {  BIAS::Vector3<T> m; GetColumn(col, m); return m; }

    /// @author woelk 11/2007 (c) www.vision-n.de
    void SetColumn(const unsigned int col, const Vector3<T>& c);

    /// @author woelk 11/2007 (c) www.vision-n.de
    void SetRow(const unsigned int row, const Vector3<T>& r);

    /** Sets matrix from vector as cross product matrix of this vector. */
    void SetAsCrossProductMatrix(const Vector3<T> &vec);

    /** Sets matrix from vector as cross product matrix of a vector. 
        with components (x, y, z) */
    void SetAsCrossProductMatrix(const T &x, const T &y, const T &z);

    /** In place matrix conversion. 
        Returns -1 if determinant is zero, 0 on success. 
        @author woelk 07/2005 */
    int InvertIP();

    /** Matrix inversion: inverts this and stores resulty in argument inv.
        Returns -1 if determinant is zero, 0 on success.
        @author woelk 07/2005 */
    int GetInverse(Matrix3x3<T>& inv) const;
    
    /** return the trace of the matrix
        @author woelk 04/2006 */
    inline T Trace() const
    { const register T *p = this->GetData();	return p[0] + p[4] + p[8]; }

    /** returns the Determinant |A| of this
        @author grest  */
    T GetDeterminant() const;

    /** return biggest and smallest entry
        @author woelk 08/2004 */
    void GetMaxMin(T& max, T&min) const;

    /// @author woelk 11/2007 (c) www.vision-n.de
    void GetAbsMaxMin(T& max, T&min) const;

    /// @author woelk 11/2007 (c) www.vision-n.de
    T GetMax() const;

    /// @author woelk 11/2007 (c) www.vision-n.de
    T GetMin() const;

    inline T NormL1() const;

    /// @author woelk 11/2007 (c) www.vision-n.de
    inline double NormFrobenius() const;

    inline double NormL2() const
    { return NormFrobenius(); }

    /** divide this by biggest absolute entry,
        returns biggest entry
        @author woelk 11/2007 (c) www.vision-n.de */
    T Normalize();

    /// @author woelk 11/2007 (c) www.vision-n.de
    void MakeSymmetric();

    /// @author woelk 11/2007 (c) www.vision-n.de
    bool IsZero(const T eps = std::numeric_limits<T>::epsilon()) const;
    
    /// @author woelk 12/2007 (c) www.vision-n.de
    bool IsIdentity(const T eps = std::numeric_limits<T>::epsilon()) const;

    bool operator==(const Matrix3x3<T>& arg) const;

    bool operator!=(const Matrix3x3<T>& arg) const
    { return !operator==(arg); }
      
    bool Load(const std::string& fname);

    bool Save(const std::string& fname) const;

  protected:
    T Data_[9];

  }; // class Matrix3x3 --------------------------------------


  ////////////////////////////////////////////////////////////////
  // inline implementations
  ////////////////////////////////////////////////////////////////
  

  template <class T>
  void Matrix3x3<T>::
  SetZero()
  {
    memset((void *)Data_, 0, 9*sizeof(T));
  }


  template <class T>
  void Matrix3x3<T>::
  Mult(const Vector3<T> &argvec, Vector3<T> &destvec) const
  {
    // direct access to the matrix values using fixed indicies:
    // 0  1  2       0
    // 3  4  5   *   1
    // 6  7  8       2
    const register T *matP = this->GetData();	
    destvec[0] = argvec[0] * matP[0]
      + argvec[1] * matP[1]
      + argvec[2] * matP[2];
    destvec[1] = argvec[0] * matP[3]
      + argvec[1] * matP[4]
      + argvec[2] * matP[5];
    destvec[2] = argvec[0] * matP[6]
      + argvec[1] * matP[7]
      + argvec[2] * matP[8];
  }


  template <class T> 
  void Matrix3x3<T>::
  TransposedMult(const Vector3<T> &argvec, Vector3<T> &destvec) const
  {
    // direct access to the matrix values using fixed indicies:
    // 0  1  2       0
    // 3  4  5   *   1
    // 6  7  8       2    
    const register T *matP = this->GetData();	
    destvec[0] = argvec[0] * matP[0]
      + argvec[1] * matP[3]
      + argvec[2] * matP[6];
    destvec[1] = argvec[0] * matP[1]
      + argvec[1] * matP[4]
      + argvec[2] * matP[7];
    destvec[2] = argvec[0] * matP[2]
      + argvec[1] * matP[5]
      + argvec[2] * matP[8];
  }


  template <class T> 
  void Matrix3x3<T>::
  Mult(const Matrix3x3<T> &argmat, Matrix3x3<T> &destmat) const
  {
    //
    // direct access using fixed indicies with matrix order:
    //  0 1 2
    //  3 4 5
    //  6 7 8
    register T *destP 	= destmat.GetData(); 
    const register T *argP	= argmat.GetData(); 
    const register T *matP	= this->GetData(); 
    // does not work for in place multiplication
    BIASASSERT(destP!=argP);
    BIASASSERT(destP!=matP);

    destP[0] = matP[0] * argP[0] + matP[1] * argP[3] + matP[2] * argP[6];
    destP[1] = matP[0] * argP[1] + matP[1] * argP[4] + matP[2] * argP[7];
    destP[2] = matP[0] * argP[2] + matP[1] * argP[5] + matP[2] * argP[8];

    destP[3] = matP[3] * argP[0] + matP[4] * argP[3] + matP[5] * argP[6];
    destP[4] = matP[3] * argP[1] + matP[4] * argP[4] + matP[5] * argP[7];
    destP[5] = matP[3] * argP[2] + matP[4] * argP[5] + matP[5] * argP[8];

    destP[6] = matP[6] * argP[0] + matP[7] * argP[3] + matP[8] * argP[6];
    destP[7] = matP[6] * argP[1] + matP[7] * argP[4] + matP[8] * argP[7];
    destP[8] = matP[6] * argP[2] + matP[7] * argP[5] + matP[8] * argP[8];
  }


  template <class T> 
  void Matrix3x3<T>::
  Scale(const  T &scalar, Matrix3x3<T> &destmat) const
  {
    register T *dest = destmat.GetData();
    const register T *src	= this->GetData(); 
    for (int i=0; i<9; i++)
      dest[i] = scalar * src[i];
  }


  template <class T> 
  void Matrix3x3<T>::
  TransposeIP()
  {
    // swap the 1,2,5 with the 3,6,7
    // 0  1  2
    // 3  4  5
    // 6  7  8
    register T *dataP = this->GetData();	// pointer to the data array
    register T tmp;
    // swap 1,3:
    tmp = dataP[1];
    dataP[1] = dataP[3];
    dataP[3] = tmp;
    // swap 2,6:
    tmp = dataP[2];
    dataP[2] = dataP[6];
    dataP[6] = tmp;
    // swap 5,7:
    tmp = dataP[5];
    dataP[5] = dataP[7];
    dataP[7] = tmp;
  }
   

  template <class T>
  void Matrix3x3<T>::
  Transpose(const Matrix3x3<T>& arg)
  {
    register const T *argp = arg.GetData();
    this->Data_[0] = argp[0]; 
    this->Data_[4] = argp[4]; 
    this->Data_[8] = argp[8];
    
    this->Data_[1] = argp[3]; 
    this->Data_[2] = argp[6]; 
    this->Data_[3] = argp[1];

    this->Data_[5] = argp[7]; 
    this->Data_[6] = argp[2]; 
    this->Data_[7] = argp[5];
  }


  template <class T> 
  void  Matrix3x3<T>::
  SetIdentity()
  {
    register T* d = this->GetData(); // pointer to this matrix's data array
    d[0] = 1;
    d[1] = 0;
    d[2] = 0;
    d[3] = 0;
    d[4] = 1;
    d[5] = 0;
    d[6] = 0;
    d[7] = 0;
    d[8] = 1;
  }


  template<class T>
  double Matrix3x3<T>::
  NormFrobenius() const 
  {
    double result = 0;
    for (int i=0; i<9; i++){
      result += (double)(Data_[i]) * (double)(Data_[i]);
    }
    return sqrt(result);
  }

  template<class T>
  T Matrix3x3<T>::
  NormL1() const 
  {
    T result = 0;
    for (int i=0; i<9; i++){
      result += (Data_[i]>0)?(Data_[i]):(-(Data_[i]));
    }
    return result;
  }



  /////////////////////////////////////////////////////////////////////////
  // non member functions
  /////////////////////////////////////////////////////////////////////////

  /** @relates Matrix3x3 */
  template<class T>
  Matrix3x3<T> operator/(const Matrix3x3<T>& mat, const T& arg)
  { Matrix3x3<T> res(mat); res/=arg; return res; }

  /** @relates Matrix3x3 */
  template<class T>
  Matrix3x3<T> operator*(const Matrix3x3<T>& mat, const T& arg)
  { Matrix3x3<T> res(mat); res*=arg; return res; }
  
  /** @relates Matrix3x3 */
  template<class T>
  Matrix3x3<T> operator*(const T& arg, const Matrix3x3<T>& mat)
  { Matrix3x3<T> res(mat); res*=arg; return res; }
  

} // end of namespace BIAS


#include "Operators.hh"

#endif

