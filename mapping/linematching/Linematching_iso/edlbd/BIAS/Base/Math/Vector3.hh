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


#ifndef _Vector3_hh_
#define _Vector3_hh_

#include <bias_config.h>
#include <BIAS_Definitions.hh>

#include <iostream>
#include <vector>
//#include <limits>
//#include <float.h>

#include <Base/Debug/Error.hh>
#include <Base/Debug/Exception.hh>
#include <Base/Math/Vector.hh>
#include <Base/Math/Matrix3x3.hh>
#include <Base/Math/Operators.hh>
#include <Base/Math/Utils.hh>

#define VECTOR3_SIZE 3

namespace BIAS {

  /** @class Vector3
      @ingroup g_math
      @brief class Vector3 contains a Vector of fixed dim. 3

      It's elment type is templated.
      manual loop unrolling is used if possible.
      The Vector is in row-major order (3 rows, 1 column)
      the indizes begin with zero (to size-1)

      operators are no longer class members
      removed memory leak by returning reference in operators
      removed inline code from class definition, woelk 01 2003

      @author Jan Woetzel, woelk
  **/

  // forward declarations
  // DLL JW
  //template <class T> class BIASMathBase_EXPORT Matrix3x3;



  template <class T>
  class BIASMathBase_EXPORT Vector3 {
  public:
    Vector3() {};

    ~Vector3() {};

    /**	copy constructor
        @author Jan Woetzel (03/05/2002)**/
    inline Vector3(const Vector3<T>& vec) { *this = vec; };

    /** assignment with a constant value for all elements
        @author Jan Woetzel (03/01/2002) **/
    explicit inline Vector3(const T & scalar) { Set(scalar); };

    /**	assignment with an array of values
        which is copied into this ones class members
        @author Jan Woetzel (02/28/2002) **/
    explicit inline Vector3(const T* pv) { Set(pv); };

    /**	constructor with element assignment
        @author Jan Woetzel
        @status alpha (03/01/2002) **/
    explicit inline Vector3(char *s);

    /** constructor with element assigment using explicit values
        @author Ingo Thomsen, tested (03/04/2002)**/
    inline Vector3(const T &x, const T &y, const T &z) {  Set(x, y, z); };

    /** cast constructor
        @author Jan Woetzel, untested (03/22/2002) **/
    Vector3(const Vector<T>& v);


    /** @name Set Functions
        @{
    */
    /** copy the array of vectorsize beginning at *T to this->data_
        @author Jan Woetzel, untested (02/28/2002) **/
    inline void Set(const T *pv);

    /** set all elements to a scalar value
        @author Jan Woetzel, untested (02/28/2002) **/
    inline void Set(const T& scalar) { Set(scalar, scalar, scalar); };

    /** set elementwise with given scalar values
        @author Jan Woetzel
        @status alpha (02/28/2002) **/
    inline void Set(const T& x, const T& y, const T& z);

    /** Sets this form elements of Vector<T>. */
    inline void Set(const Vector<T> &vec);

    /** set all values to 0
        @author Jan Woetzel 01/2003 **/
    inline void SetZero();
    /// stl conform interface
    inline void clear();

    // @}

    /** @name Get Functions
        @{
    */
    /**	@return the size of this vector
        @author Jan Woetzel (02/28/2002) **/
    inline const unsigned int Size() const { return size(); };
    // stl conform interface
    inline const unsigned int size() const { return VECTOR3_SIZE; };
    // compatibility interface
    inline const unsigned int GetNumElements() const { return size(); };

    /** @return true iff all elements are equal zero JW */
    inline bool IsZero() const;

    /** @brief returns the Euclidean Length of the Vector @author Jan Woetzel */
    inline double Length() const
    { return NormL2(); };

    /** get the data pointer
        the member function itself is const (before {..}) because it doesn't
        change the this object.
        @return the const pointer to the data array for reading
        @author Jan Woetzel
        @status untested (02/28/2002) **/
    inline const T *GetData() const { return data_; };
    inline T *GetData() { return data_; };

