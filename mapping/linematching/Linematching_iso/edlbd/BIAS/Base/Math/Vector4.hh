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


#ifndef _Vector4_hh_
#define _Vector4_hh_
#include "bias_config.h"

#include <iostream>
#include <iomanip>
#include <sstream>

#include <Base/Debug/Error.hh>
//#include <Base/Debug/Debug.hh>

#include <Base/Math/Vector.hh>
#include <Base/Math/Matrix.hh>
#include <Base/Math/Matrix4x4.hh>
#include <Base/Math/Operators.hh>
#include <Base/Math/Utils.hh>

#define VECTOR4SIZE 4
namespace BIAS {
	/** @class Vector4
	@ingroup g_math
	@brief class Vector4 contains a Vector of dim. 4 and fixed dimension

	It's elment type is templated.
	manual loop unrolling is used if possible.
	The Vector is in row-major order (4 rows, 1 column)
	the indizes begin with zero (to size-1)
	@author Jan Woetzel
	@status tested in part (02/28/2002) **/

	// forward declaration to avoid mutual inclusion
	template <class T> class BIASMathBase_EXPORT Matrix3x4;
	template <class T> class BIASMathBase_EXPORT Matrix4x4;

	template<class T>
	class BIASMathBase_EXPORT Vector4 {
	public:

		inline Vector4() {};

		/**	copy constructor
		@author Daniel Grest (06/2003)
		*/
		inline Vector4(const Vector4<T>& vec) { *this = vec; };

		/** assignment with a constant value for all elements
		@author Jan Woetzel
		@status alpha (03/01/2002) **/
		explicit inline Vector4(const T & scalar);

		/**	assignment with an array of values
		which is copied into this ones class members
		@author Jan Woetzel
		@status untested (02/28/2002) **/
		explicit inline Vector4(const T * pv);

		/**	constructor with element assignment
		@author Jan Woetzel
		@status alpha (03/01/2002) **/
		explicit inline Vector4(char *s);

		/** constructor with element assigment using explicit values
		@author Ingo Thomsen
		@status tested (03/04/2002) **/
		inline Vector4(const T v0, const T v1, const T v2, const T v3);

		/** cast constructor
		@author Jan Woetzel
		@status untested (03/22/2002)**/
		Vector4(const Vector <T> &v);

		inline ~Vector4() {};

		/** copy the array of vectorsize beginning at *T to this->data_
		@author Jan Woetzel
		@status untested (02/28/2002) **/
		inline void Copy(const T * pv);

		/** set all elements to a scalat value
		@author Jan Woetzel
		@status untested (02/28/2002) **/
		inline void Set(const T & scalar);

		/** set elementwise with given scalr values
		@author Jan Woetzel, Ingo Thomsen
		@status tested (02/28/2002) **/
		inline void Set(const T & x, const T & y, const T & z, const T & w);

		/** set all values to 0
		@author Jan Woetzel **/
		inline void SetZero();
    // stl conform interface
    inline void clear();

		/**	@return the size of this vector
		@author Jan Woetzel (02/28/2002) **/
		inline const unsigned int Size() const;
    // stl conform interface
    inline const unsigned int size() const;
    // compatibility interface
    inline const unsigned int GetNumElements() const { return size(); };


		/** get the data pointer
		the member function itself is const (before {..}) because it
		doesn't change the this object.
		@return the const pointer to the data array for reading
		@author Jan Woetzel
		@status untested (02/28/2002) **/
		inline const T *GetData() const
		{ return data_; };

		/* non const, used for assignment like this: v.GetData()[1]=42 JW */
		inline T *GetData()
		{ return data_; };

		/** assignment operator
		set the vector elementwise to scalar value
		@author Jan Woetzel
		@status alpha (02/28/2002)**/
		inline Vector4<T>& operator=(const T& scalar);


		/** assignment operator
		@author grest 06 2003
		*/
		inline Vector4<T>& operator=(const Vector4<T>& vec) {
			memcpy((void *)data_, (void *)vec.GetData(), VECTOR4SIZE*sizeof(T));
			return *this;
		};

		/** access an element  of the vector with 0-based indizes.
		read only (no write)
		member funciton const because it doesn't change this object
		@author Jan Woetzel
		@status alpha (03/01/2002) **/
		inline const T& operator[] (const int i) const;

		/** access an element  of the vector with 0-based indizes.
		write allowed
		@author Jan Woetzel
		@status alpha (03/01/2002) **/
		inline T & operator[] (const int i);

