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


#include "Matrix4x4.hh"

using namespace BIAS;
using namespace std;


namespace BIAS {

template <class T>
Matrix4x4<T>::~Matrix4x4() {
    // destroy the new allocated members of this class if they exist
}

template <class T>
Matrix4x4<T>::Matrix4x4(const std::string & s)
: Matrix<T>(4, 4, s) 
{}


/** Set the matrix values JW */
template <class T>
Matrix4x4<T>::Matrix4x4(const T  a0, const T  a1, const T  a2, const T  a3,
                        const T  a4, const T  a5, const T  a6, const T  a7,
                        const T  a8, const T  a9, const T a10, const T a11,
                        const T a12, const T a13, const T a14, const T a15 )
                        : Matrix<T>(4, 4)
{
  this->GetData()[ 0]= a0; this->GetData()[ 1]= a1; this->GetData()[ 2]= a2; this->GetData()[ 3]= a3;
  this->GetData()[ 4]= a4; this->GetData()[ 5]= a5; this->GetData()[ 6]= a6; this->GetData()[ 7]= a7;
  this->GetData()[ 8]= a8; this->GetData()[ 9]= a9; this->GetData()[10]=a10; this->GetData()[11]=a11;
  this->GetData()[12]=a12; this->GetData()[13]=a13; this->GetData()[14]=a14; this->GetData()[15]=a15;
}


template <class T>
Matrix4x4<T> & Matrix4x4<T>::operator= (const Matrix<T> &mat) {
  if ((mat.num_rows() == 4) && (mat.num_cols() == 4)) {
    Matrix<T>::operator= (mat);	// call the operator of the base class
    return *this;
  } else {
    BIASERR("cannot assign with operator= , num_rows=" << mat.
            num_rows() << " num_cols= " << mat.
            num_cols() << " are'nt appropriate for a 4x4 Matrix4x4<T>. ");
    BIASABORT;
  };
  return *this;
}

template<class T>
Matrix4x4<T>::Matrix4x4(const Matrix<T>& mat) : Matrix<T>(mat)
{
  if ((mat.num_rows() != 4) || (mat.num_cols() != 4)) {
    BIASERR("wrong matrix size "<<mat.num_rows()<<"x"
           << mat.num_cols());
  }
}

template<class T>
Matrix4x4<T>::Matrix4x4(const TNT::Matrix<T>& mat) : Matrix<T>(mat)
{
  if ((mat.num_rows() != 4) || (mat.num_cols() != 4)) {
    BIASERR("wrong matrix size "<<mat.num_rows()<<"x"
           << mat.num_cols());
  }
}

template <class T>
Matrix4x4<T> & Matrix4x4<T>::newsize(int rows, int cols) {
    if ((rows != 4) || (cols != 4)) {
        // do nothing
        // print error message if the new size should not be a 4x4 matrix
        BIASERR("The size of a Matrix4x4<T> has to be 4 rows, 4 cols. newsize can't create a " <<
               rows << " x " << cols << " Matrix4x4 !");
#if DEBUG
        BIASABORT;
#endif
        return *this;

    } else {
        BIASASSERT( rows==4 ); // 4x4 matrix
        BIASASSERT( cols==4 );
        // call base class function (usually unnecessary except for first construction)
        Matrix<T>::newsize(4, 4);
        return *this;
    };
}

template <class T>
Matrix4x4<T>::Matrix4x4(const Matrix3x3<T> & R, 
                           const Vector3<T> &trans) : Matrix<T>(4, 4)
{
  (*this)[0][0]=R[0][0]; (*this)[0][1]=R[0][1]; (*this)[0][2]=R[0][2];
  (*this)[0][3]=trans[0];
  (*this)[1][0]=R[1][0]; (*this)[1][1]=R[1][1]; (*this)[1][2]=R[1][2];
  (*this)[1][3]=trans[1];
  (*this)[2][0]=R[2][0]; (*this)[2][1]=R[2][1]; (*this)[2][2]=R[2][2];
  (*this)[2][3]=trans[2];
  (*this)[3][0]=0;(*this)[3][1]=0;              (*this)[3][2]=0;
  (*this)[3][3]=T(1);
  
}

} // end namespace BIAS

#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Matrix4x4<type>;\

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
