# Pepper_navigation

This repository contains the source code accompanying our papper Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots as
discussed in [paper_link].

The code is open source. The codes are provided “as-is” without any warranty. Before using the code, you agree to use the code at
your own risk. The authors are not responsible or liable for any damages incurred using this code.


# Build Instructions

### 1. Get Source codes from the repository 
   ```
   $ git clone https://github.com/suuman/pepper_navigation.git
   ```
	
### 2. Build executables required for mapping.
 
  $ cd pepper_navigation/mapping  
  $ ./build_linematching.sh

  The above command will build executables detectinesED and matchlines to ./linematching folder and is equivalent to the following commands
	
   $ cd linematching/Linematching_iso && mkdir build $$ cd build  
   $ cmake .. && make -j8  
	
### 3. Build Navigation code

   $ cd pepper_navigation 
   $ ./compile.sh

   The above command will build executables in the build directory and is equivalent to the following commands  
   $ mkdir build $$ cd build  
   $ cmake .. && make -j8
	
   If the CMake options and Path has to changed from default  
	$ ccmake .. or cmake-gui ..  
	Configure and Generate Makefile.  
    $ make -j8
  
  Following executables will be built  
  a) peppernav_gui => Toplogical navigation of the Pepper robot with the code running at external PC.  
  b) peppernav_inside => Topological naviagtion of the Pepper robt with the code running onborad on the robot. For this the code should be compiled in the Pepper vitual machine availabe from [this link]( https://bitbucket.org/pepper_qut/virtual-machine.git).  
  c) peppernavigation => Naviagtion of Pepper along the single reference image list.  
  d) peppernavigationoff => Offine mode using image sequence (localisation only).

### 4. Run Code 
      
 
Example Scripts run.sh, run_inside.sh, run_test.sh    
    The navigation executables requires two areguemnts  
        a) Path of toplogical map (e.g. ../data/tmap)  
        b) IP address of the robot (e.g. 172.19.226.236 to run online with robot) or path of image sequence (e.g. ../data/offlinetest) to run offile with image sequence (localisation mode only). 


For details, please refer to docs/TechnicalReport.pdf (available soon !!) and README.MD at subfolders of this repository

### 5. References
Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots.   
Bista SR, Giordano PR, Chaumette F. Appearance-based indoor navigation by IBVS using line segments. IEEE Robotics and Automation Letters. 2016 Jan 26;1(1):423-30.

### More details will available soon !!


