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


#include "Matrix2x2.hh"
#include "Vector2.hh"
#include "Base/Common/CompareFloatingPoint.hh"




using namespace BIAS;
using namespace std;

namespace BIAS {

#define MATRIX2X2_ZERO_EPS 1e-13


#define MATRIX_DEBUG(arg)

// template<class T> 
// Matrix2x2<T>::Matrix2x2(const Matrix<T> & A) 
// : Matrix<T>(2, 2)
// {
//   if (A.num_rows() != 2 || A.num_cols() != 2){
//     BIASERR("try to cast from Matrix with inavlid size "<<A);
//   } else {
//     register T *dataP = A.GetData();
//     memcpy(v_, dataP, 4*sizeof(T));
//   }
// }

template<class T>
Matrix2x2<T>::Matrix2x2(const TNT::Matrix<T>& mat) : Matrix<T>(mat)
{
  if ((mat.num_rows() != 2) || (mat.num_cols() != 2)) {
    BIASERR("Wrong matrix size "<<mat.num_rows()<<"x"<< mat.num_cols()<<
            " for 2x2 matrix!");
    BIASABORT;
  }
}


template<class T>
Matrix2x2<T>::Matrix2x2(const std::string & s)
: Matrix<T>(2, 2, s) 
{}


template<class T>
Matrix2x2<T>::Matrix2x2(const T a0, const T a1,
                        const T a2, const T a3)
                        : Matrix<T>(2, 2)
{
  this->GetData()[0]=a0; this->GetData()[1]=a1; 
  this->GetData()[2]=a2; this->GetData()[3]=a3; 
}


template<class T> 
Matrix2x2<T>::~Matrix2x2() {}

template<class T>
Matrix2x2<T>& Matrix2x2<T>::operator= (const Matrix2x2<T>& mat) 
{
  register const T *matP = mat.GetData();
  register T *destP = this->GetData();
  memcpy(destP, matP, 4*sizeof(T));
  return *this;
}

template<class T>
Matrix2x2<T>& Matrix2x2<T>::newsize(int rows, int cols) 
{
  BIASERR("newsize("  << rows << ", " << cols << ") does not make sense "
          "for fixed size 2x2 matrix!");
  return *((Matrix2x2 *)NULL);
}


template<>
int Matrix2x2<unsigned int>::Invert(Matrix2x2<unsigned int>& result) const {
  BIASERR("Unsigned integer matrix can not be inverted!");
  BIASBREAK;
  return -1;
}


template<class T>
int Matrix2x2<T>::Invert(Matrix2x2<T>& result) const
{
  T den=(*this)[0][0]*(*this)[1][1]-(*this)[0][1]*(*this)[1][0];
  //if (den==0.0) std::cerr << *this << std::endl << den << std::endl;
  if (den==0.0) return -1;
  BIASASSERT(den!=0.0);

  result[0][0]=(*this)[1][1]/den;
  result[1][0]=-(*this)[1][0]/den;
  result[0][1]=-(*this)[0][1]/den;
  result[1][1]=(*this)[0][0]/den;
  return 0;
}

template<class T>
int Matrix2x2<T>::
EigenvalueDecomposition(T& /*value1*/, Vector2<T>& /*vector1*/, 
                        T& /*value2*/, Vector2<T>& /*vector2*/) const
{
  BIASERR("Eigenvalue decomposition can not be computed for integer matrices!");
  return -1;
}

template<>
int Matrix2x2<double>::
EigenvalueDecomposition(double& value1, Vector2<double>& vec1, 
                        double& value2, Vector2<double>& vec2) const
{
  int res=2;
  double a=(*this)[0][0], b=(*this)[0][1], c=(*this)[1][1], d=(*this)[1][0];
  MATRIX_DEBUG( "a "<<a <<"\tb "<<b<<"\tc "<<c<<"\td "<<d<<endl);
  
  // this code does not work on the matrix
  // 1.74776224781231 4.82304033948846
  // 5.91024707366034 -0.89410165009984

  //output:
  // a 1.74776224781231      b 4.82304033948846      c -0.89410165009984     d 5.91024707366034
  // value1: -5.07318981623247       value2: 5.92685041394494
  // value1: 5.07318981623247        value2: 5.92685041394494
  // rank 2  5.07318981623247        5.92685041394494
  // norm1: 5.85834333486369
  // vec1: [ -0.8232771731 -0.5676395831 ]
  // vec1: [ 0.8232771731 0.5676395831 ]
  // norm2: 9.025320356
  // vec2: [ 0.7557573355 0.654851777 ]
  // vec2: [ 0.7557573355 0.654851777 ]
  // Found 2 real eigenvalues
  // eigenvectors are [ 0.8232771731 0.5676395831 ]  and [ 0.7557573355 0.654851777 ]
  
  // eigenvectors are not orthogonal
  // lapack finds values = [ 5.926850414 -5.073189816 ]  imag=[ 0 0 ] 
  // and  vectors=2 2
  // 0.755757335458229 -0.57734219951434
  // 0.654851777046591 0.816502286990027

  // I suspect symmetry is assumed somewhere, but why is b,d needed then ???
  // koeser 05/2007
  //BIASASSERT(Equal(b,d));




  // eigenvalue decoposition of symmetric 2 by 2 matrix
  // | a b | | x_12 |        | x_12 |
  // | d c | | y_12 | = l_12 | y_12 |
  // characteristic polynomial
  // (lambda-a)*(lambda-c)-b*d = lambda^2 - lambda*a - lambda*c + a*c -b*d
  // = lambda^2 - lambda*(a+c) +a*c-b*d = 0
  // -> \lambda_12 = (a+c)/2 +- \sqrt((a+c)^2/4-ac+bd)
  // x_1 = - b / sqrt(b*b+(a-l1)*(a-l1));
  // y_1 =   (a-l1) / sqrt(b*b+(a-l1)*(a-l1));
  // x_2 = - (c-l2) / sqrt(b*b+(c-l2)*(c-l2));
  // y_2 =   d / sqrt(d*d+(c-l2)*(c-l2));
  double norm1, norm2;
  //TimeMeasure t;
  //t.Start();
      
  norm1=(a+c)/2.0;
  norm2=(a+c)*(a+c)/4.0-a*c+b*d;
  if (norm2 > -numeric_limits<double>::epsilon() && norm2 < 0.0 ) { 
    norm2 = 0.0; 
  }
  if (norm2<0.0) {  // imaginary solution
    MATRIX_DEBUG("only imaginary solutions\n");
    MATRIX_DEBUG("norm2<0.0: "<<norm2<<endl);
    res=0; 
  } else {
    norm2=sqrt(norm2);
    value2=norm1+norm2;
    value1=norm1-norm2;

    // number of eigenvalues!=0
    res = ((fabs(value1)<DOUBLE_EPSILON)?0:1) + 
      ((fabs(value2)<DOUBLE_EPSILON)?0:1);
    MATRIX_DEBUG("rank "<<res<<"\t"<<value1<<"\t"<<value2<<"\n");

    norm1 = sqrt(b*b+(a-value1)*(a-value1));
    MATRIX_DEBUG("norm1: "<<norm1<<endl);
    if (norm1>=MATRIX2X2_ZERO_EPS){ 
      vec1[0] = - b / norm1;
      vec1[1] =   (a-value1) / norm1;
    } else { //happens if b=0 -> cov is already orthogonal
      vec1[0] = 1.0 ;
      vec1[1] = 0.0;
    }
    MATRIX_DEBUG("vec1: "<<vec1<<endl);
      
    norm2 = sqrt(d*d+(c-value2)*(c-value2));
    MATRIX_DEBUG("norm2: "<<norm2<<endl);
    if (norm2>=MATRIX2X2_ZERO_EPS){
      vec2[0] = - (c-value2) / norm2;
      vec2[1] =   d / norm2;
    } else { //happens if b=0 -> cov is already orthogonal
      vec2[0] = 0.0;
      vec2[1] = 1.0;
    }
    MATRIX_DEBUG("vec2: "<<vec2<<endl);
    /*
      t.Stop();
      cout << "manual took "<<t.GetRealTime()<<" us"<<endl;
      t.Reset();
    */

    //     cout << "value1 "<<value1<<"\tvalue2 "<<value2<<endl;
    //     cout << "norm1 "<<norm1<<"\tnorm2 "<<norm2<<endl;
    //     cout << "vec1 : "<<vec1[0]<<", "<<vec1[1]<<endl;
    //     cout << "vec2 : "<<vec2[0]<<", "<<vec2[1]<<endl;
    //     cout << "vec1 . vec2 : "
    //          <<vec1[0]*vec2[0]+vec1[1]*vec2[1]<<endl;
  }
    
  /*
  // compare with svd
  //    t.Start();
  Matrix2x2<double> A, U;
  Vector2<double> S;
  A[0][0]=a;
  A[0][1]=b;
  A[1][1]=c;
  A[1][0]=d;
  SVD svd(A);
  U = svd.GetU();
  S = svd.GetS();
  vec1[0] = S[0] * U[0][0];
  vec1[1] = S[0] * U[0][1];
  vec2[0] = S[1] * U[1][0];
  vec2[1] = S[1] * U[1][1];
  //t.Stop();
  //    cout << "svd   took "<<t.GetRealTime()<<" us"<<endl;
  cout << "\nsvd :\n";
  cout << "vec1 : "<<vec1[0]<<", "<<vec1[1]<<endl;
  cout << "vec2 : "<<vec2[0]<<", "<<vec2[1]<<endl;
  cout << "U: "<<svd.GetU()<<"\nS: "<<svd.GetS()<<"\nVT: "<<svd.GetVT()<<endl;
  */
  
  return res;
}

} // namespace BIAS


#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Matrix2x2<type>;\

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
