/*
This file is distributed as part of the BIAS library (Basic ImageAlgorithmS)
but it has not been developed by the authors of BIAS.

For copyright, author and license information see below.
*/


/*
*
* Template Numerical Toolkit (TNT): Linear Algebra Module
*
* Mathematical and Computational Sciences Division
* National Institute of Technology,
* Gaithersburg, MD USA
*
*
* This software was developed at the National Institute of Standards and
* Technology (NIST) by employees of the Federal Government in the course
* of their official duties. Pursuant to title 17 Section 105 of the
* United States Code, this software is not subject to copyright protection
* and is in the public domain.  The Template Numerical Toolkit (TNT) is
* an experimental system.  NIST assumes no responsibility whatsoever for
* its use by other parties, and makes no guarantees, expressed or implied,
* about its quality, reliability, or any other characteristic.
*
* BETA VERSION INCOMPLETE AND SUBJECT TO CHANGE
* see http://math.nist.gov/tnt for latest updates.
*
*/



// C compatible matrix: row-oriented, 0-based [i][j] and 1-based (i,j) indexing
//

#ifndef CMAT_H
#define CMAT_H

#include "subscript.h"
#include "vec.h"
#include <cstdlib>
#include <cassert>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <limits>
#include <cstring>
#include <cstdio>

#ifdef TNT_USE_REGIONS
#  include "region2d.h"
#endif

#include <Base/Math/Utils.hh>
//#include <Base/Debug/Error.hh>

#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
#include <Base/Math/Polynom.hh>
#endif

namespace TNT
{

  template <class T>
  class Matrix 
  {


  public:

    typedef Subscript   size_type;
    typedef         T   value_type;
    typedef         T   element_type;
    typedef         T*  pointer;
    typedef         T*  iterator;
    typedef         T&  reference;
    typedef const   T*  const_iterator;
    typedef const   T&  const_reference;

    Subscript lbound() const { return 1;}

  protected:
    Subscript m_;
    Subscript n_;
    Subscript mn_;      // total size
    T* v_;                  
    T** row_;           
    T* vm1_ ;       // these point to the same data, but are 1-based 
    T** rowm1_;

    // internal helper function to create the array
    // of row pointers

    void initialize(Subscript M, Subscript N)
    {
      mn_ = M*N;
      m_ = M;
      n_ = N;

      v_ = new T[mn_]; 
      row_ = new T*[M];
      rowm1_ = new T*[M];

      BIASASSERT(v_  != NULL);
      BIASASSERT(row_  != NULL);
      BIASASSERT(rowm1_ != NULL);

      T* p = v_;              
      vm1_ = v_ - 1;
      for (Subscript i=0; i<M; i++)
      {
        row_[i] = p;
        rowm1_[i] = p-1;
        p += N ;

      }

      rowm1_ -- ;     // compensate for 1-based offset
    }

    void copy(const T*  v)
    {
      Subscript N = m_ * n_;
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
      Subscript i;

      #ifdef TNT_UNROLL_LOOPS
      Subscript Nmod4 = N & 3;
      Subscript N4 = N - Nmod4;

      for (i=0; i<N4; i+=4)
      {
      v_[i] = v[i];
      v_[i+1] = v[i+1];
      v_[i+2] = v[i+2];
      v_[i+3] = v[i+3];
      }

      for (i=N4; i< N; i++)
      v_[i] = v[i];
      #else

      for (i=0; i< N; i++)
      v_[i] = v[i];
      #endif    
#else
      memcpy((void*) v_, (void*) v, sizeof(T)*N);
#endif
    }

    void set(const T& val)
    {
      Subscript N = m_ * n_;
      Subscript i;

#ifdef TNT_UNROLL_LOOPS
      Subscript Nmod4 = N & 3;
      Subscript N4 = N - Nmod4;

      for (i=0; i<N4; i+=4)
      {
        v_[i] = val;
        v_[i+1] = val;
        v_[i+2] = val;
        v_[i+3] = val; 
      }

      for (i=N4; i< N; i++)
        v_[i] = val;
#else

      for (i=0; i< N; i++)
        v_[i] = val;

#endif      
    }



