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


#include "Vector4.hh"
#include "Vector.hh"

#include <iostream>
#include <fstream>

using namespace BIAS;
using namespace std;

template<class T> 
Vector4<T>::Vector4(const Vector <T> &v) 
{
    // check if v has correct size:
    BIASASSERT(v.size() == VECTOR4SIZE);
    // set the 4 elements:
    data_[0] = v[0];
    data_[1] = v[1];
    data_[2] = v[2];
    data_[3] = v[3];
}


// jw
template<class T>
bool Vector4<T>::Load(const std::string & filename)
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
bool Vector4<T>::Save(const std::string & filename) const 
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

template <class T>
Matrix4x4<T> Vector4<T>::OuterProduct(const Vector4<T> &v) const {
  Matrix4x4<T> mat;
  T rVal;
  for (unsigned int row=0; row<this->Size(); row++) {
    rVal = (*this)[row];
    for (unsigned int col=0; col<v.Size(); col++)
      mat[row][col] = rVal * v[col];
  }
  return mat;
}


#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Vector4<type>;\

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
