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

#ifndef _Matrix2x2_hh_
#define _Matrix2x2_hh_
#include "bias_config.h" 

#include <Base/Debug/Error.hh>
  //#include <Base/Debug/Debug.hh>

#include "Matrix.hh"


namespace BIAS {

/**
    @class Matrix2x2
    @ingroup g_math
    @brief is a 'fixed size' quadratic matrix of dim. 
    2 x 2 which is templated over the elemnt-type.
**/

  // forward declaration to avoid nested includes
  template <class T> class BIASMathBase_EXPORT Vector2;

  template<class T=double>
  class BIASMathBase_EXPORT Matrix2x2 : public Matrix<T> 
  {
  public:
    inline Matrix2x2<T>() : Matrix<T>(2, 2) {};
    
    explicit Matrix2x2<T>(const MatrixInitType& i) : Matrix<T>(2, 2, T(0)) {
      switch (i) {
      case MatrixZero:return;
      case MatrixIdentity:{ 
        this->v_[0] = this->v_[3] = T(1); return;
      }
      default: 
        BIASERR("undefined MatrixInitType"); 
        BIASABORT;
      }
    }

    // DEPRECATED due to instantiation problem JW
    //explicit Matrix2x2<T>(const char *s) : Matrix<T>(2, 2, s) {};
    explicit Matrix2x2<T>(const std::string & s);


    /** @author Jan Woetzel
        @status untested (04/17/2002) **/
    Matrix2x2<T>(const Matrix2x2<T> & A) : Matrix<T>(A) {};

    /** cast constructor from TNT::Matrix */
    Matrix2x2<T>(const TNT::Matrix<T>& A); 

    /** Set the matrix to | a0 a1 |
                          | a2 a3 | */
    Matrix2x2<T>(
      const T a0, const T a1, 
      const T a2, const T a3);

    virtual ~Matrix2x2<T>();

    /** matrix - vector multiplicate this matrix with Vector2, 
        storing the result in destvec
        calculates:
        destvec = (this Matrix) * argvec
    **/
    inline void Mult(const Vector2<T> &argvec, Vector2<T> &destvec) const;
    
    /** matrix - vector multiplicate this matrix with Vector2, 
        storing the result in destvec
        calculates:
        destvec^T = argvec^T * (this Matrix)
    **/
    inline void MultLeft(const Vector2<T> &argvec, Vector2<T> &destvec) const;

    /** @brief transposed argvec multipied from left, wrapper function */
    Vector2<T> MultLeft(const Vector2<T> &argvec) const {
      Vector2<T> ret; MultLeft(argvec, ret); return ret; 
    }


    /** matrix-matrix multiplication with other Matrix2x2, 
        storing the result in destmat
        calculates:
        destmat = (this mat) * argmat
    **/
    inline void Mult(const Matrix2x2<T> & argmat, Matrix2x2<T> & destmat)const;
    
    /** Multiplication in place with argmat */
    inline void MultIP(const Matrix2x2<T> & argmat);

    /** assignment operator    **/
    Matrix2x2<T>& operator=(const Matrix2x2<T> &mat);

    /** returns transposed matrix*/
    inline Matrix2x2<T> Transpose() const;
 
    /** set this as transposed arg */
    inline void Transpose(const Matrix2x2<T>& arg);
   
    /** transpose matrix in place */
    inline void TransposeIP();

    /** set the elements of this matrix to the identity matrix **/
    inline void SetIdentity();

    /** set the elements of this matrix to zero  **/
    inline void SetZero();

    /// stl conform interface
    inline void clear() {
      SetZero();
    };


    /** calculate the determinante */
    inline T det() const;

    /** just neccessary to avoid resizing of this 'fixed size' matrix because
        it is derived from the resizable Matrix
        should be removed when Matrix2x2 becomes a base class.
        (04/17/2002) Felix Woelk **/
    Matrix2x2 & newsize(int rows, int cols);

    /** analyticaly inverts matrix
        @author woelk 12 2002 */
    int Invert(Matrix2x2<T>& result) const;

