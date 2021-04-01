/*
 * Copyright (c) 2012, Willow Garage, Inc.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the Willow Garage, Inc. nor the names of its
 *       contributors may be used to endorse or promote products derived from
 *       this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/* 
 * Author: Chad Rockey
 */
/* modified for freespacenavigation by Suman Raj Bista */
/* taken from https://github.com/ros-perception/depthimage_to_laserscan */


#ifndef DEPTHIMAGESCANNER
#define DEPTHIMAGESCANNER

#include "pepperlaser.h"

#include "depth_traits.h"
#include <sstream>
#include <iostream>

#include <limits.h>
#include <math.h>
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

namespace depthimagescanner
{ 
  class DepthImageScanner
  {
  public:

    /**
     * constructor
     * @params Kc instrinsic matrix of the depth camera
     */

    DepthImageScanner(cv::Mat &Kc);

    /**
     * destructor
     */

    ~DepthImageScanner();

    /**
     * sets depth as unsigned char
     */

    void setdepthasUC();

    /**
     * reads laser and sonar value
     * @param memeoryproxy memeory proxy object
     */

    void readlasersonar(qi::AnyObject memeoryproxy);

    /**
     * set image name for saving
     * @param ct : counter for file name
     */

    void setImname(int ct);

    /**
     * sets depth as double
     */

    void setdepthasD();

    /**
     * Converts depth image to 2D occupancy grid.
     * 
     * @param depth depth image.
     * @param grid  output grid image
     * @param od  input robot's transformation from prevous frame measured fro robot's odometry
     * @param wr, wrl => rotational velocity vector.
     * 
     */
    void convert_msg(cv::Mat &, cv::Mat &grid, std::vector<double> &od, std::vector<double>& wr, std::vector<double> & wrl);
    

    
    /**
     * Sets the minimum and maximum range for laser sacan
     * 
     * range_min is used to determine how close of a value to allow through when multiple radii correspond to the same
     * angular increment.  range_max is used to set the output message.
     * 
     * @param range_min Minimum range to assign points to the laserscan, also minimum range to use points in the output scan.
     * @param range_max Maximum range to use points in the output scan.
     * 
     */
    void set_range_limits(const float range_min, const float range_max);
    
    /**
     * Sets the number of image rows to use in the output LaserScan.
     * 
     * scan_height is the number of rows (pixels) to use in the output.  This will provide scan_height number of radii for each
     * angular increment.  The output scan will output the closest radius that is still not smaller than range_min.  This function
     * can be used to vertically compress obstacles into a single LaserScan.
     * 
     * @param scan_height Number of pixels centered around the center of the image to compress into the LaserScan.
     * 
     */
    void set_scan_height(const int scan_height);
    
    /**
     * Sets the frame_id for the output LaserScan.
     * 
     * Output frame_id for the LaserScan.  Will probably NOT be the same frame_id as the depth image.
     * Example: For OpenNI cameras, this should be set to 'camera_depth_frame' while the camera uses 'camera_depth_optical_frame'.
     * 
     * @param output_frame_id Frame_id to use for the output sensor_msgs::LaserScan.
     * 
     */
    void set_output_frame(const std::string output_frame_id);

  private:
    /**
     * Computes euclidean length of a cv::Point3d (as a ray from origin)
     * 
     * This function computes the length of a cv::Point3d assumed to be a vector starting at the origin (0,0,0).
     * 
     * @param ray The ray for which the magnitude is desired.
     * @return Returns the magnitude of the ray.
     * 
     */
    double magnitude_of_ray(const cv::Point3d& ray) const;

    /**
     * Computes the angle between two cv::Point3d
     * 
     * Computes the angle of two cv::Point3d assumed to be vectors starting at the origin (0,0,0).
     * Uses the following equation: angle = arccos(a*b/(|a||b|)) where a = ray1 and b = ray2.
     * 
     * @param ray1 The first ray
     * @param ray2 The second ray
     * @return The angle between the two rays (in radians)
     * 
     */
    double angle_between_rays(const cv::Point3d& ray1, const cv::Point3d& ray2) const;
    
    /**
     * Determines whether or not new_value should replace old_value in the LaserScan.
     * 
     * Uses the values of range_min, and range_max to determine if new_value is a valid point.  Then it determines if
     * new_value is 'more ideal' (currently shorter range) than old_value.
     * 
 depth_traits    * @param new_value The current calculated range.
     * @param old_value The current range in the output LaserScan.
     * @param range_min The minimum acceptable range for the output LaserScan.
     * @param range_max The maximum acceptable range for the output LaserScan.
     * @return If true, insert new_value into the output LaserScan.
     *
     */
    bool use_point(const float new_value, const float old_value, const float range_min, const float range_max) const;

    /**
    * Converts the depth image to a laserscan using the DepthTraits to assist.
    *
    * This uses a method to inverse project each pixel into a LaserScan angular increment.  This method first projects the pixel
    * forward into Cartesian coordinates, then calculates the range and angle for this point.  When multiple points coorespond to
    * a specific angular measurement, then the shortest range is used.
    *
    * @param depth_msg The UInt16 or Float32 encoded depth message.
    * @param cam_model The image_geometry camera model for this image.
    * @param scan_msg The output LaserScan.
    * @param scan_height The number of vertical pixels to feed into each angular_measurement.
    *
    */
    cv::Point3d projectPixelTo3dRay(cv::Mat& uv_rect);


    /**
    * process each row of occupancy grid to find the neareast drivable free space
    * @param ocpgrid horizontal row of an occupancy grid
    * @param ngrid number of grids
    * @param gridlen size of grid in cm
    */

    /**
     * @return best free space
     */
    double processGrid(cv::Mat& ocpgrid , int ngrid, int gridlen) const;
    /**
     * @return best free space in either side from the center of the grid
     */
    std::vector<double> processOccGrid(cv::Mat& ocpgrid , int ngrid, int gridlen) const;



    /**
     *  process depth and ouputs rotational velocity for free-space navigation
     *
     */
    template<typename T>
    void processDepth(cv::Mat& depth ,  std::vector<double>& depthscan, const int& scan_height, cv::Mat& grid, std::vector<double>& wr,std::vector<double> &od,  std::vector<double> &wrx) /*const*/;


    /**
     * @brief getLaserData
     * @param memeoryproxy object of memeory proxy
     * @param X,Y laser scan values
     *
     */

    void getLaserData(qi::AnyObject memeoryproxy,std::vector<double> &X,std::vector<double> &Y);

 


    int depth_type;
    int step;
    float  angle_max,  angle_min, angle_increment;
    float scan_time_; ///< Stores the time between scans.
    float range_min_; ///< Stores the current minimum range to use.
    float range_max_; ///< Stores the current maximum range to use.
    int scan_height_; ///< Number of pixel rows to use when producing a laserscan from an area.
    std::string output_frame_id_; ///< Output frame_id for each laserscan.  This is likely NOT the camera's frame_id.
    cv::Mat K;
    double sx,sy,cx,cy,Tx,Ty;
    cv::Mat distr;

    std::vector<double> a_cos_;
     std::vector<double> a_sin_;
    int frameno;
    std::vector<double> depthscanincr;


    cv::Mat currgrid;
    cv::Mat currgridx;
    cv::Mat prevgrid;
    cv::Mat prevgridx;


    std::vector<double> X;
    std::vector<double> Y;
    std::vector<double> x;
    std::vector<double> z;
    double lefS;

    std::vector< std::vector<double> > wrl;
    std::vector< std::vector<double> > wrlx;
    std::vector<int> sgnx;


    };
  
  
};

#endif
