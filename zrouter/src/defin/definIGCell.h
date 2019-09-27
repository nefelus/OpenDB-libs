#ifndef ADS_DEFIN_IGCELL_H
#define ADS_DEFIN_IGCELL_H

#ifndef ADS_DEFIN_TYPES
#include "definTypes.h"
#endif

class definIGCell
{
  public:
    /// GCell interface methods
    virtual void gcell( defDirection dir, int orig, int count, int step ) {}
};
    
#endif
