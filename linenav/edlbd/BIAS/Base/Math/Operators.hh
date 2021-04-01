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

#ifndef __Operators_hh__
#define __Operators_hh__

#include <Base/Math/Matrix.hh>


//#include <Base/Debug/Error.hh>
#include <Base/Math/tnt/cmat.h>

namespace BIAS {

  // forward declarations to avoid nested includes
  template<class T> class BIASMathBase_EXPORT Matrix;
  template<class T> class BIASMathBase_EXPORT Vector;
  template<class T> class BIASMathBase_EXPORT Matrix2x2;
  template<class T> class BIASMathBase_EXPORT Vector2;
  template<class T> class BIASMathBase_EXPORT Matrix2x3;
  template<class T> class BIASMathBase_EXPORT Matrix3x2;
  template<class T> class BIASMathBase_EXPORT Matrix3x3;
  template<class T> class BIASMathBase_EXPORT Vector3;
  template<class T> class BIASMathBase_EXPORT Matrix4x4;
  template<class T> class BIASMathBase_EXPORT Vector4;
  template<class T> class BIASMathBase_EXPORT Matrix3x4;


  ////////////////////////////////////////////////////////////////////////////
  //      unary vector/matrix operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Vector2
  @author herzog 2005-07-19 */
  template <class T> inline Vector2<T> operator-(const Vector2<T>& v)
  { return Vector2<T>( -v[0],-v[1] ); }

  /** @relates Vector3
  @author herzog 2005-07-19 */
  template <class T> inline Vector3<T> operator-(const Vector3<T>& v)
  { return Vector3<T>( -v[0],-v[1],-v[2] ); }

  /** @relates Vector4
  @author herzog 2005-07-19 */
  template <class T> inline Vector4<T> operator-(const Vector4<T>& v)
  { return Vector4<T>( -v[0],-v[1],-v[2],-v[3] ); }

  ////////////////////////////////////////////////////////////////////////////
  //      vector - scalar operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Vector */
  template <class T>
    inline Vector<T> operator*(const Vector<T>& v, const T& scalar)
  {
    Vector<T> res(v.size());
    v.Multiply(scalar, res);
    return res;
  }

  /** @relates Vector */
  template <class T>
    inline Vector<T> operator*(const T& scalar, const Vector<T>& v)
  {
    Vector<T> res(v.size());
    v.Multiply(scalar, res);
    return res;
  }

  /** Addition operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator+(const Vector2<T>& vec, const T& scalar)
  {
    Vector2<T> result;
    vec.Add(scalar, result);
    return result;
  }

  /** Addition operator with scalar argument, returning new vector
  @author Ingo Thomsen, djung, copy-paste **/
  template <class T>
    inline Vector2<T> operator+(const T& scalar, const Vector2<T>& vec)
  {
    Vector2<T> result;
    vec.Add(scalar, result);
    return result;
  }

  /** @relates Vector2
  Substraction operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator-(const Vector2<T>& vec, const T& scalar)
  {
    Vector2<T> result;
    vec.Sub(scalar, result);
    return result;
  }

  /** @relates Vector2
  Substraction operator with scalar argument, returning new vector
  @author Ingo Thomsen, djung, copy-paste **/
  template <class T>
    inline Vector2<T> operator-(const T& scalar, const Vector2<T>& vec)
  {
    Vector2<T> result;
    vec.Sub(scalar, result);
    return result;
  }

  /** @relates Vector2
  Multiplication operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator*(const Vector2<T>& vec, const T& scalar)
  {
    Vector2<T> result;
    vec.Multiply(scalar, result);
    return result;
  }

  /** @relates Vector2
  Multiplication operator with scalar argument, returning new vector
  @author Jung, copied for convenience **/
  template <class T>
    inline Vector2<T> operator*(const T& scalar, const Vector2<T>& vec)
  {
    Vector2<T> result;
    vec.Multiply(scalar, result);
    return result;
  }

