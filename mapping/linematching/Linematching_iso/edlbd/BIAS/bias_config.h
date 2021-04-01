/* bias_config.h is automatically generated from bias_config.h.in by CMake build system */
/* ****************************** DO NOT EDIT the .h file ***************************** */

/* export the build configuration of this library
**
** cmakedefine lines are substituted by values of
** matching cmake variables.
**
** do NOT include .h files, here !!!
*/

#ifndef __BIAS_CONFIG_H___6294562947_
#define __BIAS_CONFIG_H___6294562947_

#ifdef DEBUG_BUILD
#  define COMPILE_DEBUG
#else /*DEBUG_BUILD*/
#  define COMPILE_NDEBUG
#endif /*DEBUG_BUILD*/

/* version info */
#define BIAS_VERSION_MAJOR  2
#define BIAS_VERSION_MINOR  8
#define BIAS_VERSION_PATCH  0

/* #undef BIAS_HAVE_CENTAURUS */

/* compiled with BLAS library available? */
//#define BIAS_HAVE_BLAS

/* compiled with LAPACK lib available? */
//#define BIAS_HAVE_LAPACK

/* compiled with DevIL/OpenIL available? */
/* #undef BIAS_HAVE_DEVIL */

/* compiled with dc1394 firewire available? */
/* #undef BIAS_HAVE_DC1394 */

/* using version 2 of libdc1394 id defined */
/* #undef BIAS_DC1394_V2 */

/* compiled with CMU firewire available? */
/* #undef BIAS_HAVE_1394CAMERACMU */

/* DirectShow Support */
/* #undef BIAS_HAVE_DSHOW */

/* compiled with internal DV decoder */
/* #undef BIAS_HAVE_DV */

/* compiled with fftw3 available? */
/* #undef BIAS_HAVE_FFTW3 */

/* compiled with gsl available? */
/* #undef BIAS_HAVE_GSL */

/* compiled with GL extension handler GLEW available? */
/* #undef BIAS_HAVE_GLEW */

/* compiled with glut (GL utils) usage? */
/* #undef BIAS_HAVE_GLUT */

/* compiled with ImageMagick++ available? */
/* #undef BIAS_HAVE_IMAGEMAGICKLIB */

/* compiled with libJpeg usage */
/*#define BIAS_HAVE_LIBJPEG */

/* compiled with OpenCV usage ? */
//#define BIAS_HAVE_OPENCV

/* compiled with OpenEXR usage ? */
/* #undef BIAS_HAVE_OPENEXR */

/* compiled with OpenGL usage? */
/* #undef BIAS_HAVE_OPENGL */

/* compiled with OpenGL usage? */
/* #undef BIAS_HAVE_OPENGL3 */

/* compiled with OpenMP usage? */
/* #undef BIAS_HAVE_OPENMP */

/* compiled with Pthread usage? */
/* #undef BIAS_HAVE_PTHREADS */

/* compiled with StereoI API usage (NVidia stereo rendering) */
/* #undef BIAS_HAVE_STEREOI */

/* compiled with TIFF usage (image io library)? */
/* #undef BIAS_HAVE_TIFF */

/* IDS uEye Camera support */
/* #undef BIAS_HAVE_UEYE */

/* Video4Linux Support */
#define BIAS_HAVE_V4L

/* compiled with Visual Leak Debugger usage? */
/* #undef BIAS_HAVE_VLD */

/* compiled with Visual Leak Debugger usage? */
/* #undef BIAS_HAVE_OPENSCENEGRAPH */

/* compiled with wxWidgets available? */
/* #undef BIAS_HAVE_WXWIDGETS */

/* compiled with XML2 usage? */
/* #undef BIAS_HAVE_XML2 */


/* compiled with PMD usage? */
/* #undef BIAS_HAVE_PMD_PMDTec */
#define BIAS_PMD_PMDTec_SOURCEPLUGIN ""
#define BIAS_PMD_PMDTec_PROCPLUGIN ""

/* compiled with PMDUSB usage? */
/* #undef BIAS_HAVE_PMD_PMDZess */

