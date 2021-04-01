#This source code has been taken and modified for Pepper Robot
#The original code are available from
# https://github.com/lagadic/visp_naoqi/tree/libqi
# https://github.com/lagadic/pepper_control/tree/libqi

cmake_minimum_required(VERSION 2.6.4 FATAL_ERROR)

project(naoqi_ocv)

# By default set release configuration

set(CMAKE_BUILD_TYPE "Release" CACHE String "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel." FORCE)






set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")

find_package(OpenCV REQUIRED)
find_package(Boost  COMPONENTS filesystem system REQUIRED)
find_package(naoqi_libqi)
find_package(naoqi_libqicore)




#include_directories("include")
include_directories(
        "${PROJECT_SOURCE_DIR}/include"
)



set(CMAKE_LIBRARY_OUTPUT_DIRECTORY 
        ${PROJECT_SOURCE_DIR}/lib
)

set( EXECUTABLE_OUTPUT_PATH  ${PROJECT_SOURCE_DIR}/bin )

include_directories(
  ${OpenCV_INCLUDE_DIRS}
  ${naoqi_libqi_INCLUDE_DIRS}
  ${naoqi_libqicore_INCLUDE_DIRS}
  ${Boost_INCLUDE_DIRS}
)

set(SOURCES
  src/vpNaoqiGrabber.cpp
  src/vpNaoqiRobot.cpp
  include/al/from_any_value.cpp
)

set(HEADERS
  include/vpNaoqiGrabber.h
  include/vpNaoqiRobot.h
)
set(HEADERS_AL
  include/al/alvisiondefinitions.h
  include/al/naoqi_image.hpp
)

add_library(ocv_naoqi SHARED
  ${SOURCES}
  ${HEADERS}
  ${HEADERS_AL}
)

target_link_libraries(ocv_naoqi
    ${OpenCV_LIBRARIES}
    ${naoqi_libqi_LIBRARIES}
    ${naoqi_libqicore_LIBRARIES}
    ${Boost_LIBRARIES}
    )

set(Pepper_control_HDRS
    include/vpControl.hpp
)

set(Pepper_control_SRCS
    src/vpControl.cpp
    apps/main.cpp
)

add_executable(pepper_control ${Pepper_control_SRCS} ${Pepper_control_HDRS} )
target_link_libraries(pepper_control
  ${naoqi_libqi_LIBRARIES}
  ${naoqi_libqicore_LIBRARIES}
  ${Boost_LIBRARIES}
)





