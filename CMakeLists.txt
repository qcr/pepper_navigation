# Top level cmake file
# Suman Raj Bista

cmake_minimum_required( VERSION 3.0.0)
project(PepperNavigation)

## Libary Paths
    set(CMAKE_PREFIX_PATH
           # /home/nao/my_workspace/ros_toolchain_install/share
           # /home/nao/my_workspace/ros_toolchain_install/share
            /home/suman/soft/third_party/opencv/install/lib/cmake/opencv4
            /home/suman/soft/third_party/naoqi/install/share/naoqi_libqi/cmake
            /home/suman/soft/third_party/naoqi/install/share/naoqi_libqicore/cmake

       )

    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTOUIC ON)
    set(CMAKE_INCLUDE_CURRENT_DIR ON)

    set (CMAKE_BUILD_TYPE  RelWithDebInfo )

    option(USE_BIAS_LIBRARY "Use BIAS library for line matching if not OpenCV version is used" ON)
    option(USE_SYSTEM_ARPAK_SUPERLU "Use system Arpac, lapack, blas and superlu" OFF)
    option(RUN_INSIDE_PEPPER "Run on board" OFF)

    if(RUN_INSIDE_PEPPER)
        set(USE_SYSTEM_ARPAK_SUPERLU 0)  
    endif()

    # Custom BIAS library
    if(USE_BIAS_LIBRARY)
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/BIAS_CMakeLists.txt)
    endif()


# for graph display
    if(NOT RUN_INSIDE_PEPPER)
        include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/libqgv_CMakeLists.txt)
    endif()


    set (CMAKE_CXX_STANDARD 11)

    include_directories ( ${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR})
    include_directories(".")

find_package(OpenCV REQUIRED)
find_package(naoqi_libqi REQUIRED)
find_package(naoqi_libqicore REQUIRED)
find_package(Boost  COMPONENTS filesystem system REQUIRED)

    include_directories(    SYSTEM
                            ${OpenCV_INCLUDE_DIRS}
                            ${naoqi_libqi_INCLUDE_DIRS}
                            ${naoqi_libqicore_INCLUDE_DIRS}
                            ${Boost_INCLUDE_DIRS}      )


## Look for Arpack and SuperlU library required for line mataching

## use system arpack, blas,superlu and lapack libaries


if(USE_SYSTEM_ARPAK_SUPERLU)

    FIND_LIBRARY(ARPACK_LIBRARIES NAMES arpack)
    if(NOT ARPACK_LIBRARIES)
        message(FATAL_ERROR "ARPACK library not found")
    endif()

    FIND_LIBRARY(SUPERLU_LIBRARIES NAMES superlu)
    if(NOT SUPERLU_LIBRARIES)
        message(FATAL_ERROR "SUPERLU library not found")
    endif()

    FIND_LIBRARY(BLAS_LIBRARIES NAMES blas)
    if(NOT BLAS_LIBRARIES)
        message(FATAL_ERROR "BLAS library not found")
    endif()

    FIND_LIBRARY(LAPACK_LIBRARIES NAMES lapack)
    if(NOT LAPACK_LIBRARIES)
        message(FATAL_ERROR "LAPACK library not found")
    endif()

    set(ARLPSLU_LIBRARIES       ${ARPACK_LIBRARIES}
                                ${SUPERLU_LIBRARIES}
                                ${BLAS_LIBRARIES}
                                ${LAPACK_LIBRARIES} )
else()

## use custom built from sources => Required if built to run inside the Pepper Robot

    if(EXISTS ${CMAKE_SOURCE_DIR}/linenav/arpack++/lib/libaplpsl_lin64.a)
  	set(ARLPSLU_LIBRARIES ${CMAKE_SOURCE_DIR}/linenav/arpack++/lib/libaplpsl_lin64.a)
    else()
        message("building custom arpack_superlu")
        add_custom_target(ARLPSLU_LIBRARIES ALL)
        add_custom_command(TARGET ARLPSLU_LIBRARIES COMMAND make all WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/linenav/arpack++ COMMENT "Building" USES_TERMINAL PRE_BUILD)
        set(ARLPSLU_LIBRARIES ${CMAKE_SOURCE_DIR}/linenav/arpack++/lib/libaplpsl_lin64.a)
    endif()

endif()


#find fortan libary (dependencies of ARPACK library)
string(REGEX MATCHALL "[0-9]+" GCC_VERSION_COMPONENTS ${CMAKE_CXX_COMPILER_VERSION})
list(GET GCC_VERSION_COMPONENTS 0 GCC_MAJOR)
    FIND_LIBRARY(FORTRAN_LIBRARIES NAMES gfortran HINTS /usr/lib/gcc/x86_64-linux-gnu/${GCC_MAJOR})
    

    include_directories(linenav/arpack++/include)


# Line Matching sources and headers

if(USE_BIAS_LIBRARY)

    set(BIAS_linematching_SRCS      linenav/edlbd/EDLineDetector.cpp
                                    linenav/edlbd/LineDescriptor.cpp
                                    linenav/edlbd/PairwiseLineMatching.cpp  )

    set(BIAS_linematching_HDRS      linenav/edlbd/EDLineDetector.hh
                                    linenav/edlbd/LineDescriptor.hh
                                    linenav/edlbd/LineStructure.hh
                                    linenav/edlbd/PairwiseLineMatching.hh    )

    set(LineMatching_HDRS       ${BIAS_HEADERS}
                                ${BIAS_linematching_HDRS}    )

    set(LineMatching_SRCS       ${BIAS_SOURCES}
                                ${BIAS_linematching_SRCS}    )

