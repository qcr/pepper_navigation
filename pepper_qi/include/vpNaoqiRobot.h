/****************************************************************************
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://team.inria.fr/lagadic/visp for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://team.inria.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Description:
 * This class allows to control the robot remotely.
 *
 * Authors:
 * Fabien Spindler
 * Giovanni Claudio
 *
 *****************************************************************************/
/*-----------------------------------------------------------------------------------
 * Original Source https://github.com/lagadic/visp_naoqi/tree/libqi
 * Updates => modified to use outside visp_naoqi
 * depends upon opencv only
 * for Pepper Robot only
 * ------------------------------------------------------------------------------------*/

#ifndef vpNaoqiRobot_h
#define vpNaoqiRobot_h

#include <iostream>
#include <string>


//// Aldebaran includes
#include <qi/applicationsession.hpp>
#include <qi/anyobject.hpp>

#define BOOST_SIGNALS_NO_DEPRECATION_WARNING


class vpNaoqiRobot
{

public:
    typedef enum {
        Pepper,
        Unknown
      } RobotType;

  
protected:
  qi::AnyObject m_pMemory; //!< Memory proxy
  qi::AnyObject m_pMotion;  //!< Motion proxy
  qi::AnyObject m_pepper_control;  //!< Proxy to Pepper_control
  //qi::AnyObject m_pBaseMotion;  //!< Proxy to Pepper_control_base_Gavain modifed
  bool m_isOpen; //!< Proxy opened status
  bool m_collisionProtection; //!< Collition protection enabling status
  std::string m_robotName; //!< Name of the robot
  RobotType m_robotType; //!< Indicate if the robot is Pepper
  
public:

  vpNaoqiRobot(const qi::SessionPtr &session);
  virtual ~vpNaoqiRobot();

  /*!
    Destroy the connexion to the motion proxy.
   */
  void cleanup();

  /**
   * get joint angles
   */
  std::vector<float> getAngles(const std::string &name, const bool& useSensors=true) const;
  std::vector<float> getAngles(const std::vector<std::string> &name, const bool& useSensors=true) const;

  /**
   * get body names
   */
  std::vector<std::string> getBodyNames(const std::string &names) const;

  /**
   * get Poistions of different joints/ odeometry
   */
  
  std::vector<float> getPosition(const std::string &names, const bool &useSensors=true) const;
  void getPosition(const std::vector<std::string> &names, std::vector<float> &q, const bool& useSensors=true) const;

  /**
   * getMotionProxy
   */

  qi::AnyObject * getMotionProxy();


  std::vector<std::vector<float>> getLimits(const std::string & name) const;


  /**
   * get robot name and type=> supported Pepper, Nao, Romeo2
   */

  std::string getRobotName() const { return m_robotName; }

  RobotType getRobotType() const { return m_robotType; }

  
  /**
   * @brief open robot
   */

  void open();

  /**
   * @brief moveTo move robot to the specified (x,y,theta) from current position
   * @param x
   * @param y
   * @param theta
   */

  void moveTo(const float& x, const float& y, const float& theta) const;


  /**
   * @brief rotate180 rotate robot by 180
   * @return  sucees or fail
   */
  bool rotate180()const;

  /*!
    Enable/disable the collision protection.
    In the constructor, the collision protection is enabled by default.
    \param protection : true to enable collision protection, false to disable.
   */
  void setCollisionProtection(bool protection)
  {
    m_collisionProtection = protection;
  }

  void setExternalCollisionProtectionEnabled(const std::string& name, const bool& enable) const;

  void setPosition(const std::string &name, const float &angles, const float &fractionMaxSpeed) const;
  void setPosition(const std::vector<std::string> &names, const std::vector<float> &jointPosition, const float &fractionMaxSpeed) const;



  /**
   * setStiffness of joints
   */
  void setStiffness(const std::string &names, const float &stiffness) const;
  void setStiffness(const std::vector<std::string> &names, const std::vector<float> &stiffness) const;
  void setStiffness(const std::vector<std::string> &names, const float &stiffness) const;


  void setVelocity(const std::vector<std::string> &names, const std::vector<float> &jointVel) const;
  void getJointVelocity(const std::vector <std::string> &names, std::vector <float> &jointVel) const;

  /**
   * @brief setBaseVelocity set velocity of the base
   * @param jointVel/ vx, vy,wz => translation velocities and roational velocities.
   * Note: vy and wz can't be set together at a same time
   */
  void setBaseVelocity(const std::vector<float> &jointVel) const;
  void setBaseVelocity(const float &vx,const float &vy,const float &wz) const;
  //void setBaseVelocity_Gavain(const float &vx,const float &vy,const float &wz) const;


  /**
   * stop Pepper
   */
  void startPepperControl() const;
  void stop(const std::string &name) const;
  void stop(const std::vector<std::string> &names) const;
  void stopPepperControl() const;
  void stopBase() const;
  //void stopBase_Gavain() const;


};

#endif