		/** scalar product (=inner product) of two vectors, storing
		the result in result
		@author Jan Woetzel
		@status untested**/
		inline void ScalarProduct(const Vector4 < T > &argvec, T & result) const;
    inline T ScalarProduct(const Vector4 < T > &argvec) const;


    /** outer product, constructs a matrix.
        Often written as v * v^T for col vectors
        @author frick
    */
    Matrix4x4<T> OuterProduct(const Vector4<T> &v) const;

		/** multiply two vectors elementwise, storing the result in destvec
		@author Jan Woetzel
		@status untested (03/06/2002)**/
		inline void ElementwiseProduct(const Vector4 < T > &argvec,
			Vector4 < T > &destvec) const;

		/** Return the L1 norm: |a| + |b| + |c|+ |d|
		@author Ingo Thomsen
		@date 04/11/2002
		@status untested **/
    inline T NormL1() const;

		/** Return the L2 norm: sqrt(a^2 + b^2 + c^2 + d^2)
		@author Ingo Thomsen
		@date 04/11/2002
		@status untested **/
    inline double NormL2() const;

		/** Return the euclidean distance of 2 vectors.
		@author Birger Streckel
		@date 08/2002
		**/
		inline double Dist(const Vector4<T> &vec) const;

		/** return a new Matrix with 1 row and 4 columns representing a
		transposed vector **/
		inline Matrix<T> transposed() const;

		/** Comparison operator 'equal'
		@author Ingo Thomsen
		@status tested **/
		inline bool operator==(const Vector4<T>& arg) const;

		/** Comparison operator 'not equal'
		@author Ingo Thomsen **/
		inline bool operator!=(const Vector4<T>& arg) const;



		/** Addition operator with scalar argument
		@author Ingo Thomsen
		@status tested **/
		inline Vector4<T>& operator+=(const T& scalar);

		/** Addition (in place) of an scalar
		@author Ingo Thomsen **/
		inline void AddIP (const T& scalar);

		/** Addition with a scalar, storing results in destionation vector
		@author Ingo Thomsen **/
		inline void Add(const T& scalar, Vector4<T>& dest) const;



		/** Substraction operator with scalar argument
		@author Ingo Thomsen
		@status tested **/
		inline Vector4<T>& operator-=(const T& scalar);

		/** Substraction (in place) of an scalar
		@author Ingo Thomsen **/
		inline void SubIP(const T& scalar);

		/** Substraction with a scalar, storing results in destionation vector
		@author Ingo Thomsen **/
		inline void Sub(const T& scalar, Vector4<T>& dest) const;



		/** Multiplication operator with scalar argument
		@author Ingo Thomsen
		@status tested   **/
		inline Vector4<T>& operator*=(const T& scalar);

		/** Multiplication (in place) of an scalar
		@author Ingo Thomsen  **/
		inline void MultiplyIP(const T& scalar);

		/** Multiplication with a scalar, storing results in destionation vector
		@author Ingo Thomsen  **/
		inline void Multiply(const T& scalar, Vector4<T>& dest) const;



		/** Division operator with scalar argument
		@author Ingo Thomsen
		@status tested **/
		inline Vector4<T>& operator/=(const T& scalar);

		/** Division (in place) of an scalar
		@author Ingo Thomsen **/
		inline void DivideIP(const T& scalar);

		/** Division with a scalar, storing results in destionation vector
		@author Ingo Thomsen  **/
		inline void Divide(const T& scalar, Vector4<T>& dest) const;



		/** add operator for two Vectors
		@author Ingo Thomsen
		@status tested   **/
		inline Vector4<T>& operator+=(const Vector4<T> &argvec);

		/** in place adding
		@author Ingo Thomsen
		@status tested  **/
		inline void AddIP(const Vector4<T> &argvec);

		/** adding of two vectors, storing the result in destvec
		@author Ingo Thomsen
		@status tested    **/
		inline void Add(const Vector4<T> &argvec, Vector4<T> &destvec) const;



		/** sub operator for two Vectors
		@author Ingo Thomsen
		@status tested    **/
		inline Vector4<T> &operator-=(const Vector4<T> &argvec);

		/** in place substraction
		@author Ingo Thomsen
		@status tested   **/
		inline void SubIP(const Vector4<T> &argvec);

		/** subtracting of two Vectors, storing the result in destvec
		@author Ingo Thomsen
		@status tested **/
		inline void Sub(const Vector4<T> &argvec, Vector4<T> &destvec) const;


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


