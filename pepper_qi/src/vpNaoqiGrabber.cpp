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
 * This class allows to get images from the robot remotely.
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

#include "vpNaoqiGrabber.h"

#include "al/alvisiondefinitions.h"
#include "al/naoqi_image.hpp"
#include "al/from_any_value.hpp"

/*!
  Default constructor tat set the default parameters as:
  - camera framerate: 30 fps
  - m_cameraId: 0
  */
vpNaoqiGrabber::vpNaoqiGrabber(const qi::SessionPtr &session)
  : m_pVideo(session->service("ALVideoDevice")), m_pMemory(session->service("ALMemory")), m_fps(30), m_isOpen(false), m_width(0), m_height(0),
    m_img(), m_cameraName("CameraTop"), m_cameraId (0), m_cameraMulti(false), m_resolution(AL::kQVGA), m_robotType(""), m_pepper(0),
    m_colorspace(AL::kYUV422ColorSpace)
{

  // Get the robot type
  m_robotType = m_pMemory.call<std::string>("getData", "RobotConfig/Body/Type");
  std::transform(m_robotType.begin(), m_robotType.end(), m_robotType.begin(), ::tolower);

  if (m_robotType == "pepper" || m_robotType == "juliette")
  {
    m_robotType = "pepper";
    m_pepper = 1;
  }


  std::cout << "Connected to a " << m_robotType << " robot." << std::endl;
}

/*!
  Destructor that call cleanup().
 */
vpNaoqiGrabber::~vpNaoqiGrabber()
{
  if (m_isOpen)
    cleanup();
}

/*!
  Select the camera to use.
  \param camera_id : Camera identifier for  Pepper : CameraTop(0), CameraBottom(1), CameraDepth(2)
 */
void vpNaoqiGrabber::setCamera(const int &camera_id)
{
  m_colorspace = AL::kYUV422ColorSpace;

    if (camera_id == 0)
    {
      m_cameraName = "CameraTop";
      m_cameraId = camera_id;
    }
    else if (camera_id == 1)
    {
      m_cameraName = "CameraBottom";
      m_cameraId = camera_id;
    }
    
   else if (camera_id == 2)
  {
    m_cameraName = "DepthCamera";
    m_cameraId = camera_id;
    m_colorspace = AL::kRawDepthColorSpace;
  }
   else if (camera_id == 3)
  {
    m_cameraName = "DepthCamera";
    m_cameraId = 2;
    m_colorspace = AL::kDepthColorSpace;
  }

    else if (camera_id == 4)
   {
     m_cameraName = "DepthCamera";
     m_cameraId = 2;
     m_colorspace = AL::kDistanceColorSpace;
   }

    else if (camera_id == 5)
   {
     m_cameraName = "DepthCamera";
     m_cameraId = 2;
     m_colorspace = AL::kXYZColorSpace;
   }
    
    else if (camera_id == 6)
    {
        m_cameraName = "CameraDepth";
        m_cameraId =  AL::kDepthCamera;
        m_colorspace =  AL::kInfraredColorSpace;
    }

  return;

}



void vpNaoqiGrabber::open()
{
  if (! m_isOpen) {

    if (!m_handle.empty())
    {
      m_pVideo.call<qi::AnyValue>("unsubscribe", m_handle);
      m_handle.clear();
    }

    m_handle = m_pVideo.call<std::string>(
          "subscribeCamera",
          m_cameraName,
          m_cameraId,
          m_resolution,
          //AL::kYUV422ColorSpace,
          m_colorspace,
          m_fps
          );

    qi::AnyValue image_anyvalue = m_pVideo.call<qi::AnyValue>("getImageRemote", m_handle);
    naoqi::tools::NaoqiImage image;
    try{
      image = naoqi::tools::fromAnyValueToNaoqiImage(image_anyvalue);
    }
    catch(std::runtime_error& e)
    {
      std::cout << "Cannot retrieve image" << std::endl;
      return;
    }

    // Convert the image in RGB
    if(m_pepper && m_cameraId==2){
        cv::Mat cv_RGB(image.height, image.width, CV_16U, image.buffer);
        cv_RGB.convertTo(m_img,CV_8U);
    }
    else{
    cv::Mat cv_YUV(image.height, image.width, CV_8UC2, image.buffer);
    cv::cvtColor(cv_YUV, m_img, cv::COLOR_YUV2RGB_YUYV);
}
    m_width  = image.width;
    m_height = image.height;

    m_isOpen = true;
  }
}