    void destroy()
    {     
      /* do nothing, if no memory has been previously allocated */
      if (v_ == NULL) 
        return ;

      /* if we are here, then matrix was previously allocated */
      if (v_ != NULL) { 
        delete [] (v_); v_ = NULL; 
      }
      if (row_ != NULL) { 
        delete [] (row_); row_ = NULL; 
      }

      /* return rowm1_ back to original value */
      rowm1_ ++;
      if (rowm1_ != NULL ) { delete [] (rowm1_); rowm1_ = NULL; }

      m_= mn_=n_ =0;
    }


  public:

    // Autocast operator causes collision with [] operator in WIN32,
    // because subscription M[] is also possible via cast and 
    // [] operator on resulting array (streckel)
    //operator T**(){ return  row_; }
    //operator T**() const { return row_; }


    Subscript size() const { return mn_; }

    // constructors

    Matrix() : m_(0), n_(0), mn_(0), v_(NULL), row_(NULL), vm1_(NULL), rowm1_(NULL) {};


    Matrix(const Matrix<T> &A)
    {
      initialize(A.m_, A.n_);
      copy(A.v_);
    }

    Matrix(Subscript M, Subscript N, const T& value = T())
    {
      initialize(M,N);
      set(value);
    }

    Matrix(Subscript M, Subscript N, const T* v)
    {
      initialize(M,N);
      copy(v);
    }

    //Matrix(Subscript M, Subscript N, const char *s)
    //{
    //    initialize(M,N);
    //    std::istringstream ins(s);
    //    Subscript i, j;
    //    for (i=0; i<M; i++)
    //        for (j=0; j<N; j++)
    //            ins >> row_[i][j];
    //}

    /// replacement for above JW 
    Matrix(Subscript M, Subscript N, const std::string & s)
    {
      initialize(M,N);
      std::istringstream ins(s);
      Subscript i, j;
      for (i=0; i<M; i++)
        for (j=0; j<N; j++)
          ins >> row_[i][j];
    }


    // destructor
    //
    virtual ~Matrix()
    {
      destroy();
    }


    // reallocating
    //
    Matrix<T>& newsize(Subscript M, Subscript N)
    {
      if (num_rows() == M && num_cols() == N)
        return *this;

      destroy();
      initialize(M,N);

      return *this;
    }




    // assignments
    //
    Matrix<T>& operator=(const Matrix<T> &A)
    {
      if (v_ == A.v_)
        return *this;

      if (m_ == A.m_  && n_ == A.n_)      // no need to re-alloc
        copy(A.v_);

      else
      {
        destroy();
        initialize(A.m_, A.n_);
        copy(A.v_);
      }

      return *this;
    }

    Matrix<T>& operator=(const T& scalar)
    { 
      set(scalar); 
      return *this;
    }


    Subscript dim(Subscript d) const 
    {
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT( d >= 1);
      BIASASSERT( d <= 2);
#endif
      return (d==1) ? m_ : ((d==2) ? n_ : 0); 
    }

    Subscript num_rows() const { return m_; }
    Subscript num_cols() const { return n_; }




    inline T* operator[](Subscript i)
    {
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(0<=i);
      BIASASSERT(i < m_) ;
#endif
      return row_[i];
    }

    inline const T* operator[](Subscript i) const
    {
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(0<=i);
      BIASASSERT(i < m_) ;
#endif
      return row_[i];
    }

    inline reference operator()(Subscript i)
    { 
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(1<=i);
      BIASASSERT(i <= mn_) ;
#endif
      return vm1_[i]; 
    }

    inline const_reference operator()(Subscript i) const
    { 
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(1<=i);
      BIASASSERT(i <= mn_) ;
#endif
      return vm1_[i]; 
    }



    inline reference operator()(Subscript i, Subscript j)
    { 
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(1<=i);
      BIASASSERT(i <= m_) ;
      BIASASSERT(1<=j);
      BIASASSERT(j <= n_);
#endif
      return  rowm1_[i][j]; 
    }



    inline const_reference operator() (Subscript i, Subscript j) const
    {
#ifdef TNT_BOUNDS_CHECK
      BIASASSERT(1<=i);
      BIASASSERT(i <= m_) ;
      BIASASSERT(1<=j);
      BIASASSERT(j <= n_);
#endif
      return rowm1_[i][j]; 
    }



#ifdef TNT_USE_REGIONS

