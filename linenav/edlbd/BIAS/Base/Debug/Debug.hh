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


#ifndef __BIASDEBUG_HH__
#define __BIASDEBUG_HH__

#include <bias_config.h>


#if defined WIN32 && defined(_DLL)
//#  include <Base/Common/BIASpragmaStart.hh> // for DLL build
#pragma warning(push)
#pragma warning(disable: 4251) // disable only STL DLL linking warning noise
#endif

#include <iostream>
#include <iomanip>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <limits>
#include <typeinfo>
#ifdef __APPLE__
# include <math.h>
#endif // __APPLE__

/** \ingroup g_utils
    \file Debug.hh
    \brief some macros, classes and varaibles for debugging purposes:

    The class variables _liDebugLevel and _zDebugStream
      must exist for the macros to operate. Usually they are inherited
      from the Debug class defined below

    _liDebuglevel is a bitfield. It is set to 0 in the constr. of Debug
      The bits should be addressed via a #define macros from the header file
      of the acording class.

    _zDebugStream is a ostream. It is set to cerr in the constructor of
      Debug

    BIASDOUT(flag, stream)
       prints stream to cerr if flag is set in _liDebuglevel

    BIASDOUT_THREAD is the aequivalent to BIASDOUT for the use within a thread

    BIASGDOUT(level, stream)
       prints out debug information if BIASGlobalDebugLevel is greater than
       level. It can only be used together with the global debug level.

    The global debuglevel variable 'BIASGlobalDebugLevel' is defined in
      MainDebug.hh. It should be included only in the application.

    The global debug level 'BIASGlobalDebugLevel' should be defined in every
      executable via the include of <Debug/MainDebug.hh>.

    A function ResetDebugLevelFromGlobal() can be implemented in classes that
      need it. See Examples...

    An exhaustive example exists.

    \author Felix Woelk
**/



#ifdef BIAS_DEBUG


#define BIASDOUT(flag, arg)\
    { if ((this->_liDebugLevel & flag)!=0){\
        std::ostream _cods_(std::cout.rdbuf()); \
        this->GetDebugStream(_cods_); _cods_ << #flag << ", " << __FILE__ \
                      << ", " << __LINE__ << " : " << arg << std::endl; } }

#define BIASFLUSHDOUT \
    { std::ostream _cods_(std::cout.rdbuf()); \
    this->GetDebugStream(_cods_); _cods_.flush(); }

#define BIASDOUT_THREAD(debuglevel, debugstream, flag, arg)\
    if ((debuglevel & flag)!=0)\
        debugstream << #flag << ", " << __FILE__ << ", " << __LINE__ << " : "\
                    << arg << std::endl;

#define BIASGDOUT(level, arg)\
	if (Debug::GlobalDebugLevel >= level)\
       std::cerr << arg << std::endl;

#define BIASDOUT_CLEAN(flag, arg)\
    { if ((this->_liDebugLevel & flag)!=0){\
        std::ostream _cods_(std::cout.rdbuf()); \
        this->GetDebugStream(_cods_); _cods_ << arg << std::endl; } }

#define BIASCDOUT(flag, arg)\
    { if ((this->_liDebugLevel & flag)!=0){\
        std::ostream _cods_(std::cout.rdbuf()); \
        this->GetDebugStream(_cods_); _cods_ << arg; } }

#define BCDOUT(name, arg)\
    { if ( (this->_liDebugLevel & Debug::Name2DebugLevel(#name) ) !=0 ){\
        std::ostream _cods_(std::cout.rdbuf()); \
        this->GetDebugStream(_cods_); _cods_ << arg; } }
# ifdef __APPLE__
#  define INFNANCHECK(a) { if (std::isnan(a) || std::isinf(a)){\
  std::cout << __FILE__<<":"<<__LINE__ << "   "<<#a<<" = "<<a<<std::endl; \
  assert(!std::isnan(a) && !std::isinf(a));\
  } }
# else // __APPLE__
#  define INFNANCHECK(a) { if (isnan(a) || isinf(a)){\
  std::cout << __FILE__<<":"<<__LINE__ << "   "<<#a<<" = "<<a<<std::endl; \
  assert(!isnan(a) && !isinf(a));\
  } }