    /** Return the L1 norm: |a| + |b| + |c|
        @author Ingo Thomsen
        @date 04/11/2002
        @status tested **/
    inline T NormL1() const;

    /** the L2 norm sqrt(a^2 + b^2 + c^2)
        @author woelk 01 2003 */
    inline double NormL2() const;

    /** Return the euclidean distance of 2 vectors.
        @author Birger Streckel
        @date 08/2002
    **/
    inline double Dist(const Vector3<T> &vec) const;

    /** Return the L inf distance of 2 vectors.
        @author djung
        @date 2009/07 untested
    **/
    inline double DistLinf(const Vector3<T> &vec) const;

    // @}

    /** @name Normalization
        @{
    */
    /**	@brief normalize this vector to length 1
        @author Jan Woetzel / koeser
        @return reference to updated (*this)
        @date 04/25/2002 **/
    inline Vector3<T>& Normalize();

    /** @brief normalizes the vector to 1 and transfroms the associated
        covariance matrix
        @author woelk 03/2006 */
    Vector3<T>& Normalize(Matrix3x3<T>& cov);


    /**	@brief return a normalized vector of this

    norm. to length 1
    @author Jan Woetzel
    @date 06/2003 **/
    BIAS::Vector3<T> GetNormalized() const;

    // @}


    /** @name Arithmetic
        @{
    */
    /** cross product of two vectors
        destvec = this x argvec
        @author Ingo Thomsen, Jan Woetzel
        @status alpha **/
    inline void CrossProduct(const Vector3<T>& argvec,
                             Vector3<T>& destvec) const;

    /** @return cross product = this x argvec
        @author Jan Woetzel
        @status alpha **/
    inline Vector3<T> CrossProduct(const Vector3<T>& argvec) const
    {  Vector3<T> destvec; CrossProduct(argvec, destvec); return destvec; };

    /** scalar product (=inner product) of two vectors,
        storing the result in result
        @author Jan Woetzel **/
    inline void ScalarProduct(const Vector3<T>& argvec, T& result) const;

    /** scalar product (=inner product) of two vectors returns a scalar
        @author Jan Woetzel, Ingo Thomsen
        @status alpha **/
    inline T ScalarProduct(const Vector3<T>& argvec) const;

    /** multiply two vectors elementwise and store the result vector to destvec
        @author Jan Woetzel
        @status alpha (03/06/2002) **/
    inline void ElementwiseProduct(const Vector3<T>& argvec,
                                   Vector3<T>& destvec) const;

    /** multiply two vectors elementwise and return the result vector
        @author Jan Woetzel (03/06/2002) **/
    inline Vector3<T> ElementwiseProduct(const Vector3<T>& argvec) const
    {
      Vector3<T>destvec;
      ElementwiseProduct(argvec, destvec);
      return destvec;
    }

    /* divide elementwise @author JW */
    inline void ElementwiseDivision(const Vector3<T>& argvec,
      Vector3<T>& destvec) const;

    /** divide elementwise @author Jan Woetzel */
    inline Vector3<T> ElementwiseDivision(const Vector3<T>& argvec) const
    {
      Vector3<T>destvec;
      ElementwiseDivision(argvec, destvec);
      return destvec;
    }


    /** Addition (in place) of an scalar
        @author Ingo Thomsen **/
    inline void AddIP(const T& scalar) { Add(scalar, *this); }

    /** Addition with a scalar, storing results in destionation vector
        @author Ingo Thomsen **/
    inline void Add(const T& scalar, Vector3<T>& dest) const;

    /** Substraction (in place) of an scalar
        @author Ingo Thomsen **/
    inline void SubIP(const T& scalar) { Sub(scalar, *this); }

    /** Substraction with a scalar, storing results in destination vector
        @author Ingo Thomsen **/
    inline void Sub(const T& scalar, Vector3<T>& dest) const;

