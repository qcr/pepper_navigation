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


#ifndef __BIASERROR_HH__
#define __BIASERROR_HH__


/**
DEPRECATED macros:
- unused in BIAS and MIP and conflict with THROW macro from OpenEXR. (Jan Woetzel 2005/09/06)
- THROW, THROW_LIST 
- DEF_EXCEPTIONS
- BIAS_CHECK_NAN 
*/								     



// Jan Woetzel
#include "bias_config.h" 

// assertions are also switched of if COMPILE_NDEBUG is defined 
#include <assert.h>
#include <iostream>
#include <sstream>
#include <string>
/// exit, abort, EXIT_FAILURE, 
#include <stdlib.h> 
// required for BIASABORT and BIASEXIT macros (jw)
#ifdef WIN32
#  include <Base/Debug/biasabort.hh>
#  include <Base/Debug/biasexit.hh>
#  include <windows.h> // required for OutputDebugString (jw)
#  ifdef BIAS_HAVE_WXWIDGETS
#    include <wx/msw/winundef.h>
#  endif
#endif



#ifndef ERROR_STREAM 
#  ifdef WIN32
//   only cout goes to simple W32 console directly (without rdbuf etc.) (JW)
#    define ERROR_STREAM std::cout
#  else // WIN32
//   cout and cout goes to console on Linux
#    define ERROR_STREAM std::cerr 
#  endif // WIN32
#endif // ERROR_STREAM 



/** @brief macro to display the complete function name a line of code belongs to.
See MSDN or g++ docu for details.
Works with g++ 3.3 and MS Visual Studo 7.1 
@author Jan Woetzel */
#ifdef WIN32
#  define FUNCNAME  __FUNCTION__
#else // WIN32
#  define FUNCNAME  __PRETTY_FUNCTION__
#endif // WIN32



/** set a breakpoint programmatically 
This code is handled as a breakpoint exception. 
Thus a messagebox will pop up where you can choose 
- to "break" (which means use this like a normal breakpoint) 
- or "ignore" to continue skipping the breakpoint. 
Depends on BIAS_DEBUg and not COMPILE_DEBUG,
thus works for Debug *and* Release build!
Useful to set a breakpoint in code, 
e.g. in addition to an exit or failure return
for MS VS IDE Debugging. VS 71 debugger
see http://www.highprogrammer.com/alan/windev/visualstudio.html
and http://msdn2.microsoft.com/en-us/library//f408b4et.aspx
@author Jan Woetzel */
#if defined(BIAS_DEBUG) && defined(WIN32) && defined(_MSC_VER)
//#include <intrin.h>
//#  define BIASBREAK __asm { int 3 }
#  define BIASBREAK {__debugbreak();}
#else
#  define BIASBREAK {}
#endif



/** @brief macro for error handling
Puts message in Debug mode to a destination and adds file+line for "click+jump" in Xemacs on Linux and Visual Studio on Windows.
- The output s written to cerr 
- *AND* DebugConsole on WIN32 
- thus you can click and jump to code.
In particular you can start main *and* WinMain applications in VS71 debugger and click directly in the "output window" to jump to code.
No output is generated on Release compilation to maximize speed. 
The name of the temporary stringstream has to be unique due to the scope !!!
@author Jan Woetzel  */
#if defined(BIAS_DEBUG) && defined(WIN32)

#  define BIASERR(arg) { \
  ERROR_STREAM<<std::flush<<"Error in "<<__FILE__<<":"<<__LINE__<<" : "<<FUNCNAME<<std::endl \
  <<"  "<<arg<<std::endl<<std::flush; \
  std::stringstream ss_BIASERR_unique4713; \
  ss_BIASERR_unique4713<<std::endl<<__FILE__<<"("<<__LINE__<<") : "<<FUNCNAME<<std::endl \
  <<"  BIAS Error: "<<arg<<std::endl; \
  OutputDebugStringA( ss_BIASERR_unique4713.str().c_str() ); \
}

/** macro for warning msg. including file+line for click and jump 
The temporaries require unique names due to the scope limitation. 
@author Jan Woetzel */
#  define BIASWARN(arg) { \
  ERROR_STREAM<<std::flush<<"Warning in "<<__FILE__<<":"<<__LINE__<<" : "<<std::endl \
  <<"  "<<arg<<std::endl<<std::flush; \
  std::stringstream ss_BIASWARN_unique4193; \
  ss_BIASWARN_unique4193<<std::endl<<__FILE__<<"("<<__LINE__<<") : "<<std::endl \
  <<"  BIAS Warning: "<<arg<<std::endl; \
  OutputDebugStringA( ss_BIASWARN_unique4193.str().c_str() ); \
}

#else // defined(BIAS_DEBUG) && defined(WIN32)

// all other: output format for Emacs click and jump (JW)
#  define BIASERR(arg) { \
  ERROR_STREAM<<std::flush<<"Error in "<<__FILE__<<":"<<__LINE__<<" : "<<FUNCNAME<<"\n" \
  <<"  "<<arg<<std::endl<<std::flush; \
}

