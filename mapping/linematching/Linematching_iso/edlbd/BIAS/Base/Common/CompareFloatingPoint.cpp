
#include <Base/Common/CompareFloatingPoint.hh>
#include <cmath>

using namespace BIAS;
using namespace std;

namespace BIAS {
  template <typename T>
  T Cast(const double input)
  { return static_cast<T>(rint(input)); }

  template <> BIASCommon_EXPORT
  double Cast<double>(const double input)
  { return input; }

  template <> BIASCommon_EXPORT
  float Cast<float>(const double input)
  { return static_cast<float>(input); }

  template <typename T>
  bool Equal(const T left, const T right, const T eps) 
  {
    return (left==right); 
  }

  template <> BIASCommon_EXPORT
  bool Equal<double>(const double left, const double right, 
                     const double eps) 
  {
    // cover the trivial case
    if (left==right) return true;
    // the code below does not work when one value is zero
    if (left==0. || right==0.){
      return (fabs(left-right) <= eps);
    }
    // compare two non-zero values
    return ( fabs(left-right) <= eps * fabs(right) || 
             fabs(left-right) <= eps * fabs(left) ); 
  }
  
  template <> BIASCommon_EXPORT
  bool Equal<float>(const float left, const float right, 
                    const float eps) 
  {
    // cover the trivial case
    if (left==right) return true;
    // the code below does not work when one value is zero
    if (left==0. || right==0.){
      return (fabs(left-right) <= eps);
    }
    // compare two non-zero values
    return (fabs(left-right) <= eps * fabs(right) || 
            fabs(left-right) <= eps * fabs(left) ); 
  }
 

}


/////////////////////////////////////////////////////////////////////
// instantiation

#define INST(type)\
template BIASCommon_EXPORT type BIAS::Cast<type>(const double);\
template BIASCommon_EXPORT bool BIAS::Equal<type>(const type left, const type right, const type eps);

INST(unsigned char)
INST(int)
INST(char)
INST(short)
INST(unsigned short)
INST(long int)
INST(unsigned int)
INST(float)
INST(double)