  /** @relates Vector2
  Division operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator/(const Vector2<T>& vec, const T& scalar)
  {
    BIASASSERT(scalar !=(T)0);
    Vector2<T> result;
    vec.Divide(scalar, result);
    return result;
  }

  /** @relates Vector3 */
  template <class T>
    inline Vector3<T> operator*(const Vector3<T>& vec, const T& scalar)
  {
    Vector3<T> res;
    vec.Multiply(scalar, res);
    return res;
  }

  /** @relates Vector3 */
  template <class T>
    inline Vector3<T> operator*(const T& scalar, const Vector3<T>& vec)
  {
    Vector3<T> res;
    vec.Multiply(scalar, res);
    return res;
  }

  /** @relates Vector3
  Addition operator with scalar argument, returning new vector
  @author Ingo Thomsen, woelk, tested **/
  template <class T>
    inline Vector3<T> operator+(const Vector3<T>& vec, const T& scalar)
  {
    Vector3<T> result;
    vec.Add(scalar, result);
    return result;
  }

  /** @relates Vector3
  Substraction operator with scalar argument, returning new vector
  @author Ingo Thomsen, woelk, tested **/
  template <class T>
    inline Vector3<T> operator-(const Vector3<T>& vec, const T& scalar)
  {
    return vec.Sub(scalar);
  }

  /** @relates Vector3
  Substraction operator with scalar argument, returning new vector
  @author woelk, tested **/
  template <class T>
    inline Vector3<T> operator-(const T& scalar, const Vector3<T>& vec)
  {
    Vector3<T> dest;
    vec.Mult(-1, dest);
    dest+=scalar;
    return dest;
  }

  /** @relates Vector3
  Division operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector3<T> operator/(const Vector3<T>& vec, const T& scalar)
  {
    Vector3<T> result;
    vec.Divide(scalar, result);
    return result;
  }

  /** @relates Vector4
  Multiplication operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested  **/
  template <class T>
    inline Vector4<T> operator*(const Vector4<T>& v, const T& scalar)
  {
    Vector4<T> result;
    v.Multiply(scalar, result);
    return result;
  }

  /** @relates Vector4
  Multiplication operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested  **/
  template <class T>
    inline Vector4<T> operator*(const T& scalar, const Vector4<T>& v)
  {
    Vector4<T> result;
    v.Multiply(scalar, result);
    return result;
  }

  /** @relates Vector4
  Division operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector4<T> operator/(const Vector4<T>& v, const T& scalar)
  {
#ifdef BIAS_DEBUG
    if (scalar == 0 ) {
      BIASERR("Division by Zero!");
      return Vector4<T>((T)0,(T)0,(T)0,(T)0);
    }
#endif
    Vector4<T> result;
    v.Divide(scalar, result);
    return result;
  }

  /** @relates Vector4
  Addition operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector4<T> operator+(const Vector4<T>& v, const T& scalar)
  {
    Vector4<T> result;
    v.Add(scalar, result);
    return result;
  }

  /** @relates Vector4
  Subtraction operator with scalar argument, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector4<T> operator-(const Vector4<T>& v, const T& scalar)
  {
    Vector4<T> result;
    v.Sub(scalar, result);
    return result;
  }

  ////////////////////////////////////////////////////////////////////////////
  //      vector - vector operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Vector */
  template <class T>
    inline T operator*(const Vector<T>& v, const Vector<T> &argvec)
  {
    T destscalar;
    v.ScalarProduct(argvec, destscalar);
    return destscalar;
  }

  //   /** @relates Vector2
  //       @author Dennis Herzog, 2005-07-27 **/
  //   template <class T>
  //   inline bool operator == (const Vector2<T>& a, const Vector2<T>& b)
  //   { return a[0]==b[0] && a[1]==b[1]; }

  //   template <class T>
  //   inline bool operator != (const Vector2<T>& a, const Vector2<T>& b)
  //   { return !(a==b); }

