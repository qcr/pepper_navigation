## depthnav

### Free space navigation using depth image
Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots.   

## Dependencies
* OpenCV,
* Boost,
* naoqi_libqi, naoqi_libqicore and pepper_qi (Please refer to [../pepper_qi](https://github.com/suuman/pepper_navigation/tree/main/pepper_qi) folder).

## Usage
### Standalone usage 
Refer [*depthnav_Pepper.cpp*](https://github.com/suuman/pepper_navigation/blob/main/depthnav/depthnav_Pepper.cpp)  
  
`$ mkdir build $$ cd build  `  
`$ cmake .. && make -j8  `  
`$ ./pepper_fsnav --ip < Pepper_IP >`  

### Use as library
Refer to class *freespacenavigation*

* defined in *[freespacenavigation.h](https://github.com/suuman/pepper_navigation/blob/main/depthnav/freespacenavigation.h)* and 
* described in *[freespacenavigation.cpp](https://github.com/suuman/pepper_navigation/blob/main/depthnav/freespacenavigation.cpp)*.



