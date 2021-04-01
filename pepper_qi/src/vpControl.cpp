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
 ****************************************************************************/
/*-----------------------------------------------------------------------------------
 * Original Source https://github.com/lagadic/pepper_control/tree/libqi
 * Updates => modified to use outside visp_naoqi
 * for Pepper Robot only
------------------------------------------------------------------------------------*/

#include <iostream>
#include <sstream> 
#include <qi/clock.hpp>
#include <boost/chrono/chrono_io.hpp>
#include "vpControl.hpp"

#define FLAGACC		1	// Axe en acceleration
#define FLAGCTE		2 // Axe en vitesse constante
#define FLAGDEC		3	// Axe en deceleration
#define FLAGSTO		4	// Axe stoppe

#define DELTAQMIN	0.0001	// Delta Q minimum (rad)
#define OFFSET_BUTEE    0 //0.01	// on s'arrete un peu avant les butees


vpControl::vpControl(qi::SessionPtr session)
  : m_session(session), m_prev(), m_task(), m_vel(), m_pos(),
    m_jointNames(),m_lock(), m_taskStarted(false), m_firstTimeTask(true),
    m_status(), m_deltaQmax(), m_deltaQ(), m_Qacc(), m_Qacc_sav(), m_consFin(), m_signeDep(),
    m_PQc(), m_dist_AD(), m_pcspeed(), m_pcspeed_old(), m_ecart(), m_flagSpeed(), vmax(false), flagbutee(false),
    m_accMax(), m_vitMax(), QMin(), QMax(), m_period_ms(2)
{
  m_prev = qi::SteadyClock::now();
  m_motion = m_session->service("ALMotion");
}

vpControl::~vpControl()
{
 m_session->close();

}

void vpControl::start()
{
  setTask();
  std::cout << "Task started " << std::endl;
}

void vpControl::stopJoint()
{
  m_lock.lock();
  std::vector<float> zero (m_vel.size(), 0.0);
  m_vel = zero;
  m_lock.unlock();
  std::cout << "Stop joints" << std::endl;
}

void vpControl::stop()
{
  m_task.stop();
  std::vector<float> change(m_jointNames.size(), 0.0);
  m_motion.async<void>("changeAngles", m_jointNames, change, 1.0);
  //m_started = false;
  std::cout << "Task stopped " << std::endl;
}

void vpControl::setOneDesJointVelocity (std::string jointName, float vel)
{
  std::vector<std::string> jointName_v;
  jointName_v.push_back(jointName);
  std::vector<float> vel_v;
  vel_v.push_back(vel);
  setDesJointVelocity(jointName_v, vel_v);
}

void vpControl::setDesJointVelocity (std::vector<std::string> jointNames, std::vector<float> vel)
{
  m_lock.lock();
  std::vector<std::string> jointNames_prev = m_jointNames;
  m_lock.unlock();

//  std::cout << "jointNames.size()" << jointNames.size() << std::endl;

  if (jointNames != jointNames_prev) {
    // TODO: Call next only if joint names modified
    size_t njoint = jointNames.size();
    m_status.resize(njoint);
    m_deltaQmax.resize(njoint);
    m_deltaQ.resize(njoint);
    m_Qacc.resize(njoint);
    m_Qacc_sav.resize(njoint);
    m_consFin.resize(njoint);
    m_signeDep.resize(njoint);
    m_PQc.resize(njoint);
    m_dist_AD.resize(njoint);
    m_pcspeed.resize(njoint);
    m_pcspeed_old.resize(njoint);
    m_ecart.resize(njoint);
    m_flagSpeed.resize(njoint);
    m_accMax.resize(njoint);
    m_vitMax.resize(njoint);
    QMin.resize(njoint);
    QMax.resize(njoint);


    //  for (unsigned int i=0; i<jointNames.size(); i++)
    //  {
    //    qi::AnyValue limits = m_motion.call< qi::AnyValue >("getLimits", jointNames[i]);
    //    //std::cout << limits << std::endl;
    //    QMin[i] = limits[0].asFloat();
    //    QMax[i] = limits[1].asFloat();
    //    m_vitMax[i] = limits[2].asFloat();
    //    m_accMax[i] = 0.1; // To improve (~10deg in 2 sec)
    //    std::cout << " jointNames " << jointNames[i] << " " << QMin[i] << " " << QMax[i] << " " << m_vitMax[i] << std::endl;
    //  }

    for (unsigned int i=0; i<jointNames.size(); i++)
    {
      std::vector<std::vector<float>> limits = m_motion.call< std::vector<std::vector<float>> >("getLimits", jointNames[i]);

      //std::cout << limits << std::endl;
      QMin[i] = limits[0][0];
      QMax[i] = limits[0][1];
      m_vitMax[i] = limits[0][2];
      m_accMax[i] = 5.; // To improve (~10deg in 2 sec)
      std::cout << " jointNames " << jointNames[i] << " " << QMin[i] << " " << QMax[i] << " " << m_vitMax[i] << std::endl;

      m_flagSpeed[i] = false;
      m_status[i] 	= FLAGSTO;
      m_deltaQ[i] 	= m_deltaQmax[i] 	= 0.0;
      m_Qacc_sav[i] = m_Qacc[i] = m_accMax[i] * m_period_ms * m_period_ms / 1000000.;
      m_consFin[i] 	= m_PQc[i] = 0.0; //get_q(i);
      m_dist_AD[i] 	= 0.0;
      m_pcspeed[i]	= m_pcspeed_old[i] = 0.0;
      m_status[i]   = FLAGSTO;
      m_signeDep[i]	= 0;
      m_consFin[i] = 0;
    }

    m_pos = m_motion.call<std::vector<float> >("getAngles", jointNames, 1);

    for (unsigned int i=0; i<jointNames.size(); i++)
      m_PQc[i] = m_pos[i];

  }

  //vmax = false;
  //  m_pos = m_motion.call<std::vector<float> >("getAngles", jointNames, 1);

  //  for (unsigned int i=0; i<jointNames.size(); i++)
  //    m_PQc[i] = m_pos[i];

  m_lock.lock();
  m_vel = vel;
  m_jointNames = jointNames;
  m_lock.unlock();
}

