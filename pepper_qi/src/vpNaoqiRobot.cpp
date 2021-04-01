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
 * This class allows to the robot remotely.
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

// VispNaoqi
#include "vpNaoqiRobot.h"
#include "al/from_any_value.hpp"



/*!
  Default constructor that set the default parameters as:
  - robot ip address: "198.18.0.1"
  - collision protection: enabled
  */
vpNaoqiRobot::vpNaoqiRobot(const qi::SessionPtr &session)
  : m_pMemory(session->service("ALMemory")), m_pMotion(session->service("ALMotion"))/*, m_pBaseMotion(session->service("MotionController"))*/, m_pepper_control(session->service("pepper_control")), m_isOpen(false), m_collisionProtection(true),
    m_robotName(""), m_robotType(Unknown)
{
  // Get the robot type
  m_robotName = m_pMemory.call<std::string>("getData", "RobotConfig/Body/Type");
  std::transform(m_robotName.begin(), m_robotName.end(), m_robotName.begin(), ::tolower);

  if (m_robotName == "pepper" || m_robotName == "juliette")
  {
    m_robotName = "pepper";
    m_robotType = Pepper;
  }
  else
  {
    std::cout << "Unknown robot" << std::endl;
  }

}

/*!
  Destructor that call cleanup().
 */
vpNaoqiRobot::~vpNaoqiRobot()
{
  cleanup();
}

/*!
  Open the connection with the robot.
  All the parameters should be set before calling this function.
  \code
int main(int argc, char** argv)
{
  std::string opt_ip = "192.168.0.24";

    for (unsigned int i=0; i<argc; i++) {
      if (std::string(argv[i]) == "--ip")
        opt_ip = argv[i+1];
      else if (std::string(argv[i]) == "--help") {
        std::cout << "Usage: " << argv[0] << "[--ip <robot address>] " << std::endl;
        return 0;
      }
    }

  // Create a session to connect with the Robot
  qi::SessionPtr session = qi::makeSession();
  std::string ip_port = "tcp://" + opt_ip + ":9559";
  session->connect(ip_port);
  if (! opt_ip.empty()) {
    std::cout << "Connect to robot with ip address: " << opt_ip << std::endl;
  }

  vpNaoqiRobot robot(session);

  robot.open();

  return 0;
  \endcode
 */
void vpNaoqiRobot::open()
{
  if (! m_isOpen) {
    cleanup();

    std::cout << "Connected to a " << m_robotName << " robot." << std::endl;

    if (m_robotType == Pepper)
      m_pepper_control.call<void >("start");


    m_isOpen = true;
  }
}

void vpNaoqiRobot::cleanup()
{

  if (m_robotType == Pepper)
  {
    m_pepper_control.call<void >("stopJoint");
    m_pepper_control.call<void >("stop");
  }

  m_isOpen = false;
}

/*!
  Set the stiffness to a chain name, or to a specific joint.
  \param names :  Names of the chains, "Body", "JointActuators",
  "Joints" or "Actuators".
  \param stiffness : Stiffness parameter that should be between
  0 (no stiffness) and 1 (full stiffness).
 */
void vpNaoqiRobot::setStiffness(const std::string &names, const float &stiffness) const
{
  m_pMotion.call<void>("setStiffnesses", names, stiffness );
}


/*!
  Set the stiffness of a list of joints.
  \param names : Vector with the joint names.
  \param stiffness : Stiffness parameters that should be between
  0 (no stiffness) and 1 (full stiffness).
 */
void vpNaoqiRobot::setStiffness(const std::vector<std::string> &names, const std::vector<float> &stiffness) const
{
  m_pMotion.call<void>("setStiffnesses", names, stiffness );
}

/*!
  Set the stiffness of a list of joints.
  \param names : Vector with the joint names.
  \param stiffness : Stiffness parameter that should be between
  0 (no stiffness) and 1 (full stiffness).
 */
void vpNaoqiRobot::setStiffness(const std::vector<std::string> &names, const float &stiffness) const
{
  m_pMotion.call<void>("setStiffnesses", names, stiffness );
}



/*!
  Apply a velocity vector to a vector of joints.
  \param names :  Names the joints, chains, "Body", "JointActuators",
  "Joints" or "Actuators".
  \param jointVel : Joint velocity vector with values expressed in rad/s.
  \param verbose : If true activates printings.
 */