    /** Multiplication (in place) of an scalar
        @author Ingo Thomsen **/
    inline void MultiplyIP(const T& scalar) { Multiply(scalar, *this); }
    inline void MultIP(const T& scalar) { MultiplyIP(scalar); }

    /** Multiplication with a scalar, storing results in destination vector
        @author Ingo Thomsen **/
    inline void Multiply(const T& scalar, Vector3<T>& dest) const;
    inline void Mult(const T& scalar, Vector3<T>& dest) const
    { Multiply(scalar, dest); }

    /** Division (in place) of an scalar
        @author Ingo Thomsen**/
    inline void DivideIP(const T& scalar);
    inline void DivIP(const T& scalar) { DivideIP(scalar); }

    /** Division with a scalar, storing results in destination vector
        @author Ingo Thomsen **/
    inline void Divide(const T& scalar, Vector3<T>& dest) const ;
    inline void Div(const T& scalar, Vector3<T>& dest) const
    {Divide(scalar, dest);};

    /** in place adding
        @author Ingo Thomsen
        @status tested **/
    inline void AddIP(const Vector3<T>&argvec) { Add(argvec, *this);}

    /** adding of two vectors, storing the result in destvec
        @author Ingo Thomsen
        @status tested **/
    inline void Add(const Vector3<T>&argvec, Vector3<T>&destvec) const;

    /** in place substraction
        @author Ingo Thomsen
        @status tested **/
    inline void SubIP(const Vector3<T>& argvec) { Sub(argvec, *this); }

    /** subtracting of two Vectors, storing the result in destvec
        @author Ingo Thomsen
        @status tested **/
    inline void Sub(const Vector3<T>&argvec, Vector3<T>&destvec) const;

    // @}

    /** @name Misc
        @{
    */

    /** outer product, constructs a matrix.
    Often written as (*this) * v^T for col vectors
    made it fast
    @author Daniel Grest, Sept 2003, woelk 07/2004
    @status tested */
    void OuterProduct(const Vector3<T> &v, Matrix3x3<T>& res) const;

    inline Matrix3x3<T> OuterProduct(const Vector3<T> &v) const
    {
      Matrix3x3<T> m;
      OuterProduct(v, m);
      return m;
    }

    /** constructs a skew symmetric 3x3 matrix from (*this),
        which can be used instead of the cross product
        @author koeser 09/2003 */
    Matrix<T> GetSkewSymmetricMatrix() const;


    /** coordinate transfrom.
        compute the euclidean coord p=(x,y,z) for (this) given sphere
        coord sph=(radius, phi, theta)
        with
        radius : distance to center,
        theta  : radian angle between positive z-axis and p=(x,y,z). (0-pi)
        phi    : radian angle between positive x-axis and projection
        of p=(x,y,z) into XY plane. (0-2pi)
        assume (x,y,z) is in orthogonal right-hand-system.
        @author Jan Woetzel 09/2003 */
    BIAS::Vector3<T> CoordSphereToEuclidean() const;
    BIAS::Vector3<T> PolarToCartesian() const {
      return this->CoordSphereToEuclidean();
    };
    // deprecated version!
    inline BIAS::Vector3<T> CoordSphereToEuclidian() const {
      return this->CoordSphereToEuclidean();
    };

    /** coordinate transform.
        compute the sphere coordinates(r, phi, theta) for (this) given
        euclidean point p=(x,y,z)
        with
        radius : distance to center,
        theta  : radian angle between positive z-axis and p=(x,y,z). (0-pi)
        phi    : radian angle between positive x-axis and projection
        of p=(x,y,z) into XY plane. (0-2pi)
        assume (x,y,z) is in orthogonal right-hand-system.
        @author Jan Woetzel 09/2003 */
    BIAS::Vector3<T> CoordEuclideanToSphere() const;
    BIAS::Vector3<T> CartesianToPolar() const {
      return this->CoordEuclideanToSphere();
    }
    // deprecated version!
    inline BIAS::Vector3<T> CoordEuclidianToSphere() const {
      return this->CoordEuclideanToSphere();
    };

