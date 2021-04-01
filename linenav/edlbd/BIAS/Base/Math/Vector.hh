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


#ifndef _Vector_hh_
#define _Vector_hh_

#include "bias_config.h" 
#include "math.h"

#include <string>
#include <vector>

#include <Base/Math/tnt/tnt.h>
#include <Base/Math/tnt/vec.h>
#include <Base/Math/Operators.hh>
#include <Base/Math/Utils.hh>

namespace BIAS {

  // forward declaration to avoid mutual inclusion
  template<class T> class BIASMathBase_EXPORT Vector2;
  template<class T> class BIASMathBase_EXPORT Vector3;
  template<class T> class BIASMathBase_EXPORT Vector4;
  template<class T> class BIASMathBase_EXPORT Matrix;

  /**@class Vector
     @ingroup g_math
     @brief class for column vectors with arbitrary size

     class Vector is the vector class that should be used by common 
     BIAS algorithms and data structures.
     It is derived from TNT::Vector to inherit the basic operations and 
     algorithm.
     Special implementations should be done here and NOT in TNT:Vector 
     because the true 'base' Matrix class "TNT::Vector"
     should be interchangeable and aside from this maintained by NIST.

     changed from BIAS to BIAS 11/05/2002 (jw)

     The Vector is in row-major order (=n rows, 1 column)
     @author Jan Woetzel
  **/
  template<class T=double> 
  class BIASMathBase_EXPORT Vector : public TNT::Vector<T> 
  {
  public:

    virtual ~Vector();

    Vector() :TNT::Vector<T>() {};

    explicit Vector(const int rows) : TNT::Vector<T>(rows) {};
  
    explicit Vector(const int rows, const std::string & s);

    /** assignment with a constant value for all elements (=set)
        @author Jan Woetzel
    */
    Vector(int rows, const T & value) :TNT::Vector<T>(rows, value) {};

    Vector(int rows, const T * v) :TNT::Vector<T>(rows, v) {};

    Vector(const Vector<T>& v) :TNT::Vector<T>(v) {};

    /** @author JMF   */
    Vector(const TNT::Vector<T> &v) :TNT::Vector<T>(v) {};

    /** @author fw */
    Vector(const Vector2<T>& v);

    /** @author fw */
    Vector(const Vector3<T>& v);

    /** @author fw */
    Vector(const Vector4<T>& v);

    explicit Vector(const std::vector<T>& v);

    /** assignment operators
        calling corresponding operator from base class "TNT::Vector" if 
        appropriate
        @author Jan Woetzel (02/25/2002) **/
    Vector<T>& operator=(const TNT::Vector<T>& vec);

    /** assignment operator
        calling corresponding operator from base class "TNT::Vector" if 
        appropriate  @author grest */
    Vector<T>& operator=(const Vector<T> &vec)
      { TNT::Vector<T>::operator=(vec); return *this; }

    inline void operator+=(const Vector<T>& vec);

    inline void operator-=(const Vector<T>& vec);

    inline void operator*=(T scalar);

    /** @author woelk 12/2004 */
    inline void operator/=(T scalar);

    inline bool operator==(const Vector<T> &vec) const;

    inline bool operator!=(const Vector<T> &vec) const
    { return !operator==(vec); }

    /** @brief returns the Euclidean Length of the Vector
     */
    inline double Length() const
    { return NormL2(); };

    /** length of the vector */
    inline unsigned int Size() const
    { return this->size(); };

    /** conformance interface JW */
    inline int GetNumElements() const { 
      return this->size(); 
    };

    /** fills complete Vector with scalar value */
    void Fill(const T & scalar);
    inline void Set(const T & scalar) {Fill(scalar);}

    /** equivalent to matrix call */
    inline void SetZero()
    { 
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
      Fill((T)(0)); 
#else
      memset(GetData(), 0, GetNumElements()*sizeof(T)); // JW
#endif
    };

    /// stl conform interface JW
    inline void clear() {
      this->destroy();
    };

    /** @return true if all elements are equal zero JW */
    bool IsZero() const;

    /** scalar product (inner product) of two vectors returning a scalr
        @author Ingo Thomsen
        @status tested(03/11/2002) **/
    inline T ScalarProduct(const Vector<T>& argvec) const;

    /** scalar product (inner product) of two vectors returning a scalar, 
        storing the result in result
        @author Ingo Thomsen
        @status tested(03/11/2002) **/
    inline void ScalarProduct(const Vector<T>& argvec, T& result) const;

    /** outer product, constructs a matrix.
        Often written as v * v^T for col vectors
        @author Daniel Grest, Oct 2002
        @status tested */
    Matrix<T> OuterProduct(const Vector<T> &v) const;

    /** kronecker product
        @author woelk 08/2004 */
    void KroneckerProduct(const Vector<T> &arg, Vector<T>& dst) const;

    /** multiply components with scalar storing result in res
        @author Ingo Thomsen
        @status tested(03/11/2002) **/
    inline void Multiply(const T& scalar, Vector<T>& res) const;

