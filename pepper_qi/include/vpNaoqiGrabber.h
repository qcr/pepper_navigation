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

#ifndef vpNaoqiGrabber_h
#define vpNaoqiGrabber_h

#include <iostream>
#include <string>
#include <sys/time.h>

#include <qi/log.hpp>
#include <qi/applicationsession.hpp>
#include <qi/anyobject.hpp>

// Opencv includes for cv::Mat interface
#include <opencv2/opencv.hpp>

class vpNaoqiGrabber
{

protected:
  qi::AnyObject m_pVideo; //!< Video proxy
  qi::AnyObject m_pMemory; //!< Memory proxy
  std::string m_handle; //!< Handle Video proxy
  int m_fps; //!< Requested frame per second
  bool m_isOpen; //!< Proxy opened status
  int m_width; //!<  Image width
  int m_height; //!< Image height
  cv::Mat m_img; //!< Image data
  std::string m_cameraName; //!< Camera name
  int m_cameraId; //!< Camera identifier
  int m_resolution;  //!< Resolution camera
  int m_colorspace; //!< Colorspace
  int m_CV_imtype;
  int m_colrspaceconvop;
  std::string m_robotType; //!< Nao, Pepper or Romeo
  bool m_pepper; //!< True if robot is Pepper

  // Multi stream
  bool m_cameraMulti;

public:

  /**
   * @brief vpNaoqiGrabber constructor for grabbing image
   * @param session current session pointer
   */

  vpNaoqiGrabber(const qi::SessionPtr &session);

  /**
   * @brief ~vpNaoqiGrabber
   */
  virtual ~vpNaoqiGrabber();

  /**
   * @brief acquire acquire RBG image
   * @param I image in cv::Mat
   */
  void acquire(cv::Mat &I);
  void acquire(cv::Mat &I, struct timeval &timestamp);

  /**
   * @brief acquiredepth acquire depth image
   * @param I image in cv::Mat
   */
  void acquiredepth(cv::Mat &I);
  void acquiredepth(cv::Mat &I, struct timeval &timestamp);

  void acquirevoxel(cv::Mat &I);

  void cleanup();


 std::string getCameraName(){return m_cameraName;}
  /*!

     \return Image width.
   */
  unsigned int getWidth() const
  {
    return (unsigned int)m_width;
  }
  /*!

     \return Image height.
   */
  unsigned int getHeight() const
  {
    return (unsigned int)m_height;
  }
  /*!
    Return the video device proxy used to grab images.

  */
     qi::AnyObject getProxy() const
    {
      return m_pVideo;
    }

 /**
   * @brief open Start Camera
   */
  void open();

  /**
   * @brief setCamera id 0 => top RGB, 1=> buttom RGB, 2-6=> depth camera
   * 2 => RawDepthColorSpace, 3 => DepthColorSpace, 4 => kDistanceColorSpace,
   * 5 => XYZColorSpace, 6 => InfraredColorSpace. as defined in
   * http://doc.aldebaran.com/2-5/family/pepper_technical/video_3D_pep.html
   * @param camera_id
   */

  void setCamera(const int &camera_id);

  /*!
    Set the camera framerate.
    In the constructor, the default framerate is set to 30 Hz.
    \param fps : New framerate in Hz.

    \sa open()
    */
  void setFramerate(int fps)
  {
    m_fps = fps;
  }


  /*!
    Set the camera resolution.
    \param resolution : Index camera resolution.

    \sa open()
  */
  void setCameraResolution(const int &resolution)
  {
    m_resolution = resolution;
  }



  bool setCameraParameter( const int& parameterId, const int& value);

};

#endif