//void vpNaoqiRobot::setVelocity(const AL::ALValue& names, const std::vector<float> &jointVel, bool verbose)
//{
//  setVelocity(names, (AL::ALValue)(jointVel), verbose);
//}


void vpNaoqiRobot::setVelocity(const std::vector<std::string> &names, const std::vector<float> &jointVel) const
{
  if (m_robotType == Pepper) {
    m_pepper_control.async<void>("setDesJointVelocity", names, jointVel);

    for (unsigned int i = 0; i <names.size(); i++ )
       std::cout << names[i] << std::endl;

    for (unsigned int i = 0; i <jointVel.size(); i++ )
      std::cout << jointVel[i] << std::endl;
  }
  else
  {
    std::cout << " The velocity controller for Romeo and Nao is not implemented yet" << std::endl;
    exit(0);
  }


}


/*!
  Apply a velocity vector to a vector of joints.Use just one call to apply the velocities.
  \param names :  Names the joints, chains, "Body", "JointActuators",
  "Joints" or "Actuators".
  \param jointVel : Joint velocity vector with values expressed in rad/s (vpColVector).
  \param verbose : If true activates printings.
 */



/*!
  Apply a velocity vector to a vector of joints. Use just one call to apply the velocities.

  \param names :  Names the joints, chains, "Body", "JointActuators",
  "Joints" or "Actuators".
  \param jointVel : Joint velocity vector with values expressed in rad/s.
  \param verbose : If true activates printings.
 */
//void vpNaoqiRobot::setVelocity(const AL::ALValue &names, const AL::ALValue &jointVel, bool verbose)
//{
//  std::vector<std::string> jointNames;
//  if (names.isString()) // Suppose to be a chain
//    jointNames = m_motionProxy->getBodyNames(names);
//  else if (names.isArray()) // Supposed to be a vector of joints
//    jointNames = names; // it a vector of joints
//  else
//    throw vpRobotException (vpRobotException::readingParametersError,
//                            "Unable to decode the joint chain.");
//
//  if (jointNames.size() != jointVel.getSize() ) {
//    throw vpRobotException (vpRobotException::readingParametersError,
//                            "The dimensions of the joint array and the velocities array do not match.");
//  }
//
//  if (m_robotType == Romeo || m_robotType == Nao)
//  {
//    AL::ALValue jointListStop;
//    AL::ALValue jointListMove;
//    AL::ALValue angles;
//    std::vector<float> fractions;
//
//    for (unsigned i = 0 ; i < jointVel.getSize() ; ++i)
//    {
//      std::string jointName = jointNames[i];
//      if (verbose)
//        std::cout << "Joint name: " << jointName << std::endl;
//
//      float vel = jointVel[i];
//
//      if (verbose)
//        std::cout << "Desired velocity =  " << vel << "rad/s to the joint " << jointName << "." << std::endl;
//
//      //Get the limits of the joint
//      std::vector<std::vector<float>> limits = getLimits(jointName);
//
//      if (vel == 0.0f)
//      {
//        if (verbose)
//          std::cout << "Stop the joint" << std::endl ;
//
//        jointListStop.arrayPush(jointName);
//      }
//      else
//      {
//
//        if (vel > 0.0f)
//        {
//          //Reach qMax
//          angles.arrayPush(limits[0][1]);
//          if (verbose)
//            std::cout << "Reach qMax (" << limits[0][1] << ") ";
//        }
//
//        else if (vel < 0.0f)
//        {
//          //Reach qMin
//          angles.arrayPush(limits[0][0]);
//          if (verbose)
//            std::cout << "Reach qMin (" << limits[0][0] << ") ";
//        }
//
//
//        jointListMove.arrayPush(jointName);
//        float fraction = fabs( float (vel/float(limits[0][2])));
//        if (fraction >= 1.0 )
//        {
//          if (verbose) {
//            std::cout << "Given velocity is too high: " <<  vel << "rad/s for " << jointName << "." << std::endl;
//            std::cout << "Max allowed is: " << limits[0][2] << "rad/s for "<< std::endl;
//          }
//          fraction = 1.0;
//        }
//
//        fractions.push_back(fraction);
//
//      }
//    }
//    if (verbose) {
//      std::cout << "Apply Velocity to joints " << jointListMove << std::endl;
//      std::cout << "Stop List joints: " << jointListStop << std::endl;
//      std::cout << "with fractions " << angles << std::endl;
//      std::cout << "to angles " << fractions << std::endl;
//    }
//
//    if (jointListMove.getSize()>0)
//    {
//      m_proxy->callVoid("setAngles", jointListMove, angles, fractions);
//    }
//
//    if (jointListStop.getSize()>0)
//    {
//      std::vector<float> zeros( jointListStop.getSize() );
//      m_proxy->callVoid("changeAngles", jointListStop, zeros, 0.1f);
//
//    }
//  } // End Romeo
//  else if (m_robotType == Pepper)
//  {
//    std::vector<float> vel(jointNames.size());
//    jointVel.ToFloatArray(vel);
//    m_pepper_control.async<void >("setDesJointVelocity", jointNames, vel);
//  }
//
//}