	protected:
		// the 4 elements of the vector without additional pointers etc...
		// it is the *same* as 4 individual members, so there is *NO* additional pointer
		// in particular an array of Vector4 is the same as one big array of T (JW)
		T data_[VECTOR4SIZE];	//

	}; // end of class ==================================================




	template <class T>
		inline Vector4<T>::Vector4(const T & scalar)
	{
		Set(scalar);
	}

	template <class T>
		inline Vector4<T>::Vector4(char *s)
	{
		std::istringstream inst(s);
		inst >> data_[0];
		inst >> data_[1];
		inst >> data_[2];
		inst >> data_[3];
	}

	template <class T>
		inline Vector4<T>::Vector4(const T v0, const T v1, const T v2, const T v3)
	{
		Set(v0, v1, v2, v3);
	}

	template <class T>
		inline Vector4<T>::Vector4(const T * pv)
	{
		Copy(pv);
	}

	template <class T>
		inline void Vector4<T>::Copy(const T * pv)
	{
		data_[0] = pv[0];
		data_[1] = pv[1];
		data_[2] = pv[2];
		data_[3] = pv[3];
	}

	template <class T>
		inline void Vector4<T>::Set(const T & scalar)
	{
		Set(scalar, scalar, scalar, scalar);
	}

	template <class T>
		inline void Vector4<T>::Set(const T & x, const T & y, const T & z, const T & w)
	{
		data_[0] = x;
		data_[1] = y;
		data_[2] = z;
		data_[3] = w;
	}

  template <class T>
    inline const unsigned int Vector4<T>::size() const
  {
    return VECTOR4SIZE;
  }

  template <class T>
    inline const unsigned int Vector4<T>::Size() const
  {
    return size();
  }


	template <class T>
		inline Vector4<T>& Vector4<T>::operator=(const T& scalar)
	{
		Set(scalar);
		return *this;
	}

	template <class T>
		inline const T& Vector4<T>::operator[](const int i) const
	{
        BIASASSERT(0 <= i);
        BIASASSERT(i < VECTOR4SIZE);
		return data_[i];
	}

	template <class T>
		inline T& Vector4<T>::operator[](const int i)
	{
        BIASASSERT(0 <= i);
        BIASASSERT(i < VECTOR4SIZE);
		return data_[i];
	}



	template <class T>
  inline void
  Vector4<T>::ScalarProduct(const Vector4<T> &argvec, T & result) const
	{
    result = ScalarProduct(argvec);
  }

	template <class T>
  inline T
  Vector4<T>::ScalarProduct(const Vector4<T> &argvec) const
	{
		return data_[0] * argvec[0] +
      data_[1] * argvec[1] +
      data_[2] * argvec[2] +
      data_[3] * argvec[3];
	}

	template <class T>
		inline void Vector4<T>::ElementwiseProduct(const Vector4<T> &argvec,
		Vector4<T> &destvec) const
	{
		destvec[0] = data_[0] * argvec[0];
		destvec[1] = data_[1] * argvec[1];
		destvec[2] = data_[2] * argvec[2];
		destvec[3] = data_[3] * argvec[3];
	}

	template <class T>
		inline T Vector4<T>::NormL1() const
	{
		return (data_[0] > 0 ? data_[0] : data_[0] * -1)
			+ (data_[1] > 0 ? data_[1] : data_[1] * -1)
			+ (data_[2] > 0 ? data_[2] : data_[2] * -1)
			+ (data_[3] > 0 ? data_[3] : data_[3] * -1);
	}

	template <class T>
		inline double Vector4<T>::NormL2() const
	{
		return sqrt((double) ( data_[0]*data_[0] +
			data_[1]*data_[1] +
			data_[2]*data_[2] +
			data_[3]*data_[3] ));
	}

	template <class T>
		inline double Vector4<T>::Dist(const Vector4<T> &vec) const
	{
		return sqrt((double) ( (data_[0]-vec[0])*(data_[0]-vec[0]) +
			(data_[1]-vec[1])*(data_[1]-vec[1]) +
			(data_[2]-vec[2])*(data_[2]-vec[2]) +
			(data_[3]-vec[3])*(data_[3]-vec[3]) ));
	}

	template <class T>
		inline Matrix<T> Vector4<T>::transposed() const
	{
		Matrix<T> destmat(1, 4);
		destmat.GetData()[0] = data_[0];
		destmat.GetData()[0] = data_[1];
		destmat.GetData()[0] = data_[2];
		destmat.GetData()[0] = data_[3];
		return destmat;
	}

