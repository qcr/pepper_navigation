## Pepper_navigation

This repository contains the source code accompanying our paper Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots as
discussed in [paper_link].  

The code is open source. The codes are provided “as-is” without any warranty. Before using the code, you agree to use the code at
your own risk. The authors are not responsible or liable for any damages incurred using this code.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## Dependencies
1. [MATLAB](https://au.mathworks.com/products/matlab.html) (for mapping),
2. [OpenCV](https://opencv.org/),
3. [Boost](https://www.boost.org/),
4. [naoqi_libqi](http://wiki.ros.org/naoqi_libqi) and [naoqi_libqicore](http://wiki.ros.org/naoqi_libqicore) (ROS Package),
5. [ARPACK++](http://www.ime.unicamp.br/~chico/arpack++/), [ARPACK](https://www.caam.rice.edu/software/ARPACK/), [BLAS](http://www.netlib.org/blas/), [LAPACK](http://www.netlib.org/lapack/), and [SuperLU](https://portal.nersc.gov/project/sparse/superlu/) (for line segment matching),
6. [Qt5](https://doc.qt.io/qt-5/index.html) ([Core](https://doc.qt.io/qt-5/qtcore-index.html), [Widgets](https://doc.qt.io/qt-5/qtwidgets-index.html), [Test](https://doc.qt.io/qt-5/qttest-index.html)) (for GUI),
7. [Graphviz](https://graphviz.org/) ([GVC](https://graphviz.org/pdf/gvc.3.pdf), [CGRAPH](https://graphviz.org/pdf/cgraph.3.pdf), [CDT](https://graphviz.org/pdf/cdt.3.pdf))(for visualising topological graph),
8. [Pepper Virtual Machine](https://bitbucket.org/pepper_qut/virtual-machine.git) and [VirtualBox](https://www.virtualbox.org) (to compile binaries that can run onboard on the Pepper Robot),
9. [qgv](https://github.com/suuman/pepper_navigation/tree/main/qgv) and [pepper_qi](https://github.com/suuman/pepper_navigation/tree/main/pepper_qi) (custom libraries that are shipped with this repository).
----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Build Instructions

### 1. Get Source codes from the repository. 
   `$ git clone https://github.com/suuman/pepper_navigation.git`

### 2. Build executables required for mapping.
  `$ cd pepper_navigation/mapping`  
  `$ ./build_linematching.sh`

  The above command will build executables ***detectinesED*** and ***matchlines*** to *./linematching* folder and is equivalent to the following commands  
   `$ cd linematching/Linematching_iso && mkdir build $$ cd build`  
  ` $ cmake .. && make -j8`
### 3. Build Navigation code
`$ cd pepper_navigation`  
`$ ./compile.sh`  

   The above command will build executables in the *build/* directory and is equivalent to the following commands  
   `$ mkdir build $$ cd build`  
   `$ cmake .. && make -j8`  

   If the CMake options and Path has to be changed from the default  
	`$ ccmake .. or cmake-gui ..`   
  Configure and Generate Makefile.  
    `$ make -j8`
  
_______________________________________________________________________________________________________________________________________________________________________________________________________

  Following executables will be built  
 
1. ***peppernav_gui*** => GUI version of the topological navigation of the Pepper robot. It has to be executed on the external PC connecting to the Pepper Robot remotely.  

2. ***peppernav_inside*** => Topological navigation of the Pepper robot. It is capable of running onboard on the Pepper robot.  

3. ***peppernavigation*** => Navigation of the Pepper robot along with the single reference image list. It is capable of running onboard on Pepper robot.  

4. ***peppernavigationoff*** => Offline localisation mode from the image sequence. It does not require a robot (Images read from the folder).  

***Note:*** To run the executables ***peppernav_inside*** and ***peppernavigation*** onboard on the Pepper robot, they must be compiled in the [Pepper Virtual Machine](https://bitbucket.org/pepper_qut/virtual-machine.git). For details, please refer to < report > in [*../docs*](https://github.com/suuman/pepper_navigation/tree/main/docs) folder.

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

## Run Code 
***Example Scripts:*** [*run_onboard.sh*](https://github.com/suuman/pepper_navigation/blob/main/run_onboard.sh), [*run_remotePC.sh*](https://github.com/suuman/pepper_navigation/blob/main/run_remotePC.sh), and [*run_offline.sh*](https://github.com/suuman/pepper_navigation/blob/main/run_offline.sh).    

The navigation executables require two arguments:  
&nbsp;&nbsp;&nbsp;&nbsp; a. &nbsp; Path of topological map (e.g. `../data/tmap`), and   
&nbsp;&nbsp;&nbsp;&nbsp; b. &nbsp; IP address of the robot (e.g. `172.19.226.236` to run online with robot) or path of the image sequence (e.g. `../data/offlinetest`) to run offline with image sequence (localisation mode only). 


**For details, please refer to docs/TechnicalReport.pdf (available soon !!) and README.MD at subfolders of this repository.**

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
## References
[1] Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots.   
[2] Bista SR, Giordano PR, Chaumette F. Appearance-based indoor navigation by IBVS using line segments. IEEE Robotics and Automation Letters. 2016 Jan 26;1(1):423-30.

###