/*!
  Makes the robot move to the given pose in the ground plane, relative to FRAME_ROBOT. This is a blocking call.  \param x : Distance along the X axis in meters.
  \param y : Distance along the Y axis in meters.
  \param theta : Rotation around the Z axis in radians [-3.1415 to 3.1415]
 */

  void vpNaoqiRobot::moveTo(const float& x, const float& y, const float& theta) const
  {
    m_pMotion.async<void>("moveTo", x, y, theta);
  }


bool vpNaoqiRobot::rotate180()const
{

  return(m_pMotion.async<bool>("moveTo",0.0, 0.0, 3.1415,12.0));

}
/*!
  Stop joints.
  \param names : Vector with the joint names.
 */
void vpNaoqiRobot::stop(const std::vector<std::string> &names) const
{
   if (m_robotType == Pepper)
    m_pepper_control.call<void >("stopJoint");
}


/*!
  Stop joint in a chain.
  \param names : Chain or joint name.
 */
void vpNaoqiRobot::stop(const std::string &name) const
{
  if (m_robotType == Pepper)
    m_pepper_control.call<void >("stopJoint");
}

/*!
  Stop the service pepper_control.
 */
void vpNaoqiRobot::stopPepperControl() const
{
  m_pepper_control.call<void >("stop");
}

/*!
  Start the service pepper_control.
 */
void vpNaoqiRobot::startPepperControl() const
{
  m_pepper_control.call<void >("start");
}


/*!
  Stop the velocity of the base.
 */
void vpNaoqiRobot::stopBase() const
{
  m_pMotion.async<void>("move", 0.0, 0.0, 0.0);
}

/*void vpNaoqiRobot::stopBase2() const
{
  m_pBaseMotion.async<void>("move", 0.0, 0.0, 0.0);
}*/



/*!
  Apply a velocity Vx, Vy, Wz to Pepper.
  \param vel : Joint velocity vector with values expressed in rad/s.
 */

void vpNaoqiRobot::setBaseVelocity(const std::vector<float> &jointVel) const
{
  if (m_robotType == Pepper)
  {
    if (jointVel.size() == 3)
      m_pMotion.async<void>("move", jointVel[0], jointVel[1], jointVel[2]);
    else
      std::cerr << "ERROR: Cannot apply velocity to the base. Check the size of the vector, it has to be of size 3."
                << std::endl
                << "Current size: " << jointVel.size() << std::endl;
  }
}


/*!
  Apply a velocity Vx, Vy, Wz to Pepper.
  \param vel : Joint velocity vector with values expressed in rad/s.
 */

void vpNaoqiRobot::setBaseVelocity(const float &vx,const float &vy,const float &wz) const
{
  if (m_robotType == Pepper) 
      m_pMotion.async<void>("move", vx, vy, wz);
}

/*void vpNaoqiRobot::setBaseVelocity2(const float &vx,const float &vy,const float &wz) const
{
  if (m_robotType == Pepper)
	m_pBaseMotion.async<void>("move", vx, vy, wz);
     // m_pMotion.async<void>("move", vx, vy, wz);
}*/

/*!
  Get the value of all the joints of the chain.
  \param names :  Names the joints, chains, “Body”, “JointActuators”, “Joints” or “Actuators”.
  \return The value of the joints.
 */
std::vector<float> vpNaoqiRobot::getAngles(const std::string &name, const bool& useSensors) const
{
  return m_pMotion.call<std::vector<float> >("getAngles", name, useSensors );
}


/*!
  Get the value of all the joints in the vector.
  \param names :  Vector containing the names of the joints.
  \return The value of the joints.
 */
