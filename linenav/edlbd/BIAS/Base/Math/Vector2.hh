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


#ifndef _Vector2_hh_
#define _Vector2_hh_

#include "bias_config.h"
#include <BIAS_Definitions.hh>

//additional math constants are not used by default in WIN32 - so set usage
//#define _USE_MATH_DEFINES
//#include <math.h>

#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
// BIAS
#include <Base/Debug/Error.hh>
#include <Base/Debug/Exception.hh>
#include <Base/Math/Vector.hh>
#include <Base/Math/Matrix2x2.hh>
#include <Base/Math/Operators.hh>
#include <Base/Math/Utils.hh>

// dim of this vector
#define VECTOR2SIZE 2

/*unsigned int abs(unsigned int x){
     return((unsigned int)(abs(float(x))));
}*/

namespace BIAS {

  // forward declarations to avoid mutual inclusion
  //template <class T> class BIASMathBase_EXPORT BIAS::Vector;
  //template <class T> class BIASMathBase_EXPORT BIAS::Matrix;
  //template <class T> class BIASMathBase_EXPORT BIAS::Matrix2x2;

  // disabled these forwards for g++4.0, checked with g++3.3
  // not checkedwith W32, Friso
//   template <class T> class BIAS::Vector;
//   template <class T> class BIAS::Matrix;
//   template <class T> class BIAS::Matrix2x2;



  /** @class Vector2
      @ingroup g_math
      @brief class Vector2 contains a Vector of dim. 2 and fixed dimension

      It's elment type is templated.
      manual loop unrolling is used if possible.
      The Vector is in row-major order (2 rows, 1 column)
      the indizes begin with zero (to size-1)
      @author Jan Woetzel
      @status untested (03/04/2002) **/
  template <class T>
  class BIASMathBase_EXPORT Vector2
  {
  public:


    /** destructor
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    ~Vector2() {};


    /**	default constructor
        @author Jan Woetzel
        @status alpha (02/28/2002) **/
    Vector2() {};

    /**	copy constructor
        @author Jan Woetzel
        @status alpha (03/05/2002)**/
    inline Vector2(const Vector2<T> &vec) {
      Copy(vec.GetData());
    };

    /** assignment with a constant value for all elements
        @author Jan Woetzel
        @status alpha (03/01/2002) **/
    explicit inline Vector2(const T & scalar) {
      Set(scalar);
    };


    /** constructor with element assigment using explicit values
        @author Ingo Thomsen
        @status tested (03/04/2002) **/
    inline Vector2(const T v0, const T v1) {
      data_[0] = v0;
      data_[1] = v1;
    }


    /**	assignment with an array of values
        which is copied into this ones class members
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    explicit inline Vector2(const T * pv) {
      Copy(pv);
    };

    /**	constructor with element assignment
        @author Jan Woetzel
        @status alpha (03/01/2002) **/
    explicit inline Vector2(char *s) {
      std::istringstream inst(s);
      inst >> data_[0];
      inst >> data_[1];
    };


    /** cast constructor
        @author Jan Woetzel
        @status alpha (03/22/2002) **/
    Vector2(const Vector < T > &v);


    /** copy the array of vectorsize beginning at *T to this->data_
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    inline void Copy(const T * pv) {
      // better use memcopy ?
      data_[0] = pv[0];
      data_[1] = pv[1];
    };


    /** set all elements to a scalar value
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    inline void Set(const T & scalar) {
      Set(scalar, scalar);
    };


    /** @author Jan Woetzel */
    inline void SetZero() {
      Set(0,0);
    };

    /// stl conform interface
    inline void clear() {
      SetZero();
    };

    /** @author Jan Woetzel */
    inline bool IsZero() const {
      return ( (data_[0]==(T)0)
               && (data_[1]==(T)0));
    }


    /** set elementwise with given scalar values
        @author Jan Woetzel
        @status alpha (02/28/2002) **/
    inline void Set(const T & x, const T & y) {
      data_[0] = x;
      data_[1] = y;
    };