    inline Matrix2x2<T> Invert() const
    { Matrix2x2<T> res; Invert(res); return res; }

    /** Eigenvalue decomposition. The eigenvalues and eigenvectors of the 
        matrix are returned in value1/2 vector1/2. 
        @return number of linear independent eigenvectors = rank of matrix
        @author woelk 06/2005 */
    int EigenvalueDecomposition(T& value1, Vector2<T>& vector1, 
                                T& value2, Vector2<T>& vector2) const;
  }; // class
  
  template <class T>
  inline void Matrix2x2<T>::Transpose(const Matrix2x2<T>& arg) 
  {
    register T *dataP = this->GetData();
    register const T *argP = arg.GetData();
    dataP[0] = argP[0]; dataP[3] = argP[3];
    dataP[1] = argP[2]; dataP[2] = argP[1]; 
  }
  
  template <class T>
  inline Matrix2x2<T> Matrix2x2<T>::Transpose() const
  {
    Matrix2x2 rs;
    rs.Transpose(*this);
    return rs;
  }
  
  template <class T>
  inline void Matrix2x2<T>::TransposeIP()
  {
    T tmp = this->v_[1];
    this->v_[1] = this->v_[2];
    this->v_[2] = tmp;
  }

  template <class T>
  inline void Matrix2x2<T>::Mult(const Matrix2x2<T> & argmat, 
                                 Matrix2x2<T> & destmat) const
  {
    register T *destP 	= destmat.GetData(); 
    const register T *argP	= argmat.GetData(); 
    const register T *matP	= this->GetData(); 

    destP[0] =
      matP[0] * argP[0] + matP[1] * argP[2];
    destP[1] =
      matP[0] * argP[1] + matP[1] * argP[3];

    destP[2] =
      matP[2] * argP[0] + matP[3] * argP[2];
    destP[3] =
      matP[2] * argP[1] + matP[3] * argP[3];
  
  }

  template <class T>
  inline void Matrix2x2<T>::MultIP(const Matrix2x2<T> & argmat) 
  {
    const register T *argP = argmat.GetData(); 
    register T tmp = this->v_[0];

    this->v_[0] =
      tmp * argP[0] + this->v_[1] * argP[2];
    this->v_[1] =
      tmp * argP[1] + this->v_[1] * argP[3];

    tmp = this->v_[2];
    this->v_[2] =
      this->v_[2] * argP[0] + this->v_[3] * argP[2];
    this->v_[3] =
      tmp * argP[1] + this->v_[3] * argP[3];
  
  }

  template <class T>
  inline void Matrix2x2<T>::Mult(const Vector2<T> &argvec, 
                                 Vector2<T> &destvec) const
  {
    const register T *matP = this->GetData();	
    destvec[0] = argvec[0] * matP[0]
      + argvec[1] * matP[1];
    destvec[1] = argvec[0] * matP[2]
      + argvec[1] * matP[3];
  } 

  template <class T>
  inline void Matrix2x2<T>::MultLeft(const Vector2<T> &argvec, 
                                     Vector2<T> &destvec) const
  {
    const register T *matP = this->GetData();	
    destvec[0] = argvec[0] * matP[0]
      + argvec[1] * matP[2];
    destvec[1] = argvec[0] * matP[1]
      + argvec[1] * matP[3];
  }


  template <class T>
    inline void Matrix2x2<T>::SetIdentity() {
      register T* d = this->GetData(); // pointer to this matrix's data array
      d[0] = d[3] = 1;
      d[1] = d[2] = 0;
    }

  template <class T>
    inline void Matrix2x2<T>::SetZero() {
      register T* d = this->GetData(); // pointer to this matrix's data array
      d[0]=d[1]=d[2]=d[3] =0;
    }



  template <class T>
  inline T Matrix2x2<T>::det() const
  {
    register const T *dataP = this->GetData(); 
    return (dataP[0] * dataP[3] - dataP[1] * dataP[2]);
  }


} // end of namespace BIAS

#include "Operators.hh"

#endif // _Matrix2x2_hh_
