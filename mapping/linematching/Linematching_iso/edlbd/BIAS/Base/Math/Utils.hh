#ifndef __Base_Math_Utils_hh__
#define __Base_Math_Utils_hh__

#define _USE_MATH_DEFINES
#include <math.h>
#include <iostream>
#include <limits>


// digits used for cout,cerr console display

#ifndef CONSOLE_PRECISION_DIGITS 
#  define CONSOLE_PRECISION_DIGITS 10
#endif // CONSOLE_PRECISION_DIGITS 


namespace BIAS {



  /** @brief computes sin and cosinus with float precision

  Cg equivalent function.
  @author Jan Woetzel */
  inline void sincos(const float xRad, float & s, float & c)
  {
    s = sin(xRad);
    c = cos(xRad);
  }
  inline void sincos(const double xRad, double & s, double & c)
  {
    s = sin(xRad);
    c = cos(xRad);
  } 

  /** @brief linear interpolation
  Cg equivalent function.
  @author Jan Woetzel  */
  inline float lerp(const float a, const float b, const float f)
  {
    return (1-f)*a +b*f;
  } 
  inline double lerp(const double a, const double b, const double f)
  {
    return (1-f)*a +b*f;
  } 



  /** @brief Helper returning true on console ostreams like cerr, cout
  Useful to distinguish between console (cout,cerr) and other i.e. fout fstreams.
  @author Jan Woetzel */
  inline 
    bool IsConsoleStream(const std::ostream & os)
  {
    std::streambuf *pb = os.rdbuf();
    // if pb is one of the following we are streaming to a console
    // that may need different formatting
    return 
      (  (pb==std::cout.rdbuf()) 
      || (pb==std::cerr.rdbuf()) );
  }


  /** @brief determien the number of deciamls to store, w.g. to file

  @returns the number of decimals between 
  lower bound guaranteed precision (digits10) 
  and the upper bound max. precision (nr. of binary digits)
  @author Jan Woetzel */
#ifdef WIN32
#  pragma warning(push, 2)
#endif // WIN32
  template <class T>
    inline 
    unsigned int DecimalsToStore()
  {
    if (std::numeric_limits<T>::is_integer){
      // integer types can display exactly one deciaml more than their guaranteed precision digits
      return std::numeric_limits<T>::digits10 +1;
    } else {
      //return std::numeric_limits<T>::digits10; // lower bound
      return std::numeric_limits<T>::digits;   // upper bound
    }
  }
#ifdef WIN32
#  pragma warning(pop)
#endif // WIN32

} // namespace

#endif // __Operators_hh__