    /**	@return the size of this vector
        @author Jan Woetzel (02/28/2002) **/
    inline const unsigned int Size() const { return size(); };
    // stl conform interface
    inline const unsigned int size() const { return VECTOR2SIZE; };
    // compatibility interface
    inline const unsigned int GetNumElements() const { return size(); };


    /** get the data pointer
        the member function itself is const (before {..}) because it doesn't
        change the this object.
        @return the const pointer to the data array for reading
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    inline const T *GetData() const {
      return data_;
    };

    inline T *GetData() {
      return data_;
    };


    /** assignment operator
        set the vector elementwise to scalar value
        @author Jan Woetzel
        @status alpha (02/28/2002) **/
    inline Vector2<T> &operator=(const T& scalar) {
      Set(scalar);
      return *this;
    };

    /** assignment operator
        @author grest
    */
    inline Vector2<T> &operator=(const Vector2<T> &vec) {
      Copy(vec.GetData());
      return *this;
    };

    /** access an element  of the vector with 0-based indizes.
        read only (no write)
        member funciton const because it doesn't change this object
        @author Jan Woetzel
        @status untested (03/01/2002) **/
    inline const T & operator[] (const int i) const {
      BIASASSERT(0 <= i);
      BIASASSERT(i < VECTOR2SIZE);
      return data_[i];
    };

    /** access an element  of the vector with 0-based indizes.
        write allowed
        @author Jan Woetzel
        @status untested (03/01/2002) **/
    inline T & operator[] (const int i) {
      BIASASSERT(0 <= i);
      BIASASSERT(i < VECTOR2SIZE);
      return data_[i];
    };


    /** read only access functions to use names instead of indizes
        @return the projective coordinate of the vector
        (they are usually not euclidean!)
        @author Jan Woetzel
        @status untested (03/04/2002) **/
    inline  T & GetX() {
      return data_[0];
    };
    inline  T & GetY() {
      return data_[1];
    };
    inline  T & GetW() {
      return data_[1];
    };

    // const non reference of the above JW 05/2003:
    inline  T  GetX() const {
      return data_[0];
    };
    inline  T  GetY() const {
      return data_[1];
    };
    inline  T  GetW() const {
      return data_[1];
    };



    /** set (write) access functions to use names instead of indizes
        @author Jan Woetzel
        @status untested (03/04/2002) **/
    inline void SetX(const T x) {
      data_[0] = x;
    };
    inline void SetY(const T y) {
      data_[1] = y;
    };
    inline void SetW(const T w) {
      data_[1] = w;
    };

    /// polar form JW
    inline T& Radius()
    { return (*this)[0]; };
    inline T Radius() const
    { return (*this)[0]; };

    /// polar form JW
    inline T& Phi()
    { return (*this)[1]; };
    inline T Phi() const
    { return (*this)[1]; };


    // Define max and min function for clang compiler support
    // for gcc 4.7
    // #ifdef __clang__
    #ifndef WIN32
    inline T min(const T a, const T b) const {
      return (a>b) ? b : a;
    }

    inline T max(const T a, const T b) const {
      return (a<b) ? b : a;
    }
    #endif
    // #endif


    /** scalar product of two vectors, storing the result in result
        @author Jan Woetzel
        @status untested **/
    inline void ScalarProduct(const Vector2<T> &argvec, T & result) const {
      result = data_[0] * argvec[0] + data_[1] * argvec[1];
    }

    inline T ScalarProduct(const Vector2<T> &argvec) const {
      return ((data_[0] * argvec[0]) + (data_[1] * argvec[1]));
    }


    /** multiply two vectors elementwise, storing the result in destvec
        @author Jan Woetzel (03/06/2002) **/
    inline void ElementwiseProduct(const Vector2<T> &argvec,
      Vector2<T> &destvec) const
    {
      destvec[0] = data_[0] * argvec[0];
      destvec[1] = data_[1] * argvec[1];
    }

