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

#include "DepthImageScanner.h"
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/ximgproc.hpp>

using namespace depthimagescanner;

DepthImageScanner::DepthImageScanner(cv::Mat &Kc){
    K = Kc.clone();
    sx = 1/K.at<double>(0,0);
    sy = 1/K.at<double>(1,1);
    cx = K.at<double>(0,2);
    cy = K.at<double>(1,2);
    Tx = 0;
    Ty = 0;
    depth_type = 0;
    step = 2;
    range_min_ = 0;
    range_max_ = 5.65f;
    frameno = 0;

      X.reserve(61);
      Y.reserve(61);
      X.reserve(320);
      Y.reserve(320);
      lefS = 0.0;

      prevgrid =  cv::Mat::zeros(400,800, CV_8UC1);
       prevgridx =  cv::Mat::zeros(400,800, CV_8UC1);


       wrlx.resize(4,std::vector<double>(3,0.0));
       wrl.resize(4,std::vector<double>(3,0.0));
      sgnx.reserve(4);

}


void DepthImageScanner::setdepthasUC(){
    depth_type = 0;
    step = 16/8;

}


void DepthImageScanner::setdepthasD(){
    depth_type = 1;
    step =4;

}


DepthImageScanner::~DepthImageScanner(){
}

double DepthImageScanner::magnitude_of_ray(const cv::Point3d& ray) const{
  return sqrt(pow(ray.x, 2.0) + pow(ray.y, 2.0) + pow(ray.z, 2.0));
}

double DepthImageScanner::angle_between_rays(const cv::Point3d& ray1, const cv::Point3d& ray2) const{
  double dot_product = ray1.x*ray2.x + ray1.y*ray2.y + ray1.z*ray2.z;
  double magnitude1 = magnitude_of_ray(ray1);
  double magnitude2 = magnitude_of_ray(ray2);;
  return acos(dot_product / (magnitude1 * magnitude2));
}

bool DepthImageScanner::use_point(const float new_value, const float old_value, const float range_min, const float range_max) const{
  // Check for NaNs and Infs, a real number within our limits is more desirable than these.
  bool new_finite = std::isfinite(new_value);
  bool old_finite = std::isfinite(old_value);
  
  // Infs are preferable over NaNs (more information)
  if(!new_finite && !old_finite){ // Both are not NaN or Inf.
    if(!std::isnan(new_value)){ // new is not NaN, so use it's +-Inf value.
      return true;
    }
    return false; // Do not replace old_value
  }
  
  // If not in range, don't bother
  bool range_check = range_min <= new_value && new_value <= range_max;
  if(!range_check){
    return false;
  }
  
  if(!old_finite){ // New value is in range and finite, use it.
    return true;
  }
  
  // Finally, if they are both numerical and new_value is closer than old_value, use new_value.
  bool shorter_check = new_value < old_value;
  return shorter_check;
}

 cv::Point3d DepthImageScanner::projectPixelTo3dRay(cv::Mat& uv_rect){
     cv::Point3d ray;

     ray.x = (uv_rect.at<double>(0) - cx - Tx) * sx;
     ray.y = (uv_rect.at<double>(1) - cy - Ty) * sy;
     ray.z = 1.0;

     return ray;
 }

void DepthImageScanner::readlasersonar(qi::AnyObject memeoryproxy){
    getLaserData(memeoryproxy,X,Y);
     lefS =  memeoryproxy.call<double>("getData","Device/SubDeviceList/Platform/Front/Sonar/Sensor/Value");
}