    /** r for polar/sphere content JW */
    inline T& Radius()
    { return (*this)[0]; };
    inline T Radius() const
    { return (*this)[0]; };

    inline T& Phi()
    { return (*this)[1]; };
    inline T Phi() const
    { return (*this)[1]; };

    inline T& Theta()
    { return (*this)[2]; };
    inline T Theta() const
    { return (*this)[2]; };

    /** reads the TC part of a BOG file
        which is used by Daimler Chrysler
        for storing Camera center position.
        @return 0 for success, negative for errror
        @author Jan Woetzel 03/2005 */
    int LoadBogTC(const std::string & filename);

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


    // @}

    /** @name Operators
        @{
    */
    /** assignment operator
        @author woelk 01 2003 **/
    inline Vector3<T>& operator=(const Vector3<T>& vec)
    { memcpy((void *)data_, (void *)vec.GetData(), VECTOR3_SIZE*sizeof(T));
    return *this; };

    /** assignment operator
        @author frahm 03 2004 **/
    inline Vector3<T>& operator=(const Vector<T>& vec)
    { if (vec.size() == VECTOR3_SIZE)
      memcpy((void *)data_, (void *)vec.GetData(), VECTOR3_SIZE*sizeof(T));
    return *this; };

    /** access an element  of the vector with 0-based indizes.
        read only (no write)
        member function const because it doesn't change this object
        @author Jan Woetzel
        @status untested (03/01/2002) **/
    inline const T& operator[] (const int i) const;

    /** access an element  of the vector with 0-based indizes.
        write allowed
        @author Jan Woetzel
        @status untested (03/01/2002) **/
    inline T& operator[] (const int i);

    /** Comparison operator 'equal'
        @author Ingo Thomsen
        @status tested **/
    inline bool operator==(const Vector3<T>& arg) const
    { return ((data_[0]==arg.data_[0])&&(data_[1]==arg.data_[1])&&(data_[2]==arg.data_[2])); }

    /** Comparison operator 'not equal'
        @author woelk 01 2003 **/
    inline bool operator!=(const Vector3<T>& arg) const
    { return ! operator==(arg); }

    // @}

  protected:
	  // the 3 elements of the vector without additional pointers etc...
	  // it is the *same* as 3 individual members, so there is *NO* additional pointer
	  // in particular an array of Vector3 is th3 same as one big array of T (JW)
    T data_[VECTOR3_SIZE];

  };
  // ======================================================
  // === END OF CLASS DECLARATION =========================
  // ======================================================


  template <class T>
  Vector3<T>::Vector3(char *s) {
    std::istringstream inst(s);
    inst >> data_[0];
    inst >> data_[1];
    inst >> data_[2];
  }

  template <class T>
  void Vector3<T>::Set(const T *pv) {
    // better use memcopy ?
    data_[0] = pv[0];
    data_[1] = pv[1];
    data_[2] = pv[2];
  }

  template <class T>
  void Vector3<T>::Set(const T& x, const T& y, const T& z) {
    data_[0] = x;
    data_[1] = y;
    data_[2] = z;
  }

  template <class T>
  void Vector3<T>::Set(const Vector<T>& vec) {
    if (vec.size() != VECTOR3_SIZE){
      BIASERR("The vector for initialization of Vector3 has not length 3");
    } else {
      data_[0] = vec[0];
      data_[1] = vec[1];
      data_[2] = vec[2];
    }
  }

  // JW 01/2003
  template <class T>
  void Vector3<T>::SetZero() {
    Set( (T)0);
  }

  template <class T>
  void Vector3<T>::clear() {
    SetZero();
  }

  // JW 09/2003
  template <class T>
  bool
  Vector3<T>::IsZero() const {
    return ( (data_[0]==(T)0)
             && (data_[1]==(T)0)
             && (data_[2]==(T)0) );
  }



