# navmain

Top-level interface for the navigation as discussed in  
Bista SR, Ward B, Corke P. Image-Based Indoor Topological Navigation with Collision Avoidance for Resource-Constrained Mobile Robots. Journal of Intelligent & Robotic Systems. 2021.  

## Dependencies
* [OpenCV](https://opencv.org/),
* [Boost](https://www.boost.org/),
* [naoqi_libqi](http://wiki.ros.org/naoqi_libqi), [naoqi_libqicore](http://wiki.ros.org/naoqi_libqicore) and [pepper_qi](ttps://github.com/qcr/pepper_navigation/tree/main/pepper_qi) (Please refer to [*../pepper_qi*](https://github.com/qcr/pepper_navigation/tree/main/pepper_qi) folder for details),
* [Qt5](https://doc.qt.io/qt-5/index.html), [qgv](https://github.com/qcr/pepper_navigation/tree/main/qgv) and [Graphviz](https://graphviz.org/) (Please refer to [*../qgv*](https://github.com/qcr/pepper_navigation/tree/main/qgv) folder for details).

## Usage
### Libraries
1. ***navigation*** ([*navigation.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/navigation.h), [*navigation.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/navigation.cpp)) => Top-level interface for image-based navigation using line segments ([*../linenav*](https://github.com/qcr/pepper_navigation/tree/main/linenav)). 

2. ***peppernavigation*** ([*peppernavigation.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/peppernavigation.h)) => Top-level Interface for the Pepper robot navigation. Top-level functions for [free-space navigation](https://github.com/qcr/pepper_navigation/tree/main/depthnav) and [image-based navigation](https://github.com/qcr/pepper_navigation/tree/main/linenav). 

3. ***pepperInterface*** ([*pepperInterface.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperInterface.h), [*pepperInterface.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperInterface.cpp)) => Virtual Class with top-level virtual functions for interface with the Pepper robot or just image sequences.
    - ***pepperRobot*** ([*pepperRobot.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperRobot.h), [*pepperRobot.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperRobot.cpp))  => Derived Class to interface with the Pepper robot.  
    
    - ***pepperRobotVirtual*** ([*pepperRobotVirtual.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperRobotVirtual.h), [*pepperRobotVirtual.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperRobotVirtual.cpp)) => Derived Class to test ***pepperRobot*** functionalities via Virtual Pepper where images are read from the folder.  
    - ***pepperOffline*** ([*pepperOffline.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperOffline.h), [*pepperOffline.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperOffline.cpp)) => Derived Class to interface in offline mode i.e. read images from the folder and perform image-based localisation only (Pepper robot not used).  

7. ***topmapprocessor*** ([*topograph_processor.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/topograph_processor.h)) => Top-level interface for processing topological map (reads topological graph and reference images from disk for navigation).
8. ***astar*** ([*topograph_astar.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/topograph_astar.h)) => Performs A* search in the graph. Modified code from [Boost Graph Library example : astar-cities](https://www.boost.org/doc/libs/1_54_0/libs/graph/example/astar-cities.cpp).

9. ***pepperServices*** ([*pepperevents.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/pepperevents.h)) => Subscribe to the Pepper robot's internal events related to move and collision.

### Executables
1. ***peppernav_gui*** ([*nav_peppergui.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/nav_peppergui.cpp)) => Topological navigation of the Pepper robot with GUI. The code has to be executed on the external PC connecting to the Pepper robot remotely.  

2. ***peppernav_inside*** ([*nav_pepper_inside.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/nav_pepper_inside.cpp)) => Topological navigation of the Pepper robot. This code is capable to run onboard on the Pepper robot.  

3. ***peppernavigation*** ([*nav_pepperonline.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/nav_pepperonline.cpp)) => Image-based navigation of the Pepper robot along a single sequence of reference images.   

5. ***peppernavigationoff*** ([*nav_pepperoffline.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/nav_pepperoffline.cpp)) => Image-based localisation along the reference image list. This is the offline mode that uses image sequence and does not require the robot.  

    **Note: The executables *peppernav_inside* and  *peppernavigation* are capable of running onboard on the Pepper robot provided that they are compiled in the [Pepper Virtual Machine](https://bitbucket.org/pepper_qut/virtual-machine.git) [1]. For details, please refer to the [Reference Manual](https://github.com/qcr/pepper_navigation/tree/main/docs/Pepper_Navigation_Reference_Manual.pdf) in [*../docs*](https://github.com/qcr/pepper_navigation/tree/main/docs) folder.**  
  
[1] G. Suddrey, A. Jacobson and B. Ward. "Enabling a pepper robot to provide automated and interactive tours of a robotics laboratory." arXiv preprint [arXiv:1804.03288](https://arxiv.org/abs/1804.03288) (2018). [ACRA 2018 Proceedings](https://ssl.linklings.net/conferences/acra/acra2018_proceedings/views/includes/files/pap109s1-file1.pdf). https://bitbucket.org/pepper_qut/virtual-machine.git.