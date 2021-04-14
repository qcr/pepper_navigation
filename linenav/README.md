## linenav

### Image-based navigation using line segments
[https://github.com/suuman/line_navigation_offline.git](https://github.com/suuman/line_navigation_offline.git)

Bista SR, Giordano PR, Chaumette F. Appearance-based indoor navigation by IBVS using line segments. IEEE Robotics and Automation Letters. 2016 Jan 26;1(1):423-30.

## Dependencies
1. [Line matching code](http://www.mip.informatik.uni-kiel.de/tiki-download file.php?fileId=1965) is based on http://www.mip.informatik.uni-kiel.de/tiki-download file.php?fileId=1965 (offline now but availabe in OpenCV)
2. [BIAS library](http://www.mip.informatik.uni-kiel.de/tiki-index.php?page=BIAS) is based on http://www.mip.informatik.uni-kiel.de/tiki-index.php?page=BIAS (offline now)
3. [ARPACK++](http://www.ime.unicamp.br/~chico/arpack++/),
4. [ARPACK](https://www.caam.rice.edu/software/ARPACK/),
5. [BLAS](http://www.netlib.org/blas/),
6. [LAPACK](http://www.netlib.org/lapack/),
7. [SuperLU](https://portal.nersc.gov/project/sparse/superlu/).

## Usage
1. To use the original line detection and matching code based on the legacy BIAS library, use the codes inside the [*edlbd/*](https://github.com/suuman/pepper_navigation/tree/main/linenav/edlbd) folder.  
If you do not want to use the legacy BIAS library, the line detection and matching based on OpenCV is used.  
Please refer to [*../CMakeLists.txt*](https://github.com/suuman/pepper_navigation/blob/main/CMakeLists.txt).

2. To use linenavigation as a library, please refer to [*linenavigation.h*](https://github.com/suuman/pepper_navigation/blob/main/linenav/linenavigation.h) and [*linenavigation.cpp*
](https://github.com/suuman/pepper_navigation/blob/main/linenav/linenavigation.cpp).
3. To display the image-based localisation, please refer to [*dispnav.h*](https://github.com/suuman/pepper_navigation/blob/main/linenav/dispnav.h) and [*dispnav.cpp*](https://github.com/suuman/pepper_navigation/blob/main/linenav/dispnav.cpp).

4. For the usage of *[linenavigation](https://github.com/suuman/pepper_navigation/blob/main/linenav/linenavigation.h)* and *[dispnav](https://github.com/suuman/pepper_navigation/blob/main/linenav/dispnav.h)*, please refer to [*navigation.h*](https://github.com/suuman/pepper_navigation/blob/main/navmain/navigation.h) and [*navigation.cpp*](https://github.com/suuman/pepper_navigation/blob/main/navmain/navigation.cpp) in [*../navmain/*](https://github.com/suuman/pepper_navigation/tree/main/navmain) directory.