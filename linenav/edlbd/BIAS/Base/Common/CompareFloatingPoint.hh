/* This file is part of the BIAS library (Basic ImageAlgorithmS).

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */
#ifndef _CompareFloatingPoint_hh_
#define _CompareFloatingPoint_hh_

#include <bias_config.h>
#include <limits>
#include <cmath>
#include <bits/stl_algobase.h>


namespace BIAS {

/** @brief comparison function for floating point values
    See http://www.boost.org/libs/test/doc/components/test_tools/
                     floating_point_comparison.html
    for discussion
    @author woelk 02/2006 */
template <class T> // left == right
bool Equal(const T left, const T right,
     const T eps = std::numeric_limits<T>::epsilon());

  /** @brief comparison function for floating point values
   *  @test tested with TestCompareFloatingPoint.cpp
      @author woelk 02/2006 */
  template <typename T> // left < right
  inline bool Less(const T left, const T right, 
                   const T eps = std::numeric_limits<T>::epsilon()) 
  { if (left==0. || right==0.) return (right-left) > eps;
    return ((right-left) > eps * std::min(right,left)); }
  
  /** @brief comparison function for floating point values   
      @author woelk 02/2006 */
  template <typename T>  // left <= right
  inline bool LessEqual(const T left, const T right, 
                        const T eps = std::numeric_limits<T>::epsilon())
  { return (Less(left, right, eps) || Equal(left, right, eps)); }

  /** @brief comparison function for floating point values
      @author woelk 02/2006 */
  template <class T> // left > right
  inline bool Greater(const T left, const T right, 
                      const T eps = std::numeric_limits<T>::epsilon()) 
  { if (left==0. || right==0.) return (left-right) > eps;
    return ((left-right) > eps * std::min(right,left)); }

  /** @brief comparison function for floating point values
      @author woelk 02/2006 */
  template <class T> // left >= right
  inline bool GreaterEqual(const T left, const T right, 
                           const T eps = std::numeric_limits<T>::epsilon()) 
  { return (Greater(left,right) || Equal(left, right, eps)); }

  /** @brief correct casting to storage type, incorporates rint for integer 
      values and simple casting for floating point values */
  template <typename T> BIASCommon_EXPORT
  T Cast(const double input);

}

#endif // _CompareFloatingPoint_hh_