# endif // __APPLE__

#else

#define BIASDOUT_CLEAN(flag, arg) {}
#define BIASDOUT(flag, arg) {}
#define BIASFLUSHDOUT {}
#define BIASDOUT_THREAD(debuglevel, debugstream, flag, arg) {}
#define BIASGDOUT(level, arg) {}
#define BIASCDOUT(flag, arg) {}
#define BCDOUT(name, arg) {}
#define INFNANCHECK(a) {}

#endif // BIAS_DEBUG

/*
// TODO: Please use BIASABORT instead!
#ifdef WIN32
#define ABORT std::cout << "Please hit enter to terminate program!" << std::endl << std::flush; \
        getchar(); \
        exit(-1);
#else
#define ABORT abort();
#endif
*/

/// this returns the type as a string, e.g. PRINTTYPE(int) -> "int"
/// useful when debugging templates
/// on WIN32 this can be done more pretty with typeid( *ptr ).name() (jw)
#define PRINTTYPE(thetype)  (\
  (typeid(thetype)==typeid(char))? "char": \
  (typeid(thetype)==typeid(signed char))?"signed char":\
  (typeid(thetype)==typeid(unsigned char))?"unsigned char":\
  (typeid(thetype)==typeid(short))? "short": \
  (typeid(thetype)==typeid(signed short))? "signed short": \
  (typeid(thetype)==typeid(unsigned short))?"unsigned short":\
  (typeid(thetype)==typeid(int))? "int":\
  (typeid(thetype)==typeid(unsigned int))?"unsigned int":\
  (typeid(thetype)==typeid(signed int))?"signed int":\
  (typeid(thetype)==typeid(double))?"double":\
  (typeid(thetype)==typeid(float))?"float":\
  (typeid(thetype)==typeid(long))?"long":\
  "UnknownStorageType")


namespace BIAS{

  /* this variable os the global debuglevel and should be defined in
     every executable via include of <Debug/MainDebug.hh> */
  //extern long int GlobalDebugLevel;


  /** \class Debug
      \ingroup g_utils
      \file Debug.hh
      \brief some macros, classes and varaibles for debugging purposes
           if a class is derived from Debug, then use BIASDOUT() instead of
           cerr/cout for debugging purposes

      father of many classed to includes flags based debug information

      The class variables _liDebugLevel and _zDebugStream
      must exist for the macros to operate. Usually they are inherited
      from the Debug class defined below

      _liDebuglevel is a bitfield. It is set to 0 in the constr. of Debug
      The bits should be addressed via a #define macros from the header file
      of the acording class.

      _zDebugStream is a ostream. It is set to cerr in the constructor of
      Debug

      - BIASDOUT(flag, stream)
        prints stream to cerr if flag is set in _liDebuglevel

      - BIASDOUT_THREAD is the aequivalent to BIASDOUT for the use within a thread
      - BIASGDOUT(level, stream)
        prints out debug information if BIASGlobalDebugLevel is greater than
        level. It can only be used together with the global debug level.

      - BIASDOUT_CLEAN(flag, stream)
        is similar to BIASDOUT except that no flag,file and line is printed


      The global debuglevel variable 'BIASGlobalDebugLevel' is defined in
      Debug.cpp.

      A function ResetDebugLevelFromGlobal() can be implemented in classes that
      need it. See Examples...

      An exhaustive example exists.

      New debuglevel concept avoids the proble of multiple debuglevel
      definitions:

      Pros:
      - each class keeps tracks of available debuglevels itself
      - class inherits knowledge about debuglevel from parents
      - class can print all available debuglevels
      - inhibits double definition of debuglevels in inheritance hirarchy
      Cons:
      - slower (map lookup)
      - when using a unknown debuglevel, no compile time error is produced
        (only a runtime error)

      Usage Summary:
      - call NewDebugLevel("name") for creation of debuglevel in class
        constructor
        (replaces old #define name 0x00001)
      - use macro BCDOUT(name, "my error message"<<endl) in class
        (replaces old BIASCDOUT(name, ""))
      - for enabling debug output call AddDebugLevel("name") or
        SetDebugLevel("name")
        (replaces old SetDebugLevel(name) and AddDebugLevel(name) )

      Problem Example:

      // file father.hh
      #define DL_FATHER 0x1
      class father{
        foo(){ BIASCDOUT(DL_FATHER, "bla");}
      };
      // file child.hh
      #include "father.hh"
      #define DL_CHILD 0x1
      class child : public father
      {
        foo2(){ foo(); BIASCDOUT(DL_CHILD, "fasel"); }
      };

      calling
        child.SetDebugLevel(DL_CHILD);
        child.foo2();

      results in the unexpected additional debug output "bla".



      To avoid this use new debugconcept:

      // file father.hh
      class father{
        father() { NewDebugLevel("DL_FATHER"); }
        foo(){ BIASCDOUT(DL_FATHER, "bla");}
      };

      // file child.hh
      #include "father.hh"
      class child : public father
      {
        child() : father() { NewDebugLevel("DL_CHILD"); }
        foo2(){ foo(); BIASCDOUT(DL_CHILD, "fasel"); }
      };

      calling
        child.SetDebugLevel("DL_CHILD");
        child.foo2();

      results in the only in the debug output "fasel".

      \author Felix Woelk
  **/

