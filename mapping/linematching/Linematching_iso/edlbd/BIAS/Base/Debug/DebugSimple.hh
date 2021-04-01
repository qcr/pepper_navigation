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


#ifndef __DEBUG_SIMPLE_HH__
#define __DEBUG_SIMPLE_HH__

/** 
    @brief Macros for output instead of direct cout<<
    Can be switched on or off globally,
    can be used for logging to file, too.    
    
    The significant difference to BIASDebug is that this macro does not
    touch the class hierarchy and does not add artificial class members.
    But: This debugging scheme supports only levels, not flags.
    If you really need more granular debugging/outputs look at BIAS/Base/Debug.hh . 
    @author Jan Woetzel
**/

// for std::cout / std::cerr
#include <iostream>


/// do COUT outputs?, default is 'on'
#ifndef COUT_ONOFF
#  define COUT_ONOFF 1
#endif

/// which destination for COUT ?
#ifndef COUT_DEST
#  define COUT_DEST std::cout
#endif

// ------ define the macros ------------------------------------------------------
#if defined(COUT_ONOFF) && COUT_ONOFF
// -- COUT's argument is an ostream to be printed
#  define COUT(output) \
     COUT_DEST<<output
#else
// -- do nothing
#  define COUT(output)
#endif



#endif // do not add code after this line
