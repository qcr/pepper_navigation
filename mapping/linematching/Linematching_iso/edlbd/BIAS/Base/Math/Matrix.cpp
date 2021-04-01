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

#include <bias_config.h>

#include <math.h>



#include <Base/Debug/Exception.hh>
#include <Base/Math/Matrix.hh>
#include <Base/Math/Vector.hh>
#include <Base/Math/Vector3.hh>
#include <Base/Math/Matrix3x3.hh>
#include <Base/Math/Polynom.hh>

//additional math constants are not used by default in WIN32 - so set usage

#include <algorithm>
#include <fstream>


using namespace BIAS;
using namespace std;

namespace BIAS {

template<class T>
Matrix<T>::~Matrix()
{}

template <class T>
Matrix<T>::Matrix(int rows, int cols, const std::string & s)
: TNT::Matrix<T>(rows, cols, s)
{}

template <class T>
Matrix<T>::Matrix(const Matrix3x3<T>& mat)
  : TNT::Matrix<T>(3, 3)
{
  for (int r=0; r<3; r++){
    for (int c=0; c<3; c++){
      (*this)[r][c] = mat[r][c];
    }
  }
}


template <class T>
void Matrix<T>::Fill(const T & scalar)
{
  TNT::Matrix<T>::operator=(scalar);
}


template <class T>
void Matrix<T>::NormalizeColsToOne(const int row_to_use)
{
  if (row_to_use >= this->num_rows()) {
    // Error, index for row is expected to be in [0...num_rows-1]
    BIASERR("dim of matrix is (" << this->num_rows() << " x " << this->num_cols() <<
           ").");
    BIASERR("cannot normalize cols by elements in row " << row_to_use <<
           ". aborting. ");

  } else {

    // value of row index is OK for normalization
    double divisor;
    for (int col = 0; col < this->num_cols(); col++) {
      // get the divisor to normalize with
      divisor = (*this)[row_to_use][col];
      if (fabs(divisor) > DOUBLE_EPSILON) {

        // normalize one column (= all rows of one column)
        for (int row = 0; row < this->num_cols(); row++) {
          // normalize element by dividing it by the row_to_use element of
          //the same column
          (*this)[row][col] = ((*this)[row][col]) / (T) divisor;
        };
      } else {
        // could not divide by (nearly) zero
        BIASERR("could not divide by " << divisor <<
               " because it is too close to zero. DOUBLE_EPSILON= "
               << DOUBLE_EPSILON);
        BIASERR("skipping normalization of this column.");
      };
    };
  };
}
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
template <>
void Matrix<BIAS::Polynom>::NormalizeColsToOne(const int row_to_use)
{
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
}
#endif



template <class T>
void Matrix<T>::NormalizeRowsToOne(const int col_to_use)
{
  if (col_to_use >= this->num_cols()) {
    // Error, index for row is expected to be in [0...num_rows-1]
    BIASERR("dim of matrix is (" << this->num_rows() << "," << this->num_cols() <<
           ").");
    BIASERR("cannot normalize rows by elements in col " << col_to_use
           << ". aborting. ");
  } else {
    // value of col index is OK for normalization
    double divisor;
    for (int row = 0; row < this->num_rows(); row++) {
      // get the divisor to normalize with
      divisor = (*this)[row][col_to_use];
      if (fabs(divisor) > DOUBLE_EPSILON) {

        // normalize one row (= all cols of one row)
        for (int col = 0; col < this->num_rows(); col++) {
          // normalize element by dividing it by the col_to_use
          //element of the same column
          (*this)[row][col] = ((*this)[row][col]) / (T) divisor;
        };
      } else {
        // could not divide by (nearly) zero
        BIASERR("could not divide by " << divisor <<
               " because it is too close to zero. DOUBLE_EPSILON= "
               << DOUBLE_EPSILON);
        BIASERR("skipping normalization of this column.");
      };
    };
  };
}
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
template <>
void Matrix<BIAS::Polynom>::NormalizeRowsToOne(const int col_to_use)
{
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
}
#endif


template <class T>
void Matrix<T>::NormalizeRows()
{
  for (int i = 0; i< this->num_rows() ; i++){
    double scale = GetRow(i).NormL2();
    for (int j = 0; j < this->num_cols(); j++)
      (*this)[i][j]=(T)( (*this)[i][j]/scale);
  }
}


template <class T>
void Matrix<T>::NormalizeCols()
{
  for (int j = 0; j< this->num_cols() ; j++){
    double scale = GetCol(j).NormL2();
    for (int i = 0; i < this->num_rows(); i++)
      (*this)[i][j]=(T) ((*this)[i][j]/scale);
  }
}

template<class T>
void Matrix<T>::AbsIP(){
  for (int i = 0; i < this->num_rows(); i++)
  for (int j = 0; j < this->num_cols(); j++)
    ((*this)[i][j]) = T(fabs(float((*this)[i][j])));
}

#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
template <>
void Matrix<BIAS::Polynom>::AbsIP()
{
    BIASERR("Not implemented for BIAS::Polynom");
    BIASABORT;
}
#endif

template<class T>
void Matrix<T>::ScaleRow(int NoRow, T scale)
{
  for (int j = 0; j < this->num_cols(); j++)
    //((*this)[NoRow][j]) *= scale;
    ((*this)[NoRow][j]) = ((*this)[NoRow][j]) * scale;
}


template<class T>
void Matrix<T>::ScaleCol(int NoCol,T scale)
{
  for (int i = 0; i < this->num_rows(); i++)
    //((*this)[i][NoCol]) *= scale;
    (*this)[i][NoCol] =  (*this)[i][NoCol] * scale;
}


template<class T>
Vector<T> Matrix<T>::GetRow(const int& row) const
{
  BIASASSERT(row >= 0 && row < this->num_rows());
  // create a new Vector which will be returned
  // ? faster: copy elements during constructor
  const unsigned int numcols = this->num_cols();
  Vector<T> vec(numcols);

  // copy the elements into the vector:
  for (unsigned int col = 0; col < numcols; col++) {
    vec[col] = (*this)[row][col];
  };

  return vec;
}



template <class T>
Vector<T> Matrix<T>::GetCol(const int& col) const
{
  BIASASSERT(col >= 0 && col < this->num_cols());
  const unsigned int numrows = this->num_rows();
  Vector<T> vec(numrows);
  // copy the elements into the vector:
  for (unsigned int row = 0; row < numrows; row++) {
    vec[row] = (*this)[row][col];
  };
  return vec;
}

template<class T>
void Matrix<T>::SetRow(const int row, const Vector<T>& data)
{
  BIASASSERT(row >= 0 && row < this->num_rows());
  const  int numcols = (int)this->num_cols();
  BIASASSERT(data.size() == numcols);
  for (register int col=0; col<numcols; col++)
    (*this)[row][col] = data[col];
}

template<class T>
void Matrix<T>::SetCol(const int col, const Vector<T>& data)
{
  BIASASSERT(col >= 0 && col < this->num_cols());
  const  int numrows = (int)this->num_rows();
  BIASASSERT(data.size() == numrows);
  for (register int row=0; row<numrows; row++)
    (*this)[row][col] = data[row];
}

template<class T>
void Matrix <T>::GetSubMatrix(const size_t startRow, const size_t startCol,
                              const size_t numRows, const size_t numCols,
                              Matrix<T> &submatrix) const
{
  size_t m = size_t(this->m_);
  size_t n = size_t(this->n_);

  BIASASSERT((startRow+numRows <= m)&&(startCol+numCols <= n));
  BIASASSERT((numRows != 0)&&(numCols != 0));
  
  if ((startRow+numRows != m)||(startCol+numCols != n))
    submatrix.newsize(numRows,numCols);

  for (size_t i=0; i < numRows; i++) {
    memcpy(submatrix.GetData() + i*numCols,
           this->v_+(i+startRow)*n+startCol,sizeof(T)*numCols);
  }
}
  
template<class T>
void Matrix <T>::GetSubMatrix(const Vector<int> &rows, const Vector<int> &cols,
                              Matrix<T> &submatrix) const
{
  BIASASSERT((submatrix.num_rows() == rows.size())&&(submatrix.num_cols() == cols.size()));
  for (int m=0; m<rows.size(); m++) {
    for (int n=0; n<cols.size(); n++) {
      submatrix[m][n] = *(this->v_ + this->n_*rows[m] + cols[n]);
    }
  }
}

template<class T>
Matrix<T> Matrix<T>::GetSubMatrix(const Vector<int> &rows,
                                  const Vector<int> &cols) const
{
  Matrix<T> submatrix(rows.size(),cols.size());
  for (int m=0; m<rows.size(); m++) {
    for (int n=0; n<cols.size(); n++) {
      submatrix[m][n] = *(this->v_ + this->n_*rows[m] + cols[n]);
    }
  }
  return submatrix;
}
  
  
template<class T>
void Matrix <T>::SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                              const Matrix<T> &submatrix,
                              const size_t startRowInSub, const size_t startColInSub,
                              const size_t numRows, const size_t numCols)
{
  size_t n = size_t(this->n_);
  size_t nS = size_t(submatrix.n_);

  BIASASSERT((startRowInThis+numRows <= size_t(this->m_))&&(startColInThis+numCols <= n));
  BIASASSERT((startRowInSub+numRows <= (size_t)submatrix.m_)&&(startColInSub+numCols <= nS));
  
  if ((numRows == 0)||(numCols == 0)) return;
  
  for (size_t i=0; i < numRows; i++) {
    memcpy(this->v_+(i+startRowInThis)*n+startColInThis,
           submatrix.GetData() + (i+startRowInSub)*nS + startColInSub,
           sizeof(T)*numCols);
  }
}