void vpNaoqiGrabber::cleanup()
{
  if (!m_handle.empty())
  {
    std::cout << "Unsubscribe camera handle " << m_handle << std::endl;
    m_pVideo.call<qi::AnyValue>("unsubscribe", m_handle);
    m_handle.clear();
  }
  m_isOpen = false;
}



void vpNaoqiGrabber::acquire(cv::Mat &I)
{
  struct timeval timestamp;
  acquire(I, timestamp);
}

void vpNaoqiGrabber::acquiredepth(cv::Mat &I)
{
  struct timeval timestamp;
  acquiredepth(I, timestamp);
}


void vpNaoqiGrabber::acquire(cv::Mat &I, struct timeval &timestamp)
{
  if (! m_isOpen)
    open();

  // Get the image
  qi::AnyValue image_anyvalue = m_pVideo.call<qi::AnyValue>("getImageRemote", m_handle);
  naoqi::tools::NaoqiImage image;
  try{
    image = naoqi::tools::fromAnyValueToNaoqiImage(image_anyvalue);
  }
  catch(std::runtime_error& e)
  {
    std::cout << "Cannot retrieve image" << std::endl;
    return;
  }

  cv::Mat cv_YUV(image.height, image.width, CV_8UC2, image.buffer);
  cv::cvtColor(cv_YUV, I, cv::COLOR_YUV2BGR_YUYV);

  m_width  = image.width;
  m_height = image.height;

  double tv_sec  = (double)image.timestamp_s;
  double tv_usec = (double)image.timestamp_us;
  timestamp.tv_sec  = (unsigned long) tv_sec;
  timestamp.tv_usec = (unsigned long) tv_usec;

}

void vpNaoqiGrabber::acquirevoxel(cv::Mat &I){
  if (! m_isOpen)
    open();

  // Get the image
  qi::AnyValue image_anyvalue = m_pVideo.call<qi::AnyValue>("getImageRemote", m_handle);
  naoqi::tools::NaoqiImage image;
  try{
    image = naoqi::tools::fromAnyValueToNaoqiImage(image_anyvalue);
  }
  catch(std::runtime_error& e)
  {
    std::cout << "Cannot retrieve image" << std::endl;
    return;
  }

  cv::Mat cv_XYZ(image.height, image.width, CV_32FC3, image.buffer);
  I = cv_XYZ.clone();
  m_width  = image.width;
  m_height = image.height;


}



void vpNaoqiGrabber::acquiredepth(cv::Mat &I, struct timeval &timestamp)
{
  if (! m_isOpen)
    open();

  // Get the image
  qi::AnyValue image_anyvalue = m_pVideo.call<qi::AnyValue>("getImageRemote", m_handle);
  naoqi::tools::NaoqiImage image;
  try{
    image = naoqi::tools::fromAnyValueToNaoqiImage(image_anyvalue);
  }
  catch(std::runtime_error& e)
  {
    std::cout << "Cannot retrieve image" << std::endl;
    return;
  }

  cv::Mat cv_RGB(image.height, image.width, CV_16U, image.buffer);
  I = cv_RGB.clone();

  m_width  = image.width;
  m_height = image.height;

  double tv_sec  = (double)image.timestamp_s;
  double tv_usec = (double)image.timestamp_us;
  timestamp.tv_sec  = (unsigned long) tv_sec;
  timestamp.tv_usec = (unsigned long) tv_usec;

}



/*!
  Set a camera parameter.
  \param parameterId : Camera parameter requested.
  \param value : Value requested.

  \return True if succesfull.
*/

bool vpNaoqiGrabber::setCameraParameter(const int& parameterId, const int& value)
{

  bool result = m_pVideo.call<bool>("setCameraParameter", m_handle, parameterId, value);

  return result;
}

