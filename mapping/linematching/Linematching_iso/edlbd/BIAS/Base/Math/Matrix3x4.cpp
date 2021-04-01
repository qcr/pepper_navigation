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


#include "Matrix3x4.hh"
// STD
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>


using namespace BIAS;
using namespace std;

namespace BIAS {

template <class T>
Matrix3x4<T>::~Matrix3x4() {}


template<class T>
Matrix3x4<T>::Matrix3x4(
  const T a0, const T a1, const T  a2, const T  a3, 
  const T a4, const T a5, const T  a6, const T  a7, 
  const T a8, const T a9, const T a10, const T a11 ) 
  : Matrix<T>(3, 4)
{
  this->GetData()[0]=a0; this->GetData()[1]=a1; this->GetData()[ 2]= a2; this->GetData()[ 3]= a3;
  this->GetData()[4]=a4; this->GetData()[5]=a5; this->GetData()[ 6]= a6; this->GetData()[ 7]= a7;
  this->GetData()[8]=a8; this->GetData()[9]=a9; this->GetData()[10]=a10; this->GetData()[11]=a11;
}


template<class T>
Matrix3x4<T>::Matrix3x4(const std::string & s)
: Matrix<T>(3, 4, s) 
{}


template<class T>
Matrix3x4<T>& Matrix3x4<T>::operator= (const Matrix<T> &mat) {
    if ((mat.num_rows() == 3) && (mat.num_cols() == 4)) {
        Matrix<T>::operator= (mat);	// call the operator of the base class
        return *this;
    } else {
        BIASERR("cannot assign with operator= , num_rows=" << mat.num_rows() << " num_cols= " << mat.num_rows()
               << " are'nt appropriate for a 3x4 Matrix3x4<T>.");
#if DEBUG
        BIASABORT;
#endif
    };
    return *this;
}

template
<class T>
Matrix3x4<T> & Matrix3x4<T>::newsize(int rows, int cols) {
    if ((rows != 3) || (cols != 4)) {
        // do nothing
        // print error message if the new size should not be a 3x4 matrix
        BIASERR("The size of a Matrix3x4<T> has to be 3 rows, 4 cols. newsize can't create a " <<rows<< " x " <<cols<< " Matrix3x4 !");
#if DEBUG
        BIASABORT;
#endif
        return *this;

    } else {
        BIASASSERT( rows==3 ); // 3x4 matrix
        BIASASSERT( cols==4 );
        // call base class function (usually unnecessary except for first construction)
        Matrix<T>::newsize(3, 4);
        return *this;
    }
}


// JW
template <class T>
int 
Matrix3x4<T>::LoadBan(const std::string & filename, const unsigned int & Time){
    ifstream fs( filename.c_str() );
    if (!fs) {
        return -1; // error, could not open file. 
    } else {
        // parse and read in 
        const unsigned int nMax=1024;
        // character array for getline
        char caLine[nMax];
        // std string for find
        string strLine;
        bool found=false;
        string::size_type pos=string::npos;
        // pattern to find
        stringstream patternSS; 
        patternSS<<"AddCameraKey { Time "<<setw(1)<<int(Time)<<" Camera [ ";
        string pattern=patternSS.str();
        while (!fs.eof() && (fs) &&(!found)){
            fs.getline(&caLine[0], nMax);
            strLine = caLine;
            pos = strLine.find(pattern);
            if (pos != string::npos){
                // found pattern
                found=true;
                pos+=pattern.size();
                string sub;
                sub = strLine.substr(pos,  strLine.size()-pos );
                // helper to stream string into numeric variable
                istringstream iss( sub );

                // read numerics
                for (unsigned int row=0; row<3; row++) {
                    for (unsigned int col=0; col<4; col++) {
                        iss >> (*this)[row][col];
                    }
                }
                return 0; // OK.
            };
        };
        fs.close();
    };
    return -2; // not found.
}


template <class T>
int 
Matrix3x4<T>::LoadPARTXT(const std::string & filenameParTxt,
                         const unsigned int & index,
                         std::string & correspImgFilename,
                         int & nTotal)
{  
  ifstream fs;
  fs.open(filenameParTxt.c_str() );   
  if (!fs){    
    BIASERR("could not open "<<filenameParTxt);
    return(-1);
  }
  unsigned int uNumOfMat=0;
  // read number of matrices in file
  fs >> uNumOfMat;  
    
  nTotal=uNumOfMat;
  if(index>=uNumOfMat)
  {
    BIASERR("index greater then number of matrices");
    return(-1);
  }
  
  unsigned int uMc=0;
  bool found=false;
  
  std::string tempImgFilename;
  Matrix3x4<T> tempMat;  
  Matrix3x3<T> tempK;
  Matrix3x3<T> tempR;
  Vector3<T>   tempC;
  
  unsigned int uRow=0;
  unsigned int uCol=0;
  
  while (!found &&fs && !fs.eof())
  {    
    fs>>tempImgFilename;
    if(fs.eof())
    {
      BIASERR("Unexpected end of file");
      return(-1);
    } 
    // read k-matrix
    for(uRow=0;uRow<3;uRow++)
      for(uCol=0;uCol<3;uCol++)
      {
        fs>>tempK[uRow][uCol];
        if(fs.eof())
        {
          BIASERR("Unexpected end of file");
          return(-1);
        }
      }   
     
    // read r-matrix
    for(uRow=0;uRow<3;uRow++)
      for(uCol=0;uCol<3;uCol++)
      {
        fs>>tempR[uRow][uCol];
        if(fs.eof())
        {
          BIASERR("Unexpected end of file");
          return(-1);
        }
      }
             
    // read t-vector
    for(uRow=0;uRow<3;uRow++)
    {
      if(fs.eof())
      {
        BIASERR("Unexpected end of file");
        return(-1);
      }
      fs>>tempC[uRow];
    }
    if(uMc==index)
    {
      found=true;       
    }
    uMc++;
  }
  // The Seitz' projection matrix representation is given by K*[R t]  
  Matrix3x3<T> mKr=tempK*tempR;  
  //save K*R in P
  for(uRow=0;uRow<3;uRow++)
    for(uCol=0;uCol<3;uCol++)
      {
        tempMat[uRow][uCol]=mKr[uRow][uCol];
        tempMat[uRow][uCol]=mKr[uRow][uCol];
        tempMat[uRow][uCol]=mKr[uRow][uCol];
      }
  //save K*t in P
  Vector3<T> vKt = tempK * tempC;
  tempMat[0][3]=vKt[0]; 
  tempMat[1][3]=vKt[1];
  tempMat[2][3]=vKt[2];  
      
  //everything went fine!
  //save the results.
  (*this)=tempMat;
  correspImgFilename=tempImgFilename;
  
  return(0); // OK
}


} // end namespace BIAS 

#define INST(type) \
template class BIASMathBase_EXPORT BIAS::Matrix3x4<type>;\

INST(unsigned char)
INST(char)
INST(float)
INST(short)
INST(unsigned short)
INST(long int)
INST(int)
INST(unsigned int)
INST(double)