#  define BIASWARN(arg) { \
  ERROR_STREAM<<std::flush<<"Warning in "<<__FILE__<<":"<<__LINE__<<" :\n\t" \
  <<"  "<<arg<<std::endl<<std::flush; \
}

#endif // defined(BIAS_DEBUG) && defined(WIN32)



/** Issue a warning only once and only in debug mode at runtime.
Avoids flooding with warning noise.
Particularly useful to issue a runtime warning in an inner loop. 
@author Jan Woetzel */
#if defined (BIAS_DEBUG) && defined(WIN32)

#  define BIASWARNONCE(arg) { \
  static bool _oncerun_BIASWARNONCE_localscope_=false;\
  if (!_oncerun_BIASWARNONCE_localscope_) { \
    ERROR_STREAM<<std::flush<<"Warning once in "<<__FILE__<<":"<<__LINE__<<" :"<<std::endl \
    <<"\t"<<arg<<std::endl<<std::flush; \
    std::stringstream ss_BIASWARN_unique4193; \
    ss_BIASWARN_unique4193<<std::endl<<__FILE__<<"("<<__LINE__<<") : "<<std::endl \
    <<"\tBIAS Warning: "<<arg<<std::endl; \
    OutputDebugStringA( ss_BIASWARN_unique4193.str().c_str() ); \
    _oncerun_BIASWARNONCE_localscope_ = true;\
  }\
}

#elif defined (BIAS_DEBUG) && !defined(WIN32)

#  define BIASWARNONCE(arg) { \
  static bool _oncerun_BIASWARNONCE_localscope_=false;\
  if (!_oncerun_BIASWARNONCE_localscope_) { \
    ERROR_STREAM<<std::flush<<"Warning once in "<<__FILE__<<":"<<__LINE__<<" :\n\t" \
    <<"\t"<<arg<<std::endl<<std::flush; \
    _oncerun_BIASWARNONCE_localscope_ = true;\
  }\
}

#else // ! BIAS_DEBUG

#  define BIASWARNONCE(arg)

#endif // WIN32


/** macro around new catching exceptions */
#define BIASNEW(arg1,arg2)\
  try {arg1= new arg2;} catch (std::bad_alloc){ BIASERR("new failed");};


/** @brief macro to be used instead of abort()
\todo: call MIP::biasabort instead of abort - but this introduces one library dependency.
@author Jan Woetzel **/
#if defined(WIN32) && defined(_MSC_VER)
#define BIASABORT { \
  std::cout<<std::flush; std::cerr<<std::flush \
  <<__FILE__<<"("<<__LINE__<<") : aborting."<<std::endl<<std::flush; \
  std::stringstream ss_BIASABORT_unique_1375; \
  ss_BIASABORT_unique_1375<<std::endl \
  <<__FILE__<<"("<<__LINE__<<") : aborting."<<std::endl; \
  OutputDebugStringA( ss_BIASABORT_unique_1375.str().c_str() ); \
  __debugbreak(); \
  BIAS::biasabort(); \
}
#else
#define BIASABORT { \
  std::cout<<std::flush; std::cerr<<std::flush \
  <<__FILE__<<", "<<__LINE__<<" aborting."<<std::endl<<std::flush; \
  abort(); \
}
#endif



/** @brief macro to be used instead of assert()
Expands to output on debug and on release:
-Linux release nothing,
-Windows release: Attention!
Assume that condition holds and let the compiler exploit 
this knowledge for optimization. 
Be warned: This may unintentionally optimize away your error handling code!
see http://msdn2.microsoft.com/en-us/library//1b3fsfxw.aspx
@author Jan Woetzel **/
#if defined(BIAS_DEBUG) && defined(WIN32) && defined(COMPILE_DEBUG) && defined(_MSC_VER) && !defined(COMPILE_NDEBUG)
// Windows and Microsoft compiler, break in debugger
#  define BIASASSERT(cond) { \
  if (!(cond)) { \
  std::cout<<std::flush; \
  std::cerr<<std::flush; \
  std::cerr \
  <<__FILE__<<"("<<__LINE__<<") : BIASASSERT failed. "<<std::endl<<std::flush; \
  std::stringstream ss_BIASASSERT_unique_1142; \
  ss_BIASASSERT_unique_1142<<std::endl \
  <<__FILE__<<"("<<__LINE__<<") : BIASASSERT failed. "<<std::endl; \
  OutputDebugStringA( ss_BIASASSERT_unique_1142.str().c_str() ); \
  __debugbreak(); \
  }; \
  assert(cond); \
}
#define BIASASSERT_ISACTIVE

