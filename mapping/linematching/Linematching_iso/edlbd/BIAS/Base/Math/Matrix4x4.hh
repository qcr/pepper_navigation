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

#ifndef _BIAS_Matrix4x4_hh_
#define _BIAS_Matrix4x4_hh_
#include "bias_config.h" 

#include <Base/Debug/Error.hh>

#include "Matrix.hh"
#include "Matrix3x3.hh"
#include "Vector3.hh"
#include "Vector4.hh"


namespace BIAS {

  /**
  @class Matrix4x4
  @ingroup g_math
  @brief is a 'fixed size' quadratic matrix of dim. 4 x 4 which is templated
  over the element-type

  @author Jan Woetzel,  (04/17/2002) 
  **/

  // forward declaration to avoid mutual inclusion
  //template <class T> class BIASMathBase_EXPORT Matrix3x3;
  //template <class T> class BIASMathBase_EXPORT Vector4;
  //template <class T> class BIASMathBase_EXPORT Vector3;

  template <class T=double>
  class BIASMathBase_EXPORT Matrix4x4 : public Matrix<T> 
  {
  public:

    /// constructors
    /// call the corresponding constructors of the base class if appropriate
    ///

    /**	default constructor    
    **/
    inline Matrix4x4<T>() : Matrix<T>(4, 4) {};

    // DEPRECATED due to instantiation problem JW
    //explicit Matrix4x4<T>(const char *s) : Matrix<T>(4, 4, s) {};
    /// JW
    explicit Matrix4x4<T>(const std::string & s);


    /** Set the matrix values JW */
    Matrix4x4<T>(
      const T  a0, const T  a1, const T  a2, const T  a3, 
      const T  a4, const T  a5, const T  a6, const T  a7, 
      const T  a8, const T  a9, const T a10, const T a11,
      const T a12, const T a13, const T a14, const T a15 );

    /** @brief init matrix with identity or zero 
    @author koeser */
    explicit Matrix4x4<T>(const MatrixInitType& i) : Matrix<T>(4, 4, T(0)) {
      switch (i) {
  case MatrixZero:return;
  case MatrixIdentity:{ 
    this->v_[0] = this->v_[5] = this->v_[10] = this->v_[15] = T(1);
    return;
                      }
  default: 
    BIASERR("undefined MatrixInitType"); 
    BIASABORT;
      }
    }


    Matrix4x4<T>(const Matrix4x4<T> & A) : Matrix<T>(A) {};

    Matrix4x4<T>(const Matrix<T>& arg);

    /** cast constructor from TNT::Matrix */
    Matrix4x4<T>(const TNT::Matrix<T>& A);

    /** Set as Rotation and Translation for similarity transforms
    @author grest, Dec. 2004
    */
    Matrix4x4<T>(const Matrix3x3<T> & R, 
      const Vector3<T> &trans);


    /** destructor
    @status untested (04/17/2002)
    **/
    virtual ~Matrix4x4<T>();


    /** matrix - vector multiplicate this matrix with Vector4, 
    storing the result in destvec,  calculates:
    destvec = (this Matrix) * argvec
    @author  Jan Woetzel
    @status untested (04/17/2002)
    **/
    inline void Mult( const Vector4<T> &argvec, Vector4<T> &destvec) const {
      // direct access to the matrix values using fixed indicies:
      //  0  1  2  3       0
      //  4  5  6  7   *   1
      //  8  9 10 11       2
      // 12 13 14 15       3

      // pointer to the beginning of the data array of this matrix
      const register T *matP = this->GetData();	

      // first row:
      destvec[0] =
        matP[0] * argvec[0]
        +matP[1] * argvec[1]
        +matP[2] * argvec[2]
        +matP[3] * argvec[3];
        // second row:
        destvec[1] =
          matP[4] * argvec[0]
          +matP[5] * argvec[1]
          +matP[6] * argvec[2]
          +matP[7] * argvec[3];
          // third row:
          destvec[2] =
            matP[8]  * argvec[0]
            +matP[9]  * argvec[1]
            +matP[10] * argvec[2]
            +matP[11] * argvec[3];
            // fourth row:
            destvec[3] =
              matP[12] * argvec[0]
              +matP[13] * argvec[1]
              +matP[14] * argvec[2]
              +matP[15] * argvec[3];
    };




    /** calculate the determinant recursively using sub matrices of size 3x3
    @author  koeser 10/2003
    @status  experimental, slow implementation, not yet heavily tested
    */
    inline T det() const {
      T d = 0;
      Matrix3x3<T> SubMatrix;
      for (register unsigned int sub=0; sub<4; sub++) {
        // first column is used, 
        // set up the 3x3 submatrices from the right matrix part
        // this is done 4 times (loop counter="sub")
        for (register unsigned int i=0; i<3; i++) {          
          // construct the sub matrix under inspection,
          // skip the first column
          SubMatrix[0][i] = (*this)[(sub+1)%4][i+1];
          SubMatrix[1][i] = (*this)[(sub+2)%4][i+1];
          SubMatrix[2][i] = (*this)[(sub+3)%4][i+1];
        }
        // add value of subdeterminant to overall sum
        d += SubMatrix.GetDeterminant() * (*this)[sub][0];
      }
      return d;
    }