  /** @relates Vector2
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator<= (const Vector2<T>& a, const Vector2<T>& b)
  { return a[0]<=b[0] && a[1]<=b[1]; }

  /** @relates Vector2
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator< (const Vector2<T>& a, const Vector2<T>& b)
  { return a[0]<b[0] && a[1]<b[1]; }

  /** @relates Vector2
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator>= (const Vector2<T>& a, const Vector2<T>& b)
  { return b<=a; }

  /** @relates Vector2
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator> (const Vector2<T>& a, const Vector2<T>& b)
  { return b<a; }

  /** @relates Vector2
  add operator for two Vectors, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator+(const Vector2<T> &vec, const Vector2<T> &argvec)
  {
    Vector2<T> destvec;
    vec.Add(argvec, destvec);
    return destvec;
  }

  /** @relates Vector2
  sub operator for two Vectors, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector2<T> operator-(const Vector2<T> &vec, const Vector2<T> &argvec)
  {
    Vector2<T> destvec;
    vec.Sub(argvec, destvec);
    return destvec;
  }

  /** @relates Vector2 */
  template <class T>
    inline T operator*(const Vector2<T> &vec, const Vector2<T> &argvec)
  {
    return vec[0]*argvec[0]+vec[1]*argvec[1];
  }

  //   /** @relates Vector3
  //       @author Dennis Herzog, 2005-07-27 **/
  //   template <class T>
  //   inline bool operator == (const Vector3<T>& a, const Vector3<T>& b)
  //   { return a[0]==b[0] && a[1]==b[1] && a[2]==b[2]; }

  //   /** @relates Vector3
  //       @author Dennis Herzog, 2005-07-27 **/
  //   template <class T>
  //   inline bool operator != (const Vector3<T>& a, const Vector3<T>& b)
  //   { return !(a==b); }

  /** @relates Vector3
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator<= (const Vector3<T>& a, const Vector3<T>& b)
  { return a[0]<=b[0] && a[1]<=b[1] && a[2]<=b[2]; }

  /** @relates Vector3
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator< (const Vector3<T>& a, const Vector3<T>& b)
  { return a[0]<b[0] && a[1]<b[1] && a[2]<b[2]; }

  /** @relates Vector3
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator>= (const Vector3<T>& a, const Vector3<T>& b)
  { return b<=a; }

  /** @relates Vector3
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator> (const Vector3<T>& a, const Vector3<T>& b)
  { return b<a; }

  /** @relates Vector3 */
  template <class T>
    inline T operator*(const Vector3<T>& vec, const Vector3<T>& arg)
  { T res; vec.ScalarProduct(arg, res); return res; }

  /** @relates Vector3 */
  template <class T>
    inline Vector3<T> operator+(const T& scalar, const Vector3<T>& vec)
  {
    Vector3<T> result;
    vec.Add(scalar, result);
    return result;
  }

  /** @relates Vector3
  add operator for two Vectors, returning new vector
  @author Ingo Thomsen, woelk, tested **/
  template <class T>
    inline Vector3<T> operator+(const Vector3<T>& vec, const Vector3<T>& argvec)
  {
    Vector3<T> destvec;
    vec.Add(argvec, destvec);
    return destvec;
  }

  /** @relates Vector3
  add operator for Vector3, and Vector, so u dont have to cast...
  @author D. Grest, tested **/
  template <class T>
    inline Vector3<T> operator+(const Vector3<T>& vec, const Vector<T>& argvec)
  {
    Vector3<T> destvec;
#ifdef BIAS_DEBUG
    if (argvec.size()!=3) {
      BIASERR("operator+: argument does not havesize 3");
    }
#endif
    destvec[0]=vec[0]+argvec[0];
    destvec[1]=vec[1]+argvec[1];
    destvec[2]=vec[2]+argvec[2];
    return destvec;
  }

  /** @relates Vector3
  sub operator for two Vectors, returning new vector
  @author Ingo Thomsen, tested **/
  template <class T>
    inline Vector3<T> operator-(const Vector3<T>& vec, const Vector3<T>& argvec)
  {
    Vector3<T> destvec;
    vec.Sub(argvec, destvec);
    return destvec;
  }

  //   /** @relates Vector4
  //       @author Dennis Herzog, 2005-07-27 **/
  //   template <class T>
  //   inline bool operator == (const Vector4<T>& a, const Vector4<T>& b)
  //   { return a[0]==b[0] && a[1]==b[1] && a[2]==b[2] && a[3]==b[3]; }

