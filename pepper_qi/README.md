# pepper_qi

## Interface with qi library

**This source code has been taken and modified for the Pepper robot**  
  
Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots.  

**The original code is available from**  
  
* [https://github.com/lagadic/visp_naoqi/tree/libqi](https://github.com/lagadic/visp_naoqi/tree/libqi)
* [https://github.com/lagadic/pepper_control/tree/libqi](https://github.com/lagadic/pepper_control/tree/libqi)  

**Reference of the original code**  
  
E. Marchand, F. Spindler, F. Chaumette. ViSP for visual servoing: a generic software platform with a wide class of robot control skills. IEEE Robotics and Automation Magazine, Special Issue on “Software Packages for Vision-Based Control of Motion”, P. Oh, D. Burschka (Eds.), 12(4):40-52, December 2005. [inria-00351899](https://hal.archives-ouvertes.fr/inria-00351899). https://visp.inria.fr/
  
**This modified code supports image acquistion from depth camera.**

##  Dependencies
* [OpenCV](https://opencv.org/), 
* [Boost](https://www.boost.org/),  
* [naoqi_libqi](http://wiki.ros.org/naoqi_libqi) (ROS library),
* [naoqi_libqicore](http://wiki.ros.org/naoqi_libqicore)  (ROS library).

**Note:**  
  
  * The thirdparty folder consists of source code of naoqi_libqi and naoqi_libqicore that has been modified to work with Boost version 1.65.1
  * The latest code of libqi and qicore can be obtained from  
      [https://github.com/aldebaran/libqi](https://github.com/aldebaran/libqi)   
      [https://github.com/aldebaran/libqicore](https://github.com/aldebaran/libqicore)  