void DepthImageScanner::convert_msg(cv::Mat& depth, cv::Mat& grid, std::vector<double> &od, std::vector<double>& wr,  std::vector<double>& wrx){
  // Set camera model
  //cam_model_.fromCameraInfo(info_msg);
  
  // Calculate angle_min and angle_max by measuring angles between the left ray, right ray, and optical center ray
  cv::Mat raw_pixel_left = (cv::Mat_<double>(2,1) << 0, cy);
 /* cv::Mat rect_pixel_left;
  cv::undistortPoints(raw_pixel_left, rect_pixel_left, K,cv::Mat());*/
  cv::Point3d left_ray = projectPixelTo3dRay(raw_pixel_left);
  
  cv::Mat raw_pixel_right = (cv::Mat_<double>(2,1) <<depth.cols, cy);
  /*cv::Mat rect_pixel_right;
  cv::undistortPoints(raw_pixel_right, rect_pixel_right, K,cv::Mat());*/
  cv::Point3d right_ray = projectPixelTo3dRay(raw_pixel_right);
  
  cv::Mat raw_pixel_center =(cv::Mat_<double>(2,1) <<cx,cy);
  /*cv::Mat rect_pixel_center;
  cv::undistortPoints(raw_pixel_center, rect_pixel_center, K,cv::Mat());*/

  cv::Point3d center_ray = projectPixelTo3dRay(raw_pixel_center);
  
 angle_max = angle_between_rays(left_ray, center_ray);
 angle_min = -angle_between_rays(center_ray, right_ray); // Negative because the laserscan message expects an opposite rotation of that from the depth image
  
angle_increment = (angle_max - angle_min) / (depth.cols - 1);


  
  // Check scan_height vs image_height
  if(scan_height_/2 > cy || scan_height_/2 > depth.rows - cy){
    std::stringstream ss;
    ss << "scan_height ( " << scan_height_ << " pixels) is too large for the image height.";
    throw std::runtime_error(ss.str());
  }

  std::vector<double> depthscan;
 // uint32_t ranges_size = 320;
  //int n = ranges_size;
  // depthscan.assign(ranges_size, std::numeric_limits<float>::quiet_NaN());
 // depthscan.assign(ranges_size,0);
  depthscan.reserve(12);
  if (depth_type == 0)
  {
    processDepth<uint16_t>(depth, depthscan, scan_height_,grid,wr,od, wrx);
  }
  else if (depth_type == 1)
  {
    processDepth<float>(depth , depthscan, scan_height_,grid,wr,od, wrx);
  }
  else
  {
    std::stringstream ss;
    ss << "Depth image has unsupported encoding: ";
    throw std::runtime_error(ss.str());
  }


 frameno++;

}



void DepthImageScanner::set_range_limits(const float range_min, const float range_max){
  range_min_ = range_min;
  range_max_ = range_max;
}

void DepthImageScanner::set_scan_height(const int scan_height){
  scan_height_ = scan_height;
}

void DepthImageScanner::set_output_frame(const std::string output_frame_id){
  output_frame_id_ = output_frame_id;
}


double DepthImageScanner::processGrid(cv::Mat& ocpgrid , int ngrid, int gridlen) const{

    int midgrid = ngrid/2;
     double fs = 0.0;
    int nL =0 ; int nR =0;
    int oL = 0; int oR = 0;
    int flagR = 0; int flagL = 0;

    int lpL = 0; //14;//0
    int upL = ngrid;//65;

     int basedist = 60;
     int nbgrd = basedist/gridlen;
  //   std::cout<<"ngrid = "<<nbgrd<<std::endl;
    int jL = midgrid-1;
    int jR = midgrid;

    while(ocpgrid.at<uchar>(jL)==0 & jL>=lpL){
        jL--; nL++;
    }

    while(ocpgrid.at<uchar>(jR)==0 & jR<upL){
        jR++; nR++;
    }

  //  std::cout<<nL<<"\t"<<nR<<"\t"<<fs<<"\t"<<nL+nR<<std::endl;

    if(nL+nR<nbgrd){

        while(ocpgrid.at<uchar>(jL)>0 & jL>=lpL) { jL--; oL++;}
        while(ocpgrid.at<uchar>(jR)>0 & jR<upL) { jR++; oR++;}

        flagR = jR-midgrid;
        flagL = midgrid-1-jL;
        nL =0; nR =0;

        while(ocpgrid.at<uchar>(jL)==0 & jL>=lpL){
            jL--; nL++;
        }

        while(ocpgrid.at<uchar>(jR)==0 & jR<upL){
            jR++; nR++;
        }


        if(nL>=nbgrd || nR>=nbgrd ){

            if (oR>40/gridlen && oL>40/gridlen){

                fs = std::numeric_limits<float>::quiet_NaN();
                std::cout<<"Obstacle in middle"<<std::endl;
            }

            else if (flagL>flagR && nR>=nbgrd){
                 fs = -(nbgrd/2+flagR);
                  std::cout<<"turn rightt \t"<<fs<<std::endl;
            }
            else {
                fs = (flagL+nbgrd/2);
                 std::cout<<"turn leftt \t"<<fs<<std::endl;
            }



        }
        else{
            fs = std::numeric_limits<float>::quiet_NaN();
            std::cout<<"No way !!!!"<<std::endl;
        }

     //   std::cout<<flagL<<"\t"<<midgrid-1-jL<<"\t"<<flagL-jL<<"\t"<<flagR<<"\t"<<jR-midgrid<<"\t"<<jR-flagR<<"\t"<<fs<<std::endl;

    }


    else{


       if(nL>=nbgrd/2 || nR>=nbgrd/2 ){
              if(nL>nR)
              {
                 if (nR >= (nbgrd/2+0.5)){ fs = 0.00001;   std::cout<<"LT straight \t"<<fs<<std::endl;}
                  else { fs = (0.5+nbgrd/2-nR); std::cout<<"turn left \t"<<fs<<std::endl;}
              }
              else if(nL<nR) {
                 if (nL >= (nbgrd/2+0.5)){ fs = -0.00001;   std::cout<<"RT straight \t"<<fs<<std::endl;}
                 else { fs = -(0.5+nbgrd/2-nL); std::cout<<"turn right \t"<<fs<<std::endl;}
              }
              else{
                  fs = 0;
                    std::cout<<"straight \t"<<fs<<std::endl;
                 }

       }
       else{
           fs = (nL-nR);
            std::cout<<"straight \t"<<fs<<std::endl;
       }

     // std::cout<<"istlevel = \t"<<nL<<"\t"<<nR<<"\t"<<fs<<std::endl;

    }

    // std::cout<<nL<<"\t"<<nR<<"\t"<<fs<<"\t"<<nL+nR<<std::endl;
    // std::cout<<oL<<"\t"<<oR<<"\t"<<midgrid<<"\t"<<ocpgrid.cols<<std::endl;

     return fs*gridlen;

}




