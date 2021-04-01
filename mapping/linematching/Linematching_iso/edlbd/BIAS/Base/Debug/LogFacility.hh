#ifndef __LOGFACILITY_H__
#define __LOGFACILITY_H__



#include <bias_config.h>
//#include <Base/Common/FileHandling.hh>
//#include <Base/Debug/TimeMeasure.hh>
#include <iostream>
#include <fstream>
#include <string>

/*************************************************
@class LogFacility


@brief general class to handle logging in BIAS. Classes that want to
use logging have to be derived from LogFacility, set the Loglevel and
all logging will be send to the given stream (file/stdout...)

The idea of LogFacility is, to set up a third stream (in addision to
cout and cerr) using a LogLevel-mechanism to control
verbosity. Appications and high-level libs can write information into
this stream depending on one global log-level.  This stream ist sent
to cerr by default, but it can be redirected into a file or to any
rdbuf available. 

In Contrast to BIAS::Debug is also works for non-debug builds (#undef
BIAS_DEBUG). Lowlevel-lib (like here in BIAS and in MIP) should not
use this channel to discuss problems with the user.

The LogLevel is an enum (therefor an int) with the following order:
BIAS_LOG_NONE < BIAS_LOG_FATAL < BIAS_LOG_WARNING < BIAS_LOG_INFO <
BIAS_LOG_DEBUG.  Increasing the current LogLevel results in more
verbose reporting. 

To send a message the usage of one of the magic macros is
recommened. The generic macro is BIASLOG(x,y) with two parameters: x
is the log level and y is the message.  Programmers should think
carefully which information belongs to which level. Time-critical code
sections should only use BIAS_LOG_DEBUG, while BIAS_LOG_NONE should
not be used in BIASLOG().  For convenience and to save space in the
code line the following abbreviations are available:

BLF(x) : BIASLOG(BIAS_LOG_FATAL,x)
BLW(x) : BIASLOG(BIAS_LOG_WARNING,x)
BLI(x) : BIASLOG(BIAS_LOG_INFO,x)
BLD(x) : BIASLOG(BIAS_LOG_DEBUG,x)

The flag bCodeInfo_ controls if file names and line numbers are shown.

@author ischiller evers
@date 11/05

*************************************************/



//call BIASLOG(BIAS_LOG_LEVEL,"output")
#define BIASLOG(x,y){\
  std::ostream _cods_(std::cout.rdbuf()); \
  BIAS::LogFacility::Instance()->GetLogStream(_cods_); \
  if(x<=BIAS::LogFacility::Instance()->GetLogLevel() && _cods_.good()) {  \
    if(BIAS::LogFacility::Instance()->GetShowCodeInfo()){ \
      std::string file = BIAS::FileHandling::Filename(__FILE__);\
      std::string time = BIAS::TimeMeasure::GetCurrentTimeAsString();\
			std::string date = BIAS::TimeMeasure::GetDateAsString();\
      _cods_<<date<<", "<<time<<": "<<file<<", "<< __LINE__ << " : ";\
    }\
    _cods_<<y<<std::endl<<std::flush;   \
  }\
  if( BIAS::LogFacility::Instance()->GetAbortOnFatal() && x==BIAS::BIAS_LOG_FATAL){ \
  _cods_<<"Aborting in "<<__FILE__<<", "<<__LINE__<<std::endl<<std::flush;\
  abort();}\
}

//if no logging level is given, always log to stream
#define BIASLOGALWAYS(y){\
  std::ostream _cods_(std::cout.rdbuf()); \
  this->GetLogStream(_cods_); \
  if(x<=this->dLogLevel_ && _cods_.good()) {  \
  _cods_<<y<<std::endl; _cods_.flush();\
  }\
}



// for Kevin:
//#define PLAIN_COUT_LOGGING


