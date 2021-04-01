/*
This file is part of the BIAS library (Basic ImageAlgorithmS).

Copyright (C) 2003, 2004    (see file CONTACTS for details)
  Multimediale Systeme der Informationsverarbeitung
  Institut fuer Informatik
  Christian-Albrechts-Universitaet Kiel


BIAS is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

BIAS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with BIAS; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

/*
/// defines the BIAS API
/// required for building/using a WIN32 DLL
///
/// Is edited by hand.
/// Add one unique define for each additinal lib.
///
/// case 1: Linux. expand to nothing, no difference betwen static (.a) and shared (.so) build
///
/// case 2: Windows:
/// case 2.1: On compilation:
/// case 2.1.1: static : expand to nothing
/// case 2.1.2: shared : expand to ...export (on compilation)
///
/// case 2.2: On using the headers/linking against this by third party lib: 
/// case 2.2.1: was static build : expand to nothing
/// case 2.2.2: was shared build : expand to ...import (because it *was* compiled as a dll with export)
///
/// Thus on *usage* of this file we need to know if we are using BIAS as shared or static build.
/// This file is typically globally included through <bias_config.h>
///
/// @author Jan Woetzel 2005/11/01-2006
///
*/

#ifndef _BIAS_DeclSpec__hh__
#define _BIAS_DeclSpec__hh__


#include "BIAS/bias_config.h"

#if defined (WIN32) && defined(BIAS_BUILD_SHARED_LIBS) 

/*
 we need to define export on compilation  or import on use.
 We exploit that cmakes auto setting xxx_EXPORTS (with S) on compliation
 
 We need to write one switches for each built lib explicitly 
 because nested macros dont work with preprocessor. 
 Jan Woetzel, 2005
 */


#  ifdef BIASCommon_EXPORTS 
#    define BIASCommon_EXPORT __declspec(dllexport)
#  else  
#    define BIASCommon_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASDebug_EXPORTS 
#    define BIASDebug_EXPORT __declspec(dllexport)
#  else  
#    define BIASDebug_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASFeatureDetector_EXPORTS 
#    define BIASFeatureDetector_EXPORT __declspec(dllexport)
#  else  
#    define BIASFeatureDetector_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASFilter_EXPORTS
#    define BIASFilter_EXPORT __declspec(dllexport)
#  else  
#    define BIASFilter_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASGeometry_EXPORTS 
#    define BIASGeometry_EXPORT __declspec(dllexport)
#  else  
#    define BIASGeometry_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASGeometryBase_EXPORTS 
#    define BIASGeometryBase_EXPORT __declspec(dllexport)
#  else  
#    define BIASGeometryBase_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASGui_EXPORTS 
#    define BIASGui_EXPORT __declspec(dllexport)
#  else  
#    define BIASGui_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASImage_EXPORTS 
#    define BIASImage_EXPORT __declspec(dllexport)
#  else  
#    define BIASImage_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASImageBase_EXPORTS 
#    define BIASImageBase_EXPORT __declspec(dllexport)
#  else  
#    define BIASImageBase_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASImageUtilsBase_EXPORTS 
#    define BIASImageUtilsBase_EXPORT __declspec(dllexport)
#  else  
#    define BIASImageUtilsBase_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASMatcher2D_EXPORTS 
#    define BIASMatcher2D_EXPORT __declspec(dllexport)
#  else  
#    define BIASMatcher2D_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASMathAlgo_EXPORTS 
#    define BIASMathAlgo_EXPORT __declspec(dllexport)
#  else  
#    define BIASMathAlgo_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASMathBase_EXPORTS 
#    define BIASMathBase_EXPORT __declspec(dllexport)
#  else  
#    define BIASMathBase_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASOpenGLFramework_EXPORTS 
#    define BIASOpenGLFramework_EXPORT __declspec(dllexport)
#  else  
#    define BIASOpenGLFramework_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASPanTilt_EXPORTS 
#    define BIASPanTilt_EXPORT __declspec(dllexport)
#  else  
#    define BIASPanTilt_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASStateEstimator_EXPORTS 
#    define BIASStateEstimator_EXPORT __declspec(dllexport)
#  else  
#    define BIASStateEstimator_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASNetworkComm_EXPORTS 
#    define BIASNetworkComm_EXPORT __declspec(dllexport)
#  else  
#    define BIASNetworkComm_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASVideoSource_EXPORTS 
#    define BIASVideoSource_EXPORT __declspec(dllexport)
#  else  
#    define BIASVideoSource_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASUtils_EXPORTS 
#    define BIASUtils_EXPORT __declspec(dllexport)
#  else  
#    define BIASUtils_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASVideoSink_EXPORTS
#    define BIASVideoSink_EXPORT __declspec(dllexport)
#  else  
#    define BIASVideoSink_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASSimpleGLApp_EXPORTS
#    define BIASSimpleGLApp_EXPORT __declspec(dllexport)
#  else  
#    define BIASSimpleGLApp_EXPORT __declspec(dllimport)
#  endif 