  template <class T>
  const T& Vector3<T>::operator[] (const int i) const {
    BIASASSERT(0 <= i);
    BIASASSERT(i < VECTOR3_SIZE);
    return data_[i];
  }

  template <class T>
  T& Vector3<T>::operator[] (const int i) {
    BIASASSERT(0 <= i);
    BIASASSERT(i < VECTOR3_SIZE);
    return data_[i];
  }

  template <class T>
  void Vector3<T>::CrossProduct(const Vector3<T>& argvec,
                                Vector3<T>& destvec) const {
    destvec[0] = data_[1] * argvec[2] - data_[2] * argvec[1];
    destvec[1] = data_[2] * argvec[0] - data_[0] * argvec[2];
    destvec[2] = data_[0] * argvec[1] - data_[1] * argvec[0];
  }


  template <class T>
  void Vector3<T>::ScalarProduct(const Vector3<T>& argvec, T& result) const {
    result = data_[0] * argvec[0];
    result += data_[1] * argvec[1];
    result += data_[2] * argvec[2];
  }

  template <class T>
  T Vector3<T>::ScalarProduct(const Vector3<T>& argvec) const {
    T result;
    ScalarProduct(argvec, result);
    return result;
  }

  template <class T>
    void Vector3<T>::ElementwiseProduct(const Vector3<T>& argvec,
    Vector3<T>& destvec) const {
      destvec[0] = data_[0] * argvec[0];
      destvec[1] = data_[1] * argvec[1];
      destvec[2] = data_[2] * argvec[2];
    }

  template <class T>
    void Vector3<T>::ElementwiseDivision(const Vector3<T>& argvec,
    Vector3<T>& destvec) const {
      destvec[0] = data_[0] / argvec[0];
      destvec[1] = data_[1] / argvec[1];
      destvec[2] = data_[2] / argvec[2];
    }

  template <class T>
  double Vector3<T>::NormL2() const {
    return sqrt(double(data_[0]*data_[0] + data_[1]*data_[1]
                       + data_[2]*data_[2]));
  }

  template <class T>
  double Vector3<T>::Dist(const Vector3<T> &vec) const {
    return sqrt(double((data_[0]-vec[0])*(data_[0]-vec[0]) +
                       (data_[1]-vec[1])*(data_[1]-vec[1]) +
                       (data_[2]-vec[2])*(data_[2]-vec[2])));
  }

  template <class T>
  double Vector3<T>::DistLinf(const Vector3<T> &vec) const
  {
    double
      res = 0.,
      tmp = 0.;
    for(unsigned int i = 0; i < 3; i++)
    {
      tmp = fabs(static_cast<double> (data_[i] - vec[i]));
      if(res < tmp)
      {
        res = tmp;
      }
    }
    return res;
  }

  template <class T>
  Vector3<T>& Vector3<T>::Normalize(){
    const T act_length = (T) NormL2(); // euclidean length
    // check if not zero
    if (act_length > std::numeric_limits<T>::epsilon() )
    {
      // divide each element by length
      this->DivideIP(act_length);
    } else {
      //BEXCEPTION("no normalization possible because act_length="<<act_length);
    }
    return (*this);
  }

  template <class T>
  T Vector3<T>::NormL1() const {
    return (data_[0] > 0 ? data_[0] : data_[0] * -1)
      + (data_[1] > 0 ? data_[1] : data_[1] * -1)
      + (data_[2] > 0 ? data_[2] : data_[2] * -1);
  }

  template <class T>
  void Vector3<T>::Add(const T& scalar, Vector3<T>& dest) const {
    dest.data_[0] = data_[0] + scalar;
    dest.data_[1] = data_[1] + scalar;
    dest.data_[2] = data_[2] + scalar;
  }

