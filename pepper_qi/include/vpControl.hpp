/****************************************************************************
 *
 * Copyright (C) 2005 - 2017 by Inria. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using this software that can not be combined with the GNU
 * GPL, please contact Inria about acquiring a Professional
 * Edition License.
 *
 * See http://visp.inria.fr for more information.
 *
 * This software was developed at:
 * Inria Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 *
 * If you have questions regarding the use of this file, or concerning the
 * license please contact Inria at Fabien.Spindler@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Authors:
 * Fabien Spindler, Giovanni Claudio
 *
 *
 ****************************************************************************/
/*-----------------------------------------------------------------------------------
 * Original Source https://github.com/lagadic/pepper_control/tree/libqi
 * Updates => modified to use outside visp_naoqi
 * for Pepper Robot only
------------------------------------------------------------------------------------*/

#include <string> 
#include <vector>
#include <qi/anyobject.hpp>
#include <qi/applicationsession.hpp>
#include <qi/periodictask.hpp>


class vpControl
{

private:
  qi::SessionPtr m_session;
  qi::SteadyClock::time_point m_prev;
  qi::PeriodicTask m_task;
  std::vector<float> m_vel; // protected by mutex
  std::vector<float> m_pos;
  std::vector<std::string> m_jointNames; // protected by mutex
  qi::AnyObject m_motion;
  boost::mutex m_lock;

  int m_period_ms;

  bool m_taskStarted;
  bool m_firstTimeTask;

  std::vector<int>	 m_status;	    // Flag d'etat des axes
  std::vector<float> m_deltaQmax;   // Increment de consigne maximum
  std::vector<float> m_deltaQ;		    // Increment de consigne en cours
  std::vector<float> m_Qacc;		    // Increment d'increment de consigne
  std::vector<float> m_Qacc_sav;    // Sauvegarde
  std::vector<float> m_consFin;    	// Consigne finale (butee)
  std::vector<int>   m_signeDep;	  // Signe du deplacement: +1 = incrementer consigne
  std::vector<float> m_PQc;         // Position mesuree et position calculee en rad
  std::vector<float> m_dist_AD;		  // Distance requise pour acce et decel
  std::vector<float> m_pcspeed;		  // Recopie des consigne de vitesse
  std::vector<float> m_pcspeed_old;
  std::vector<float> m_ecart;		    // Difference entre consigne et consigne finale
  std::vector<float> m_flagSpeed;
  bool vmax;
  bool flagbutee;

  // Constant
  std::vector<float> m_accMax;    // Acceleration maximale
  std::vector<float> m_vitMax;    // Vitesses maximales
  std::vector<float> QMax;	      // Butee softs maximale
  std::vector<float> QMin;	      // Butee softs minimale

public:
  /**
   * @brief vpControl starrt vpControl Service
   * @param session pointer to the current session
   */
  vpControl(qi::SessionPtr session);
  virtual ~vpControl();

  std::vector<float> getJointValues (std::vector<std::string> jointNames) const;
  void setDesJointVelocity (std::vector<std::string> jointNames, std::vector<float> vel);
  void setOneDesJointVelocity (std::string jointName, float vel);
  void setTask();
  qi::PeriodicTask::Callback printTime();
  void applyJointVelocity();
  void start();
  void stopJoint();
  void stop();

};
QI_REGISTER_OBJECT(vpControl, getJointValues, printTime, setDesJointVelocity, setOneDesJointVelocity, start, stop, stopJoint);