std::vector<float> vpControl::getJointValues (std::vector<std::string> jointNames) const
{
  std::cout << "Function getJointValues called" << std::endl;
  qi::AnyObject tts = m_session->service("ALMotion");
  return tts.call<std::vector<float> >("getAngles", jointNames, 1 );
}

qi::PeriodicTask::Callback vpControl::printTime()
{
  qi::SteadyClock::time_point now = qi::SteadyClock::now();
  //qi::MilliSeconds ms = boost::chrono::duration_cast<qi::MilliSeconds>(now - prev);
  qi::MicroSeconds us = boost::chrono::duration_cast<qi::MicroSeconds>(now - m_prev);
  double t =  us.count();
  std::cout << "spent " << t/1000000<< " sec" << std::endl;
  m_prev = now;
}

void vpControl::applyJointVelocity()
{
  qi::SteadyClock::time_point now = qi::SteadyClock::now();

  m_lock.lock();
  std::vector<std::string> jointNames = m_jointNames;
  std::vector<float> vel = m_vel;
  m_lock.unlock();

  if (vel.size()>0 && jointNames.size()>0)
  {
    qi::MicroSeconds us = boost::chrono::duration_cast<qi::MicroSeconds>(now - m_prev);
    double delta_t =  us.count()/1000000.;

    //      std::cout << " delta " << delta_t << std::endl;

    /*
     * Test si changement de consigne.
     */
    for (int i=0;i<jointNames.size();i++) {
      m_pcspeed[i] = vel[i]; // TODO fuse in same var

      if (m_pcspeed[i] != m_pcspeed_old[i]) flagbutee = false;

      if (m_pcspeed[i] != m_pcspeed_old[i]) {
        m_Qacc[i] = m_Qacc_sav[i];

        if (m_pcspeed[i] > m_vitMax[i]) {
          m_pcspeed[i] = m_vitMax[i];
          if (vmax == false)
          {
            vmax = true;
          }
        }
        else if (m_pcspeed[i] < (-m_vitMax[i])) {
          m_pcspeed[i] = -m_vitMax[i];
          if (vmax == false)
          {
            vmax = true;
          }
        }
        else vmax = false;

        if (m_flagSpeed[i] == false) {
          //if (pt_movespeed.m_flagSpeed[i] == false) {
          /* Changement de consigne et non en phase de chang sens */

          if ( m_status[i] == FLAGSTO) /* Si arret */
          {
            if (m_pcspeed[i] > 0)
            {
              m_deltaQmax[i] = m_pcspeed[i]*delta_t;
              m_signeDep[i] = 1;
              m_consFin[i] = QMax[i] - OFFSET_BUTEE;
              m_deltaQ[i] = 0;
              m_status[i] = FLAGACC;
            }
            else if (m_pcspeed[i] < 0)
            {
              m_deltaQmax[i] = - m_pcspeed[i]*delta_t;
              m_signeDep[i] = -1;
              m_consFin[i] = QMin[i] + OFFSET_BUTEE;
              m_deltaQ[i] = 0;
              m_status[i] = FLAGACC;
            }
          }
          // Si non en arret et changement de sens

          else if ( (m_pcspeed[i] * m_signeDep[i]) < 0) {
            m_flagSpeed[i] = true;
            m_status[i] = FLAGDEC;
            m_deltaQmax[i] = 0;
          }
          // Pas de changement de sens

          else {	/* Non arret et pas de changement de sens */
            if ( m_signeDep[i] == 1) {
              if ( m_pcspeed[i] > m_pcspeed_old[i])
                m_status[i] = FLAGACC;
              else  m_status[i] = FLAGDEC;
              m_deltaQmax[i] = m_pcspeed[i]*delta_t;
            }
            else {
              if ( m_pcspeed[i] > m_pcspeed_old[i])
                m_status[i] = FLAGDEC;
              else  m_status[i] = FLAGACC;
              m_deltaQmax[i] = - m_pcspeed[i]*delta_t;
            }
          }

          int n = (int) (m_deltaQmax[i] / m_Qacc[i]);
          m_dist_AD[i]=n*(m_deltaQmax[i]-(n+1)*m_Qacc[i]/2);
        }
        m_pcspeed_old[i] = m_pcspeed[i];
      }
    }

    //    std::cout << "m_consFin: ";
    //    for (int i=0;i<jointNames.size();i++)
    //      std::cout << jointNames[i] << "(" << m_consFin[i] << ") ";

    /*
     * calcul des consignes selon les cas:
     *		- acceleration
     *		- deceleration
     *		- arret
     */

    for (int i=0;i<jointNames.size();i++) {
      /*
       * Securite butee en vitesse constante
       */
      m_ecart[i] = ( m_consFin[i] - m_PQc[i]) * m_signeDep[i];
      if ((m_ecart[i] - m_deltaQmax[i]) <=  m_dist_AD[i]) {
        if (m_dist_AD[i] > 0) {
          if (!flagbutee) printf("Flagbutee axe %d\n",i);
          flagbutee = true;
          for(int k=0;k<jointNames.size();k++)
          {
            if (m_status[k] != FLAGSTO) m_status[k] = FLAGDEC;
            m_deltaQmax[k] = 0;
          }
        }
      }
      /*
       * Deceleration.
       */
      if ( m_status[i] == FLAGDEC) {
        m_deltaQ[i] -=  m_Qacc[i];
        if (m_deltaQ[i] <=  m_deltaQmax[i]) {
          if (m_deltaQmax[i] < DELTAQMIN)  {
            m_status[i] = FLAGSTO;
            m_deltaQ[i] = 0.0;
            // Test si on etait en phase de changement de sens.
            if (m_flagSpeed[i] == true) {
              //if (pt_movespeed.m_flagSpeed[i] == true) {
              if (m_pcspeed[i] > 0) {
                m_deltaQmax[i] = m_pcspeed[i]*delta_t;
                m_signeDep[i] = 1;
                m_consFin[i] = QMax[i] - OFFSET_BUTEE;
              }
              else if (m_pcspeed[i] < 0) {
                m_deltaQmax[i] = -m_pcspeed[i]*delta_t;
                m_signeDep[i] = -1;
                m_consFin[i] = QMin[i] + OFFSET_BUTEE;
              }
              m_status[i] = FLAGACC;
              m_flagSpeed[i] = false;

              int n = (int) (m_deltaQmax[i] / m_Qacc[i]);
              m_dist_AD[i]=n*(m_deltaQmax[i]-(n+1)*m_Qacc[i]/2);
            }
          }
          else if ((m_deltaQmax[i] > 0) && !flagbutee)  {
            if (m_deltaQmax[i] < (m_deltaQ[i] + 2*m_Qacc[i])) {
              m_deltaQ[i] = m_deltaQmax[i];
              m_status[i] = FLAGCTE;
            }
            else if (!flagbutee) {
              /* acceleration moins rapide*/
              m_deltaQ[i] += (2*m_Qacc[i]);
              m_status[i] = FLAGACC;
            }
          }
        }
      }
      /*
       * Acceleration.
       */
      else if (m_status[i] == FLAGACC) {
        m_deltaQ[i] += m_Qacc[i];

        if (m_deltaQ[i] >= m_deltaQmax[i]) {
          m_deltaQ[i] = m_deltaQmax[i];
          m_status[i] = FLAGCTE;
        }
      }
      /*
       * Sinon a vitesse constante increment non change.
       */
      m_PQc[i] += m_signeDep[i] * m_deltaQ[i];

    } /* endfor */

    // Test si un axe arrive pres des butees. Si oui, arret de tous les axes
    for (int i=0;i<jointNames.size();i++) {
      float butee = QMin[i] + OFFSET_BUTEE;
      if (m_PQc[i] < butee) {
        for (int j=0;j<jointNames.size();j++) m_PQc[j] -= m_signeDep[j]*m_deltaQ[j];
        m_PQc[i] = butee;
        printf("Butee axe %d\n",i);
        break;
      }
      butee = (float) (QMax[i] - OFFSET_BUTEE);
      if (m_PQc[i] > butee) {
        for (int j=0;j<jointNames.size();j++) m_PQc[j] -= m_signeDep[j]*m_deltaQ[j];
        m_PQc[i] = butee;
        printf("Butee axe %d\n",i);
        break;
      }
    }
    //    std::cout << "new_pos: ";
    //    for (int i=0;i<jointNames.size();i++)
    //      std::cout << jointNames[i] << "(" << m_PQc[i] << ") ";
    //    std::cout << std::endl;

    // Apply new position
    m_motion.async<void>("setAngles", jointNames, m_PQc, 1.0);
  }

  m_prev = now;
}


void vpControl::setTask()
{
  if (!m_taskStarted)
  {
    m_task.setName("Setvelocity");
    m_task.setPeriod(qi::MilliSeconds(m_period_ms));

    m_task.setCallback(&vpControl::applyJointVelocity, this);
    m_taskStarted = true;
  }
  m_vel.clear();
  m_jointNames.clear();
  m_pos.clear();
  m_prev = qi::SteadyClock::now();
  m_firstTimeTask = true;
  m_task.start();
}