    /** in place multiplication with scalar
        @author Ingo Thomsen
        @status untested **/ 
    inline void MultiplyIP(const T& scalar)
    { Multiply(scalar, *this); };

    /** in place subtracting
        @author Ingo Thomsen
        @status untested **/
    inline void SubIP(const Vector<T>& argmat);

    /** Substraction of vector, storing results in destination vector
        @author woelk 07/2006 **/
    inline void Sub(const Vector<T>& arg, Vector<T>& dest) const;

    /** get the pointer to the data array of the vector 
        (for faster direct memory access)
        @author Jan Woetzel
        @return a pointer to the data array (beginning with GetData[0])
        -added const (jw. 03/12/2002) **/
    inline T *GetData() const 
    { return this->v_; };

    /** Get the pointer to last element of the data array
        @author Ingo Thomsen
        @date 04/11/2002
        @return pointer to last element **/
    inline T *GetDataLast() const 
    { return GetData() + this->size() - 1; };

    /** Return the L1 norm: |a| + |b| + |c| + ...
        @author Ingo Thomsen
        @date 04/11/2002
    **/
    inline T NormL1() const;

    /** Return the L2 norm: sqrt(a^1 + a^2 + ...)
        @author Ingo Thomsen
        @date 04/11/2002
    **/
    inline double NormL2() const;

    /** Return the euclidean distance of 2 vectors.
        @author Birger Streckel
        @date 08/2002
    **/
    inline double Dist(const Vector<T> &vec) const;

    /** Return the L infinity distance of 2 vectors.
        @author djung
        @date 2009/07 untested
    **/
    inline double DistLinf(const Vector<T> &vec) const;

    /** multiply this with arg elementwise and store the result in dest
        @author woelk 12/2004 */
    inline void ElementwiseProduct(const Vector<T> &arg,Vector<T> &dest) const;

    /** multiply this with arg elementwise and return the result 
        @author woelk 12/2004 */
    inline Vector<T> ElementwiseProduct(const Vector<T> &arg) const;

    /** returns the subvector with elements between offset and offset+length-1
        @author Dennis Herzog
        @date 2005-07-29 */
    Vector<T> SubVec(int length, int offset = 0) const;

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

    /** @author woelk 11/2007 (c) www.vision-n.de */
    void GetSTLVec(std::vector<T>& vec) const;
    std::vector<T> GetSTLVec() const;

		/** @author Ove Schimmer, www.sequid.com 
				@date 2008-07-17 */
		double GetMean(void) const;

  }; // class ===============================================================

  /** @relates Vector
      Output with streams
      @author woelk 10/2004 major corrections by Jan Woetzel/Sun Min 11/2004 */
  template  <class T> 
  std::ostream & operator<<(std::ostream& os, const BIAS::Vector<T>& vec);


  // ==== implementation:  ==========================================

  template <class T>
  inline void Vector<T>::operator+=(const Vector<T> &argvec) 
  {
    BIASASSERT(this->size() == argvec.size());
    T* data= GetData(), *argData = argvec.GetData();

    for (int i=0; i <this->size(); i++) {
      *data = *data + *argData;
      data++; argData++;
    }
  }

  template <class T>
  inline void Vector<T>::operator-=(const Vector<T> &argvec) 
  {
    BIASASSERT(this->size() == argvec.size());
    T* data= GetData(), *argData = argvec.GetData();

    for (int i=0; i <this->size(); i++) {
      *data = *data - *argData;
      data++; argData++;
    }
  }

  template <class T>
  inline void Vector<T>::operator*=(T scalar)
  {
    T* data= GetData();

    for (int i=0; i <this->size(); i++) {
      *data = *data * scalar;
      data++; 
    }
  }

  template <class T>
  inline void Vector<T>::operator/=(T scalar)
  {
    T* data= GetData();
    const int size=Size();
    for (int i=0; i<size; i++) {
      *data = *data / scalar;
      data++; 
    }
  }

  template <class T>
  inline bool Vector<T>::operator==(const Vector<T> &vec) const
  {
    if (this->size() != vec.size()) {
      return false;
    }

    const size_t s = sizeof(T) * this->size();
    return (memcmp((void *)GetData(), (void*)vec.GetData(), s) == 0);
  }

  template <class T>
  inline T Vector<T>::ScalarProduct(const Vector < T > &argvec) const
  {
    T result;
    ScalarProduct(argvec, result);
    return result;
  }

  template <class T> inline void 
  Vector<T>::ScalarProduct(const Vector<T> &argvec, T & result) const
  {
    BIASASSERT(this->size() == argvec.size());
    // initialized with the first product, so no initialization 
    // with 0 is necessary     
    result = this->v_[0] * argvec[0];	
    for (int i = 1; i < this->size(); i++) {
      result += this->v_[i] * argvec[i];
    }
  }