  template <class T>
  void Vector3<T>::Add(const Vector3<T>& argvec, Vector3<T>& destvec) const {
    destvec[0] = data_[0] + argvec[0];
    destvec[1] = data_[1] + argvec[1];
    destvec[2] = data_[2] + argvec[2];
  }

  template <class T>
  void Vector3<T>::Multiply (const T& scalar, Vector3<T>& dest) const {
    dest.data_[0] = data_[0] * scalar;
    dest.data_[1] = data_[1] * scalar;
    dest.data_[2] = data_[2] * scalar;
  }

  template <class T>
  void Vector3<T>::Sub(const T& scalar, Vector3<T>& dest) const {
    dest.data_[0] = data_[0] - scalar;
    dest.data_[1] = data_[1] - scalar;
    dest.data_[2] = data_[2] - scalar;
  }

  template <class T>
  void Vector3<T>::Sub(const Vector3<T>&argvec, Vector3<T>&destvec) const {
    destvec[0] = data_[0] - argvec[0];
    destvec[1] = data_[1] - argvec[1];
    destvec[2] = data_[2] - argvec[2];
  }

  template <class T>
  void Vector3<T>::DivideIP(const T& scalar) {
    Divide(scalar, *this);
  }

  template <class T>
  void Vector3<T>::Divide(const T& scalar, Vector3<T>& dest) const {
    if (scalar == 0.0)
      BEXCEPTION("Division by Zero!");
    dest.data_[0] = data_[0] / scalar;
    dest.data_[1] = data_[1] / scalar;
    dest.data_[2] = data_[2] / scalar;
  }


  //////////////////////////////////////////////////////////////////////////
  // operators
  // woelk 01 2003
  /////////////////////////////////////////////////////////////////////////

 /** @relates Vector3 */
  template <class T>
  inline Vector3<T>& operator*=(Vector3<T>& vec, const T& scalar) {
    vec.MultiplyIP(scalar);
    return vec;
  }

  /** @relates Vector3
      Addition operator with scalar argument
      @author Ingo Thomsen, woelk
      @status tested **/
  template <class T>
  inline Vector3<T>& operator+=(Vector3<T>& vec, const T& scalar) {
    vec.AddIP(scalar);
    return vec;
  }

  /** @relates Vector3
      add operator for two Vectors
      @author Ingo Thomsen, woelk
      @status tested **/
  template <class T>
  inline Vector3<T>& operator+=(Vector3<T>& vec, const Vector3<T>& argvec) {
    vec.AddIP(argvec);
    return vec;
  }


  /** @relates Vector3
      Substraction operator with scalar argument
      @author Ingo Thomsen
      @status tested   **/
  template <class T>
  inline Vector3<T>& operator-=(Vector3<T>& vec, const T& scalar) {
    vec.SubIP(scalar);
    return vec;
  }

  /** @relates Vector3
      sub operator for two Vectors
      @author Ingo Thomsen, tested **/
  template <class T>
  inline Vector3<T>& operator-=(Vector3<T>& vec, const Vector3<T>&argvec) {
    vec.SubIP(argvec);
    return vec;
  }


  /** @relates Vector3
      Division operator with scalar argument
      @author Ingo Thomsen
      @status tested **/
  template <class T>
  inline Vector3<T>& operator/=(Vector3<T>& vec, const T& scalar) {
    vec.DivideIP(scalar);
    return vec;
  }

  /////////////////////
  // stream io
  /////////////////////////