/* compiled with SwissRanger usage? */
/* #undef BIAS_HAVE_SWISSRANGER */

/* compiled with Kinect usage? */
/* #undef BIAS_HAVE_KINECT */

/* compiled with OpenNI usage? */
/* #undef BIAS_HAVE_OPENNI */

/* compiled with PanTilt Usage? */
/* #undef BIAS_HAVE_PANTILT */

/* compiled with Directed Perception PanTilt Headers and Code? */
/* #undef BIAS_HAVE_PANTILT_DP */

/* MATLAB interface found? */
#define BIAS_HAVE_MATLAB

/* umfpack found? */
/* #undef BIAS_HAVE_UMFPACK */

/*
** defines that represent a "build" setting that does not correspond 1:1 to lib
*/

/* compiled with fonts fot GL drawing available (not a lib!) ? */
/* #undef BIAS_HAVE_FONTGL */

/* VideoSource_DCAM was compiled (typically if one of dc1394 and 1394CAMERACMU is available) */
/* #undef BIAS_HAVE_DCAM */

/* BIAS/Gui was compiled (typically if either CV or wx available) */
//#define BIAS_HAVE_GUI

/* #undef BIAS_HAVE_FFMPEG */


/* enable extra warnings, e.g. missing uuids in image? */
/* #undef BIAS_EXTRA_WARN */

/* disable warning when UUID in image is invalid */
/* #undef NO_EXTRA_WARN */

/* define the path to the source dir to be able to load test files from source (for dart and Examples) */
#define BIAS_TESTS_DATA "/home/sbista/soft/3rd-party/bias/BIAS-bzr-2014-01-29/Tests/data/"

#define BIAS_IMG_DIR "/home/sbista/soft/3rd-party/bias/BIAS-bzr-2014-01-29/img/"

/* if defined an instance of BIAS::Image<T> (and dependant classed) ist explicitly created
** uchar und float instances are always available */
#define BUILD_IMAGE_UCHAR
#define BUILD_IMAGE_FLOAT
/* additonally instances are optionally compiled */
#define BUILD_IMAGE_INT
#define BUILD_IMAGE_CHAR
/* #undef BUILD_IMAGE_SHORT */
/* #undef BUILD_IMAGE_USHORT */
/* #undef BUILD_IMAGE_UINT */
/* #undef BUILD_IMAGE_DOUBLE */

/* #undef BUILD_DYNAMIC_LOAD_PROJECTION */

/*
** build/link specific options
*/

/* #undef BUILD_ALLOW_NON_INTEGRAL_MATRIX_ELEMENTS */

/* forced override of BIAS_DEBUG in WIN32 Release. Controls BIAS_DEBUG implicitly */
/* #undef BIAS_DEBUG_ONLY_IN_DEBUG */


#if defined(BIAS_DEBUG_ONLY_IN_DEBUG) && defined (WIN32)

/* set BIAS_DEBUG only in DEBUG-builds when BIAS_DEBUG-flag is set */
#  if defined(_DEBUG) && !defined(NDEBUG)
#define BIAS_DEBUG
#  endif

#else /* no override */

/* Linux or no override, BIAS_DEBUG is orthogonal to _DEBUG and NDEBUG
** compiled in debug mode? */
#define BIAS_DEBUG

#endif /* override? */


/* compiled libraries static or shared ?
** used to set declspec(import) in headers if required */
#define BIAS_BUILD_SHARED_LIBS

/* use precompiled headers? */
/* #undef BIAS_USE_PRECOMPILED_HEADERS */

/* new namespaces for ImageMagick? */
/* #undef BIAS_IMAGEMAGICKLIB_V2 */

/* required for building DLL on Win32 and using the header files after build
** This is an exception from the rule of not including anything here. */
#include "BIAS/BIAS_DeclSpec.hh"

/* prevent applications from loading vcomp[d].dll without using manifest-file
** --> caused by a bug in VS8 */
/*#if defined(BIAS_HAVE_OPENMP) && defined(WIN32)
#include <omp.h>
#endif*/

#endif /* __BIAS_CONFIG_H___6294562947_ */