  //   /** @relates Vector4
  //       @author Dennis Herzog, 2005-07-27 **/
  //   template <class T>
  //   inline bool operator != (const Vector4<T>& a, const Vector4<T>& b)
  //   { return !(a==b); }

  /** @relates Vector4
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator<= (const Vector4<T>& a, const Vector4<T>& b)
  { return a[0]<=b[0] && a[1]<=b[1] && a[2]<=b[2] && a[3]<=b[3]; }

  /** @relates Vector4
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator< (const Vector4<T>& a, const Vector4<T>& b)
  { return a[0]<b[0] && a[1]<b[1] && a[2]<b[2] && a[3]<b[3]; }

  /** @relates Vector4
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator>= (const Vector4<T>& a, const Vector4<T>& b)
  { return b<=a; }

  /** @relates Vector4
  @author Dennis Herzog, 2005-07-27 **/
  template <class T>
    inline bool operator> (const Vector4<T>& a, const Vector4<T>& b)
  { return b<a; }

  /** @relates Vector4
  op* to multiply two vectors by their dotproduct
  @author Jan Woetzel, untested (03/06/2002)**/
  template <class T>
    inline T operator*(const Vector4<T>& v, const Vector4<T>& argvec)
  {
    T destscalar;
    v.ScalarProduct(argvec, destscalar);
    return destscalar;
  }

  /** @relates Vector4
  sub operator for two Vectors, returning new vector
  @author Ingo Thomsen, tested    **/
  template <class T>
    inline Vector4<T> operator-(const Vector4<T>& v, const Vector4<T> &argvec)
  {
    Vector4<T> destvec;
    v.Sub(argvec, destvec);
    return destvec;
  }

  /** @relates Vector4
  add operator for two Vectors, returning new vector
  @author Ingo Thomsen, tested   **/
  template <class T>
    inline Vector4<T> operator+(const Vector4<T>& v, const Vector4<T> &argvec)
  {
    Vector4<T> destvec;
    v.Add(argvec, destvec);
    return destvec;
  }



  ////////////////////////////////////////////////////////////////////////////
  //      matrix - scalar operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Matrix
  addition operator, returning new matrix
  @author Ingo Thomsen, woelk **/
  template<class T>
    inline Matrix<T> operator+(const Matrix<T>& mat, const T& scalar)
  { Matrix<T> res; mat.Add(scalar, res); return res; }

  /** @relates Matrix */
  template<class T>
    inline Matrix<T> operator+(const T& scalar, const Matrix<T>& mat)
  { Matrix<T> res; mat.Add(scalar, res); return res; }

  /** @relates Matrix
  substraction operator, returning new matrix
  @author Ingo Thomsen **/
  template<class T>
    inline Matrix<T> operator-(const Matrix<T>& mat, const T& scalar)
  { Matrix<T> res; mat.Sub(scalar, res); return res; }

  /** @relates Matrix */
  template<class T>
    inline Matrix<T> operator-(const T& scalar, const Matrix<T>& mat)
  { Matrix<T> res; mat.Sub(scalar, res); return res; }

  /** @relates Matrix2x3
  multiplication operator, returning new matrix
  @author apetersen**/
  template<class T>
    inline Matrix2x3<T> operator*(const Matrix2x3<T>& mat, const T& scalar)
  {
    Matrix2x3<T> res;
    T* resP = res.GetData(); T* matP = mat.GetData();
    for (size_t i=0; i<6; i++) resP[i] = scalar*matP[i];
    return res;
  }
  /** @relates Matrix2x3
  multiplication operator, returning new matrix
  @author apetersen**/
  template<class T>
  inline Matrix2x3<T> operator*(const T& scalar, const Matrix2x3<T>& mat)
  {
    Matrix2x3<T> res;
    T* resP = res.GetData(); const T* matP = mat.GetData();
    for (size_t i=0; i<6; i++) resP[i] = scalar*matP[i];
    return res;
  }