    inline Vector2<T> ElementwiseProduct(const Vector2<T>& argvec) const
    {
      Vector2<T>destvec;
      ElementwiseProduct(argvec, destvec);
      return destvec;
    }


    /* elementwise division JW */
    inline void ElementwiseDivision(const Vector2<T> &argvec,
      Vector2<T> &destvec) const
    {

      if (argvec[0]==0.0) BEXCEPTION("Division by zero !");
      if (argvec[1]==0.0) BEXCEPTION("Division by zero !");
      destvec[0] = data_[0] / argvec[0];
      destvec[1] = data_[1] / argvec[1];
    }

    inline Vector2<T> ElementwiseDivision(const Vector2<T>& argvec) const
    {
      Vector2<T>destvec;
      ElementwiseDivision(argvec, destvec);
      return destvec;
    }



    /** @brief outer product, constructs a matrix (e.g. for covariance).
    @author koeser 08/2005 (cloned from vector3) */
    inline void OuterProduct(const BIAS::Vector2<T> &v,
      BIAS::Matrix2x2<T>& mat) const
    {
      mat[0][0] = data_[0]*v[0];
      mat[0][1] = data_[0]*v[1];
      mat[1][0] = data_[1]*v[0];
      mat[1][1] = data_[1]*v[1];
    }

    inline Matrix2x2<T> OuterProduct(const BIAS::Vector2<T> &v) const
    { Matrix2x2<T> m; OuterProduct(v, m); return m; }


    /** Return the L1 norm: |a| + |b|
        @author Ingo Thomsen
        @date 04/11/2002
        @status untested **/
    inline T NormL1() const {
      return (data_[0] > 0 ? data_[0] : data_[0] * -1)
        + (data_[1] > 0 ? data_[1] : data_[1] * -1);
    }

    /** Return the L2 norm: sqrt(a^2 + b^2)
        @author Ingo Thomsen
        @date 04/11/2002
        @status untested **/
    inline double NormL2() const {
      return sqrt( (double) ( data_[0] * data_[0] + data_[1] * data_[1] ) );
    }

    /// @brief return euclidean length @author JW
    inline double Length() const
    { return NormL2(); }

    /**
     * @brief Vector2<T>::Normalize
     */
    inline void Normalize(){
      const T len = (T) NormL2();
      if(len > std::numeric_limits<T>::epsilon()){
        data_[0] /= len;
        data_[1] /= len;
      }

    }

    /** Return the euclidean distance of 2 vectors.
        @author Birger Streckel
        @date 08/2002
    **/
    inline double Dist(const Vector2<T> &vec) const {
      return sqrt( (double) ( (data_[0]-vec[0])*(data_[0]-vec[0]) +
                              (data_[1]-vec[1])*(data_[1]-vec[1]) ));
    }

    /** Return the squared distance of 2 vectors.
        @author djung, untested
        @date 2009/6
    **/
    inline double SqrDist(const Vector2<T> &vec) const
    {
      return static_cast<double> ((data_[0]-vec[0])*(data_[0]-vec[0]) +
                                    ( data_[1]-vec[1])*(data_[1]-vec[1]));
    }

