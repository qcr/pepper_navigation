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
#ifndef _Exception_hh_
#define _Exception_hh_

#include <exception>
#include <sstream>
#include <iostream>

#ifdef WIN32
/** disable vc++ .net warning 4290 stating that exception types are not 
    checked */
/// \todo never disabled warnign in headers because of the global side effect (jw)
#  pragma warning( disable : 4290 )
#endif

namespace BIAS {
    
  /** macro throwing a BaseException using stream as argument,
      i.e. BEXCEPTION("variable 'a' has content "<<a) */
#ifdef BIAS_DEBUG
#  define BEXCEPTION(arg)\
  { \
    std::ostringstream os;\
    os << __FILE__ << ":" << __LINE__ <<" : "<< arg;\
    throw BIAS::FatalException(os.str().c_str());\
  }
#else
#  define BEXCEPTION(arg)\
  { \
    std::ostringstream os;\
    os << arg;\
    throw BIAS::FatalException(os.str().c_str());\
}
#endif

#ifdef BIAS_DEBUG
#  define BEXCEPTION_NONFATAL(arg)\
  { \
    std::ostringstream os;\
    os << __FILE__ << ":" << __LINE__ <<" : "<< arg;\
    throw BIAS::NonFatalException(os.str().c_str());\
  }
#else
#  define BEXCEPTION_NONFATAL(arg)\
  { \
    std::ostringstream os;\
    os << arg;\
    throw BIAS::NonFatalException(os.str().c_str());\
}
#endif



  /** @class BaseException
      @brief generic exception  
      @author woelk 01/2006 */
  class BaseException
    : public std::exception
  {
  public:
    BaseException(const char *error_message) throw()
    { 
      Message_ = error_message; 
      std::cerr << "exception raised: \""<<Message_<<"\"\n";
    } 

    BaseException(const std::string &error_message) throw()
    { 
      Message_ = error_message; 
      std::cerr << "exception raised: \""<<Message_<<"\"\n";
    } 

    ~BaseException() throw() {};

    virtual const std::string& What() const throw()
    { return Message_; }

    /** Returns a C-style character string describing the general cause
        of the current error.  */
    virtual const char* what() const throw()
    { return Message_.c_str(); }


  protected:
    std::string Message_;
  };


  /** @class NonFatalException
      @brief non fatal exception
      @author woelk 01/2006 */
  class NonFatalException
    : public BaseException
  {
  public:
    NonFatalException(const char *error_message) throw()
      : BaseException(error_message)
    {} 
    
    NonFatalException(const std::string &error_message) throw()
      : BaseException(error_message)
    {} 

    ~NonFatalException() throw() {};
  };


  /** @class FatalException
      @brief fatal exception
      @author woelk 01/2006 */
  class FatalException
    : public BaseException
  {
  public:
    FatalException(const char *error_message) throw()
      : BaseException(error_message)
    {} 
    
    FatalException(const std::string &error_message) throw()
      : BaseException(error_message)
    {} 

    ~FatalException() throw() {};
  };

} // namespace

#endif // _Exception_hh_