#elif defined(BIAS_DEBUG) && defined(WIN32) && defined(COMPILE_DEBUG) && !defined(_MSC_VER) && !defined(COMPILE_NDEBUG)
// Windows - but possibly not Micrsoft compiler, drop __debugbreak
#  define BIASASSERT(cond) { \
  if (!(cond)) { \
  std::cout<<std::flush; \
  std::cerr<<std::flush; \
  std::cerr \
  <<__FILE__<<"("<<__LINE__<<") : BIASASSERT failed. "<<std::endl<<std::flush; \
  std::stringstream ss_BIASASSERT_unique_1142; \
  ss_BIASASSERT_unique_1142<<std::endl \
  <<__FILE__<<"("<<__LINE__<<") : BIASASSERT failed. "<<std::endl; \
  OutputDebugStringA( ss_BIASASSERT_unique_1142.str().c_str() ); \
  }; \
  assert(cond); \
}
#define BIASASSERT_ISACTIVE

#elif defined(BIAS_DEBUG) && !defined(WIN32) && !defined(COMPILE_NDEBUG)
// Linux - xemacs output format
#  define BIASASSERT(cond) { \
  if (!(cond)) { \
  std::cout<<std::flush; \
  std::cerr<<std::flush; \
  std::cerr \
  <<__FILE__<<","<<__LINE__<<" : BIASASSERT failed. "<<std::endl<<std::flush; \
  }; \
  assert(cond); \
}
#define BIASASSERT_ISACTIVE

///  to be discussed for opimizing builds - assume conditions hold. (JW)
//#elif !defined(BIAS_DEBUG) && defined(WIN32)
//#  define BIASASSERT(cond) __assume(cond)

#else
#  define BIASASSERT(cond)
#endif


/** @brief macro to be used instead of exit(int)
Sample usage:
\verbatim
  BIASEXIT(EXIT_FAILIRE, "my short description");
\endverbatim
@autor Jan Woetzel **/
#if defined(WIN32) && defined(_MSC_VER)
#define BIASEXIT(exitvalue_, mesg_) { \
  std::cerr<<std::flush<<std::endl<<"exit request in "<<__FILE__<<","<<__LINE__<<" : ret="<<exitvalue_<<" "<<std::endl<<mesg_<<std::endl; \
  __debugbreak(); \
  BIAS::biasexit(exitvalue_); \
}

#else // WIN32

#define BIASEXIT(exitvalue_, mesg_) { \
  std::cerr<<std::flush<<std::endl<<"exit request in "<<__FILE__<<","<<__LINE__<<" : ret="<<exitvalue_<<" "<<std::endl<<mesg_<<std::endl; \
  exit(exitvalue_); \
}
#endif // WIN32


/** verbose writing calling information to screen in Debug mode.
Typical usage: 
\verbatim
void MyClass::MyFunction(...)
{
  CALLINFO; ...
}
\endverbatim
@author Jan Woetzel */
#ifdef BIAS_DEBUG
#  define CALLINFO {std::cout<<"called "<<__FILE__<<", "<<__LINE__<<": "<<FUNCNAME<<std::endl<<std::flush;}
#else //BIAS_DEBUG
#  define CALLINFO
#endif // BIAS_DEBUG



/** alias for pthread C interface for mutex locking */
#define BIAS_MUTEX_LOCK(arg)   pthread_mutex_lock(arg);
#define BIAS_MUTEX_UNLOCK(arg) pthread_mutex_unlock(arg);



/** DEPRECATED - JW 12/2005
- unused in BIAS,MIP,S2DT 
- bad style, please use templated isnan/isinf etc. */
#define BIAS_CHECK_NAN  BIAS_CHECK_NAN_is_deprecated_because_never_used_see_BIAS_Error_hh


/** DEPRECATED - JW 12/2005 
not used  (at least in BIAS,MIP,SCan2DTracking... ) */
/*
//#define DEF_EXCEPTION(theClass, theText) \
//class  theClass : public Exception { theClass() : Exception(theText) {} };
*/
//#define DEF_EXCEPTION   DEF_EXCEPTION_is_deprecated_beause_never_used_see_BIAS_Error_hh


/** 
not finished... to be continued... (JW) 
checked_cast uses fast static_cast in Release build
but checks cast with an ASSERT in Debug.
Typical usage:
class Foo { ...  };
class Bar : public Foo { ...  };
Foo * pFoo = new Bar;
Bar * pBar = checked_cast<Bar *>(pFoo);
see inspiration source:  http://www.highprogrammer.com/alan/windev/visualstudio.html  
and http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vcmex/html/vclrf__try_cast.asp
and http://msdn.microsoft.com/library/default.asp?url=/library/en-us/vclang/html/_pluslang_Casting_Operators.asp
-- does not (yet) work as expected, furterh work required (JW) --
@author Jan Woetzel 2005 */
/*
//template <class destT, class srcT>
//TypeTo checked_cast(srcT p)
//{
//    BIASASSERT(dynamic_cast<destT>(p));
//    return static_cast<destT>(p);
//};
*/

#endif // __BIASERROR_HH__

