#ifndef __biasabort_hh__
#define __biasabort_hh__

#include <bias_config.h>

namespace BIAS {

  /// wrapper for abort()
  /// Disables unreachable warnings. 
  /// Cannot be header implemented to avoid multiply defined symbols and wring cdecl on WIn32 DLL build.
  /// @author Jan Woetzel
  void BIASDebug_EXPORT biasabort();

} // end namescape

#endif // __biasabort_hh__