std::vector<double> DepthImageScanner::processOccGrid(cv::Mat& ocpgrid , int ngrid, int gridlen) const{

    int midgrid = ngrid/2;
     double fs = std::numeric_limits<float>::quiet_NaN();
     double fsL = std::numeric_limits<float>::quiet_NaN();;
     double fsR = std::numeric_limits<float>::quiet_NaN();;
    int nL =0 ; int nR =0;
    int oL = 0; int oR = 0;
    int flagR = 0; int flagL = 0;

    int lpL = 0; //14;//0
    int upL = ngrid;//65;

     int basedist = 60;
     int nbgrd = basedist/gridlen;
  //   std::cout<<"ngrid = "<<nbgrd<<std::endl;
    int jL = midgrid-1;
    int jR = midgrid;

    /* find free space in middle */
    while(ocpgrid.at<uchar>(jL)==0 & jL>=lpL){
        jL--; nL++;
    }

    while(ocpgrid.at<uchar>(jR)==0 & jR<upL){
        jR++; nR++;
    }

  //  std::cout<<nL<<"\t"<<nR<<"\t"<<fs<<"\t"<<nL+nR<<std::endl;

/* if not in midlle look for other free space */

    if(nL+nR<nbgrd){

        while(ocpgrid.at<uchar>(jL)>0 & jL>=lpL) { jL--; oL++;}
        while(ocpgrid.at<uchar>(jR)>0 & jR<upL) { jR++; oR++;}

        flagR = jR-midgrid;
        flagL = midgrid-1-jL;
        nL =0; nR =0;

        while(ocpgrid.at<uchar>(jL)==0 & jL>=lpL){
            jL--; nL++;
        }

        while(ocpgrid.at<uchar>(jR)==0 & jR<upL){
            jR++; nR++;
        }


        if(nL>=nbgrd || nR>=nbgrd ){
            fsL = (flagL+nbgrd/2);
            fsR =  -(nbgrd/2+flagR);

            if (oR>40/gridlen && oL>40/gridlen){

                fs = std::numeric_limits<float>::quiet_NaN();
                std::cout<<"Obstacle in middle"<<std::endl;
            }


          //  else if(nL>nR)
            else if(-fsR>fsL)
            {
                fs = fsL;
                 std::cout<<"turn leftt \t"<<fs<<std::endl;

                // std::cout<<flagL/nbgrd<<std::endl;
            }
            else
            {
                fs = fsR;
                 std::cout<<"turn rightt \t"<<fs<<std::endl;

               //  std::cout<<flagR/nbgrd<<std::endl;
            }


        }
        else{
            fs = std::numeric_limits<float>::quiet_NaN();
            std::cout<<"No way !!!!"<<std::endl;
        }



       std::cout<<flagL<<"\t"<<fsL<<"\t"<<nL<<"\t"<<flagR<<"\t"<<fsR<<"\t"<<nR<<std::endl;

    }


    else{


       if(nL>=nbgrd/2 || nR>=nbgrd/2){
              fsL = (nbgrd/2-nR+0.5);
              fsR = -(nbgrd/2-nL+0.5);
              if(nL>nR)
              {
                  if (nR >=(nbgrd/2+0.5)){ fs = 0.000001;   std::cout<<"LT straight \t"<<fs<<std::endl;}
                  else {fs = fsL;  std::cout<<"turn left \t"<<fs<<std::endl; }
              }
              else if(nL<nR) {
                  if (nL >= (nbgrd/2+0.5)){ fs = -0.000001;   std::cout<<"RT straight \t"<<fs<<std::endl;}
                  else { fs = fsR; std::cout<<"turn right \t"<<fs<<std::endl; }
              }
              else{
                  fs = 0;
                    std::cout<<"straight \t"<<fs<<std::endl;
                 }

       }
       else{
           fsL = 0;
           fsR = 0;
           fs = (nL-nR);
            std::cout<<"straightt\t"<<fs<<std::endl;
       }

     // std::cout<<"istlevel = \t"<<nL<<"\t"<<nR<<"\t"<<fs<<std::endl;

    }

    // std::cout<<nL<<"\t"<<nR<<"\t"<<fs<<"\t"<<nL+nR<<std::endl;
    // std::cout<<oL<<"\t"<<oR<<"\t"<<midgrid<<"\t"<<ocpgrid.cols<<std::endl;
    std::vector<double> fsv;
    fsv.reserve(3);
    fsv[0] = fs*gridlen;
    fsv[1] = fsL*gridlen;
    fsv[2] = fsR*gridlen;


     return fsv;

}