  class BIASDebug_EXPORT Debug {

  public:
    inline Debug()
    {
#ifdef BIAS_DEBUG
      _liDebugLevel = 0;
      _liNextDebugLevel = 1;
#endif
    };

    inline Debug(const Debug& deb)
    {
#ifdef BIAS_DEBUG
      *this = deb;
#endif
    }

    inline ~Debug() {};

    inline void SetDebugLevel(const long int lv)
    {
#ifdef BIAS_DEBUG
      _liDebugLevel = lv;
#endif
    };

    inline void SetDebugLevel(const std::string &name)
    {
#ifdef BIAS_DEBUG
      SetDebugLevel(Name2DebugLevel(name));
#endif
    };

    inline int GetDebugLevel() const
    {
#ifdef BIAS_DEBUG
      return _liDebugLevel;
#else
      return 0;
#endif
    };

    inline bool DebugLevelIsSet(const long int lv) const
    {
#ifdef BIAS_DEBUG
      return ((_liDebugLevel & lv)!=0);
#else
      return false;
#endif
    };

    inline bool DebugLevelIsSet(const std::string& name) const
    {
      return DebugLevelIsSet(Name2DebugLevel(name));
    };

    inline void AddDebugLevel(const long int lv)
    {
#ifdef BIAS_DEBUG
      _liDebugLevel = _liDebugLevel | lv;
#endif
    };

    inline void AddDebugLevel(const std::string& name)
    {
#ifdef BIAS_DEBUG
      AddDebugLevel(Name2DebugLevel(name));
#endif
    };

    inline void RemoveDebugLevel(const long int lv)
    {
#ifdef BIAS_DEBUG
      _liDebugLevel = _liDebugLevel & (~lv);
#endif
    };

    inline void RemoveDebugLevel(const std::string& name)
    {
#ifdef BIAS_DEBUG
      RemoveDebugLevel(Name2DebugLevel(name));
#endif
    };

    inline void PrintDebugLevel(std::ostream& os=std::cout) const
    {
#ifdef BIAS_DEBUG
#define WS sizeof(long int)*8
      os << "0x" << std::hex << std::setw(sizeof(long int)<<1)
         << std::setfill('0') << GetDebugLevel() << std::dec << " = ";
      for (unsigned i=1; i<=WS; i++){
        os << !!((1<<(WS-i))&GetDebugLevel());
        if ((i%4)==0) os << " ";
        if ((i%8)==0) os << " ";
      }
      os << std::endl;
#endif
    }

    inline void SetDebugStream(const std::ostream& os)
    {
#ifdef BIAS_DEBUG
      _zDebugStream.rdbuf(os.rdbuf());
#endif
    }

    inline std::ostream& GetDebugStream() const
    {
#ifdef BIAS_DEBUG
      return _zDebugStream;
#else
      return std::cout;
#endif
    }