    typedef Region2D<Matrix<T> > Region;


    Region operator()(const Index1D &I, const Index1D &J)
    {
      return Region(*this, I,J);
    }


    typedef const_Region2D< Matrix<T> > const_Region;
    const_Region operator()(const Index1D &I, const Index1D &J) const
    {
      return const_Region(*this, I,J);
    }
#endif

    /** @author Sandro Esquivel */
    inline
    std::ostream&
    PrintPretty(std::ostream &s,
                const std::string &name = "",
                const int width = 8,
                const bool alignLeft = true) const;

    /** @author Ingo Schiller */
    inline
    std::ostream&
    Print(std::ostream &s,
    			const int width,
    			const int precision,
     		  bool scientific=true) const;

    /** @author Jan Woetzel */
    inline
      std::ostream& 
      Print(std::ostream &s, 
      const bool intCastOutput=false, 
      const bool binaryOutput=false,
      const bool forceFullPrecision=false ) const;


    /** @author Jan Woetzel */
    inline
      std::istream& 
      Read(std::istream &s, 
      const bool intCastInput=false, 
      const bool binaryInput=false );

  }; // ------------------ class ------------------------




  /* ***************************  I/O  ********************************/

  /** @relates Matrix
      @author Sandro Esquivel
  */
  template <class T>
  inline std::ostream& Matrix<T>::
  PrintPretty(std::ostream &s, const std::string &name,
              const int width, const bool alignLeft) const
  {
     const int m = this->num_rows();
     const int n = this->num_cols();
     const int w = width + 8;
     std::ios_base::fmtflags oldFlags = s.flags(); // store flags
     std::string leftspace = "";
     if (!name.empty()) {
       s << std::endl << name << " =";
       leftspace = name + "  ";
       for (unsigned int i = 0; i < name.size(); i++)
         leftspace.at(i) = ' ';
     } else {
       s << std::endl;
     }
     char chars[128];
     for (int i = 0; i < m; i++) {
       if (i > 0)
         s << leftspace;
       s << " [ ";
       for (int j = 0; j < n; j++) {
#if WIN32
         sprintf_s(chars, "% .8g", (double)(*this)[i][j]);
#else
         sprintf(chars, "% .8g", (double)(*this)[i][j]);
#endif
         std::string str(chars);
         if (alignLeft) s << str;
         for (int k = 0; k < w - (int)str.size(); k++)
           s << " ";
         if (!alignLeft) s << str;
       }
       s << " ]" << std::endl;
     }
     s.flags(oldFlags); // restore flags
     return s;
   }

  /** @relates Matrix
   @author Ingo Schiller */

   template <class T>
     inline
     std::ostream&
     Matrix<T>::Print(std::ostream &s,
    		 const int width,
    		 const int precision,
    		 bool scientific) const
   {
     int M=this->num_rows();
     int N=this->num_cols();

     std::ios_base::fmtflags oldFlags = s.flags(); // push

     s<<std::setw(width);
     s<<std::setprecision(precision);
     // header with dimensions
     s<<M<<" "<<N<<"\n";

     if(scientific) s<<std::scientific;

     // s.setf(std::ios_base::left, std::ios_base::adjustfield);
     for (int i=0; i<M; i++)
     {
       for (int j=0; j<N; j++)
       {
         s<<std::setw( width );
         s<<std::setprecision( precision );
         s<<(*this)[i][j];
         s<<" ";
       }
       s<<"\n";
     }
     s.flags(oldFlags); // pop
     return s;
   }

  /** @relates Matrix
  @author Jan Woetzel */