    /** Return the maximum distance of 2 vectors.
        @author djung, untested
        @date 2009/6
    **/
    inline T DistLInf(const Vector2<T> &vec) const
    {
      return max(abs((T)(data_[0]-vec[0])), abs((T)(data_[1]-vec[1])));
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////// Comparison operators using scalars //////////////////
    ///////////////////////////////////////////////////////////////////////


    /** Comparison operator 'equal'
        @author Ingo Thomsen
        @status tested **/
    inline bool operator== (const Vector2<T>& arg) const {
      return ( (data_[0] == arg.data_[0]) && (data_[1] == arg.data_[1]) );
    }

    /** Comparison operator 'not equal'
        @author Ingo Thomsen
        @status tested **/
    inline bool operator!= (const Vector2<T>& arg) const {
      return ( (data_[0] != arg.data_[0]) || (data_[1] != arg.data_[1]) );
    }



    /////////////////////////////////////////////////////////////////////
    //////////////////// Basic Operators using scalars //////////////////
    /////////////////////////////////////////////////////////////////////


    /** Addition (in place) of an scalar
        @author Ingo Thomsen **/
    inline void AddIP (const T& scalar) {
      Add(scalar, *this);
    }

    /** Addition with a scalar, storing results in destionation vector
        @author Ingo Thomsen **/
    inline void Add(const T& scalar, Vector2<T>& dest) const{
      dest.data_[0] = data_[0] + scalar;
      dest.data_[1] = data_[1] + scalar;
    }

    /** Substraction (in place) of an scalar
        @author Ingo Thomsen **/
    inline void SubIP (const T& scalar) {
      Sub(scalar, *this);
    }

    /** Substraction with a scalar, storing results in destionation vector
        @author Ingo Thomsen **/
    inline void Sub (const T& scalar, Vector2<T>& dest) const {
      dest.data_[0] = data_[0] - scalar;
      dest.data_[1] = data_[1] - scalar;
    }

    /** Multiplication (in place) of an scalar
        @author Ingo Thomsen **/
    inline void MultiplyIP (const T& scalar) {
      Multiply(scalar, *this);
    }

    /** Multiplication with a scalar, storing results in destionation vector
        @author Ingo Thomsen **/
    inline void Multiply (const T& scalar, Vector2<T>& dest) const {
      dest.data_[0] = data_[0] * scalar;
      dest.data_[1] = data_[1] * scalar;
    }

    /** Division (in place) of an scalar
        @author Ingo Thomsen **/
    inline void DivideIP (const T& scalar) {
      Divide(scalar, *this);
    }

    /** elementwise Division (in place) by a vector
    @author JW **/
    inline void DivideIP (const Vector2<T>& vec) {
      Divide(vec, *this);
    }


    /** Division with a scalar, storing results in destination vector
        @author Ingo Thomsen **/
    inline void Divide (const T& scalar, Vector2<T>& dest) const {

      dest.data_[0] = data_[0] / scalar;
      dest.data_[1] = data_[1] / scalar;
    }

    /* elementwise division by vector @author JW */
    inline void Divide (const Vector2<T>& vec, Vector2<T>& dest) const {

      this->ElementwiseDivision(vec, dest);
    }


    /////////////////////////////////////////////////////////////////////
    //////////////////// Basic Operators using vectors //////////////////
    /////////////////////////////////////////////////////////////////////

    /** in place adding
        @author Ingo Thomsen
        @status tested **/
    inline void AddIP(const Vector2<T> &argvec) {
      Add(argvec, *this);
    }


    /** adding of two vectors, storing the result in destvec
        @author Ingo Thomsen
        @status tested **/
    inline void Add(const Vector2<T> &argvec, Vector2<T> &destvec) const {
      destvec[0] = data_[0] + argvec[0];
      destvec[1] = data_[1] + argvec[1];
    }

    /** in place subtraction
        @author Ingo Thomsen
        @status tested **/
    inline void SubIP(const Vector2<T> &argvec) {
      Sub(argvec, *this);
    }


    /** subtracting of two Vectors, storing the result in destvec
        @author Ingo Thomsen
        @status tested **/
    inline void Sub(const Vector2<T> &argvec, Vector2<T> &destvec) const {
      destvec[0] = data_[0] - argvec[0];
      destvec[1] = data_[1] - argvec[1];
    }

    inline void Fill(T value)
    { data_[0] = data_[1] = value; };


    /** coordinate transform
        compute the euclidean coord p=(x,y)
        for (this) given polarcoord (radius, phi) with
        radius : distance to center,
        phi    : radian angle between positive x-axis and p=(x,y)
        assume (x,z) is in orthogonal right-hand-system.
        @author Jan Woetzel 09/2003 */
    BIAS::Vector2<T> CoordPolarToEuclidean() const;

    // deprecated version!
    inline BIAS::Vector2<T> CoordPolarToEuclidian() const {
      return this->CoordPolarToEuclidean();
    };

    /** coordinate transform
        compute the polar coordinates (radius, phi) for (this) given
        eucldian point (x,y).  with
        radius : distance to center,
        phi    : radian angle between positive x-axis and p=(x,y)
        assume (x,z) is in orthogonal right-hand-system.
        @author Jan Woetzel 09/2003 */
    BIAS::Vector2<T> CoordEuclideanToPolar() const;

    // deprecated version!
    BIAS::Vector2<T> CoordEuclidianToPolar() const {
      return this->CoordEuclideanToPolar();
    };

    /** method to load directly from a given filename.
	internally using stream operator
	@author Jan Woetzel 09/2005
	@return false in case of error, true in case of success  */
    bool Load(const std::string & filename);

    /** method to save directly to a given filename.
	internally using stream operator
	@author Jan Woetzel 09/2009
	@return false in case of error, true in case of success */
    bool Save(const std::string & filename) const;

#ifdef WIN32
    inline T abs(const T a) const {
      T b=(a<0) ? (T)(-1) : (T)(1);
      return a*b;
    }

    inline T max(const T a, const T b) const {
      return (a<b) ? b : a;
    }
#endif


  protected:
	  // the 2 elements of the vector without additional pointers etc...
	  // it is the *same* as n individual members, so there is *NO* additional pointer
	  // in particular an array of Vector2 is the same as one big array of T (JW)
    T data_[VECTOR2SIZE];	// the elements of the vector

  };
  // ==============================================================
  // ===  END OF CLASS Vector2  ===================================
  // ==============================================================




