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

#include "Vector3.hh"
#include "Vector.hh"
#include "math.h"

// STD
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace BIAS;
using namespace std;

namespace BIAS {


  // JW 06/2003
  template < class T >
    Vector3<T> 
    Vector3<T>::GetNormalized() const
  {
    Vector3<T> vec=(*this);
    vec.Normalize();
    return vec;
  }

  template < class T >
    Vector3<T>& Vector3<T>::
    Normalize(Matrix3x3<T>& cov)
  {
    /** the jacobian of the function x' = x/|x| can be written as
    J = 1/|x| ( I - (xx^T)/(x^Tx) ), 
    cf. Foerstner, 2005 "Uncertainty and Projective Geometry" */
    const T act_length = (T) NormL2(); // euclidean length
    if (act_length > std::numeric_limits<T>::epsilon() ){
      Matrix3x3<T> J(MatrixIdentity);
      Matrix3x3<T> tmp;
      OuterProduct(*this, tmp);
      tmp /= (T) (act_length * act_length);
      J -= tmp;
      J /= act_length;
      cov = J * cov * J.Transpose();
    } 
    return Normalize();
  }


  // JW 09/2003 - default: not implemented
  template < class T >
    Vector3<T> 
    Vector3<T>::CoordSphereToEuclidean() const 
  {
   // BIASERR("CoordSphereToEuclidean not implemented for this template type t - but e.g. for double");

    return Vector3<T>(0,0,0);

  }


  // JW 09/2003
  template <>
    Vector3<double> 
    Vector3<double>::CoordSphereToEuclidean() const 
  {
    // symbolic names for clarity:
    const double r     = (*this)[0];
    const double phi   = (*this)[1];
    const double theta = (*this)[2];

    if (r==0) return Vector3<double>( 0, 0, 0 ); // singularity nullvector.

    const double xx( r * sin(theta) * cos(phi) );
    const double yy( r * (sin(theta) * sin(phi)) );
    const double zz( r * cos(theta) ); // TODO: plus or minus here -> RHS or LHS?

    return Vector3<double>( xx, yy, zz );
  }


  // JW 09/2003 - default: not implemented 
  template < class T >
    BIAS::Vector3<T>
    BIAS::Vector3<T>::CoordEuclideanToSphere() const 
  {
    BIASERR("CoordEuclideanToSphere not implemented for this template type T"
      <<" - but e.g. for double");

    return Vector3<T>(0,0,0);
  }


  // JW 09/2003
  template <>
    BIAS::Vector3<double>
    BIAS::Vector3<double>::CoordEuclideanToSphere() const
  {
    // symbolic names for clarity:
    const double xx = (*this)[0];
    const double yy = (*this)[1];
    const double zz = (*this)[2];

    const double r( NormL2() );

    if (r==0) return Vector3<double>(0,0,0); // singularity null vector

    //const double theta( atan2(yy, xx) );
    const double phi(M_PI_2- atan2(xx, yy) );

    // length of projection into XY plane (for phi)
    const double projLength=sqrt( xx * xx  +  yy * yy );

    const double theta(atan2(projLength, zz) );

    return Vector3<double>(r, phi, theta);
  }


  template <class T>
    void Vector3<T>::OuterProduct(const Vector3<T> &v, Matrix3x3<T>& mat) const 
  {
    mat[0][0]=data_[0]*v[0]; mat[0][1]= data_[0]*v[1]; mat[0][2]= data_[0]*v[2];
    mat[1][0]=data_[1]*v[0]; mat[1][1]= data_[1]*v[1]; mat[1][2]= data_[1]*v[2];
    mat[2][0]=data_[2]*v[0]; mat[2][1]= data_[2]*v[1]; mat[2][2]= data_[2]*v[2];
  }


  // JW 03/2005
  template <class T>
    int Vector3<T>::LoadBogTC(const std::string & filename)
  {
    SetZero();// reset this
    ifstream fs( filename.c_str() );
    if (!fs) {
      BIASERR("could not open file "<<filename);
      return -1; // error, could not open file.
    };
    // parse and read in 
    const unsigned int nMax=1024;
    char caLine[nMax];
    // std string for find
    string strLine;
    // helper for string to number /atof)
    istringstream iss;
    string::size_type pos=string::npos;
    unsigned int line=0;
    vector<string> pattern;
    pattern.clear();
    pattern.push_back("TC_X ="); // 0
    pattern.push_back("TC_Y ="); // 1
    pattern.push_back("TC_Z ="); // 2
    vector< bool > found;
    found.resize( pattern.size(), false);
    BIASASSERT(found.size()>2);
    while (!fs.eof() && (fs) 
      && (!found[0] || !found[1] || !found[2] ) ) 
    {
      line++;
      fs.getline(&caLine[0], nMax);
      strLine = caLine;
      for (unsigned int i=0; i<pattern.size(); i++){
        pos = strLine.find(pattern[i]);
        if (pos != string::npos){
          found[i]=true;
          iss.clear();
          iss.str(  // after pattern: start, length
            strLine.substr(pos+pattern[i].size(),  strLine.size()-pos-pattern[i].size()) 
            );
          BIASASSERT(i<3); // dim
          iss>> (*this)[i];
        };
      };
    };
    // check if we found all patterns    
    BIASASSERT(found.size()==pattern.size());
    bool foundAll=true;
    for (unsigned int j=0; j<found.size(); j++) foundAll=foundAll&&found[j];
    if (!foundAll) return -99; // missed something
    return 0; // OK.
  }

  // jw
  template<class T>
    bool Vector3<T>::Load(const std::string & filename)
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
    bool Vector3<T>::Save(const std::string & filename) const 
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

  // specialization
  template<>
    Matrix<unsigned int> Vector3<unsigned int>::GetSkewSymmetricMatrix() const 
  {
    Matrix<unsigned int> result(3,3);
    result.SetZero();
    BIASERR("not implemented because minus does not make sense for unsigned types");
    BIASBREAK;
    return result;
  }


  // impl. moved here for DLL instantiation 
  template <class T>
    Matrix<T> Vector3<T>::GetSkewSymmetricMatrix() const 
  {
    Matrix<T> result(3,3);
    result.SetZero();
    T* pData = result.GetData() + 1;
    *pData++ = -data_[2];
    *pData++ =  data_[1];
    *pData++ =  data_[2];
    pData++;
    *pData++ = -data_[0];
    *pData++ = -data_[1];
    *pData   =  data_[0];
    return result;
  }


  template <class T> 
    Vector3<T>::Vector3(const Vector<T>& v) 
  {
    // check if v has correct size:
    BIASASSERT(v.size() == VECTOR3_SIZE);
    data_[0] = v[0];
    data_[1] = v[1];
    data_[2] = v[2];
  }



} // namespace BIAS


#define INST(type) \
  template class BIASMathBase_EXPORT BIAS::Vector3<type>;

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