#ifndef PLAIN_COUT_LOGGING
# define BLD(y) BIASLOG(BIAS::BIAS_LOG_DEBUG, y)
# define BLI(y) BIASLOG(BIAS::BIAS_LOG_INFO, y)
# define BLW(y) BIASLOG(BIAS::BIAS_LOG_WARNING, y)
# define BLF(y) BIASLOG(BIAS::BIAS_LOG_FATAL, y)
#else
# define BLD(y) cout<< y<<endl;
# define BLI(y) cout<< y<<endl;
# define BLW(y) cout<< y<<endl;
# define BLF(y) cout<< y<<endl;
#endif


namespace BIAS{


  enum BIAS_LOG_LEVEL{
    BIAS_LOG_NONE = 0,
    BIAS_LOG_FATAL,
    BIAS_LOG_WARNING,
    BIAS_LOG_INFO,
    BIAS_LOG_DEBUG
  };

  class BIASDebug_EXPORT LogFacility
  {
  public:

    static LogFacility * Instance(){
      if (pInstance_== NULL) 
        pInstance_ = new LogFacility;
      return pInstance_;
    };

    static void SetInstance(LogFacility *other);

    /// avoid memory leaks by explicit delete on instance JW
    static void DeleteInstance();

    bool OpenLogFile(const std::string &sFileName){
      if(zLogFileStream_.is_open())
        zLogFileStream_.close();

      // zLogStream_.clear();

      zLogFileStream_.open(sFileName.c_str(), std::ios_base::out | std::ios_base::trunc);
      if (zLogFileStream_)
        zLogStream_.rdbuf(zLogFileStream_.rdbuf());
      return true;
    };

    bool SetLogLevel(BIAS_LOG_LEVEL dLogLevel){
      dLogLevel_ = dLogLevel;
      return true;
    };

    inline BIAS_LOG_LEVEL GetLogLevel(){
      return dLogLevel_;
    };

    bool SetLogStream(std::ostream zStream){
      if (zLogFileStream_.is_open())
        zLogFileStream_.close();
      zLogStream_.rdbuf(zStream.rdbuf());
      return true;
    };

    inline bool GetLogStream(std::ostream &zStream){
      zStream.rdbuf(zLogStream_.rdbuf());
      return true;
    };

    inline std::ostream & GetLogStream(){

      return zLogStream_;
    };

    bool ResetStream(){
      if(zLogFileStream_.is_open())
        zLogFileStream_.close();
      zLogStream_.rdbuf(std::cerr.rdbuf());
      return true;
    };

    inline void PrintLogLevel(){
      zLogStream_<<"current log level is ";
      switch (dLogLevel_) 
      {
      case BIAS_LOG_NONE: zLogStream_<<"BIAS_LOG_NONE"<<std::endl; break;
      case BIAS_LOG_FATAL: zLogStream_<<"BIAS_LOG_FATAL"<<std::endl; break;  
      case BIAS_LOG_WARNING: zLogStream_<<"BIAS_LOG_WARNING"<<std::endl; break;  
      case BIAS_LOG_INFO: zLogStream_<<"BIAS_LOG_INFO"<<std::endl; break; 
      case BIAS_LOG_DEBUG: zLogStream_<<"BIAS_LOG_DEBUG"<<std::endl; break; 
      }
    };

    inline void SetShowCodeInfo(bool b) {
      bCodeInfo_ = b;
    };
    inline bool GetShowCodeInfo(){
      return bCodeInfo_;
    };

    inline void SetAbortOnFatal(bool b) {
      bAbortOnFatal_ = b;
    };
    inline bool GetAbortOnFatal(){
      return bAbortOnFatal_;
    };

  protected:
    /// no one can inherit from me
    LogFacility(void);

    /// no one can copy me 
    LogFacility(const LogFacility &l);
    LogFacility &operator=(const LogFacility &l);

    ~LogFacility(void)
    {
      if (zLogFileStream_.is_open()) {
        zLogFileStream_.close();
        zLogStream_.rdbuf(std::cerr.rdbuf());
      }
    };


    //the Stream to write logging to
    std::ostream  zLogStream_;
    //the int holding the LogLevel
    BIAS_LOG_LEVEL dLogLevel_;

    std::ofstream zLogFileStream_;
    bool bCodeInfo_;
    bool bAbortOnFatal_;

    static LogFacility *pInstance_;

  };
}// end namespace
#endif