else()

    set(LineMatching_HDRS               linenav/EDLineDetector.cpp
                                        linenav/LineDescriptor.cpp
                                        linenav/PairwiseLineMatching.cpp     )

    set(LineMatching_SRCS               linenav/EDLineDetector.hh
                                        linenav/LineDescriptor.hh
                                        linenav/LineStructure.hh
                                        linenav/PairwiseLineMatching.hh      )

 endif()


## NAOqi interface taken from visp_naoqi library

    set(ocv_naoqi_hdrs      pepper_qi/include/vpNaoqiGrabber.h
                            pepper_qi/include/vpNaoqiRobot.h
                            pepper_qi/include/vpControl.hpp      )

    set(pepper_qi_HDRS      ${ocv_naoqi_hdrs}
                            pepper_qi/include/al/alvisiondefinitions.h
                            pepper_qi/include/al/naoqi_image.hpp     )

    set(pepper_qi_SRCS      pepper_qi/src/vpNaoqiGrabber.cpp
                            pepper_qi/src/vpNaoqiRobot.cpp
                            pepper_qi/include/al/from_any_value.cpp
                            pepper_qi/src/vpControl.cpp          )


    include_directories(pepper_qi/include)

    set(HEADERS     ${LineMatching_HDRS}
                    navmain/navigation.h
                    linenav/linenavigation.h
                    linenav/linematch.h
                    linenav/dispnav.h
                    linenav/kimread.h
                    depthnav/DepthImageScanner.h
                    depthnav/alpose2d.h
                    depthnav/freespacenavigation.h       )


    set(SOURCES     ${LineMatching_SRCS}
                    navmain/navigation.cpp
                    linenav/linenavigation.cpp
                    linenav/linematch.cpp
                    linenav/dispnav.cpp
                    linenav/kimread.cpp
                    depthnav/DepthImageScanner.cpp
                    depthnav/alpose2d.cpp
                    depthnav/freespacenavigation.cpp     )


     set(SRCS1   navmain/nav_pepperonline.cpp   )
     set(SRCS2   navmain/nav_pepperoffline.cpp   )

if(NOT RUN_INSIDE_PEPPER)
    set(navgui_SRCS     navmain/maingui/navwindow.ui
                        navmain/maingui/navwindow.cpp
                        navmain/maingui/navwindow.h
                        navmain/maingui/window_QT.h
                        navmain/maingui/window_QT.cpp
                        navmain/maingui/window_QT.qrc
                        navmain/topograph_astar.h
                        navmain/topograph_processor.h
                        navmain/nav_peppergui.cpp )
endif()

    set(navinside_SRCS
			            navmain/maingui/navinside.cpp
                        navmain/maingui/navinside.h
                        navmain/topograph_astar.h
                        navmain/topograph_processor.h
                        navmain/nav_pepper_inside.cpp)



    set(Interface_SRCS
                        navmain/pepperRobot.h
                        navmain/pepperRobot.cpp
                        navmain/peppernavigation.h
                        navmain/pepperInterface.h
                        navmain/pepperOffline.h
                        navmain/pepperOffline.cpp
                        navmain/pepperInterface.cpp
                        navmain/pepperevents.h      )



   include_directories(navmain)

add_library(ocv_naoqi SHARED  ${pepper_qi_SRCS}  ${pepper_qi_HDRS} )
target_link_libraries(ocv_naoqi ${OpenCV_LIBRARIES} ${naoqi_libqi_LIBRARIES} ${naoqi_libqicore_LIBRARIES} ${Boost_LIBRARIES} )


set(extraLIBS ${ARLPSLU_LIBRARIES} ${FORTRAN_LIBRARIES} ocv_naoqi)
message("${extraLIBS}")

add_executable (peppernavigation ${SRCS1} ${SOURCES} ${HEADERS} )
target_link_libraries (peppernavigation ${extraLIBS} ${naoqi_libqicore_LIBRARIES} ${naoqi_libqi_LIBRARIES}  ${OpenCV_LIBS})


add_executable (peppernavigationoff ${SRCS2} ${SOURCES} ${HEADERS} )
target_link_libraries (peppernavigationoff ${extraLIBS} ${naoqi_libqicore_LIBRARIES} ${naoqi_libqi_LIBRARIES} ${OpenCV_LIBS})

if(NOT RUN_INSIDE_PEPPER) 
    add_executable (peppernav_gui ${Interface_SRCS} ${navgui_SRCS} ${SOURCES} ${HEADERS})
    target_link_libraries (peppernav_gui ${qgv_LIBS} ${extraLIBS} ${naoqi_libqicore_LIBRARIES} ${naoqi_libqi_LIBRARIES}  ${OpenCV_LIBS})
endif()

add_executable (peppernav_inside ${Interface_SRCS} ${navinside_SRCS} ${SOURCES} ${HEADERS})
target_link_libraries (peppernav_inside  ${extraLIBS} ${naoqi_libqicore_LIBRARIES} ${naoqi_libqi_LIBRARIES}  ${OpenCV_LIBS})