  // --------------- stream I/O ---------------------------------------


  /** @relates Vector2
  @brief Output with streams
  Specialized for uc numerical display
  @author Jan Woetzel */
  inline
    std::ostream& operator<<(std::ostream & os, const Vector2<char> & vec)
  {
    if (!IsConsoleStream(os))
    {
      os << VECTOR2SIZE << NL;
      for (int row = 0; row < VECTOR2SIZE; row++) {
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" "<<NL;
      }
    } else {
      os << "[ ";
      for (int row = 0; row < VECTOR2SIZE; row++)
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" ";
      os << "] ";
    }
    return os;
  }

   inline
    std::ostream& operator<<(std::ostream & os, const Vector2<unsigned char> & vec)
  {
    if (!IsConsoleStream(os))
    {
      os << VECTOR2SIZE << NL;
      for (int row = 0; row < VECTOR2SIZE; row++) {
        os<<std::setw(3)<<(unsigned int)vec.GetData()[row]<<" "<<NL;
      }
    } else {
      os << "[ ";
      for (int row = 0; row < VECTOR2SIZE; row++)
        os<<std::setw(3)<<(unsigned int)vec.GetData()[row]<<" ";
      os << "] ";
    }
    return os;
  }


  /** @relates Vector2
  @brief Output with streams
  @author Jan Woetzel */
  template <class T>
    inline
  std::ostream& operator<<(std::ostream & os, const Vector2<T>& vec)
  {
    if (!IsConsoleStream(os))
    {
      os << VECTOR2SIZE << NL;
      for (int row = 0; row < VECTOR2SIZE; row++) {
        os<<std::setprecision( DecimalsToStore<T>()  );
        os<<vec.GetData()[row]<<" "<<NL;
      }
    } else {
      // cout or cerr
      os<<"[ ";
      for (int row = 0; row < VECTOR2SIZE; row++){
        os<<std::setprecision(CONSOLE_PRECISION_DIGITS);
        os<<vec.GetData()[row]<<" "; // less precise display in console
      }
      os<<"] ";
    }
    return os;
  }


