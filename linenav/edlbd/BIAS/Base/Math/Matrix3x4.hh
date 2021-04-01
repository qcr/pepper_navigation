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

#ifndef _Matrix3x4_hh_
#define _Matrix3x4_hh_

// std
#include <string>

// BIAS
#include "bias_config.h" 

#include <Base/Debug/Error.hh>

#include "Matrix.hh"
#include "Vector4.hh"


namespace BIAS {

  template <class T> class BIASMathBase_EXPORT Matrix3x3;

  /**
    @class Matrix3x4
    @ingroup g_math
    @brief is a 'fixed size' rectangular matrix of dim. 3 x 4 
    which is templated over the element-type
    
    @author Jan Woetzel
    @status untested (04/17/2002)
  **/
  template<class T=double>
  class BIASMathBase_EXPORT Matrix3x4 : public Matrix<T> {

  public:
    /**	default constructor **/
    inline Matrix3x4<T>() : Matrix<T>(3, 4) {}

    /// JW
    explicit Matrix3x4<T>(const std::string & s);

    /** Set the matrix values JW */
    Matrix3x4<T>(
      const T a0, const T a1, const T  a2, const T  a3, 
      const T a4, const T a5, const T  a6, const T  a7, 
      const T a8, const T a9, const T a10, const T a11 );

    /** @brief init matrix with identity or zero 
        @author koeser */
    explicit inline
    Matrix3x4<T>(const MatrixInitType& i) : Matrix<T>(3, 4, T(0)) {
      switch (i) {
      case MatrixZero:return;
      case MatrixIdentity:{ 
        this->v_[0] = this->v_[5] = this->v_[10] = T(1);
        return;
      }
      default: 
        BIASERR("undefined MatrixInitType"); 
        BIASABORT;
      }
    }

    /** @author Jan Woetzel
        @status untested (04/17/2002) **/
    Matrix3x4<T>(const Matrix3x4<T> & A) : Matrix<T>(A) {}

    /** destructor
        @status untested (04/17/2002)**/
    virtual ~Matrix3x4<T>();

    /** matrix - vector multiplicate this matrix with Vector4, 
        storing the result in destvec
        calculates:
        destvec = (this Matrix) * argvec
        @author  Jan Woetzel
        @status untested (04/17/2002) **/
    inline void Mult(const Vector4<T> &argvec, Vector3<T> &destvec) const;

    /** assignment operator
        calling corresponding operator from base class if appropriate.
        @author Jan Woetzel
        @status untested (02/25/2002) **/
    Matrix3x4 & operator= (const Matrix<T> &mat);

    /** set the elements of this matrix to the matrix
        1  0  0  0
        0  1  0  0
        0  0  1  0
        which is no strict identity (!)
        @author Jan Woetzel
        @date 04/17/2002
        @status untested **/
    inline void SetIdentity();

    /** just neccessary to avoid resizing of this 'fixed size' matrix 
        because it is derived from the resizable Matrix
        Should be removed if Matrix3x4 becomes a base class.
        @woetzel (04/17/2002) 
        @author Jan Woetzel  **/
    Matrix3x4 & newsize(int rows, int cols);

    /** @brief Loads a PMatrix from .ban file given by AddCameraKey and Time index
        @return 0 in case of success and negative nr. in case of error.
        @author Jan Woetzel */
    int LoadBan(const std::string & filename, const unsigned int & Time=0);
    
    /** @brief Load a PMatrix from a _par.txt (S. Seitz' format) 

    Useful to load camera parameters of S. Seitz' sequences, e.g. temple Ring  "templeR_par.txt".
    The file format is: 
    There is one line for each image.  The format for each line is:
    "imgname.png k11 k12 k13 k21 k22 k23 k31 k32 k33 r11 r12 r13 r21 r22 r23 r31 r32 r33 t1 t2 t3"
    The Seitz' projection matrix representation is given by K*[R t] in contrast to BIAS::PMatrix semantics.
    However, we convert into our 3x4 PMatrix representation on Load internally.

    @param filenameParTxt input xxx_par.txt file in Steven Seitz' format with linewise camparams
    @param index desired image nr.
    @param correspImgFilename corresponding "original" image filename
    @param nTotal total number of cameras for that sequence, given at beginning of file. 
    @author Jan Woetzel, Marc Grauel */
    int LoadPARTXT(const std::string & filenameParTxt,
                   const unsigned int & index,
                   std::string & correspImgFilename,
                   int & nTotal);

    /// stl conform interface
    inline void clear() {
      this->SetZero();
    }
    
    /** matrix-matrix multiplication with Matrix4x4 storing the result in destmat
     **/
    inline void Mult(const Matrix4x4<T> & argmat, Matrix3x4<T> & destmat) const {
      //
      // direct access using fixed indicies with matrix order:
      //  (this matrix) *      arg
      //
      //   0  1  2  3        0  1  2  3
      //   4  5  6  7   *    4  5  6  7
      //   8  9 10 11        8  9 10 11
      //                    12 13 14 15

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
    }

  }; // class Matrix3x4 -------------------------------
  

  /////
  // inline implementations
  /////


  template <class T> inline void 
  Matrix3x4<T>::Mult(const Vector4<T> &argvec, Vector3<T> &destvec) const
  {
    // direct access to the matrix values using fixed indicies:
    //  0  1  2  3       0
    //  4  5  6  7   *   1
    //  8  9 10 11       2
    //                   3
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
    // third row (because of 3x4 there are only 3 rows):
    destvec[2] =
      matP[8]  * argvec[0]
      +matP[9]  * argvec[1]
      +matP[10] * argvec[2]
      +matP[11] * argvec[3];
  }


  template <class T> inline void 
  Matrix3x4<T>::SetIdentity()
  {
    // set this matrix to the following (which is no strict identity matrix)
    //  1  0  0  0
    //  0  1  0  0
    //  0  0  1  0
    register T* d = this->GetData(); // pointer to this matrix's data array
    d[0] = 1;
    d[1] = 0;
    d[2]  = 0;
    d[3] = 0;
    d[4] = 0;
    d[5] = 1;
    d[6]  = 0;
    d[7] = 0;
    d[8] = 0;
    d[9] = 0;
    d[10] = 1;
    d[11] = 0;
  }


} // namespace BIAS


#include "Operators.hh"

#endif
