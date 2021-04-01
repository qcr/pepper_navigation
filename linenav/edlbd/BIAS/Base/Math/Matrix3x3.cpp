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
//

#include <Base/Common/CompareFloatingPoint.hh>
#include "Matrix3x3.hh"

#include <Base/Math/Matrix.hh>
#include <Base/Math/Matrix3x4.hh>

#include <fstream>

using namespace BIAS;
using namespace std;


/*template<class T>
  Matrix3x3<T>::Matrix3x3(const std::string & s)
  {
  BEXCEPTION("unfinished")
  }*/


namespace BIAS{

template<class T>
Matrix3x3<T>::
Matrix3x3(const T a0, const T a1, const T a2,
          const T a3, const T a4, const T a5,
          const T a6, const T a7, const T a8)
{
  Data_[0]=a0; Data_[1]=a1; Data_[2]=a2;
  Data_[3]=a3; Data_[4]=a4; Data_[5]=a5;
  Data_[6]=a6; Data_[7]=a7; Data_[8]=a8;
}


template<class T>
Matrix3x3<T>::
Matrix3x3(const MatrixInitType& i)
{
  SetZero();
  switch (i) {
  case MatrixZero:
    return;
  case MatrixIdentity:
    this->Data_[0] = this->Data_[4] = this->Data_[8] =T(1);
    return;
  default:
    BIASERR("undefined MatrixInitType");
    BIASABORT;
  }
}


template<class T>
Matrix3x3<T>::
Matrix3x3(const Matrix3x3<T>& A)
{
  memcpy((void *)Data_, (const void *)A.Data_, 9*sizeof(T));
}


template<class T>
Matrix3x3<T>::
Matrix3x3(const Matrix<T>& mat)
{
  if ((mat.num_rows() != 3) || (mat.num_cols() != 3)) {
    BIASERR("wrong matrix size "<<mat.num_rows()<<"x"
            << mat.num_cols());
    BIASABORT;
  }
  memcpy((void *)Data_, (void *)mat.GetData(), 9*sizeof(T));
}


template<class T>
Matrix3x3<T>::
Matrix3x3(const TNT::Matrix<T>& mat)
{
  if ((mat.num_rows() != 3) || (mat.num_cols() != 3)) {
    BIASERR("wrong matrix size "<<mat.num_rows()<<"x"
            << mat.num_cols());
    BIASABORT;
  }
  Data_[0] = mat[0][0];
  Data_[1] = mat[0][1];
  Data_[2] = mat[0][2];
  Data_[3] = mat[1][0];
  Data_[4] = mat[1][1];
  Data_[5] = mat[1][2];
  Data_[6] = mat[2][0];
  Data_[7] = mat[2][1];
  Data_[8] = mat[2][2];
}


template<class T>
Matrix3x3<T>::
~Matrix3x3()
{}


template<class T>
Matrix3x3<T>& Matrix3x3<T>::
operator+=(const Matrix3x3<T>& arg)
{
  for (int i=0; i<9; i++)
    Data_[i] += arg.Data_[i];
  return *this;
}


template<class T>
Matrix3x3<T>& Matrix3x3<T>::
operator-=(const Matrix3x3<T>& arg)
{
  for (int i=0; i<9; i++)
    Data_[i] -= arg.Data_[i];
  return *this;
}


template <class T>
Matrix3x3<T>& Matrix3x3<T>::
operator*=(const T &scalar)
{
  register T *src	= this->GetData();
  for (int i=0; i<9; i++)
    src[i] *= scalar;
  return *this;
}


template <class T>
Matrix3x3<T>& Matrix3x3<T>::
operator/=(const T &scalar)
{
  BIASASSERT(!Equal(scalar, (T)0.0));
  register T *src	= this->GetData();
  for (int i=0; i<9; i++)
    src[i] /= scalar;
  return *this;
}


template<class T>
Matrix3x3<T> Matrix3x3<T>::
Transpose() const
{
  Matrix3x3<T> ret;

  T *retd = ret.GetData();
  const T *thisd = Data_;
  retd[0] = thisd[0];
  retd[1] = thisd[3];
  retd[2] = thisd[6];

  retd[3] = thisd[1];
  retd[4] = thisd[4];
  retd[5] = thisd[7];

  retd[6] = thisd[2];
  retd[7] = thisd[5];
  retd[8] = thisd[8];
  return ret;
}


template<class T>
void Matrix3x3<T>::
SetFromVector(const TNT::Vector<T>& vec)
{
  BIASASSERT(vec.size() == 9 );
  // fill the matrix rowwise with the first 9 elements of the vector
  // example:
  // 1 2 3 4 5 6 7 8 9
  //  -->
  // 1 2 3
  // 4 5 6
  // 7 8 9
  for (int i=0; i<9; i++) {
    Data_[i] = vec[i];
  }
}


template<class T>
void Matrix3x3<T>::
SetFromColumnVectors(const BIAS::Vector3<T> & v0,
                     const BIAS::Vector3<T> & v1,
                     const BIAS::Vector3<T> & v2 )
{
  Data_[0] = v0[0];
  Data_[1] = v1[0];
  Data_[2] = v2[0];
  Data_[3] = v0[1];
  Data_[4] = v1[1];
  Data_[5] = v2[1];
  Data_[6] = v0[2];
  Data_[7] = v1[2];
  Data_[8] = v2[2];
}


template<class T>
void Matrix3x3<T>::
SetFromRowVectors(const BIAS::Vector3<T> & v0,
                  const BIAS::Vector3<T> & v1,
                  const BIAS::Vector3<T> & v2 )
{
  Data_[0] = v0[0];
  Data_[1] = v0[1];
  Data_[2] = v0[2];
  Data_[3] = v1[0];
  Data_[4] = v1[1];
  Data_[5] = v1[2];
  Data_[6] = v2[0];
  Data_[7] = v2[1];
  Data_[8] = v2[2];
}


template<>
void Matrix3x3<unsigned int>::
SetAsCrossProductMatrix(const unsigned int & /*x*/,
                        const unsigned int & /*y*/,
                        const unsigned int & /*z*/) {
  BIASERR("not implemented because minus does not make sense for "
          "unsigned type");
  BIASABORT;
}

template <class T>
void Matrix3x3<T>::
SetAsCrossProductMatrix(const T &x, const T &y, const T &z)
{
  T *mat=this->GetData();
  mat[0]=mat[4]=mat[8]=0;
  mat[1]= -z; mat[2]= y;
  mat[3]=  z; mat[5]=-x;
  mat[6]= -y; mat[7]= x;
}


template<>
void Matrix3x3<unsigned int>::
SetAsCrossProductMatrix(const Vector3<unsigned int> & /*vec*/){
	BIASERR("not implemented because minus does not make sense for "
			"unsigned type");
	BIASABORT;
}


template <class T>
void Matrix3x3<T>::
SetAsCrossProductMatrix(const Vector3<T> &vec)
{
  T *mat=this->GetData();
  mat[0]=mat[4]=mat[8]=0;
  mat[1]=-vec[2]; mat[2]=vec[1];
  mat[3]= vec[2]; mat[5]=-vec[0];
  mat[6]= -vec[1]; mat[7]=vec[0];
}


template<class T>
void Matrix3x3<T>::
GetRow(const unsigned int row, Vector3<T>& v) const
{
  BIASASSERT(row<3);// this has only 3 rows[0,1,2]
  const T* p =Data_ + 3*row;
  v[0]=*p++;
  v[1]=*p++;
  v[2]=*p;
}


template<class T>
void Matrix3x3<T>::
GetColumn(const unsigned int col, BIAS::Vector3<T>& v) const
{
  BIASASSERT(col<3);// this has only 3 cols[0,1,2]
  const T* p =Data_ + col;
  v[0] = p[0];
  v[1] = p[3];
  v[2] = p[6];
}


template<class T>
void Matrix3x3<T>::
SetRow(const unsigned int row, const Vector3<T>& r)
{
  BIASASSERT(row<3);// this has only 3 cols[0,1,2]
  T* p = Data_ + 3*row;
  p[0] = r[0];
  p[1] = r[1];
  p[2] = r[2];
}


template<class T>
void Matrix3x3<T>::
SetColumn(const unsigned int col, const Vector3<T>& c)
{
  BIASASSERT(col<3);// this has only 3 cols[0,1,2]
  T* p = Data_ + col;
  p[0] = c[0];
  p[3] = c[1];
  p[6] = c[2];
}

#ifdef WIN32
template<> BIASMathBase_EXPORT
unsigned int Matrix3x3<unsigned int>::
NormL1() const
{
  unsigned int result = 0;
  for (int i=0; i<9; i++){
    result += Data_[i];
  }
  return result;
}
#endif

template<class T>
T Matrix3x3<T>::
GetDeterminant() const
{
  double t4 = (*this)[0][0]*(*this)[1][1];
  double t6 = (*this)[0][0]*(*this)[1][2];
  double t8 = (*this)[0][1]*(*this)[1][0];
  double t10 = (*this)[0][2]*(*this)[1][0];
  double t12 = (*this)[0][1]*(*this)[2][0];
  double t14 = (*this)[0][2]*(*this)[2][0];
  return T((t4*(*this)[2][2]-t6*(*this)[2][1]-t8*(*this)[2][2]+
            t10*(*this)[2][1]+t12*(*this)[1][2]-t14*(*this)[1][1]));
}


template<>
int Matrix3x3<unsigned int>::
GetInverse(Matrix3x3<unsigned int>& x) const
{
  BIASERR("not implemented because minus does not make sense for "
          "unsigned type");
  BIASBREAK;
  return -1;
}


template<class T>
int Matrix3x3<T>::
GetInverse(Matrix3x3<T>& x) const
{
  int res=0;
  BIASASSERT(x.GetData()!=Data_);
  // following lines have been produced by Maple
  const double t4 = (*this)[0][0]*(*this)[1][1];
  const double t6 = (*this)[0][0]*(*this)[1][2];
  const double t8 = (*this)[0][1]*(*this)[1][0];
  const double t10 = (*this)[0][2]*(*this)[1][0];
  const double t12 = (*this)[0][1]*(*this)[2][0];
  const double t14 = (*this)[0][2]*(*this)[2][0];

  const double det=(t4*(*this)[2][2]-t6*(*this)[2][1]-t8*(*this)[2][2]+t10
                    *(*this)[2][1]+t12*(*this)[1][2]-t14*(*this)[1][1]);
  // matrices with zero determinant are not invertible
  if (det==0.0)
    return -1;

  const double t17 = 1.0/det;

  x[0][0] = T(((*this)[1][1]*(*this)[2][2]-(*this)[1][2]*(*this)[2][1])*t17);
  x[0][1] = T(-((*this)[0][1]*(*this)[2][2]-(*this)[0][2]*(*this)[2][1])*t17);
  x[0][2] = T(-(-(*this)[0][1]*(*this)[1][2]+(*this)[0][2]*(*this)[1][1])*t17);
  x[1][0] = T(-((*this)[1][0]*(*this)[2][2]-(*this)[1][2]*(*this)[2][0])*t17);
  x[1][1] = T(((*this)[0][0]*(*this)[2][2]-t14)*t17);
  x[1][2] = T(-(t6-t10)*t17);
  x[2][0] = T(((*this)[1][0]*(*this)[2][1]-(*this)[1][1]*(*this)[2][0])*t17);
  x[2][1] = T(-((*this)[0][0]*(*this)[2][1]-t12)*t17);
  x[2][2] = T((t4-t8)*t17);

  return res;
}


template<>
int Matrix3x3<unsigned int>::
InvertIP()
{
  BIASERR("not implemented because minus does not make sense for "
          "unsigned type");
  BIASBREAK;
  return -1;
}


template<class T>
int Matrix3x3<T>::
InvertIP()
{
  int res=0;

  // following lines have been produced by Maple
  const double t4 = (*this)[0][0]*(*this)[1][1];
  const double t6 = (*this)[0][0]*(*this)[1][2];
  const double t8 = (*this)[0][1]*(*this)[1][0];
  const double t10 = (*this)[0][2]*(*this)[1][0];
  const double t12 = (*this)[0][1]*(*this)[2][0];
  const double t14 = (*this)[0][2]*(*this)[2][0];

  const double det=(t4*(*this)[2][2]-t6*(*this)[2][1]-t8*(*this)[2][2]+t10
                    *(*this)[2][1]+t12*(*this)[1][2]-t14*(*this)[1][1]);
  // matrices with zero determinante ar not invertible
  if (det==0.0) return -1;

  const double t17 = 1.0/det;

  T tmp[9];

  tmp[0] = T(((*this)[1][1]*(*this)[2][2]-(*this)[1][2]*(*this)[2][1])*t17);
  tmp[1] = T(-((*this)[0][1]*(*this)[2][2]-(*this)[0][2]*(*this)[2][1])*t17);
  tmp[2] = T(-(-(*this)[0][1]*(*this)[1][2]+(*this)[0][2]*(*this)[1][1])*t17);
  tmp[3] = T(-((*this)[1][0]*(*this)[2][2]-(*this)[1][2]*(*this)[2][0])*t17);
  tmp[4] = T(((*this)[0][0]*(*this)[2][2]-t14)*t17);
  tmp[5] = T(-(t6-t10)*t17);
  tmp[6] = T(((*this)[1][0]*(*this)[2][1]-(*this)[1][1]*(*this)[2][0])*t17);
  tmp[7] = T(-((*this)[0][0]*(*this)[2][1]-t12)*t17);
  tmp[8] = T((t4-t8)*t17);

  memcpy((void*)(Data_), (void*)tmp, 9*sizeof(T));

  return res;
}


template <class T>
void Matrix3x3<T>::
Mult(const Matrix3x4<T> &argmat, Matrix3x4<T> &destmat) const
{
  //
  // direct access using fixed indicies with matrix order:
  //  0 1 2
  //  3 4 5
  //  6 7 8
  // oder 3x4
  //  0 1 2  3
  //  4 5 6  7
  //  8 9 10 11
  T *destP 	= (T*) (destmat.GetData());
  const T *argP	= (T*)(argmat.GetData());
  const register T *matP	= this->GetData();
  // does not work for in place multiplication
  BIASASSERT(matP!=argP);
  BIASASSERT(matP!=destP);

  destP[0] =
    matP[0] * argP[0] + matP[1] * argP[4] + matP[2] * argP[8];
  destP[1] =
    matP[0] * argP[1] + matP[1] * argP[5] + matP[2] * argP[9];
  destP[2] =
    matP[0] * argP[2] + matP[1] * argP[6] + matP[2] * argP[10];
  destP[3] =
    matP[0] * argP[3] + matP[1] * argP[7] + matP[2] * argP[11];

  destP[4] =
    matP[3] * argP[0] + matP[4] * argP[4] + matP[5] * argP[8];
  destP[5] =
    matP[3] * argP[1] + matP[4] * argP[5] + matP[5] * argP[9];
  destP[6] =
    matP[3] * argP[2] + matP[4] * argP[6] + matP[5] * argP[10];
  destP[7] =
    matP[3] * argP[3] + matP[4] * argP[7] + matP[5] * argP[11];

  destP[8] =
    matP[6] * argP[0] + matP[7] * argP[4] + matP[8] * argP[8];
  destP[9] =
    matP[6] * argP[1] + matP[7] * argP[5] + matP[8] * argP[9];
  destP[10] =
    matP[6] * argP[2] + matP[7] * argP[6] + matP[8] * argP[10];
  destP[11] =
    matP[6] * argP[3] + matP[7] * argP[7] + matP[8] * argP[11];
}


template <class T>
bool Matrix3x3<T>::
IsZero(T eps) const
{
  for (int i=0; i<9; i++)
    if (!Equal(Data_[i], (T)0.0, eps)) return false;
  return true;
}


template <class T>
bool Matrix3x3<T>::
IsIdentity(T eps) const
{
  if (!Equal(Data_[0], (T)1.0, eps)) return false;
  if (!Equal(Data_[1], (T)0.0, eps)) return false;
  if (!Equal(Data_[2], (T)0.0, eps)) return false;
  if (!Equal(Data_[3], (T)0.0, eps)) return false;
  if (!Equal(Data_[4], (T)1.0, eps)) return false;
  if (!Equal(Data_[5], (T)0.0, eps)) return false;
  if (!Equal(Data_[6], (T)0.0, eps)) return false;
  if (!Equal(Data_[7], (T)0.0, eps)) return false;
  if (!Equal(Data_[8], (T)1.0, eps)) return false;

  return true;
}


template<class T>
bool Matrix3x3<T>::
operator==(const Matrix3x3<T>& arg) const
{
  for (int i=0; i<9; i++)
    if (!Equal(Data_[i], arg.Data_[i])) return false;
  return true;
}


template<class T>
bool Matrix3x3<T>::
Load(const std::string & filename)
{
  std::ifstream fs( filename.c_str() );
  if (!fs.good()) {
    return false;
  }

  fs>>(*this);
  if (!fs.good()) return false;
  fs.close();

  return true;
}


template<class T>
bool Matrix3x3<T>::
Save(const std::string & filename) const
{
  std::ofstream fs( filename.c_str() );
  if (!fs.good()) {
    return false;
  }
  fs<<(*this);
  if (!fs.good()) {
    fs.close(); return false; }
  fs.close();

  return true;
}


template<class T>
T Matrix3x3<T>::
Normalize()
{
  T greatest = 0;
  for (int i=0; i<9; i++)
    if ( (Data_[i])*(Data_[i]) > greatest*greatest )
      greatest = Data_[i];
  (*this) /= greatest;
  return greatest;
}


template<class T>
void Matrix3x3<T>::
MakeSymmetric()
{
  for (int r=0; r<3; r++){
    for (int c=r; c<3; c++){
      (*this)[c][r] = (*this)[r][c] =
        T(((*this)[c][r] + (*this)[r][c]) / 2.0);
    }
  }
}


template<class T>
T Matrix3x3<T>::
GetMax() const
{
  register const T *d = this->GetData();
  T max = *d;
  if (d[1]>max) max=d[1];
  if (d[2]>max) max=d[2];
  if (d[3]>max) max=d[3];
  if (d[4]>max) max=d[4];
  if (d[5]>max) max=d[5];
  if (d[6]>max) max=d[6];
  if (d[7]>max) max=d[7];
  if (d[8]>max) max=d[8];
  return max;
}


template<class T>
T Matrix3x3<T>::
GetMin() const
{
  register const T *d = this->GetData();
  T min = *d;
  if (d[1]<min) min=d[1];
  if (d[2]<min) min=d[2];
  if (d[3]<min) min=d[3];
  if (d[4]<min) min=d[4];
  if (d[5]<min) min=d[5];
  if (d[6]<min) min=d[6];
  if (d[7]<min) min=d[7];
  if (d[8]<min) min=d[8];
  return min;
}


template<class T>
void Matrix3x3<T>::
GetMaxMin(T& max, T& min) const
{
  register const T *d = this->GetData();
  max = min = *d;

  if (d[1]>max) max=d[1];
  if (d[2]>max) max=d[2];
  if (d[3]>max) max=d[3];
  if (d[4]>max) max=d[4];
  if (d[5]>max) max=d[5];
  if (d[6]>max) max=d[6];
  if (d[7]>max) max=d[7];
  if (d[8]>max) max=d[8];

  if (d[1]<min) min=d[1];
  if (d[2]<min) min=d[2];
  if (d[3]<min) min=d[3];
  if (d[4]<min) min=d[4];
  if (d[5]<min) min=d[5];
  if (d[6]<min) min=d[6];
  if (d[7]<min) min=d[7];
  if (d[8]<min) min=d[8];
}


template <class T>
void Matrix3x3<T>::
GetAbsMaxMin(T& max, T& min) const
{
  BIASERR("does not exist for this storaget type");
  BIASABORT;
}


template<>
void Matrix3x3<double>::
GetAbsMaxMin(double& max, double& min) const
{
  max = min = fabs( Data_[0] );
  for(int i=0; i<9; i++) {
    double ad = fabs(Data_[i]);
    if (ad<min) min = ad;
    else if (ad>max) max = ad;
  }
}


template<>
void Matrix3x3<float>::
GetAbsMaxMin(float& max, float& min) const
{
	max = min = fabs( Data_[0] );
	for(int i=0; i<9; i++) {
		float ad = fabs(Data_[i]);
		if (ad<min) min = ad;
		else if (ad>max) max = ad;
	}
}


template<class T>
Matrix3x3<T>& Matrix3x3<T>::
operator=(const Matrix3x3<T>& mat)
{
  memcpy((void *)Data_, (const void *)mat.Data_, 9*sizeof(T));
  return *this;
}

} //end namespace BIAS

////////////////////////////////////////////////////////////////
// non class functions
////////////////////////////////////////////////////////////////

template <class T>
std::istream& BIAS::operator>>(std::istream &s, Matrix3x3<T> &A)
{
  Matrix<T> tmp;

  s >> tmp;

  if (tmp.num_cols()!=3 || tmp.num_rows()!=3) {
    s.setstate(std::ios::badbit | std::ios::failbit);
    return s;
  }

  A = Matrix3x3<T>(tmp);

  return s;
}

template <class T>
std::ostream& BIAS::operator<<(std::ostream &s, const Matrix3x3<T> &A)
{
  Matrix<T> tmp = Matrix<T>(A);
  // use stream operator from Matrix for compatibility
  s << tmp;
  return s;
}



#define INST(type) \
  template class BIASMathBase_EXPORT BIAS::Matrix3x3<type>; \
  template BIASMathBase_EXPORT ostream& BIAS::operator<<<type>(ostream& os, const Matrix3x3<type>& mat); \
  template BIASMathBase_EXPORT istream& BIAS::operator>><type>(istream& is, Matrix3x3<type>& mat);

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
