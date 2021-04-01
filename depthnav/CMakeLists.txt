#Free Space Naviagtion
# Suman Raj Bista

cmake_minimum_required(VERSION 3.0)
project(pepper_fsnav)
set(CMAKE_PREFIX_PATH

         /home/suman/soft/thirdparty/OpenCV/install_lat/lib/cmake/opencv4
        /opt/ros/kinetic/share/naoqi_libqi/cmake
        /opt/ros/kinetic/share/naoqi_libqicore
   )
#set(CMAKE_CXX_STANDARD 14)
set(CMAKE_BUILD_TYPE "Relwithdebinfo")

find_package(OpenCV 4 REQUIRED)
find_package(naoqi_libqi)
find_package(naoqi_libqicore)
find_package(Boost  COMPONENTS filesystem system REQUIRED)

include_directories(".")

include_directories(SYSTEM
   ${OpenCV_INCLUDE_DIRS}
  ${naoqi_libqi_INCLUDE_DIRS}
  ${naoqi_libqicore_INCLUDE_DIRS}
   ${Boost_INCLUDE_DIRS}
)

set(ocv_naoqi_hdrs
  ../pepper_qi/include/vpNaoqiGrabber.h
  ../pepper_qi/include/vpNaoqiRobot.h
  ../pepper_qi/include/vpControl.hpp
)

set(pepper_qi_HDRS
  ${ocv_naoqi_hdrs}
  ../pepper_qi/include/al/alvisiondefinitions.h
  ../pepper_qi/include/al/naoqi_image.hpp

)

set(pepper_qi_SRCS
  ../pepper_qi/src/vpNaoqiGrabber.cpp
  ../pepper_qi/src/vpNaoqiRobot.cpp
  ../pepper_qi/include/al/from_any_value.cpp
  ../pepper_qi/src/vpControl.cpp
)

include_directories(../pepper_qi/include)

add_library(ocv_naoqi SHARED
  ${pepper_qi_SRCS}
  ${pepper_qi_HDRS}
)
target_link_libraries(ocv_naoqi
    ${OpenCV_LIBRARIES}
    ${naoqi_libqi_LIBRARIES}
    ${naoqi_libqicore_LIBRARIES}
    ${Boost_LIBRARIES}
    )

#set (HEADERS pepperlaser.h)
set (HEADERS DepthImageScanner.cpp /
           DepthImageScanner.h /
           alpose2d.h /
           alpose2d.cpp /
           freespacenavigation.h /
           freespacenavigation.cpp



                            )

add_executable(pepper_fsnav pepperevents.cpp ${HEADERS})
target_link_libraries(pepper_fsnav

  ocv_naoqi
  ${naoqi_libqicore_LIBRARIES}
  ${naoqi_libqi_LIBRARIES}
  ${OpenCV_LIBS}
)