  template <class T>    
    inline
    std::ostream& 
    Matrix<T>::Print(std::ostream &s, 
    const bool intCastOutput, 
    const bool binaryOutput,
    const bool forceFullPrecision) const
  {
    int M=this->num_rows();
    int N=this->num_cols();

    std::ios_base::fmtflags oldFlags = s.flags(); // push

    s<<std::setprecision(std::numeric_limits<int>::digits10 +1 );
    // header with dimensions 
    s<<M<<" "<<N;

    unsigned int dig(0), w(0);

    if (BIAS::IsConsoleStream(s) && (!forceFullPrecision)){
      // cout or cerr uses shorter human redable format with less precision
      dig = std::numeric_limits<T>::digits10;
      w=dig+1;
    } else {
      dig  = BIAS::DecimalsToStore<T>();
      w=dig+8;
      // binary or ascii output? 
      if (binaryOutput){
        s<< " b";
        BIASERR("not implemented");
        BIASBREAK;
        //s<<std::binary;
      } else {
        s<<std::scientific;
      }
    }
    s<<"\n"; //<< header line done

    //#ifdef TNT_BOUNDS_CHECK
    BIASASSERT(dig>0);
    BIASASSERT(w>0);
    //#endif

    // s.setf(std::ios_base::left, std::ios_base::adjustfield);
    for (int i=0; i<M; i++)
    {
      for (int j=0; j<N; j++)
      {
        s<<std::setw( w );
        s<<std::setprecision( dig );
#ifndef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
        if (intCastOutput){
          s<<(int)((*this)[i][j]);
        } else 
#endif
        {
          s<<(*this)[i][j];
        }
        s<<" ";
      }
      s<<"\n";
    }

    s.flags(oldFlags); // pop

    return s;
  }


  /// JW
  template <class T>    
    inline
    std::istream& 
    Matrix<T>::Read(std::istream &s, 
    const bool intCastInput, 
    const bool binaryInput) 
  {
    if (binaryInput){
      BIASERR("not implemented");
      BIASBREAK;
    }
    int M=0, N=0;
    s>>M>>N;
    if (M<0 || N<0) {
      BIASERR("could not read from stream. wrong format?");
      BIASBREAK;
      s.setstate(std::ios::badbit | std::ios::failbit);
      return s;
    }
    if ( !(M==this->num_rows() && N==this->num_cols() ))
    {
      this->newsize(M,N);
    }

#ifndef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
    if (intCastInput){

      // << was doing cast to int, e.g. for char
      int tmp (0);
      for (int i=0; i<M; i++){
        for (int j=0; j<N; j++){
          s>>tmp;
          //#ifdef TNT_BOUNDS_CHECK
          BIASASSERT((long)tmp>=(long)std::numeric_limits<T>::min() );
          BIASASSERT((long)tmp<=(long)std::numeric_limits<T>::max() );
          //#endif
          this->row_[i][j] = (T)tmp;
        }
      }

    } else 
#endif    
    {

      // no cast required
      for (int i=0; i<M; i++){
        for (int j=0; j<N; j++)
        {
          //s >> ((this)[i][j]);
          s>>this->row_[i][j];
        }
      }

    }
    return s;
  }



  /** @relates Matrix @author Jan Woetzel */
  inline
    std::ostream& operator<<(std::ostream &s, const Matrix<char> &A)
  {
    return A.Print(s, true); // cast to int
  }

  /** @relates Matrix @author Jan Woetzel */
  inline
    std::ostream& operator<<(std::ostream &s, const Matrix<unsigned char> &A)
  {
    return A.Print(s, true ); // cast to int
  }

#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
  inline
  std::ostream& operator<<(std::ostream &s, const Matrix<BIAS::Polynom> &A)
  { 
    int M=A.num_rows();
    int N=A.num_cols();
    
    s<<M<<" "<<N << "\n";
    for (int i=0; i<M; i++) {
      for (int j=0; j<N; j++) {
        s << i << "," << j << " -> " << A[i][j] << "\n";
      }
    }
    return s;
  }
#endif

  /** @relates Matrix @author Jan Woetzel */
  template <class T>
    inline
    std::ostream& operator<<(std::ostream &s, const Matrix<T> &A)
  {
    return A.Print(s);
  }


  /** @relates Matrix @author Jan Woetzel */
  inline
    std::istream& operator>>(std::istream &s, Matrix<unsigned char> &A)
  {
    return A.Read(s, true); // cast to int
  }


  /** @relates Matrix @author Jan Woetzel */
  inline
    std::istream& operator>>(std::istream &s, Matrix<char> &A)
  {
    return A.Read(s, true); // cast to int
  }

