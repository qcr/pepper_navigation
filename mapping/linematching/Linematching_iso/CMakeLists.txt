
cmake_minimum_required( VERSION 3.0.0)

project(linematch)

#set(CMAKE_PREFIX_PATH /home/suman/soft/thirdparty/OpenCV/install/lib/cmake/opencv4)


set(CMAKE_INCLUDE_CURRENT_DIR ON)

set ( CMAKE_BUILD_TYPE  RelWithDebInfo )


option(USE_BIAS_LIBRARY "Use BIAS library for line matching if not OpenCV version is used" ON)
option(BUILD_CUSTOM_ARPACK_SUPERLU_LIBS "Build ARPACK, BLAS, SUPERLU, LAPCK" OFF)

FIND_LIBRARY(ARPACK_LIBRARIES NAMES arpack)
    
FIND_LIBRARY(SUPERLU_LIBRARIES NAMES superlu)
 
FIND_LIBRARY(BLAS_LIBRARIES NAMES blas)

FIND_LIBRARY(LAPACK_LIBRARIES NAMES lapack)


if(USE_BIAS_LIBRARY)

    include(${CMAKE_CURRENT_SOURCE_DIR}/BIAS_CMakeLists.txt)
    set(BIAS_linematching_SRCS      edlbd/EDLineDetector.cpp
                                    edlbd/LineDescriptor.cpp
                                    edlbd/PairwiseLineMatching.cpp  )

    set(BIAS_linematching_HDRS      edlbd/EDLineDetector.hh
                                    edlbd/LineDescriptor.hh
                                    edlbd/LineStructure.hh
                                    edlbd/PairwiseLineMatching.hh    )

    set(LineMatching_HDRS       ${BIAS_HEADERS}
                                ${BIAS_linematching_HDRS}    )

    set(LineMatching_SRCS       ${BIAS_SOURCES}
                                ${BIAS_linematching_SRCS}    )

    if(BUILD_CUSTOM_ARPACK_SUPERLU_LIBS)
        add_custom_target(ARLPSLU_LIBRARIES ALL)
        add_custom_command(TARGET ARLPSLU_LIBRARIES COMMAND make all WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/arpack++ COMMENT "Building" USES_TERMINAL PRE_BUILD)
        set(ARLPSLU_LIBRARIES ${CMAKE_SOURCE_DIR}/arpack++/lib/libaplpsl_lin64.a)
    else()
        set(ARLPSLU_LIBRARIES
        ${ARPACK_LIBRARIES}
        ${SUPERLU_LIBRARIES}
        ${BLAS_LIBRARIES}
        ${LAPACK_LIBRARIES}
        )  
    endif()       

else()

    set(LineMatching_HDRS               EDLineDetector.cpp
                                        LineDescriptor.cpp
                                        PairwiseLineMatching.cpp     )

    set(LineMatching_SRCS               EDLineDetector.hh
                                        LineDescriptor.hh
                                        LineStructure.hh
                                        PairwiseLineMatching.hh      )
   
    set(ARLPSLU_LIBRARIES
      ${ARPACK_LIBRARIES}
      ${SUPERLU_LIBRARIES}
      ${BLAS_LIBRARIES}
      ${LAPACK_LIBRARIES}
    )                                  

 endif()

FIND_LIBRARY(FORTRAN_LIBRARIES NAMES gfortran HINTS /usr/lib/gcc/x86_64-linux-gnu/7)
include_directories(arpack++/include)

set (CMAKE_CXX_STANDARD 11)


find_package(OpenCV REQUIRED)



include_directories(SYSTEM
    ${OpenCV_INCLUDE_DIRS}
    
)

set(SRCS1
detectlines.cpp
)



set(SRCS
linematchfil.cpp
)

set(extraLIBS
${ARLPSLU_LIBRARIES}
${FORTRAN_LIBRARIES}
)

message("${extraLIBS}")

set(EXECUTABLE_OUTPUT_PATH "../../")
add_executable (detectlinesED ${SRCS1}  ${LineMatching_SRCS} ${LineMatching_HDRS} )
target_link_libraries (detectlinesED ${extraLIBS} ${OpenCV_LIBS}) 


add_executable (matchlines ${SRCS}  ${LineMatching_SRCS} ${LineMatching_HDRS} )
target_link_libraries (matchlines ${extraLIBS} ${OpenCV_LIBS})


