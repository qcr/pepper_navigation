#ifndef __biasexit_hh__
#define __biasexit_hh__

#include <bias_config.h>

namespace BIAS {

  /// wrapper for exit()
  /// Disables unreachable warnings. 
  /// Cannot be header implemented to avoid multiply defined symbols and wring cdecl on DLL build.
  /// @author Jan Woetzel
  void BIASDebug_EXPORT biasexit(const int status = 1);

} // end namescape

#endif // __biasexit_hh__