  /** @relates Vector2
  @brief Input with streams
  @author Jan Woetzel (03/01/2002) **/
  inline
  std::istream& operator>>(std::istream &is, Vector2 <char>& vec)
  {
    int N=0;			// first argument of the stream is the size
    is >> N;			// read size first to be compatible to Vector<>
    if (!(N == (int)vec.Size())) {
      BIASERR("can't assign a Vector2<T> with a dim " << N << " vector");
    } else {
      int tmp(0);
      for (int row = 0; row < N; row++) {
        is >> tmp;
        BIASASSERT(tmp>=std::numeric_limits<char>::min() );
        BIASASSERT(tmp<=std::numeric_limits<char>::max() );
        vec.GetData()[row]=(char)tmp;
      };
    };
    return is;
  }

  inline
  std::istream& operator>>(std::istream &is, Vector2 <unsigned char>& vec)
  {
    int N=0;			// first argument of the stream is the size
    is >> N;			// read size first to be compatible to Vector<>
    if (!(N == (int)vec.Size())) {
      BIASERR("can't assign a Vector2<T> with a dim " << N << " vector");
    } else {
      int tmp(0);
      for (int row = 0; row < N; row++) {
        is >> tmp;
        BIASASSERT(tmp>=std::numeric_limits<unsigned char>::min() );
        BIASASSERT(tmp<=std::numeric_limits<unsigned char>::max() );
        vec.GetData()[row]=(unsigned char)tmp;
      };
    };
    return is;
  }



  /** @relates Vector2
  @brief Input with streams
  @author Jan Woetzel (03/01/2002) **/
  template <class T>
    inline
  std::istream& operator>>(std::istream &is, Vector2 <T>& vec)
  {
    int N=0;			// first argument of the stream is the size
    is >> N;			// read size first to be compatible to Vector<>
    if (!(N == (int)vec.Size())) {
      // wrong size
      BIASERR("can't assign a Vector2<T> with a dim " << N << " vector");
    } else {
      // correct size, sequential read elementwise
      for (int row = 0; row < N; row++) {
        is >> vec.GetData()[row];
      };
    };
    return is;
  }

  // ------------------ operators with vectors seee Operators.hh ---------------------------



  /** add operator for two Vectors
      @author Ingo Thomsen, tested **/
  template <class T> BIASMathBase_EXPORT
  inline Vector2<T>& operator+=(Vector2<T> &vec, const Vector2<T> &argvec) {
    vec.Add(argvec, vec);
    return vec;
  }

  /** sub operator for two Vectors
      @author Ingo Thomsen
      @status tested **/
  template <class T>
  inline Vector2<T> &operator-= (Vector2<T> &vec, const Vector2<T> &argvec) {
    vec.Sub(argvec, vec);
    return vec;
  }

    /** elementwise division @author JW */
  template <class T> BIASMathBase_EXPORT
  inline Vector2<T>& operator/=(Vector2<T> &vec, const Vector2<T> &argvec) {
    vec.DivideIP(argvec, vec);
    return vec;
  }

  // --------------- operators with scalars -----------------------------


  /** Addition operator with scalar argument
      @author Ingo Thomsen, tested */
  template <class T>
  inline Vector2<T>& operator+=(Vector2<T>& vec, const T& scalar) {
    vec.AddIP(scalar);
    return vec;
  }

  /** Substraction operator with scalar argument
      @author Ingo Thomsen, tested **/
  template <class T>
  inline Vector2<T>& operator-=(Vector2<T>& vec, const T& scalar) {
    vec.SubIP(scalar);
    return vec;
  }

  /** Multiplication operator with scalar argument
      @author Ingo Thomsen, tested **/
  template <class T>
  inline Vector2<T>& operator*= (Vector2<T>& vec, const T& scalar) {
    vec.MultiplyIP(scalar);
    return vec;
  }

  /** Division operator with scalar argument
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T>& operator/= (Vector2<T>& vec, const T& scalar) {
      vec.DivideIP(scalar);
      return vec;
    }


} // namespace BIAS
#endif

