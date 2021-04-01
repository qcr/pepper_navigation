/* 
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2004
  Johan Skoglund
  skoglund@isy.liu.se
  CVL/isy
  university of Linköping
  sweden

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

#ifdef __Filterinst_hh__
#error Filterinst already defined
#endif

#define __Filterinst_hh__


  /** @file Filterinst.hh
      @brief inline code for instantiation of filter classes

      usage: define FILTER_INSTANTION_CLASS as your filter class, e.g. 
      #define FILTER_INSTANTION_CLASS Gauss
      Decide whether you want to allow unsigned output, which makes no sense
      for gradient filters, because they might produce negative values
      if you want to prohibit instantiation of unsigned output images,
      #define FILTER_INSTANTIATION_NO_UNSIGNED_OUTPUT
      Do all this in the cpp file of your class implementation and then
      #include "Filterinst.hh"
      which explicitly creates all instances that are alowed by your current
      cmake configuration, e.g. BUILD_IMAGE_SHORT, ...

      WARNING: This class steers the generation of all filter objects of
      BIAS, dont touch this unless you REALLY know what you are doing.
  */



#ifndef FILTER_INSTANTIATION_NO_UNSIGNED_OUTPUT
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned char, unsigned char>;
 #if defined(BUILD_IMAGE_USHORT) 
 template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned short,unsigned short>;
 template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned char, unsigned short>;
 #endif
 #if defined(BUILD_IMAGE_UINT)
 template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned int, unsigned int>;
 #endif
#endif

template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<float, float>;
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned char, float>;

#if defined(BUILD_IMAGE_CHAR)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned char, char>;
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<char, char>;
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<char, float>;
#endif

#if defined(BUILD_IMAGE_SHORT)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<short, short>;
#endif

#if defined(BUILD_IMAGE_SHORT)&&defined(BUILD_IMAGE_USHORT)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned short, short>;
#endif

#if defined(BUILD_IMAGE_INT)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<int,int>;
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned char,int>;
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<int,float>;
#endif

#if defined(BUILD_IMAGE_USHORT)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned short,float>;
#endif

#if defined(BUILD_IMAGE_USHORT) && defined(BUILD_IMAGE_INT)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<unsigned short,int>;
#endif

#if defined(BUILD_IMAGE_DOUBLE)
template class BIASFilter_EXPORT FILTER_INSTANTIATION_CLASS<double,double>;
#endif

#undef FILTER_INSTANTIATION_CLASS
