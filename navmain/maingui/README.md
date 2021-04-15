## navmain/maingui

Interface for Topological Navigation.  
GUI version of topological navigation runs on a remote PC.  
The code communicates with the Peper Robot remotely via the naoqi interface.

### Navigation Pipeline

***navwindow*** ([*navwindow.h*](https://github.com/suuman/pepper_navigation/blob/main/navmain/maingui/mainwindow.h), *[navwindow.cpp](https://github.com/suuman/pepper_navigation/blob/main/navmain/maingui/navwindow.cpp)*, [*navwindow.ui*](https://github.com/suuman/pepper_navigation/blob/main/navmain/maingui/navwindow.ui)) => Navigation with GUI control. Requires Qt5 and qgv libaries.  
***navinside*** ([*navinside.h*](https://github.com/suuman/pepper_navigation/blob/main/navmain/maingui/navinside.h), [*navinside.cpp*](https://github.com/suuman/pepper_navigation/blob/main/navmain/maingui/navinside.cpp)) => Navigation without GUI. Used to run navigation onboard on the Pepper Robot.  
Please refer to [*../../CMakeLists.txt*](https://github.com/suuman/pepper_navigation/blob/main/CMakeLists.txt).

### window_QT and files_Qt
Uses OpenCV for displaying the image in QT UI.  
Code taken from [OpenCV highgui module repository](https://github.com/opencv/opencv/tree/master/modules/highgui/src).

### Usage
please refer to < report > in [*../docs*](https://github.com/suuman/pepper_navigation/tree/main/docs) folder.