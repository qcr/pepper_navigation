/*
 * BIAS_Definitions.hh
 *
 *  Created on: Feb 27, 2009
 *      Author: djung
 */

#ifndef _BIAS_DEFINITIONS_HH__
#define _BIAS_DEFINITIONS_HH__

// use NL instead of endl. endl flushes the stream which is bad for file output
#define NL "\r\n"

#define PRINT_FLOATING_POINT(number, leading, precision) setw(leading+precision+1) << setiosflags(ios::fixed) << \
  setprecision(precision) << (double) number


#define LOCALE_C_ON   string __CurrLocale__ = setlocale(LC_NUMERIC, NULL);\
  setlocale(LC_NUMERIC, "C");
  // cout <<"Found Locale: "<<__CurrLocale__<<" setting to C temporarily"<<endl;

#define LOCALE_C_OFF setlocale(LC_NUMERIC,__CurrLocale__.c_str());
//cout <<"Resetting Locale to: "<<__CurrLocale__<<endl;


#endif /* BIAS_DEFINITIONS_HH_ */
