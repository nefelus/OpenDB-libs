/* Copyright Athena Design Systems 2003 */

#ifndef DBGDEFINES_H 
#define DBGDEFINES_H 

#include "ads.h"

#define def_dbg

BEGIN_NAMESPACE_ADS

//
//  Class to filter debug() level messages according to a virtual predicate
//  function _pred()  by applying getFilteredModuleName() as the
//   module_name argument
//
class debugMsgFilter {
 public:
  explicit debugMsgFilter(const char *module_name);
  const char  *getFilteredModuleName();
  const char  *getModuleName();
  virtual bool  pred() { return false; }

 protected:
  char                _module_name[20];
};

END_NAMESPACE_ADS

#endif
