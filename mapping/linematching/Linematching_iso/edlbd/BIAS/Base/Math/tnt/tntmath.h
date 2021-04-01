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



// Header file for scalar math functions

#ifndef TNTMATH_H
#define TNTMATH_H

// conventional functions required by several matrix algorithms

// min and max macros are defined in WIN32, collision results
#ifdef WIN32
#undef min
#undef max
#endif //WIN32

namespace TNT 
{

  inline double abs(double t)
  {
    return ( t > 0 ? t : -t);
  }

  inline double min(double a, double b)
  {
    return (a < b ? a : b);
  }

  inline double max(double a, double b)
  {
    return (a > b ? a : b);
  }

  // JW 04/23/2002
  inline double max(int a, int b)
  {
    return (a > b ? a : b);
  }

  // JMF 01/27/2003
  inline double min(int a, int b)
  {
    return (a < b ? a : b);
  }

  inline float abs(float t)
  {
    return ( t > 0 ? t : -t);
  }

  inline float min(float a, float b)
  {
    return (a < b ? a : b);
  }

  inline float max(float a, float b)
  {
    return (a > b ? a : b);
  }

  inline double sign(double a)
  {
    return (a > 0 ? 1.0 : -1.0);
  }



  inline float sign(float a)
  {
    return (a > 0.0 ? 1.0f : -1.0f);
  }

} /* namespace TNT */


#endif
/* TNTMATH_H */