  template <class T>
  inline void Vector<T>::Multiply(const T& scalar, Vector<T>& res) const
  {
    for (int i = 0; i < this->size(); i++) {
      res[i] = this->v_[i] * scalar;
    }
  }

  template <class T>
  inline void Vector<T>::SubIP(const Vector<T>& argmat) 
  {
    BIASASSERT(this->size() == argmat.size());

    for (int i = 0; i < this->size(); i++) {
      this->v_[i] = this->v_[i] - argmat[i];
    }
  }

  template <class T>
  void Vector<T>::Sub(const Vector<T> &arg, Vector<T> &dest) const
  {
    const int size = (int)Size();
    dest.newsize(size);
    for (int i = 0; i < size; i++) {
      dest[i] = this->v_[i] - arg[i];
    }
  }

  template <class T>
  inline T Vector<T>::NormL1() const
  {
    T result = 0;
    for(register T* dataP = GetDataLast(); dataP >= GetData(); dataP--) 
      {
        result += *dataP > 0 ? *dataP : *dataP * -1;
      }
    return result;
  }

  template <class T>
  inline double Vector<T>::NormL2() const
  {
    double result = 0;
    for(register T* dataP = GetDataLast(); dataP >= GetData(); dataP--) {
      result += (double)(*dataP * *dataP);
    }
    result = sqrt((double) result);
    return result;
  }
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
  template <>
  inline double Vector<BIAS::Polynom>::NormL2() const {
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
    return 0;
  }
#endif

  template <class T>
  inline double Vector<T>::Dist(const Vector<T> &vec) const 
  {
    BIASASSERT(this->size() == vec.size());
    double result = 0;
    register T* thisP;
    register T* vecP;
    for(thisP=GetDataLast(), vecP=vec.GetDataLast(); 
        thisP >= GetData(); 
        thisP--, vecP--) {
      result += ((*thisP)-(*vecP))*((*thisP)-(*vecP));
    }
    result = sqrt(result);
    return result;
  }
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
  template <>
  inline double Vector<BIAS::Polynom>::Dist(const Vector<BIAS::Polynom> &vec) const {
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
    return 0;
  }
#endif

  template <class T>
  inline double Vector<T>::DistLinf(const Vector<T> &vec) const
  {
    BIASASSERT(this->size() == vec.size());
    double result = 0., tmp;
    register T* thisP;
    register T* vecP;
    for(thisP=GetDataLast(), vecP=vec.GetDataLast();
        thisP >= GetData();
        thisP--, vecP--)
    {
      tmp = static_cast<double>(*thisP - *vecP);
      if(tmp < 0.)
      {// abs(tmp)
        tmp = -tmp;
      }
      if(result < tmp)
      {// max(tmp, result)
        result = tmp;
      }
    }
    return result;
  }
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
  template <>
  inline double Vector<BIAS::Polynom>::DistLinf(const Vector<BIAS::Polynom> &vec) const {
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
    return 0;
  }
#endif

  template <class T>
  inline void Vector<T>::ElementwiseProduct(const Vector<T> &arg,
                                            Vector<T> &dest) const
  {
    const int size=Size();
    BIASASSERT((int)arg.Size()==size);
    dest.newsize(size);
    for (int i=0; i<size; i++)
      dest[i] = (*this)[i] * arg[i];
  }

  template <class T>
  inline Vector<T> Vector<T>::ElementwiseProduct(const Vector<T> &arg) const
  {
    Vector<T> dest;
    ElementwiseProduct(arg, dest);
    return dest;
  }


  inline
  std::ostream & operator<<(std::ostream& os, const BIAS::Vector<char>& vec) 
  {
    std::ios_base::fmtflags oldFlags = os.flags(); // push

    if (!IsConsoleStream(os) || (vec.size()>5))
      { 
        // not cout and not cerr, use default operator from TNT (printing trailing dimension)
        os << (TNT::Vector<char>)(vec);

      } else { // cout or cerr
      os << "[ ";
      for (int row = 0; row < vec.size(); row++){
        os<<std::setw(3);
        os<<vec.GetData()[row]<< " ";
      }
      os << "] ";
    }

    os.flags(oldFlags); // pop
    return os;
  }

  template  <class T> 
  inline
  std::ostream & operator<<(std::ostream& os, const BIAS::Vector<T>& vec) 
  {
    std::ios_base::fmtflags oldFlags = os.flags(); // push

    if (!IsConsoleStream(os) || (vec.size()>5))
      { 
        // not cout and not cerr, use default operator from TNT (printing trailing dimension)
        os<<std::setprecision( BIAS::DecimalsToStore<T>()  );
        os<<(TNT::Vector<T>)(vec);
      } else { // cout or cerr
      os << "[ ";
      for (int row = 0; row < vec.size(); row++){
        os<<std::setprecision( CONSOLE_PRECISION_DIGITS ); // less precise display in console
        os<<vec.GetData()[row]<< " ";
      }
      os << "] ";
    }

    os.flags(oldFlags); // pop
    return os;
  }



} // namespace BIAS
#endif // _Vector_hh_