template<class T>
void Matrix<T>::SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                              const Matrix3x3<T> &submatrix)
{
  BIASASSERT((startRowInThis+3 <= size_t(this->m_))
             &&(startColInThis+3 <= size_t(this->n_)));
  
  for (size_t m=0; m < 3; m++) {
    for (size_t n=0; n < 3; n++) {
      this->v_[(m+startRowInThis)*this->n_ + startColInThis+n ]
        = submatrix[m][n];
    }
  }
}


template<class T>
void Matrix<T>::SetSubMatrix(const size_t startRowInThis, const size_t startColInThis,
                              const Vector3<T> &subvector)
{
  BIASASSERT((startRowInThis+3 <= size_t(this->m_))
             &&(startColInThis < size_t(this->n_)));
  
  for (size_t m=0; m < 3; m++) {
      this->v_[(m+startRowInThis)*this->n_ + startColInThis]
        = subvector[m];
  }
}


template<class T>
void Matrix <T>::SetIdentity()
{
  SetZero();
  for (int i = 0; i < this->num_cols() && i < this->num_rows(); i++) {
    //cout << "actual: " << i << endl;
    (*this)[i][i] = 1;
  }
}


/*
template<class T>
void Matrix<T>::set_diagonal_elements(const Vector<T>& argVec,
                                      const bool zero_others)
{
  // Check the dimensions
  BIASASSERT( argVec.size() == min(num_cols(), num_rows()) );
  // Are the otther elements to be set to zero?
  if (zero_others)
    SetZero();
  for(int i = 0; i < argVec.size(); i++)
    (*this)[i][i] = argVec[i];
}
*/