  /**
  @relates Vector3
  @brief Output with streams
  specialization for numerical display of unsigned char instead of alphabetically.
  fout format should be consistent with template implementation and Vector<>
  \todo refactor into parameterized Print class member method
  @author Jan Woetzel */
  inline
    std::ostream &
    operator<<(std::ostream& os, const BIAS::Vector3<unsigned char>& vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR3_SIZE << NL;
      for (int row = 0; row < VECTOR3_SIZE; row++) {
        os << (unsigned int)vec.GetData()[row] << " " << NL;
      }
    } else {
      // shorter and aligned for console
      os << "[ ";
      for (int row = 0; row < VECTOR3_SIZE; row++)
        os<<std::setw(3)<<(unsigned int)vec.GetData()[row]<<" ";
      os<<"] ";
    }
    return os;
  }

  inline
    std::ostream &
    operator<<(std::ostream& os, const BIAS::Vector3<char>& vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR3_SIZE << NL;
      for (int row = 0; row < VECTOR3_SIZE; row++) {
        os << (unsigned int)vec.GetData()[row] << " " << NL;
      }
    } else {
      // shorter and aligned for console
      os << "[ ";
      for (int row = 0; row < VECTOR3_SIZE; row++)
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" ";
      os<<"] ";
    }
    return os;
  }


  /** @relates Vector3
  Output with streams. Usese different format for cout,cerr and other ostreasm
  to be consistenv with Vector<> but have pretty display in console.
  @author Jan Woetzel, JMF  (08/08/2002) **/
  template  <class T>
    inline
    std::ostream &
    operator<<(std::ostream& os, const BIAS::Vector3<T>& vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR3_SIZE << NL;
      for (int row = 0; row < VECTOR3_SIZE; row++) {
        os<<std::setprecision( DecimalsToStore<T>()  ); // at least digits10
        // print out one element per line
        os<<vec.GetData()[row] << " " << NL;
      }
    } else {
      // cout or cerr
      os<<"[ ";
      for (int row = 0; row < VECTOR3_SIZE; row++){
        os<<std::setprecision(CONSOLE_PRECISION_DIGITS);
        os<<vec.GetData()[row] << " ";
      }
      os<<"] ";
    }
    return os;
  }



  /**
  @relates Vector3
  @brief Input with streams
  specialization for numerical display of unsigned char instead of alphebetically.
  fout format should be consistent with template implementation and Vector<>
  @author Jan Woetzel */
  inline std::istream &
    operator>>(std::istream & is, Vector3<unsigned char>&vec)
  {
    int N;
    is >> N;
    if (N != VECTOR3_SIZE) {
      BIASERR("can't assign to a Vector3<unsigned char> from a vector with dim " << N );
    } else {
      // correct size, sequential read elementwise
      int tmp(0); //< used for castign between unsignaed char are written int
      for (int row = 0; row < N; row++)
      {
        is >> tmp;
        BIASASSERT(tmp>=std::numeric_limits<unsigned char>::min() );
        BIASASSERT(tmp<=std::numeric_limits<unsigned char>::max() );
        vec.GetData()[row] = (unsigned char)tmp;
      };
    };
    return is;
  }

  inline std::istream &
    operator>>(std::istream & is, Vector3< char>&vec)
  {
    int N;
    is >> N;
    if (N != VECTOR3_SIZE) {
      BIASERR("can't assign to a Vector3<unsigned char> from a vector with dim " << N );
    } else {
      // correct size, sequential read elementwise
      int tmp(0); //< used for castign between unsignaed char are written int
      for (int row = 0; row < N; row++)
      {
        is >> tmp;
        BIASASSERT(tmp>=std::numeric_limits< char>::min() );
        BIASASSERT(tmp<=std::numeric_limits< char>::max() );
        vec.GetData()[row] = (char)tmp;
      };
    };
    return is;
  }


  /** @relates Vector3
  @brief Input with streams
  @author Jan Woetzel  (03/01/2002) **/
  template <class T>
    inline
    std::istream &
    operator>>(std::istream & is, Vector3<T>&vec)
  {
    int N;
    is >> N;
    if (N != VECTOR3_SIZE) {
      BIASERR("can't assign to a Vector3<T> from a vector with dim " << N );
    } else {
      // correct size, sequential read elementwise
      for (int row = 0; row < N; row++) {
        is >> vec.GetData()[row];
      };
    };
    return is;
  }


} // namespace BIAS


#endif // _Vector3_hh_