std::vector<float> vpNaoqiRobot::getAngles(const std::vector<std::string> &name, const bool& useSensors) const
{
  return m_pMotion.call<std::vector<float> >("getAngles", name, useSensors );
}


/*!
  Get the name of all the joints of the chain.

  \param names : Names the joints, chains, "Body", "JointActuators",
  "Joints" or "Actuators".

  \return The name of the joints.
 */
std::vector<std::string>
vpNaoqiRobot::getBodyNames(const std::string &names) const
{
  if (! m_isOpen){
      std::cout<< "The connexion with the robot was not open"<<std::endl;
    throw;
  }
  std::vector<std::string> jointNames = m_pMotion.call<std::vector<std::string> >("getBodyNames", names);
  return jointNames;
}



/*!
   Get the motion proxy
   \return pointer to the motion proxy
*/
qi::AnyObject * vpNaoqiRobot::getMotionProxy()
{
    return &m_pMotion;
}


/*!
  Get the position of all the joints in the vector.

  \param names :  Names the joints.
  \param useSensors :  If true, sensor positions will be returned. If
  false, it will be the command.
  \param q : Joint position in radians.
 */

void vpNaoqiRobot::getPosition(const std::vector<std::string> &names, std::vector<float>& q, const bool& useSensors) const
{
  q = getAngles(names, useSensors);
  return;
}


/*!
  Get the minAngle (rad), maxAngle (rad), maxVelocity (rad.s-1) and maxTorque (N.m). for a given joint or actuator in the body.
  \param name :  Name of a joint, chain, “Body”, “JointActuators”, “Joints” or “Actuators”.
  \return Vector containing the minAngle, maxAngle, maxVelocity and maxTorque for all the joints specified.
 */
std::vector<std::vector<float>> vpNaoqiRobot::getLimits(const std::string & name) const
{
  std::vector<std::vector<float>> values;

  qi::AnyValue limits = m_pMotion.call<qi::AnyValue>("getLimits", name);
  naoqi::tools::fromAnyValueToFloatVectorOfVector(limits,values);

  return values;

}

/*!
  Set External collision
  \param name :  The name {“All”, “Move”, “Arms”, “LArm” or “RArm”}.
  \param enable: Activate or deactivate the external collision of the desired name.
 */
  void vpNaoqiRobot::setExternalCollisionProtectionEnabled(const std::string& name, const bool& enable) const
  {
    m_pMotion.call<void>("setExternalCollisionProtectionEnabled", name, enable);
  }

/*!
  Set the position of all the joints of the chain.

  \param names :  Names the chain.
  \param angles :  Joint positions in radians.
  \param fractionMaxSpeed : The fraction of maximum speed to use. Value should be comprised between 0 and 1.

 */
void vpNaoqiRobot::setPosition(const std::string &name, const float &angle, const float& fractionMaxSpeed) const
{
  m_pMotion.async<void>("setAngles", name, angle, fractionMaxSpeed);
}



/*!
   Set joint positions.

  \param names :  std::vector with the names the joints.
  \param jointPosition :  Joint positions in radians.
  \param fractionMaxSpeed : The fraction of maximum speed to use. Value should be comprised between 0 and 1.

 */
void vpNaoqiRobot::setPosition(const std::vector<std::string> &names, const std::vector<float> &jointPosition, const float &fractionMaxSpeed) const
{
  m_pMotion.async<void>("setAngles", names, jointPosition, fractionMaxSpeed);
}






/*!
  Get the joints velocities.

  \param names :  Vector with the joint names.
  \param names :  Vector to fill with the joint velocities.

 */
void vpNaoqiRobot::getJointVelocity(const std::vector <std::string> &names, std::vector <float> &jointVel) const
{

  std::vector<std::string> list;

  for (unsigned int i = 0; i < names.size(); i++)
  {
    std::string key;
    if (m_robotType == Pepper )
      key = "Motion/Velocity/Sensor/" + names[i];
    else
      key = "Device/SubDeviceList/" + names[i] + "/Speed/Actuator/Value";

    list.push_back(key);
  }

  qi::AnyValue memData_vel = m_pMemory.call<qi::AnyValue>("getListData", list);
  qi::AnyReferenceVector memData_anyref = memData_vel.asListValuePtr();

  for(int i=0; i<memData_anyref.size();i++)
  {
    if(memData_anyref[i].content().kind() == qi::TypeKind_Float)
      jointVel[i] = memData_anyref[i].content().asFloat();
  }
  return;
}



