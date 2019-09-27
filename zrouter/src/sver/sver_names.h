#ifndef SVER_NAMES_H
#define SVER_NAMES_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class sver_names {
 private:
  char *_blkV[32768];
  int _blkNmax;
  int _blkN;
  char *_p;
  char *_pend;
 public:
  sver_names();
  ~sver_names();
  char *store(const char *str);
  char *alloc(int len);
  void clear();
  int get_used_kb();
};

END_NAMESPACE_ADS

#endif