template<typename T>
void DepthImageScanner::processDepth(cv::Mat& depth ,  std::vector<double>& depthscan, const int& scan_height, cv::Mat& grid, std::vector<double>& wr, std::vector<double> &od, std::vector<double> &wrx) /*const*/{
    // Use correct principal point from calibration
    float center_x = cx;
    float center_y = cy;
    double fs = 0;
    // Combine unit conversion (if necessary) with scaling by focal length for computing (X,Y)
    double unit_scaling = depthimagescanner::DepthTraits<T>::toMeters( T(1) );
    float constant_x = unit_scaling * sx;
    float constant_y = unit_scaling * sy;

    const T* depth_row = reinterpret_cast<const T*>(depth.data);
    int row_step = depth.cols * step / sizeof(T);

    int offset = (int)(cy-scan_height/2);
    depth_row += offset*row_step; // Offset to center of image
//sx,sy
    grid = cv::Mat::zeros(800,800, CV_8UC3);

  /*  for(int i=0;i<400;i++){
        for(int j=0;j<800;j++){
            currgrid[i][j] = 0;
        }}
*/
//P1.clear();
     currgrid =  cv::Mat::zeros(400,800, CV_8UC1);
     currgridx =  cv::Mat::zeros(400,800, CV_8UC1);
     //m1.clear();

    double dx = -od[0];
    double dy = -od[1];
    double dt = od[2];


    std::cout<<"\n od = \t"<<od[0]<<"\t"<<od[1]<<"\t"<<od[2]<<std::endl;

    double xn = 0.0;
    double zn = 0.0;
    double xo = 0.0;
    double zo = 0.0;

    int gridlen = 10;
    for(int i=0;i<800;i+=100)
        cv::line(grid,cv::Point(0,i),cv::Point(799,i),cv::Scalar(200,200,200,0),1);


    for(int i=0;i<800;i+=gridlen)
        cv::line(grid,cv::Point(i,0),cv::Point(i,799),cv::Scalar(150,150,150,0),1);

    for(int i=0;i<800;i+=100)
        cv::line(grid,cv::Point(i,0),cv::Point(i,799),cv::Scalar(200,200,200,0),1);


   int ngrid = 800/gridlen;

   int offx = 400;
   int offy = 100;
   int offmy = 700;


   cv::Mat ocpgrid = cv::Mat::zeros(1,ngrid,CV_8U);
    cv::Mat ocpgrid2 = cv::Mat::zeros(1,ngrid,CV_8U);
    cv::Mat ocpgrid3 = cv::Mat::zeros(1,ngrid,CV_8U);
    cv::Mat ocpgridx = cv::Mat::zeros(1,ngrid,CV_8U);

    for(int v = offset; v < offset+scan_height_; v++, depth_row += row_step){
      for (int u = 0; u < depth.cols; u++) // Loop over each pixel in row
      {
        T depth = depth_row[(u)];

        double r = depth; // Assign to pass through NaNs and Infs
        double th = -atan2((double)(u - center_x) * constant_x, unit_scaling); // Atan2(x, z), but depth divides out
        int index = (th - angle_min) / angle_increment;
         double x,y,z;
        if (depthimagescanner::DepthTraits<T>::valid(depth)){ // Not NaN or Inf
          // Calculate in XYZ
           x = (u - center_x) * depth * constant_x - 0.039;
           z = depthimagescanner::DepthTraits<T>::toMeters(depth) + 0.3463;
           y = (v -center_y) * depth * constant_y + 0.04344;
          // Calculate actual distance
          r = sqrt(pow(x, 2.0) + pow(z, 2.0));

         // std::cout<<y <<"\t";
        }




    // Determine if this point should be used.
 /*   if(use_point(r, depthscan[index], range_min_, range_max_)){
      depthscan[index] = r;
    //cv::circle(grid, cv::Point(-r*sin(th)*100+offx,offmy - r*cos(th)*100), 4.0, cv::Scalar(255,255,255),-1);
    }
*/

   // }
  //  if(z<2){

   if(z>0.5 && z<1.5){

    cv::circle(grid, cv::Point(x*100+offx,offmy-z*100), 4.0, cv::Scalar(255,0,0),-1);
  //
    cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+20,10,10),cv::Scalar(255,255,0),-1);
     cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+10,10,10),cv::Scalar(0,255,255),-1);
     cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+0,10,10),cv::Scalar(255,0,255),-1);

     ocpgrid.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
     ocpgrid2.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
     ocpgrid3.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
     //m1.insertPoint(z,x);
   //  P1.push_back(pcl::PointXYZ(z,x,0.0));

         // currgrid[int(z*100+0.5)][int(x*100+offx+0.5)]=1;
    if(z>1)
     currgrid.at<uchar>(int(z*100+0.5),int(x*100+offx+0.5))=1;
     currgridx.at<uchar>(int(z*100+0.5),int(x*100+offx+0.5))=1;
    // if(int(x*100+400)/gridlen==40)  std::cout<<"( "<<x<<" , "<<z<<" )"<<"\t";
    }


   if(z>=1.5 && z<2.5){
    cv::circle(grid, cv::Point(x*100+offx,offmy-z*100), 4.0, cv::Scalar(255,0,0),-1);
    cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+10,10,10),cv::Scalar(0,255,255),-1);
    cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+0,10,10),cv::Scalar(255,0,255),-1);
    ocpgrid2.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
    ocpgrid3.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
        //P1.push_back(pcl::PointXYZ(z,x,0.0));

 //  currgrid[int(z*100+0.5)][int(x*100+offx+0.5)]=1;
    // m1.insertPoint(z,x);
     currgrid.at<uchar>(int(z*100+0.5),int(x*100+offx+0.5))=1;
     currgridx.at<uchar>(int(z*100+0.5),int(x*100+offx+0.5))=1;
   }


   if(z>=2.5 && z<4){
     cv::circle(grid, cv::Point(x*100+offx,offmy-z*100), 4.0, cv::Scalar(255,0,0),-1);
    cv::rectangle(grid, cv::Rect(int(x*100+offx),offy+0,10,10),cv::Scalar(255,0,255),-1);
    ocpgrid3.at<uchar>(int(x*100+offx+0.5)/gridlen)=1;
  // currgrid[int(z*100+0.5)][int(x*100+offx+0.5)]=1;
   // currgrid.at<uchar>(int(z*100+0.5),int(x*100+offx+0.5))=1;
    // m1.insertPoint(z,x);
        //P1.push_back(pcl::PointXYZ(z,x,0.0));
   }







  }
    }






    for(int i=0;i<400;i++){
        for(int j=0;j<800;j++){

            if(prevgrid.at<uchar>(i,j)>0){
              zo = i/100.0;
              xo = (j-offx)/100.0;
              xn = (xo*cos(dt)-zo*sin(dt)+dy)*100.0+offx+0.5;
             // xn = (xo*transformation_matrix (1, 1)+zo* transformation_matrix (1, 0)+ transformation_matrix (1, 3))*100.0+offx+0.5;
             zn = (xo*sin(dt)+zo*cos(dt)+dx)*100+0.5;
            //  zn = (zo*transformation_matrix (0, 0) + xo* transformation_matrix (0, 1) + transformation_matrix (0, 3))*100+0.5;
              if(xn>=0 && zn>=0)
                //currgrid[int(zn)][int(xn)]=2;
                  currgrid.at<uchar>(int(zn),int(xn))=2;

          }



        }
    }


   cv::imshow("dim",depth);
   cv::waitKey(1);


    for(int i=0;i<400;i++)

        for(int j=0;j<800;j++){
            //prevgrid[i][j] = currgrid[i][j];
            prevgrid.at<uchar>(i,j) = currgrid.at<uchar>(i,j);
             prevgridx.at<uchar>(i,j) = currgridx.at<uchar>(i,j);
          // if(currgrid[i][j]==1)
             if(currgridx.at<uchar>(i,j)==1)
                cv::circle(grid, cv::Point(j,offmy-i), 4.0, cv::Scalar(0,0,255),-1);

            if(currgrid.at<uchar>(i,j)==1)
           cv::circle(grid, cv::Point(j,offmy-i), 4.0, cv::Scalar(255,0,0),-1);
         // else if(currgrid[i][j]==2)
            else if(currgrid.at<uchar>(i,j)==2)
                 cv::circle(grid, cv::Point(j,offmy-i), 4.0, cv::Scalar(0,255,0),-1);
           if(currgrid.at<uchar>(i,j)>0 && i>40 && i<100){
               ocpgridx.at<uchar>(int(j)/gridlen)=1;

                cv::rectangle(grid, cv::Rect(int(j),offy+30,10,10),cv::Scalar(255,255,0),-1);
           }

        }


    for(int hh=0;hh<4;hh++){
        for(int gg=0;gg<3;gg++){
            wrl[hh][gg]=0;
              wrlx[hh][gg]=0;
        }
        wr[hh] = 0;
        wrx[hh] = 0;

        }


    cv::medianBlur(prevgrid,prevgrid,3);

    //cv::ximgproc::weightedMedianFilter(prevgrid,prevgrid,1);
    cv::imwrite(output_frame_id_,grid);


    double fs1 = processGrid(ocpgrid ,ngrid, gridlen)/100;
    std::cout<<fs1<<std::endl;
    double fs2 = processGrid(ocpgrid2 ,ngrid, gridlen)/200;
   std::cout<<fs2<<std::endl;
    double fs3 = processGrid(ocpgrid3 ,ngrid, gridlen)/325;
   std::cout<<fs3<<std::endl;
   double fsx = processGrid(ocpgridx ,ngrid, gridlen)/70;
  std::cout<<fsx<<std::endl;
  //   std::cout<<ocpgrid<<std::endl<<std::endl;
      cv::circle(grid, cv::Point(400,offmy), 4.0, cv::Scalar(255,255,255),-1);
      cv::rectangle(depth,cv::Rect(0,offset,depth.cols,scan_height),cv::Scalar(5555,5555,5555));


    /*double w1 =  ((fs1-0)+fs1*0.2/1)/(1+fs1*fs1);
    double w2 =  ((fs2-0)+fs2*0.2/2)/(1+fs2*fs2);
    double w3 =  ((fs3-0)+fs3*0.2/3.25)/(1+fs3*fs3);
    double w4 =  ((fsx-0)+fsx*0.2/0.9)/(1+fsx*fsx);*/

    wr[0] = fs1;
    wr[1] = fs2;
    wr[2] = fs3;
    wr[3] = fsx;

    std::cout<<"Process gridsssssssssssss"<<std::endl;
    std::vector<double> wr1 = processOccGrid(ocpgrid ,ngrid, gridlen);
    std::cout<<wr1[0]/100<<"\t"<<wr1[1]/100<<"\t"<<wr1[2]/100<<std::endl;

    std::vector<double> wr2 = processOccGrid(ocpgrid2 ,ngrid, gridlen);
    std::cout<<wr2[0]/200<<"\t"<<wr2[1]/200<<"\t"<<wr2[2]/200<<std::endl;

    std::vector<double> wr3 = processOccGrid(ocpgrid3 ,ngrid, gridlen);
    std::cout<<wr3[0]/325<<"\t"<<wr3[1]/325<<"\t"<<wr3[2]/325<<std::endl;

    std::vector<double> wrX = processOccGrid(ocpgridx ,ngrid, gridlen);
    std::cout<<wrX[0]/75<<"\t"<<wr3[1]/70<<"\t"<<wr3[2]/75<<std::endl;

    for(int kk=0;kk<3;kk++){
        wrl[0][kk] = wr1[kk]/100;
        wrl[1][kk] = wr2[kk]/200;
        wrl[2][kk] = wr3[kk]/325;
        wrl[3][kk] = wrX[kk]/70;
    }

    for(int hh=0;hh<4;hh++){
         sgnx[hh] = 0;
         if(wrl[hh][0]>0.001) sgnx[hh] = 1;
         else if(wrl[hh][0]<-0.001) sgnx[hh] = -1;
    }

    wrx[0]=wrl[0][0];

  for(int hh=1;hh<4;hh++) {
    if((sgnx[hh] == sgnx[0]) || sgnx[hh]==0)
         wrx[hh]=wrl[hh][0];
    else {
        if(sgnx[0] == 1)
            wrx[hh]=wrl[hh][1];
        else
             wrx[hh]=wrl[hh][2];

    }
}

  std::cout<<"\n"<<output_frame_id_<<std::endl;



}




   void DepthImageScanner::setImname(int ct){

     std::stringstream currimName;
       if(ct<10)
        currimName<<"imgG_0000" <<ct<<".png";
       else if(ct<100)
           currimName<<"imgG_000" <<ct<<".png";
       else if(ct<1000)
           currimName<<"imgG_00" <<ct<<".png";
       else if(ct<10000)
           currimName<<"imgG_0" <<ct<<".png";
       else
           currimName<<"imgG_" <<ct<<".png";

       output_frame_id_= currimName.str();
   }





   void  DepthImageScanner::getLaserData(qi::AnyObject memeoryproxy,std::vector<double> &X,std::vector<double> &Y)
   {

   for(int i=0;i<61;i++){
       X[i]=0;
       Y[i]=0;
   }

     static const std::vector<std::string> laser_keys_value(laserMemoryKeys, laserMemoryKeys+90);

     std::vector<float> result_value;

     try {
         qi::AnyValue anyvalues = memeoryproxy.call<qi::AnyValue>("getListData", laser_keys_value);
         naoqi::tools::fromAnyValueToFloatVector(anyvalues, result_value);
       } catch (const std::exception& e) {
       std::cerr << "Exception caught in LaserConverter: " << e.what() << std::endl;
       return;
     }


      int pos = 0;
   double theta = 1.757;
   theta = 1.5708;
     // ranges between 0-29
     for( size_t i=0; i<30; i=i+2, ++pos)
     {
       const float lx = result_value[28-i]; // segments are internally flipped
       const float ly = result_value[28-i+1]; // segments are internally flipped
       float bx = lx*std::cos(-theta) - ly*std::sin(-theta) - 0.018;
       float by = lx*std::sin(-theta) + ly*std::cos(-theta) - 0.0889;

      X[pos]= bx;
      Y[pos]= by;
     }

     pos = pos+8; // leave out 8 blanks ==> pos = 15+8

     // ranges between 30-59
     for( size_t i=0; i<30; i=i+2, ++pos)
     {
       const float lx = result_value[58-i];
       const float ly = result_value[58-i+1];
       float bx = lx + 0.0562 ;
       float by = ly;
       //float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
       //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
       //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
       X[pos]= bx;
       Y[pos]= by;
     }

     pos = pos+8; // leave out again 8 blanks ==> pos = 15+8+15+8

     for( size_t i=0; i<30; i=i+2, ++pos)
     {
       const float lx = result_value[88-i];
       const float ly = result_value[88-i+1];
       float bx = lx*std::cos(theta) - ly*std::sin(theta) - 0.018;
       float by = lx*std::sin(theta) + ly*std::cos(theta) + 0.0889;
      // float dist = std::sqrt( std::pow(bx,2) + std::pow(by,2) );
       //float dist = std::sqrt( std::pow(lx,2) + std::pow(ly,2) );
       //std::cout << "got a distance at "<< pos << " with "  << dist << std::endl;
       X[pos]= bx;
       Y[pos]= by;
     }


   }