#  ifdef BIASGLviewer_EXPORTS
#    define BIASGLviewer_EXPORT __declspec(dllexport)
#  else
#    define BIASGLviewer_EXPORT __declspec(dllimport)
#  endif



/* 
 getting the export for STL/DTL into DLLs is quite tricky on MS Windows.
 We use "manual" explicit instantiation to reach interfaced class variable

 See http://support.microsoft.com/kb/q168958/
 and
 http://www.unknownroad.com/rtfm/VisualStudio/warningC4251.html


 See BIAS/Filter/Label.hh for usage example. 
*/
#define BIAS_EXPORT_STL_VECTOR( dllmacro, vectype ) \
  template class dllmacro std::allocator< vectype >; \
  template class dllmacro std::vector<vectype, std::allocator< vectype > >;


#define BIAS_EXPORT_STL_SET( dllmacro, settype ) \
  template class dllmacro std::allocator< settype >; \
  template struct dllmacro std::less< settype >; \
  template class dllmacro std::allocator< \
    std::_Tree_nod<std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> > >; \
  template class dllmacro std::allocator<  \
    std::_Tree_ptr<std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> > >; \
  template class dllmacro std::_Tree_ptr< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::_Tree_nod< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::_Tree_val< \
    std::_Tset_traits<settype,std::less<settype>, \
    std::allocator<settype>,false> >; \
  template class dllmacro std::set< settype, std::less< settype >, \
    std::allocator< settype > >;

/* include BIAS-W32 compatibility */
#include <Base/Common/W32Compat.hh>

#  else /* (WIN32) && defined(BIAS_BUILD_SHARED_LIBS) */

/* we are either running Linux or building static,  
   thus expand to nothing 
   Jan Woetzel 
*/

#    define BIASCommon_EXPORT
#    define BIASDebug_EXPORT
#    define BIASFeatureDetector_EXPORT
#    define BIASFilter_EXPORT
#    define BIASGeometry_EXPORT
#    define BIASGeometryBase_EXPORT
#    define BIASGui_EXPORT
#    define BIASImage_EXPORT
#    define BIASImageBase_EXPORT
#    define BIASImageUtilsBase_EXPORT
#    define BIASMatcher2D_EXPORT
#    define BIASMathAlgo_EXPORT
#    define BIASMathBase_EXPORT
#    define BIASOpenGLFramework_EXPORT
#    define BIASPanTilt_EXPORT
#    define BIASStateEstimator_EXPORT
#    define BIASUtils_EXPORT
#    define BIASNetworkComm_EXPORT
#    define BIASVideoSource_EXPORT
#    define BIASVideoSink_EXPORT
#    define BIASSimpleGLApp_EXPORT
#    define BIASGLviewer_EXPORT



/* DLL: nothing to export (Jan Woetzel) */
#define BIAS_EXPORT_STL_VECTOR( dllmacro, vectype ) 
#define BIAS_EXPORT_STL_SET( dllmacro, settype ) 
/* #define BIAS_EXPORT_STL_MAP( dllmacro, mapkey, mapvalue )  */

#endif /* (WIN32) && defined(BIAS_BUILD_SHARED_LIBS)  --------------------- */

#endif /*  _BIAS_DeclSpec__hh__ */