  /** @relates Matrix3x2
  multiplication operator, returning new matrix
  @author apetersen**/
  template<class T>
    inline Matrix3x2<T> operator*(const Matrix3x2<T>& mat, const T& scalar)
  {
    Matrix2x3<T> res;
    T* resP = res.GetData(); const T* matP = mat.GetData();
    for (size_t i=0; i<6; i++) resP[i] = scalar*matP[i];
    return res;
  }
  /** @relates Matrix3x2
  multiplication operator, returning new matrix
  @author apetersen**/
  template<class T>
  inline Matrix3x2<T> operator*(const T& scalar, const Matrix3x2<T>& mat)
  {
    Matrix2x3<T> res;
    T* resP = res.GetData(); T* matP = mat.GetData();
    for (size_t i=0; i<6; i++) resP[i] = scalar*matP[i];
    return res;
  }

  /** @relates Matrix
  multiplication operator, returning new matrix
  @author Ingo Thomsen**/
  template<class T>
    inline Matrix<T> operator*(const Matrix<T>& mat, const T& scalar)
  { Matrix<T> res; mat.Multiply(scalar, res); return res; }

  /** @relates Matrix */
  template<class T>
    inline Matrix<T> operator*(const T& scalar, const Matrix<T>& mat)
  { Matrix<T> res; mat.Multiply(scalar, res); return res; }

  /** @relates Matrix
  division operator, returning new matrix
  @author Ingo Thomsen **/
  template<class T>
    inline Matrix<T> operator/(const Matrix<T>& mat, const T& scalar)
  { Matrix<T> res; mat.Divide(scalar, res); return res; }

  /** @relates Matrix
  division operator
  @author Ingo Thomsen **/
  template<class T>
    inline Matrix<T>& operator/=(Matrix<T>& mat, const T& scalar)
  { mat.DivideIP(scalar); return mat; }



  ////////////////////////////////////////////////////////////////////////////
  //      vector - matrix operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Matrix3x3
      @relates Vector3
      matrix-vector multiplication using 'Mult'
      @author Ingo Thomsen, Jan Woetzel,
      fw moved out of class, untested (04/17/2002)**/
  template <class T>
  inline Vector3<T> operator*(const Matrix3x3<T>& mat,
                              const Vector3<T> &argvec)
  {
    Vector3<T> destvec;
    mat.Mult(argvec, destvec);
    return destvec;
  }

  /** @relates Matrix4x4
      @relates Vector4
      matrix-vector multiplication using 'Mult'
      @author Jan Woetzel, untested (04/17/2002) */
  template <class T>
  inline Vector4<T> operator*(const Matrix4x4<T>& mat,
                              const Vector4<T> &argvec)
  {
    Vector4<T> destvec;
    mat.Mult(argvec, destvec);
    return destvec;
  }

  /** @relates Matrix
  @relates Vector */
  template <class T>
    inline Vector<T> operator*(const Matrix<T>& mat, const Vector2<T>& vec)
  {
    BIASASSERT(mat.num_cols() == 2 );
    Vector<T> res(mat.num_rows());
    for (register int i=0; i<mat.num_rows(); i++)
      res[i]=mat[i][0]*vec[0]+mat[i][1]*vec[1];
    return res;
  }

  /** @relates Matrix2x2
  @relates Vector2 */
  template <class T>
    inline Vector2<T> operator*(const Matrix2x2<T>& mat, const Vector2<T>& vec)
  {
    Vector2<T> res;
    res[0]=mat[0][0]*vec[0]+mat[0][1]*vec[1];
    res[1]=mat[1][0]*vec[0]+mat[1][1]*vec[1];
    return res;
  }

  /** @relates Matrix2x3
  @relates Vector3 */
  template <class T>
    inline Vector2<T> operator*(const Matrix2x3<T>& mat, const Vector3<T>& vec)
  {
    Vector2<T> res;
    res[0]= mat[0][0]*vec[0] + mat[0][1]*vec[1] + mat[0][2]*vec[2];
    res[1]= mat[1][0]*vec[0] + mat[1][1]*vec[1] + mat[1][2]*vec[2];
    return res;
  }