template<class T>
T Matrix<T>::GetMin() const
{
  // initialize result value with last element
  register const T* dataP = GetDataLast();
  T result = *dataP;

  for(; dataP >= GetData(); dataP--)
    if (*dataP < result)
      result = *dataP;

  return result;
}

template<class T>
Matrix<T> Matrix<T>::GetMin(Matrix<T>& m){
  Matrix<T> out(this->num_rows(),this->num_cols());
  for(int i=0; i < this->num_rows(); i++)
    for(int j=0; j < this->num_cols(); j++)
      if(((*this)[i][j]) <= m[i][j]){
        out[i][j] = ((*this)[i][j]);
      }else{
        out[i][j] = m[i][j];
      }

  return out;
}

template<class T>
T Matrix<T>::Normalize()
{
  T greatest = 0;
  for (int i=0; i < this->num_rows(); i++)
    for (int j=0; j < this->num_cols(); j++)
      if ( ((*this)[i][j])*((*this)[i][j]) > greatest*greatest )
        greatest = (*this)[i][j];
  (*this) /= greatest;
  return greatest;
}

template<class T>
T Matrix<T>::GetMax() const
{
  // initialize result value with last element
  register const T* dataP = GetDataLast();
  register T result = *dataP;

  for(; dataP >= GetData(); dataP--)
    if (*dataP > result)
      result = *dataP;

  return result;
}

