#include "LogFacility.hh"


using namespace BIAS;
using namespace std;


BIAS::LogFacility * BIAS::LogFacility::pInstance_ = NULL;



LogFacility::LogFacility():zLogStream_(std::cerr.rdbuf())
{

  dLogLevel_ = BIAS_LOG_DEBUG;
  bCodeInfo_ = false;
  bAbortOnFatal_ = false;
}



LogFacility::LogFacility(const LogFacility &):zLogStream_(std::cerr.rdbuf())
{ 
}

LogFacility &LogFacility::operator=(const LogFacility &)
{
 return *Instance();
}

void LogFacility::SetInstance(LogFacility *other)
{
  if (pInstance_ != other) {
    delete pInstance_;
    pInstance_= other;
  }
}


void LogFacility::DeleteInstance()
{
  SetInstance(NULL);
}

