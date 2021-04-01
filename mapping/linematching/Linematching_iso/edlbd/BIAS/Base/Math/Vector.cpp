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

#include <math.h>
#include "Vector.hh"

#include "Vector2.hh"
#include "Vector3.hh"
#include "Vector4.hh"
#include "Matrix.hh"

#include "Polynom.hh"

#include <iostream>
#include <fstream>



using namespace std;

namespace BIAS {

template<class T>
Vector<T>::~Vector()
{}

template<class T>
Vector<T>::Vector(const int rows, const std::string & s)
  : TNT::Vector<T>(rows, s)
{}


template<class T>
Vector<T>::Vector(const Vector2<T>& v)
  : TNT::Vector<T>(2)
{ (*this)[0]=v[0]; (*this)[1]=v[1]; }


template<class T>
Vector<T>::Vector(const Vector3<T>& v)
  : TNT::Vector<T>(3)
{ (*this)[0]=v[0]; (*this)[1]=v[1]; (*this)[2]=v[2]; }


template<class T>
Vector<T>::Vector(const Vector4<T>& v)
  : TNT::Vector<T>(4)
{ (*this)[0]=v[0]; (*this)[1]=v[1]; (*this)[2]=v[2]; (*this)[3]=v[3]; }


template<class T>
Vector<T>::Vector(const std::vector<T>& v)
  : TNT::Vector<T>((TNT::Subscript)v.size())
{
  const int sze = Size();
  for (int i=0; i<sze; i++){
    (*this)[i] = v[i];
  }
}



template<class T>
Vector<T>& Vector<T>::operator=(const TNT::Vector<T> &vec)
{
    TNT::Vector<T>::operator = (vec);	// call the operator of the base class
    return *this;
}

template<class T>
void Vector<T>::Fill(const T & scalar)
{
    TNT::Vector<T>::operator=(scalar);
}


template <class T>
Matrix<T> Vector<T>::OuterProduct(const Vector<T> &v) const {
  Matrix<T> mat(this->size(),v.size());
  T rVal;
  for (int row=0; row<this->size(); row++) {
    rVal = (*this)[row];
    for (int col=0; col<v.size(); col++)
      mat[row][col] = rVal * v[col];
  }
  return mat;
}

template <class T>
void Vector<T>::KroneckerProduct(const Vector<T> &arg, Vector<T>& dst) const
{
  const unsigned s1=this->size(), s2=arg.size();
  unsigned l=0;
  dst.newsize(s1*s2);
  for (register unsigned i=0; i<s1; i++){
    for (register unsigned k=0; k<s2; k++){
      dst[l]=(*this)[i]*arg[k];
      l++;
    }
  }
}



template <class T>
bool Vector<T>::IsZero() const {
  int i=this->size();
  // return false when the first non-zero element is found.
  while (i) {
    i--;
    if (this->v_[i] != (T)0) return false;
  };
  return true; // all elementa are zero
}

template <class T>
Vector<T> Vector<T>::SubVec(int length, int offset) const
{

  Vector<T> res ( length );
  for (int i = 0; i < length; i++)
    res[i] = (*this)[i+offset];
  return res;
}

// jw
template<class T>
bool Vector<T>::Load(const std::string & filename)
{
  std::ifstream fs( filename.c_str() );
  if (!fs) {
    return false; // error, could not open file.
  } else {
    // read in
    fs>>(*this);
    fs.close();
  };
  return true;
}

// jw
template<class T>
bool Vector<T>::Save(const std::string & filename) const
{
  std::ofstream fs( filename.c_str() );
  if (!fs) {
    return false; // error, could not open file.
  } else {
    // write out to disk
    fs<<(*this);
    fs.close();
  };
  return true;
}

template  <class T>
void Vector<T>::GetSTLVec(std::vector<T>& vec) const
{
  const int sze = Size();
  vec.resize(sze);
  for (int i=0; i<sze; i++){
    vec[i] = (*this)[i];
  }
}


template  <class T>
std::vector<T> Vector<T>::GetSTLVec() const
{
  std::vector<T> vec;
  GetSTLVec(vec);
  return vec;
}


template  <class T>
double Vector<T>::GetMean() const
{
  const int sze = (*this).size();
	double mn=0;
	for(int i=0; i<sze; i++)
		mn += (*this)[i];

	return mn/(double)sze;
}

#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
template  <>
double Vector<BIAS::Polynom>::GetMean() const
{
  BIASERR("Cannot cast from BIAS::Polynom to double !");
  BIASABORT;
	return -1.0;
}
#endif

}

///
/// explicit instantiation
///
#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Vector<type>;\
template class BIASMathBase_EXPORT TNT::Vector<type>;\


INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
INST(BIAS::Polynom)
#endif