template<class T>
Matrix<T> Matrix<T>::GetMax(Matrix<T>& m){
Matrix<T> out(this->num_rows(),this->num_cols());
for(int i=0; i < this->num_rows(); i++)
    for(int j=0; j < this->num_cols(); j++)
      if(((*this)[i][j]) >= m[i][j]){
        out[i][j] = ((*this)[i][j]);
      }else{
        out[i][j] = m[i][j];
      }

return out;
}

template<class T>
void Matrix<T>::GetMaxMin(T& max, T& min) const
{
  max = min = *GetDataLast();
  for(register const T* dataP = GetDataLast(); dataP >= GetData(); dataP--) {
    if (*dataP<min) min = *dataP;
    else if (*dataP>max) max = *dataP;
  }
}

template<class T>
void Matrix<T>::GetAbsMaxMin(T& max, T& min) const
{
  BIASERR("does not exist for this storaget type");
  BIASABORT;
}

template<>
void Matrix<double>::GetAbsMaxMin(double& max, double& min) const
{
  max = min = fabs( (*GetDataLast()) );
  for(register const double* dataP = GetDataLast(); dataP >= GetData(); dataP--) {

    if (fabs(*dataP)<min) min = fabs(*dataP);
    else if (fabs(*dataP)>max) max = fabs(*dataP);
  }
}

template<class T>
T Matrix<T>::GetMean() const
{
  // initialize result
  T result = 0;
  for(register const T* dataP = GetDataLast(); dataP >= GetData(); dataP--)
      result = result + T(*dataP);
    //result += T(*dataP);

  result = result/T(GetNumElements());
  return result;
}