	template <class T>
		inline bool Vector4<T>::operator==(const Vector4<T>& arg) const
	{
		return ( (data_[0] == arg.data_[0]) && (data_[1] == arg.data_[1]) &&
			(data_[2] == arg.data_[2]) && (data_[3] == arg.data_[3]) );
	}

	template <class T>
		inline bool Vector4<T>::operator!=(const Vector4<T>& arg) const
	{
		return !operator==(arg);
	}



	template <class T>
		inline Vector4<T>& Vector4<T>::operator+=(const T& scalar)
	{
		AddIP(scalar);
		return *this;
	}

	template <class T>
		inline void Vector4<T>::AddIP(const T& scalar)
	{
		Add(scalar, *this);
	}

	template <class T>
		inline void Vector4<T>::Add(const T& scalar, Vector4<T>& dest) const
	{
		dest.data_[0] = data_[0] + scalar;
		dest.data_[1] = data_[1] + scalar;
		dest.data_[2] = data_[2] + scalar;
		dest.data_[3] = data_[3] + scalar;
	}



	template <class T>
		inline Vector4<T>& Vector4<T>::operator-=(const T& scalar)
	{
		SubIP(scalar);
		return *this;
	}

	template <class T>
		inline void Vector4<T>::SubIP(const T& scalar)
	{
		Sub(scalar, *this);
	}

	template <class T>
		inline void Vector4<T>::Sub (const T& scalar, Vector4<T>& dest) const
	{
		dest.data_[0] = data_[0] - scalar;
		dest.data_[1] = data_[1] - scalar;
		dest.data_[2] = data_[2] - scalar;
		dest.data_[3] = data_[3] - scalar;
	}



	template <class T>
		inline Vector4<T>& Vector4<T>::operator*=(const T& scalar)
	{
		MultiplyIP(scalar);
		return *this;
	}

	template <class T>
		inline void Vector4<T>::MultiplyIP(const T& scalar)
	{
		Multiply(scalar, *this);
	}

	template <class T>
		inline void Vector4<T>::Multiply(const T& scalar, Vector4<T>& dest) const
	{
		dest.data_[0] = data_[0] * scalar;
		dest.data_[1] = data_[1] * scalar;
		dest.data_[2] = data_[2] * scalar;
		dest.data_[3] = data_[3] * scalar;
	}



	template <class T>
		inline Vector4<T>& Vector4<T>::operator/=(const T& scalar)
	{
		DivideIP(scalar);
		return *this;
	}

	template <class T>
		inline void Vector4<T>::DivideIP(const T& scalar)
	{ Divide(scalar, *this);}

	template <class T>
		inline void Vector4<T>::Divide(const T& scalar, Vector4<T>& dest) const
	{

		dest.data_[0] = data_[0] / scalar;
		dest.data_[1] = data_[1] / scalar;
		dest.data_[2] = data_[2] / scalar;
		dest.data_[3] = data_[3] / scalar;
	}



	template <class T>
		inline Vector4<T>&  Vector4<T>::operator+=(const Vector4<T> &argvec)
	{
		Add(argvec, *this);
		return *this;
	}

	template <class T>
		inline void  Vector4<T>::AddIP(const Vector4<T> &argvec)
	{
		Add(argvec, *this);
	}

	template <class T> inline void
		Vector4<T>::Add(const Vector4<T> &argvec, Vector4<T> &destvec) const
	{
		destvec[0] = data_[0] + argvec[0];
		destvec[1] = data_[1] + argvec[1];
		destvec[2] = data_[2] + argvec[2];
		destvec[3] = data_[3] + argvec[3];
	}



	template <class T>
		inline Vector4<T>& Vector4<T>::operator-=(const Vector4<T> &argvec)
	{
		SubIP(argvec);
		return *this;
	}

	template <class T>
		inline void Vector4<T>::SubIP(const Vector4<T> &argvec)
	{
		Sub(argvec, *this);
	}

	template <class T> inline void
		Vector4<T>::Sub(const Vector4<T> &argvec, Vector4<T> &destvec) const
	{
		destvec[0] = data_[0] - argvec[0];
		destvec[1] = data_[1] - argvec[1];
		destvec[2] = data_[2] - argvec[2];
		destvec[3] = data_[3] - argvec[3];
	}


