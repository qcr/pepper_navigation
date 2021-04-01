set(BIAS_SOURCES  
    BIAS/Base/Common/W32Compat.cpp
    BIAS/Base/Common/FileHandling.cpp
    BIAS/Base/Common/CompareFloatingPoint.cpp
    BIAS/Base/Debug/TimeMeasure.cpp
    BIAS/Base/Debug/Debug.cpp
    BIAS/Base/Debug/biasexit.cpp
    BIAS/Base/Debug/biasabort.cpp
    BIAS/Base/Math/Vector3.cpp
    BIAS/Base/Math/Vector2.cpp
    BIAS/Base/Math/Matrix4x4.cpp
    BIAS/Base/Math/Matrix3x4.cpp
    BIAS/Base/Math/Matrix3x3.cpp
    BIAS/Base/Math/Matrix2x2.cpp
    BIAS/Base/Math/Matrix.cpp
    BIAS/Base/Math/Vector4.cpp
    BIAS/Base/Math/Vector.cpp
    BIAS/Base/Image/UUID.cpp
    BIAS/Base/Image/ROI.cpp
    BIAS/Base/Image/MetaData.cpp
    BIAS/Base/Image/ImageIO.cpp
    BIAS/Base/Image/ImageConvert.cpp
    BIAS/Base/Image/ImageBase.cpp
    BIAS/Base/Image/Image.cpp
    BIAS/Utils/Param.cpp
    BIAS/Utils/ParamTypes.cpp
    BIAS/MathAlgo/SVD.cpp
    BIAS/Filter/FilterNTo2N.cpp
    BIAS/Filter/FilterMask.cpp
    BIAS/Filter/FilterBase.cpp
    BIAS/Filter/Convolution.cpp
    BIAS/Filter/Mean.cpp
    BIAS/Filter/Rescale.cpp
    BIAS/Filter/Gauss.cpp
    BIAS/Filter/FilterNToN.cpp
    BIAS/Filter/Binomial.cpp
    BIAS/Base/Image/WrapBias2Ipl.cpp
    BIAS/Base/Debug/LogFacility.cpp
    BIAS/MathAlgo/Lapack.cpp
    BIAS/Filter/GaussThreshold.cpp
)


set(BIAS_HEADERS  
    BIAS/Base/Common/FileHandling.hh
    BIAS/Base/Common/CompareFloatingPoint.hh
    BIAS/Base/Common/W32Compat.hh
    BIAS/Base/Common/BIASpragmaEnd.hh
    BIAS/Base/Common/BIASpragma.hh
    BIAS/Base/Common/BIASpragmaStart.hh
    BIAS/Base/Debug/TimeMeasure.hh
    BIAS/Base/Debug/biasexit.hh
    BIAS/Base/Debug/Exception.hh
    BIAS/Base/Debug/Error.hh
    BIAS/Base/Debug/biasabort.hh
    BIAS/Base/Debug/Debug.hh
    BIAS/Base/Math/tnt/tntmath.h
    BIAS/Base/Math/tnt/region1d.h
    BIAS/Base/Math/tnt/tntreqs.h
    BIAS/Base/Math/tnt/subscript.h
    BIAS/Base/Math/tnt/qr.h
    BIAS/Base/Math/tnt/version.h
    BIAS/Base/Math/tnt/triang.h
    BIAS/Base/Math/tnt/fmat.h
    BIAS/Base/Math/tnt/cmat.h
    BIAS/Base/Math/tnt/index.h
    BIAS/Base/Math/tnt/tnt.h
    BIAS/Base/Math/tnt/lapack.h
    BIAS/Base/Math/tnt/fcscmat.h
    BIAS/Base/Math/tnt/fspvec.h
    BIAS/Base/Math/tnt/transv.h
    BIAS/Base/Math/tnt/region2d.h
    BIAS/Base/Math/tnt/cholesky.h
    BIAS/Base/Math/tnt/vecadaptor.h
    BIAS/Base/Math/tnt/stopwatch.h
    BIAS/Base/Math/tnt/fortran.h
    BIAS/Base/Math/tnt/trisolve.h
    BIAS/Base/Math/tnt/lu.h
    BIAS/Base/Math/tnt/vec.h
    BIAS/Base/Math/Utils.hh
    BIAS/Base/Math/Polynom.hh
    BIAS/Base/Math/Matrix4x4.hh
    BIAS/Base/Math/Vector3.hh
    BIAS/Base/Math/Vector4.hh
    BIAS/Base/Math/Matrix.hh
    BIAS/Base/Math/Operators.hh
    BIAS/Base/Math/Matrix3x4.hh
    BIAS/Base/Math/Vector2.hh
    BIAS/Base/Math/Matrix2x2.hh
    BIAS/Base/Math/Vector.hh
    BIAS/Base/Math/Matrix3x3.hh
    BIAS/Base/Math/Matrix3x2.hh
    BIAS/Base/Image/ImageIO.hh
    BIAS/Base/Image/ImageInline.hh
    BIAS/Base/Image/ROI.hh
    BIAS/Base/Image/ImageConvert.hh
    BIAS/Base/Image/MetaData.hh
    BIAS/Base/Image/ImageBaseInline.hh
    BIAS/Base/Image/UUID.hh
    BIAS/Base/Image/Image.hh
    BIAS/Base/Image/ImageBase.hh
    BIAS/Utils/ParamTypes.hh
    BIAS/Utils/Param.hh
    BIAS/MathAlgo/SVD.hh
    BIAS/Filter/FilterNTo2N.hh
    BIAS/Filter/FilterMask.hh
    BIAS/Filter/Convolution.hh
    BIAS/Filter/Binomial.hh
    BIAS/Filter/Rescale.hh
    BIAS/Filter/Gauss.hh
    BIAS/Filter/Mean.hh
    BIAS/Filter/FilterNToN.hh
    BIAS/Filter/FilterBase.hh
    BIAS/Base/Image/WrapBias2Ipl.hh
    BIAS/Base/Debug/DebugSimple.hh
    BIAS/Base/Debug/TimeMeasurement.hh
    BIAS/Base/Debug/LogFacility.hh
    BIAS/Base/Image/ViffHeader.hh
    BIAS/MathAlgo/Lapack.hh
    BIAS/Filter/GaussThreshold.hh
    BIAS/Filter/Filterinst.hh

)


include_directories(arpack++/include)
include_directories(BIAS)

FIND_LIBRARY(ARPACK_LIBRARIES
  NAMES arpack
)
#message("ARPACK_LIBRARIES: ${ARPACK_LIBRARIES}")

FIND_LIBRARY(SUPERLU_LIBRARIES
    NAMES superlu
)

#message("SUPERLU_LIBRARIES: ${SUPERLU_LIBRARIES}")

FIND_LIBRARY("UUID_LIBRARIES"
    NAMES uuid
)

set(BIAS_extraLIBS
${ARPACK_LIBRARIES}
${SUPERLU_LIBRARIES}
${UUID_LIBRARIES}
)