    /** matrix-matrix multiplication with other Matrix4x4, 
    storing the result in destmat
    using 'manual inlining'
    @author Jan Woetzel
    @status untested (04/17/2002)
    **/
    inline void Mult(const Matrix4x4<T> & argmat, Matrix4x4<T> & destmat)const{
      //
      // direct access using fixed indicies with matrix order:
      //  (this matrix) *      arg
      //
      //   0  1  2  3        0  1  2  3
      //   4  5  6  7   *    4  5  6  7
      //   8  9 10 11        8  9 10 11
      //  12 13 14 15       12 13 14 15

      // pointer to the destination matrix's data array
      register T *destP 	= destmat.GetData(); 
      // pointer to the argument (other) matrix's data array
      const register T *argP	= argmat.GetData();
      // pointer to this matrix's data array
      const register T *matP	= this->GetData(); 

      // first row:
      destP[0] =
        matP[0]*argP[0] + matP[1]*argP[4] + matP[2]*argP[8] + matP[3]*argP[12];
      destP[1] =
        matP[0]*argP[1] + matP[1]*argP[5] + matP[2]*argP[9] + matP[3]*argP[13];
      destP[2] =
        matP[0]*argP[2] + matP[1]*argP[6] + matP[2]*argP[10] + matP[3]*argP[14];
      destP[3] =
        matP[0]*argP[3] + matP[1]*argP[7] + matP[2]*argP[11] + matP[3]*argP[15];
      // second row:
      destP[4] =
        matP[4]*argP[0] + matP[5]*argP[4] + matP[6]*argP[8] + matP[7]*argP[12];
      destP[5] =
        matP[4]*argP[1] + matP[5]*argP[5] + matP[6]*argP[9] + matP[7]*argP[13];
      destP[6] =
        matP[4]*argP[2] + matP[5]*argP[6] + matP[6]*argP[10] + matP[7]*argP[14];
      destP[7] =
        matP[4]*argP[3] + matP[5]*argP[7] + matP[6]*argP[11] + matP[7]*argP[15];
      // third row:
      destP[8] =
        matP[8]*argP[0] + matP[9]*argP[4] + matP[10]*argP[8] + matP[11]*argP[12];
      destP[9] =
        matP[8]*argP[1] + matP[9]*argP[5] + matP[10]*argP[9] + matP[11]*argP[13];
      destP[10] =
        matP[8]*argP[2] + matP[9]*argP[6] + matP[10]*argP[10] + matP[11]*argP[14];
      destP[11] =
        matP[8]*argP[3] + matP[9]*argP[7] + matP[10]*argP[11] + matP[11]*argP[15];
      // fourth row:
      destP[12] =
        matP[12]*argP[0] + matP[13]*argP[4] + matP[14]*argP[8] + matP[15]*argP[12];
      destP[13] =
        matP[12]*argP[1] + matP[13]*argP[5] + matP[14]*argP[9] + matP[15]*argP[13];
      destP[14] =
        matP[12]*argP[2] + matP[13]*argP[6] + matP[14]*argP[10] + matP[15]*argP[14];
      destP[15] =
        matP[12]*argP[3] + matP[13]*argP[7] + matP[14]*argP[11] + matP[15]*argP[15];
    }


    /** assignment operators
    calling corresponding operator from base class "TNT::Matrix" if appropriate
    @author Jan Woetzel
    @status untested (02/25/2002)
    **/
    Matrix4x4 & operator = (const Matrix<T> &mat);


    /** set the elements of this matrix to the identity matrix
    (posisbly overriding the inherited method)
    @author  , Jan Woetzel
    @date 04/17/2002
    @status untested
    **/
    inline void SetIdentity() {
      register T* d = this->GetData(); // pointer to this matrix's data array
      d[0]  = 1;
      d[1]  = 0;
      d[2]  = 0;
      d[3]  = 0;
      d[4]  = 0;
      d[5]  = 1;
      d[6]  = 0;
      d[7]  = 0;
      d[8]  = 0;
      d[9]  = 0;
      d[10] = 1;
      d[11] = 0;
      d[12] = 0;
      d[13] = 0;
      d[14] = 0;
      d[15] = 1;
    }

    /// stl conform interface
    inline void clear() {
      this->SetZero();
    };


    /** just neccessary to avoid resizing of this 'fixed size' matrix because it is derived from the resizable Matrix
    Should be removed if Matrix4x4 becomes a base class.
    (04/17/2002) Jan Woetzel
    **/
    Matrix4x4 & newsize(int rows, int cols);


    /**	tranpose this matrix "in place"
    example: 
    0  1  2  3  -->  0  4  8 12  
    4  5  6  7  -->  1  5  9 13
    8  9 10 11  -->  2  6 10 14
    12 13 14 15  -->  3  7 11 15
    @author Jan-Friso Evers-Senne
    @status untested (07/08/2002)
    **/
    inline void TransposeIP() {
      register T *dataP = this->GetData();	// pointer to the data array
      register T tmp;
#define swap(a,b,tmp) tmp=a;a=b;b=tmp;
      swap(dataP[1],dataP[4],tmp);
      swap(dataP[2],dataP[8],tmp);
      swap(dataP[12],dataP[3],tmp);
      swap(dataP[13],dataP[7],tmp);
      swap(dataP[14],dataP[11],tmp);
      swap(dataP[9],dataP[6],tmp);
#undef swap
    }

    inline Matrix4x4 Transpose() const {
      Matrix4x4 tmp = (*this);
      tmp.TransposeIP();
      return tmp;
    }



  }; // class Matrix4x4
} // namespace BIAS

#include "Operators.hh"

#endif
