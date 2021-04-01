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

#include <Base/Math/Vector2.hh>
#include <Base/Math/Vector.hh>
#include <Base/Math/Matrix2x2.hh>
#include <Base/Math/Utils.hh>
#include <iostream>
#include <fstream>

//additional math constants are not used by default in WIN32 - so set usage

using namespace BIAS;
using namespace std;

namespace BIAS {

  template<class T> 
    Vector2<T>::Vector2(const Vector <T> &v) 
  {
    // check if v has correct size:
    BIASASSERT(v.size() == VECTOR2SIZE);
    // set the 2 elements:
    data_[0] = v[0];
    data_[1] = v[1];
  }


  // JW 09/2003 - default: not implemented template < class T >
  template<class T> 
    BIAS::Vector2<T>
    BIAS::Vector2<T>::CoordPolarToEuclidean() const 
  {
    return Vector2<T>(0,0);
  }


  // JW
  template <>
    BIAS::Vector2<double>
    BIAS::Vector2<double>::CoordPolarToEuclidean() const 
  {
    // for clarity symbolic names:
    const double r   = (*this)[0];
    const double phi = (*this)[1];

    // polar to euclidean
    const double xx( r * cos(phi) );
    const double yy( r * sin(phi) );

    return Vector2<double>( xx, yy );
  }


  // JW 09/2003 - default: not implemented 
  template < class T >
    BIAS::Vector2<T>
    BIAS::Vector2<T>::CoordEuclideanToPolar() const 
  {

    return Vector2<T>(0,0);
  }


  // JW
  template <>
    BIAS::Vector2<double>
    BIAS::Vector2<double>::CoordEuclideanToPolar() const 
  {
    // for clarity symbolic names:
    const double x = (*this)[0];
    const double y = (*this)[1];

    // polar to euclidean
    const double r( NormL2() );

    if (r==0) return  Vector2<double>( 0,0 ); // special case nullvector

    if (y==0) {
      // on x-axis.
      if (x>=0) {
        // special case point on positive x-axis
        return Vector2<double>( r, 0 );
      } else {
        // special case point on negative x-axis
        return Vector2<double>(r, M_PI);

      };

    } else {

      // not on x-axis
      const double phi( atan2(y,x) );
      return Vector2<double>( r, phi );

    }
  }



  // jw
  template<class T>
    bool Vector2<T>::Load(const std::string & filename)
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
    bool Vector2<T>::Save(const std::string & filename) const 
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

} // namespace BIAS


///
/// explicit instantiation
///
#define INST(type) \
  template class BIASMathBase_EXPORT BIAS::Vector2<type>;

INST(unsigned char)
INST(float)
INST(char)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
//INST(unsigned int)
INST(double)