  /** @relates Matrix3x2
  @relates Vector2 */
  template <class T>
    inline Vector3<T> operator*(const Matrix3x2<T>& mat, const Vector2<T>& vec)
  {
    Vector2<T> res;
    res[0]= mat[0][0]*vec[0] + mat[0][1]*vec[1];
    res[1]= mat[1][0]*vec[0] + mat[1][1]*vec[1];
    res[2]= mat[2][0]*vec[0] + mat[2][1]*vec[1];
    return res;
  }

  /** @relates Matrix
  @relates Vector3 */
  template <class T>
    inline Vector<T>
    operator*(const BIAS::Matrix<T> &mat, const Vector3<T> &vec)
  {
    BIASASSERT(mat.num_cols() == 3);
    Vector<T> result(mat.num_rows());
    result.Set((T)0);
    register int y, i;
    for (y=0; y<(int)result.Size(); y++)
      for (i=0; i<3; i++)
        result[y]+=mat[y][i]*vec[i];
    return result;
  }

  /** @relates Matrix3x4
  @relates Vector4 */
  template <class T> inline Vector3<T>
    operator*(const Matrix3x4<T>& mat, const Vector4<T> &argvec)
  {
    Vector3<T> destvec;
    mat.Mult(argvec, destvec);
    return destvec;
  }

  /** @relates Matrix
  @relates Vector4 */
  template <class T> inline Vector<T>
    operator*(const BIAS::Matrix<T> &mat, const Vector4<T> &vec)
  {
    BIASASSERT(mat.num_cols() == 4);

    Vector<T> result(mat.num_rows());
    result.Set((T)0);
    register int y, i;
    for (y=0; y<result.size(); y++)
      for (i=0; i<4; i++)
        result[y]+=mat[y][i]*vec[i];
    return result;
  }

  ////////////////////////////////////////////////////////////////////////////
  //      matrix - matrix operators
  ////////////////////////////////////////////////////////////////////////////

  /** @relates Matrix
      @brief zero padded convolution of two matrices
      @author woelk 01/2008 (c) www.vision-n.de */
  template <class T> inline void
  Conv(const Matrix<T>& left, const Matrix<T> &right ,Matrix<T> &res)
  {
    const int lc=left.num_cols(), rc=right.num_cols();
    const int lr=left.num_rows(), rr=right.num_rows();
    const int res_x = lc+rc-1, res_y = lr+rr-1;
    res.newsize(res_y, res_x);
    double sum;
    for (int y=0; y<res_y; y++){
      const int miny = (y<rr)?(0):(y-rr+1);
      const int maxy = (y<lr)?(y+1):(lr);
      //std::cout << "y="<<y<<"\tsum ["<<miny<<", "<<maxy<<"]\n";
      for (int x=0; x<res_x; x++){
        const int minx = (x<rc)?(0):(x-rc+1);
        const int maxx = (x<lc)?(x+1):(lc);
        //std::cout << "x="<<x<<"\tsum ["<<minx<<", "<<maxx<<"]\n";
        sum = 0.0;
        for (int r=miny; r<maxy; r++){
          for (int c=minx; c<maxx; c++){
            BIASASSERT(r>=0 && r<lr);
            BIASASSERT(c>=0 && c<lc);
            BIASASSERT(y-r>=0 && y-r<rr);
            BIASASSERT(x-c>=0 && x-c<rc);
            sum += left[r][c] * right[y-r][x-c];
          }
        }
        res[y][x] = (T) sum;
      }
    }
  }

  /** @relates Matrix2x2
      untested */
  template <class T> inline Matrix2x2<T>
  operator*(const Matrix2x2<T>& mat, const Matrix2x2<T> & argMat)
  {
    Matrix2x2<T> destmat;
    register T *destP 	= destmat.GetData();
    const register T *argP	= argMat.GetData();
    const register T *matP	= mat.GetData();

    destP[0] =
      matP[0] * argP[0] + matP[1] * argP[2];
    destP[1] =
      matP[0] * argP[1] + matP[1] * argP[3];

    destP[2] =
      matP[2] * argP[0] + matP[3] * argP[2];
    destP[3] =
      matP[2] * argP[1] + matP[3] * argP[3];
    return destmat;
  }