	// JW
	template <class T>
		inline void Vector4<T>::SetZero()
	{
		Set( (T)0);
	}


  // JW
  template <class T>
    inline void Vector4<T>::clear()
  {
    SetZero();
  }



	//////////////////////////////////////////////////////////
	// operators
	///////////////////////////////////////////////////////////


  /**
  @relates Vector4
  @brief Input with streams
  specialization for numerical display of unsigned char instead of alphebetically.
  fout format should be consistent with template implementation and Vector<>
  @author Jan Woetzel */
  inline
    std::ostream& operator<<(std::ostream & os, const Vector4<unsigned char> &vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR4SIZE << NL;
      for (int row = 0; row < VECTOR4SIZE; row++) {
        // print out one element per line
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" "<<NL;
      }
    } else {// cout or cerr
      os << "[ ";
      for (int row = 0; row < VECTOR4SIZE; row++)
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" ";
      os<<" ]";
    }
    return os;
  }


  inline
    std::ostream& operator<<(std::ostream & os, const Vector4<char> &vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR4SIZE << NL;
      for (int row = 0; row < VECTOR4SIZE; row++) {
        // print out one element per line
        os<<std::setw(3)<<(int)vec.GetData()[row]<<" "<<NL;
      }
    } else {// cout or cerr
      os << "[ ";
      for (int row = 0; row < VECTOR4SIZE; row++)
        os<<std::setw(3)<<(unsigned int)vec.GetData()[row]<<" ";
      os<<" ]";
    }
    return os;
  }


  /**
  @relates Vector4
  @brief Input with streams
  fout format should be consistent with template implementation and Vector<>
  @author Jan Woetzel */
  template<class T>
    inline
    std::ostream& operator<<(std::ostream & os, const Vector4<T> &vec)
  {
    if (!IsConsoleStream(os))
    { // not cout and not cerr, display trailing dim for consistency with Vector<>
      os << VECTOR4SIZE << NL;
      for (int row = 0; row < VECTOR4SIZE; row++) {
        //os<<std::setw(14); // set width for output format
        os<<std::setprecision( DecimalsToStore<T>() );
        // print out one element per line
        os<<vec.GetData()[row]<<" "<<NL;
      }
    } else {// cout or cerr
      os<<"[ ";
      for (int row = 0; row < VECTOR4SIZE; row++){
        os<<std::setprecision(CONSOLE_PRECISION_DIGITS);
        os<<vec.GetData()[row]<<" ";
      }
      os<<" ]";
    }
    return os;
  }


  /**
  @relates Vector4
  @brief Input with streams
  specialization for numerical display of unsigned char instead of alphebetically.
  fout format should be consistent with template implementation and Vector<>
  @author Jan Woetzel */
  inline std::istream &
    operator>>(std::istream & is, Vector4<unsigned char> & vec)
  {
    int N;
    is >> N;
    if (N !=  (int)vec.Size()){
      BIASERR("can't assign to a Vector4<unsigned char> from a vector with dim " << N );
    } else {
      // correct size, sequential read elementwise
      int tmp(0); //< used for casting between unsigned char are written int
      for (unsigned int row = 0; row < vec.Size(); row++)
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
    operator>>(std::istream & is, Vector4<char> & vec)
  {
    int N;
    is >> N;
    if (N !=  (int)vec.Size()){
      BIASERR("can't assign to a Vector4<unsigned> from a vector with dim " << N );
    } else {
      // correct size, sequential read elementwise
      int tmp(0); //< used for casting between unsigned char are written int
      for (unsigned int row = 0; row < vec.Size(); row++)
      {
        is >> tmp;
        BIASASSERT(tmp>=std::numeric_limits< char>::min() );
        BIASASSERT(tmp<=std::numeric_limits< char>::max() );
        vec.GetData()[row] = (char)tmp;
      };
    };
    return is;
  }

  /** @relates Vector4
  @brief Input with streams
  fout format should be consistent with template implementation and Vector<>
  @author Jan Woetzel */
  template <class T>
    std::istream& operator>>(std::istream & is, Vector4<T> &vec)
  {
    int N;
    is >> N;
    if (!(N == (int)vec.Size())) {
      BIASERR("can't assign a Vector4<T> with a dim " << N << " vector");
      BIASBREAK;
    } else {
      for (unsigned int row = 0; row < vec.Size(); row++) {
        is>>vec.GetData()[row];
      };
    };
    return is;
  }


} // namespace BIAS
#endif