    inline void GetDebugStream(std::ostream& os) const
    {
#ifdef BIAS_DEBUG

      os.rdbuf(_zDebugStream.rdbuf());
#else
      os.rdbuf(std::cout.rdbuf());
#endif
    }

    inline static void SetGlobalDebugLevel(long int lev)
    {
#ifdef BIAS_DEBUG
      GlobalDebugLevel = lev;
#endif
    }

    inline static long int GetGlobalDebugLevel()
    {
#ifdef BIAS_DEBUG
      return GlobalDebugLevel;
#else
      return 0;
#endif
    }

    inline Debug& operator=(const Debug& deb)
    {
#ifdef BIAS_DEBUG
      _liDebugLevel = deb._liDebugLevel;
      _zDebugStream.rdbuf(deb._zDebugStream.rdbuf());
      _liNextDebugLevel = deb._liNextDebugLevel;
      _String2Debuglevel = deb._String2Debuglevel;
#endif
      return *this;
    }

    /** looks up a debuglevel in the internal map,
        returns 0 if not found
        @author woelk 09/2006 */
    inline long int Name2DebugLevel(const std::string& name) const
    {
#ifdef BIAS_DEBUG
      std::map<std::string, long int>::const_iterator it =
        _String2Debuglevel.find(name);
      if (it == _String2Debuglevel.end()){
        std::cerr << "Debuglevel \""<<name<<"\" is unknown. Please use one "
                  <<"of the following: \n";
        ShowDebugLevel(std::cerr);
        std::cerr << "\n\n\n";
        return 0;
      }
      return it->second;
#else
      return 0;
#endif
    }

    /** creates a new debuglevel
        @author woelk 09/2006 */
    inline long int NewDebugLevel(const std::string& name)
    {
#ifdef BIAS_DEBUG
      long int res = ConsumeNextFreeDebuglevel_();
      std::map<std::string, long int>::iterator it
        = _String2Debuglevel.find(name);
      if (it != _String2Debuglevel.end()){
        std::cerr << "debuglevel with name \""<<name<<"\" does already exist, "
                  <<"please choose a different name!\n\n\n";
       // ABORT;
        abort();
      } else {
        _String2Debuglevel.insert(make_pair(name, res));
        /* ShowDebugLevel(std::cerr); */
      }
      return res;
#else
      return 0;
#endif
    }

    /** @brief prints all internally known debuglevels
        @author woelk 09/2006 */
    inline void ShowDebugLevel(std::ostream& os = std::cout) const
    {
#ifdef BIAS_DEBUG
      std::map<std::string, long int>::const_iterator it;
      for (it = _String2Debuglevel.begin();
           it != _String2Debuglevel.end(); it++){
        os << " 0x"<<std::hex<<it->second<<" : "
           <<it->first<<std::dec<<std::endl;
      }
#endif
    }

  protected:
#ifdef BIAS_DEBUG
    long int _liDebugLevel;
    static std::ostream _zDebugStream;

    static long int GlobalDebugLevel;

    /// new concept, debuglevel are managed here in the debug class
    long int _liNextDebugLevel;
    std::map<std::string, long int> _String2Debuglevel;

    /** @brief returns the next available debuglevel
        @author woelk 09/2006 */
    long ConsumeNextFreeDebuglevel_() {
      long dl = _liNextDebugLevel;
      //if ((long)_liNextDebugLevel == (long)(std::numeric_limits<long>::max())) // FIXME, max redef.
      if ((long)_liNextDebugLevel > (long)(1<<30)) // workaround
      {
        std::cerr << "maximum number of debuglevels reached\n";
       // ABORT;
        abort();
      }
      _liNextDebugLevel = _liNextDebugLevel<<1;
      return dl;
    }
#endif

#ifdef WIN32
    static int dummy_; //< export something to avoid conditional Win32 Release exports
#endif
  };
} // namespace


#if defined WIN32 && defined(_DLL)
//#  include <Base/Common/BIASpragmaEnd.hh> // for DLL build
#pragma warning(pop)
#endif


#endif // __BIASDEBUG_HH__