  /** @relates Matrix2x3
      @relates Matrix3x3
      untested */
  template <class T> inline Matrix2x3<T>
  operator*(const Matrix2x3<T>& mat, const Matrix3x3<T> & argmat)
  {
    Matrix2x3<T> destmat;
    register T *destP = destmat.GetData(); 
    const register T *argP = argmat.GetData(); 
    const register T *matP = mat.GetData(); 

    destP[0] = matP[0] * argP[0] + matP[1] * argP[3] + matP[2] * argP[6];
    destP[1] = matP[0] * argP[1] + matP[1] * argP[4] + matP[2] * argP[7];
    destP[2] = matP[0] * argP[2] + matP[1] * argP[5] + matP[2] * argP[8];
    destP[3] = matP[3] * argP[0] + matP[4] * argP[3] + matP[5] * argP[6];
    destP[4] = matP[3] * argP[1] + matP[4] * argP[4] + matP[5] * argP[7];
    destP[5] = matP[3] * argP[2] + matP[4] * argP[5] + matP[5] * argP[8];

    return destmat;
  }

  /** @relates Matrix3x2
      @relates Matrix2x2
      untested */
  template <class T> inline Matrix3x2<T>
  operator*(const Matrix3x2<T>& mat, const Matrix2x2<T> & argmat)
  {
    Matrix3x2<T> destmat;
    register T *destP = destmat.GetData(); 
    const register T *argP = argmat.GetData(); 
    const register T *matP = mat.GetData(); 

    destP[0] = matP[0] * argP[0] + matP[1] * argP[2];
    destP[1] = matP[0] * argP[1] + matP[1] * argP[3];
    destP[2] = matP[0] * argP[2] + matP[1] * argP[4];
    destP[3] = matP[2] * argP[0] + matP[3] * argP[2];
    destP[4] = matP[2] * argP[1] + matP[3] * argP[3];
    destP[5] = matP[2] * argP[2] + matP[3] * argP[4];

    return destmat;
  }

  /** @relates Matrix3x3
  matrix-matrix multiplication using 'Mult'
  @author Jan Woetzel, tested (04/17/2002) */
  template <class T>
  inline Matrix3x3<T> operator*(const Matrix3x3<T> & argL,
                                const Matrix3x3<T> & argR)
  {
    Matrix3x3<T> resMat;
    argL.Mult(argR, resMat);
    return resMat;
  }

  /** @relates Matrix3x3
      @author woelk (c) www.vision-n.de */
  template <class T>
  Matrix3x3<T> operator+(const Matrix3x3<T>& left, const Matrix3x3<T>&  right)
  { Matrix3x3<T> res(left); res+=right; return res; }

  /** @relates Matrix3x3
      @author woelk (c) www.vision-n.de */
  template <class T>
  Matrix3x3<T> operator-(const Matrix3x3<T>& left, const Matrix3x3<T>&  right)
  { Matrix3x3<T> res(left); res-=right; return res; }

  /** @relates Matrix3x3
  @relates Matrix
  matrix-matrix multiplication using 'Mult'
  @author Jan Woetzel, tested (04/17/2002) */
  template <class T>
  inline Matrix<T> operator*(const Matrix3x3<T> & argLeft,
                             const Matrix<T> & argMat)
  {
    return TNT::matmult((Matrix<T>)argLeft, argMat);
  }

  /** @relates Matrix4x4
  matrix-matrix multiplication using 'Mult'
  @author Jan Woetzel, status untested (04/17/2002) */
  template <class T>
    inline Matrix4x4<T> operator*(const Matrix4x4<T>& mat,
    const Matrix4x4<T> & argMat)
  {
    Matrix4x4<T> resMat;
    mat.Mult(argMat, resMat);
    return resMat;
  }

  /** @relates Matrix3x4 */
  template <class T> inline Matrix3x4<T>
    operator*(const Matrix3x3<T>& mat, const Matrix3x4<T> &arg)
  {
    Matrix3x4<T> dest;
    mat.Mult(arg, dest);
    return dest;
  }


} // namespace -------------------




#endif // __Operators_hh__
