## navmain/maingui

* Interface for Topological Navigation.  
* The GUI version of topological navigation runs on a remote PC. The code communicates with the Pepper robot remotely via the [naoqi](http://doc.aldebaran.com/2-5/naoqi/index.html) interface.  
* The Non-GUI version of topological navigation is capable of running onboard on the Pepper robot if the code is compiled in the [Pepper Virtual Machine](https://bitbucket.org/pepper_qut/virtual-machine.git).

#### Topological Navigation 

***navwindow*** ([*navwindow.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/maingui/mainwindow.h), *[navwindow.cpp](https://github.com/qcr/pepper_navigation/blob/main/navmain/maingui/navwindow.cpp)*, [*navwindow.ui*](https://github.com/qcr/pepper_navigation/blob/main/navmain/maingui/navwindow.ui)) => Navigation with GUI control. Requires Qt5 and qgv libaries.  
***navinside*** ([*navinside.h*](https://github.com/qcr/pepper_navigation/blob/main/navmain/maingui/navinside.h), [*navinside.cpp*](https://github.com/qcr/pepper_navigation/blob/main/navmain/maingui/navinside.cpp)) => Navigation without GUI. This version can be used to run navigation onboard on the Pepper robot.  
Please refer to [*../../CMakeLists.txt*](https://github.com/qcr/pepper_navigation/blob/main/CMakeLists.txt).

#### window_QT and files_Qt
Uses OpenCV for displaying the image in QT UI.  
Code taken from [OpenCV highgui module repository](https://github.com/opencv/opencv/tree/master/modules/highgui/src).

#### Usage
Please refer to the [Reference Manual](https://github.com/qcr/pepper_navigation/tree/main/docs/Pepper_Navigation_Reference_Manual.pdf) in [*../docs*](https://github.com/qcr/pepper_navigation/tree/main/docs) folder.