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

#ifndef _Matrix3x2_hh_
#define _Matrix3x2_hh_
#include "bias_config.h" 

#include <Base/Debug/Error.hh>

#include "Matrix.hh"
#include "Matrix2x3.hh"
#include "Matrix2x2.hh"
#include "Vector2.hh"
#include "Vector3.hh"

namespace BIAS {

/**
    @class Matrix3x2
    @ingroup g_math
    @brief is a 'fixed size'
**/

  template<class T=double>
  class BIASMathBase_EXPORT Matrix3x2 : public Matrix<T> 
  {
  public:
    inline Matrix3x2<T>() : Matrix<T>(3, 2) {};
    
    explicit Matrix3x2<T>(const MatrixInitType& i) : Matrix<T>(3, 2, T(0)) {
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

    /** @author apetersen 2/3/2012 **/
    Matrix3x2<T>(const Matrix3x2<T> & A) : Matrix<T>(A) {};

    /** cast constructor from TNT::Matrix */
    Matrix3x2<T>(const TNT::Matrix<T>& A) : TNT::Matrix<T>(A) {};

    /** Set the matrix to | a0 a1 |
                          | a2 a3 |
                          | a4 a5 | */
    Matrix3x2<T>(
      const T a0, const T a1,
      const T a2, const T a3,
      const T a4, const T a5);

    virtual ~Matrix3x2<T>(){};

    /** matrix - vector multiplicate this matrix with Vector3, 
        storing the result in destvec
        calculates:
        destvec = (this Matrix) * argvec
    **/
    inline void Mult(const Vector2<T> &argvec, Vector3<T> &destvec) const;
    
    /** matrix - vector multiplicate this matrix with Vector2, 
        storing the result in destvec
        calculates:
        destvec^T = argvec^T * (this Matrix)
    **/
    inline void MultLeft(const Vector3<T> &argvec, Vector2<T> &destvec) const;

    /** @brief transposed argvec multipied from left, wrapper function */
    Vector2<T> MultLeft(const Vector3<T> &argvec) const {
      Vector2<T> ret; MultLeft(argvec, ret); return ret; 
    }

    /** matrix-matrix multiplication with Matrix2x2, 
        storing the result in destmat
        calculates:
        destmat = (this mat) * argmat
    **/
    inline void Mult(const Matrix2x2<T> & argmat, Matrix3x2<T> & destmat)const;
    
    /** assignment operator    **/
    Matrix3x2<T>& operator=(const Matrix3x2<T> &mat);

    /** returns transposed matrix*/
    inline Matrix2x3<T> Transpose() const;
 
    /** set the elements of this matrix to the identity matrix **/
    inline void SetIdentity();

    /** set the elements of this matrix to zero  **/
    inline void SetZero();

    /// stl conform interface
    inline void clear() {
      SetZero();
    };

    /** Fixed size matrix, resize not allowed **/
    Matrix3x2 & newsize(int rows, int cols)
    { BIASERR("CANNOT RESIZE FIXED SIZE MATRIX!!!");
      BIASABORT; };
  }; // class
  

  template <class T>
  Matrix3x2<T>::Matrix3x2(const T a0, const T a1,
                          const T a2, const T a3,
                          const T a4, const T a5)
  { this->v_[0] = a0; this->v_[1] = a1;
    this->v_[2] = a2; this->v_[3] = a3;
    this->v_[4] = a4; this->v_[5] = a5; }

  template <class T>
  Matrix3x2<T>& Matrix3x2<T>::operator=(const Matrix3x2<T> &mat)
  {
    register T *vP = this->v_;
    register const T *matP = mat.GetData();

    vP[0] = matP[0]; vP[1] = matP[1];
    vP[2] = matP[2]; vP[3] = matP[3];
    vP[4] = matP[4]; vP[5] = matP[5];

    return *this;
  }

  template <class T>
  inline Matrix2x3<T> Matrix3x2<T>::Transpose() const
  {
    Matrix2x3<T> res;

    register const T *dataP = this->GetData();
    register T *resP = res.GetData();
    resP[0] = dataP[0]; resP[1] = dataP[2]; resP[2] = dataP[4];
    resP[3] = dataP[1]; resP[4] = dataP[3]; resP[5] = dataP[5];

    return res;
  }
  
  template <class T>
  inline void Matrix3x2<T>::Mult(const Matrix2x2<T> & argmat, 
                                 Matrix3x2<T> & destmat) const
  {
    register T *destP = destmat.GetData(); 
    const register T *argP = argmat.GetData(); 
    const register T *matP = this->GetData(); 

    destP[0] = matP[0] * argP[0] + matP[1] * argP[2];
    destP[1] = matP[0] * argP[1] + matP[1] * argP[3];
    destP[2] = matP[2] * argP[0] + matP[3] * argP[2];
    destP[3] = matP[2] * argP[1] + matP[3] * argP[3];
    destP[4] = matP[4] * argP[0] + matP[5] * argP[2];
    destP[5] = matP[4] * argP[1] + matP[5] * argP[3];
  
  }

  template <class T>
  inline void Matrix3x2<T>::Mult(const Vector2<T> &argvec, 
                                 Vector3<T> &destvec) const
  {
    const register T *matP = this->GetData();
    destvec[0] = matP[0] * argvec[0] + matP[1] * argvec[1];
    destvec[1] = matP[2] * argvec[0] + matP[3] * argvec[1];
    destvec[2] = matP[4] * argvec[0] + matP[5] * argvec[1];
  }

  template <class T>
  inline void Matrix3x2<T>::MultLeft(const Vector3<T> &argvec, 
                                     Vector2<T> &destvec) const
  {
    const register T *matP = this->GetData();	
    destvec[0] = argvec[0] * matP[0] + argvec[1] * matP[2] + argvec[2] * matP[4];
    destvec[1] = argvec[0] * matP[1] + argvec[1] * matP[3] + argvec[2] * matP[5];
  }


  template <class T>
    inline void Matrix3x2<T>::SetIdentity() {
      register T* d = this->GetData(); // pointer to this matrix's data array
      d[0] = d[3] = 1;
      d[1] = d[2] = d[4] = d[5] = 0;
    }

  template <class T>
    inline void Matrix3x2<T>::SetZero() {
      register T* d = this->GetData(); // pointer to this matrix's data array
      d[0]=d[1]=d[2]=d[3]=d[4]=d[5] =0;
    }

} // end of namespace BIAS

#include "Operators.hh"

#endif // _Matrix3x2_hh_