  /** @relates Matrix @author Jan Woetzel */
  template <class T> 
    inline
    std::istream& operator>>(std::istream &s, Matrix<T> &A)
  {
    return A.Read(s);
  }


  // *******************[ basic matrix algorithms ]***************************

  /** @relates Matrix
  */
  template <class T>
    Matrix<T> operator+(const Matrix<T> &A, 
    const Matrix<T> &B)
  {
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    BIASASSERT(M==B.num_rows());
    BIASASSERT(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
      for (j=0; j<N; j++)
        tmp[i][j] = A[i][j] + B[i][j];

    return tmp;
  }

  /** @relates Matrix
  */
  template <class T>
    Matrix<T> operator-(const Matrix<T> &A, 
    const Matrix<T> &B)
  {
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    BIASASSERT(M==B.num_rows());
    BIASASSERT(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
      for (j=0; j<N; j++)
        tmp[i][j] = A[i][j] - B[i][j];

    return tmp;
  }

  template <class T>
    Matrix<T> mult_element(const Matrix<T> &A, 
    const Matrix<T> &B)
  {
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    BIASASSERT(M==B.num_rows());
    BIASASSERT(N==B.num_cols());

    Matrix<T> tmp(M,N);
    Subscript i,j;

    for (i=0; i<M; i++)
      for (j=0; j<N; j++)
        tmp[i][j] = A[i][j] * B[i][j];

    return tmp;
  }


  template <class T>
    Matrix<T> transpose(const Matrix<T> &A)
  {
    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Matrix<T> S(N,M);
    Subscript i, j;

    for (i=0; i<M; i++)
      for (j=0; j<N; j++)
        S[j][i] = A[i][j];

    return S;
  }



  template <class T>
    inline Matrix<T> matmult(const Matrix<T>  &A, 
    const Matrix<T> &B)
  {

#ifdef TNT_BOUNDS_CHECK
    BIASASSERT(A.num_cols() == B.num_rows());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    Matrix<T> tmp(M,K);
    T sum;

    for (Subscript i=0; i<M; i++)
      for (Subscript k=0; k<K; k++)
      {
        sum = 0;
        for (Subscript j=0; j<N; j++)
          sum = sum +  A[i][j] * B[j][k];

        tmp[i][k] = sum; 
      }

      return tmp;
  }

  /** @relates Matrix
  */
  template <class T>
    inline Matrix<T> operator*(const Matrix<T>  &A, 
    const Matrix<T> &B)
  {
    return matmult(A,B);
  }

  template <class T>
    inline int matmult(Matrix<T>& C, const Matrix<T>  &A, 
    const Matrix<T> &B)
  {

    BIASASSERT(A.num_cols() == B.num_rows());

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();
    Subscript K = B.num_cols();

    C.newsize(M,K);

    T sum;

    const T* row_i;
    const T* col_k;

    for (Subscript i=0; i<M; i++)
      for (Subscript k=0; k<K; k++)
      {
        row_i  = &(A[i][0]);
        col_k  = &(B[0][k]);
        sum = 0;
        for (Subscript j=0; j<N; j++)
        {
          sum  += *row_i * *col_k;
          row_i++;
          col_k += K;
        }
        C[i][k] = sum; 
      }

      return 0;
  }


  template <class T>
    Vector<T> matmult(const Matrix<T>  &A, const Vector<T> &x)
  {

#ifdef TNT_BOUNDS_CHECK
    BIASASSERT(A.num_cols() == x.dim());
#endif

    Subscript M = A.num_rows();
    Subscript N = A.num_cols();

    Vector<T> tmp(M);
    T sum;

    for (Subscript i=0; i<M; i++)
    {
      sum = 0;
      const T* rowi = A[i];
      for (Subscript j=0; j<N; j++)
        sum = sum +  rowi[j] * x[j];

      tmp[i] = sum; 
    }

    return tmp;
  }


  /** @relates Matrix  */
  template <class T>
    inline Vector<T> operator*(const Matrix<T>  &A, const Vector<T> &x)
  {
    return matmult(A,x);
  }

} // namespace TNT

#endif
// CMAT_H
