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


#ifndef _TIMEMEASUREMENT_H_
#define _TIMEMEASUREMENT_H_

#include <bias_config.h>
#include <string>
#include <iostream>
#include <Base/Debug/TimeMeasure.hh>

/** @ingroup g_utils
@file TimeMeasurement.hh
@brief Time measurement based on instance construction and destruction 

Usage:
\verbatim:
// -- previous other code ... 
{ 
  // -- more previous other code ... 
  // construct an instance as "Start" function:
  BIAS::TimeMeasurement timerInstance("my optional description");
  // >> code to be timed here, e.g.: 
  sleep(1000);
  // <<
} // Stop by automatic destruction of instance 
  // due to end of scope at closing bracket
// -- later other code -- ...
\endverbatim
@author Jan Woetzel */
namespace BIAS {
  class TimeMeasurement 
  {
  public:
    /// destructor working as STOP function
    ~TimeMeasurement(){
      timer_.Stop();
      if (this->description_.size()>0)
        std::cout<<"TimeMeasurement "<<this->description_<<" :"<<std::endl;
      timer_.Print();
    }

    /// constructor working as START function
    TimeMeasurement()
    {
      Start();
    }

    /// constructor with additional description
    TimeMeasurement(const std::string & str){
      description_ = str;
      Start();
    }

    /** @brief Reset and Start the timer

    Useful for earlier construction 
    @author Jan Woetzel */
    void Start(){
      timer_.Reset();
      timer_.Start();
    }

  protected:
    /// local timer used by ctor and detor 
    BIAS::TimeMeasure timer_;
    std::string description_;
  }; // end of class TimeMeasurement 
} // end of namespace


#endif // _TIMEMEASUREMENT_H_