template<class T>
bool Matrix<T>::Load(const std::string & filename){
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


template<class T>
bool Matrix<T>::Save(const std::string & filename) const {
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


template<class T>
int Matrix<T>::
WriteMatlab(std::ostream& ostr, const std::string& name) const {
  if (!ostr) {
    BIASERR("Invalid ostream");
    return -1;
  } else {
    ostr << name << " = [ ..." << endl;
    for (int i=0; i<this->num_rows(); i++) {
      for (int j=0; j<=this->num_cols(); j++) {
        ostr << (*this)[i][j] << " ";
      }
      ostr << "; ..." << endl;
    }
    ostr << "];" << endl;
  }
  return 0;
}

template<class T>
bool Matrix<T>::BinRead(const char * filename) const
{
  ifstream filein;
  //filein.open(filename, ios::in);
  filein.open(filename, ios::in|ios::binary);
  if ( !filein.is_open() )
    {
      cerr << "Error opening file '" << filename << "'" << endl;
      return false;
    }

  // read and verify dimension
  int m,n;
  filein.read( (char*) &m, sizeof(int) );
  filein.read( (char*) &n, sizeof(int) );

  if ( m!=this->m_ || n!=this->n_ )
    {
      BIASERR("Matrix has wrong dimension! This one is "
              << this->m_ << "x" << this->n_ << " but file is "
              << m << "x" << n << "." << endl);
      return false;
    }

  // read values
  for(int i=0; i<this->m_; i++)
    for(int j=0; j<this->n_; j++)
      filein.read( (char*) &((*this)[i][j]), sizeof(T) );

  filein.close();
  return true;
}


template<class T>
bool Matrix<T>::BinWrite(const char * filename) const
{
  ofstream fileout;
  //fileout.open(filename, ios::out);
  fileout.open(filename, ios::out|ios::binary);
  if ( !fileout.is_open() )
    {
      cerr << "Error opening file '" << filename << "'" << endl;
      return false;
    }

  // store the dimension
  fileout.write( (char*) &this->m_, sizeof(int) );
  fileout.write( (char*) &this->n_, sizeof(int) );

  // store values
  for(int i=0; i<this->m_; i++)
    for(int j=0; j<this->n_; j++)
      fileout.write( (char*) &((*this)[i][j]), sizeof(T) );

  fileout.close();
  return true;
}


template<>
bool Matrix<double>::IsZero( double eps /*=0.0*/) const
{
  bool result=true;
  for (int i=0; i<this->num_rows(); i++)
    for (int j=0; j<this->num_cols(); j++)
      if ( fabs((*this)[i][j]) > eps )
        result=false;
  return result;
}


template<class T>
bool Matrix<T>::IsZero( double eps /*=0.0*/) const
{
  if ( eps != 0 )
    {
      BIASERR("Epsilon environment is not implemented for this type!");
    }
  bool result=true;
  for (int i=0; i<this->num_rows(); i++)
    for (int j=0; j<this->num_cols(); j++)
      if ( (*this)[i][j] != 0 )
        result=false;
  return result;
}


template<>
bool Matrix<double>::IsIdentity( double eps /*=0.0*/ ) const
{
  bool result=true;
  for (int i=0; i<this->num_rows(); i++)
    for (int j=0; j<this->num_cols(); j++)
      {
        if ( i==j && fabs((*this)[i][j]-1.0) > eps )
          result=false;
        else if ( i!=j && fabs((*this)[i][j]) > eps )
          result=false;
      }
  return result;
}

template<class T>
bool Matrix<T>::IsIdentity( double eps /*=0.0*/ ) const
{
  if ( eps != 0 )
    {
      BIASERR("Epsilon environment is not implemented for this type!");
    }
  bool result=true;
  for (int i=0; i<this->num_rows(); i++)
    for (int j=0; j<this->num_cols(); j++)
      {
        if ( i==j && (*this)[i][j]!=1 )
          result=false;
        else if ( i!=j && (*this)[i][j] != 0 )
          result=false;
      }
  return result;
}

/** vec-operator returns the elements of the matrix columnwise as vector */
template<class T>
void Matrix<T>::Vec(Vector<T> &dest) const {
  unsigned int rows = this->num_rows();
  unsigned int cols = this->num_cols();
  dest.newsize(rows*cols);
  for (unsigned int i=0; i<cols; i++) {
    for (unsigned int j=0; j<rows; j++) {
      dest[i*rows+j] = (*this)[j][i];
    }
  }
}

/** Kronecker-product with matrix B, result in dest */
template<class T>
void Matrix<T>::Kronecker(Matrix<T> const B, Matrix<T> &dest) const {
  unsigned int A_rows = this->num_rows();
  unsigned int A_cols = this->num_cols();
  unsigned int B_rows = B.num_rows();
  unsigned int B_cols = B.num_cols();

  dest.newsize (A_rows*B_rows, A_cols*B_cols);
  for (unsigned int i=0; i<A_rows; i++) {
    for (unsigned int j=0; j<A_cols; j++) {
      for (unsigned int r=0; r<B_rows; r++) {
        for (unsigned int s=0; s<B_cols; s++) {
          dest[i*B_rows+r][j*B_cols+s] = (*this)[i][j] * B[r][s];
        }
      }
    }
  }
}





template<class T>
void Matrix<T>::SwapRows(const int i, const int r)
{
  T tmp;
  const int numc = this->num_cols();
  for (int c=0; c<numc; c++) {
    tmp = (*this)[i][c];
    (*this)[i][c] = (*this)[r][c];
    (*this)[r][c] = tmp;
  }
}

//#define MDOUT(arg) if (verbose) cout << arg;
#define MDOUT(arg) {}

template<class T>
void Matrix<T>::GaussJordan()
{
  //const bool verbose = false;
  const int numr = this->num_rows(), numc = this->num_cols();
  const int max = min(numr, numc);
  ///<< offset to right from diagonal, the working column is given by r+offs
  int offs = 0;
  for (int r=0; r<max; r++){ ///<< r is the working row
    MDOUT("working row: "<<r<<endl);
    // search for the next column wich has non zero elements in or
    // below working row
    for ( ;r+offs<numc && (*this)[r][r+offs]==0.; offs++){
      // search for the next row with non-zero element in current column
      MDOUT(*this<<"\nsearching for the next row with non-zero element "
            <<"in current column "<<r+offs<<endl);
      int rr;
      for (rr=r+1; rr<numr && (*this)[rr][r+offs]==0.; rr++){}
      if (rr!=numr) {
        MDOUT("found next non-zero element in column "<<r+offs<<" and row "<<rr
              <<", swapping rows"<<endl);
        SwapRows(r, rr);
        break;
      }
      MDOUT("only zeros below ["<<r<<"]["<<r+offs<<"], increasing offset\n");
    }
    if (r+offs==numc) {
      // only zeros below and right of [rr][r+offs]
      MDOUT("no further non-zero elements below and right\n");
      break;
    }
    MDOUT("working column: "<<r+offs<<endl);

    /// this is guaranteed by the above
    BIASASSERT((*this)[r][r+offs] != 0.0);

    //	divide working row by value of working position to get a 1 on the
    //	diagonal
    T tempval=(*this)[r][r+offs];
    MDOUT((*this)<<"\nscaling working row by "<<tempval<<endl);
    for (int col=0; col<numc; col++) {
      (*this)[r][col]=(*this)[r][col]/tempval;
    }

    //	eliminate values below working position by subtracting a multiple of
    //	the current row
    MDOUT("eliminitaing entries in working column ("<<r+offs
          <<") below working row ("<<r<<")\n");
    for (long row=r+1; row<numr; ++row) {
      T wval=(*this)[row][r+offs];
      for (int col=0; col<numc; col++) {
        (*this)[row][col]-=wval*(*this)[r][col];
      }
    }
    MDOUT("finished manipulation of row "<<r<<"\n"<<*this<<endl<<"---------\n");
  }

  // zero the elements above the leading element in each row
  MDOUT("zero the elements above the leading element in each row\n");
  for (int r=numr-1; r>=0; --r) {
    // search for leading element
    int c;
    for (c=r; c<numc && (*this)[r][c]==0.; c++){}
    //	eliminate value above working position by subtracting a multiple of
    //	the current row
    for (int row=r-1; row>=0; --row) {
      T wval=(*this)[row][c];
      for (int col=0; col<numc; ++col) {
        (*this)[row][col]-=wval*(*this)[r][col];
      }
    }
  }
  MDOUT("finished computation of reduced row echelon form: "<<*this<<endl);
}
#undef MDOUT

template<class T>
void Matrix<T>::Set(const int row, const int col, const Matrix<T> &data)
{
  if ( row+data.num_rows() > this->num_rows() ||
       col+data.num_cols() > this->num_cols() ){
    // BIASERR("cannot carry data, own size to small");
    BEXCEPTION("cannot carry data, own size to small");
  }
  const int numr = data.num_rows(), numc = data.num_cols();
  int r, c, tr, tc;
  for (r=0, tr=row; r<numr; r++, tr++){
    for (c=0, tc=col; c<numc; c++, tc++){
      (*this)[tr][tc] = data[r][c];
    }
  }
}


template<class T>
void Matrix<T>::Set(const int row, const int col, const Vector<T> &data)
{
  if ( row+data.size() > this->num_rows() ||
       col >= this->num_cols() ){
    // BIASERR("cannot carry data, own size to small");
    BEXCEPTION("cannot carry data, own size to small");
  }
  const int num = data.size();
  int r, tr;
  for (r=0, tr=row; r<num; r++, tr++){
    (*this)[tr][col] = data[r];
  }
}


template<class T>
void Matrix<T>::SetTranspose(const int row, const int col,
                             const Vector<T> &data)
{
  if ( row >= this->num_rows() ||
       col+data.size() > this->num_cols() ){
    // BIASERR("cannot carry data, own size to small");
    BEXCEPTION("cannot carry data, own size to small");
  }
  const int num = data.size();
  int c, tc;
  for (c=0, tc=col; c<num; c++, tc++){
    (*this)[row][tc] = data[c];
  }
}

} // namespace BIAS


///
/// explicit instantiation
///
#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Matrix<type>;\
template class BIASMathBase_EXPORT TNT::Matrix<type>;\


INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
#ifdef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS
INST(BIAS::Polynom)
#endif
