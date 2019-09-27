#ifndef ADS_DEFIN_IPIN_PROPS_H
#define ADS_DEFIN_IPIN_PROPS_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definIPinProps
{
  public:
    // Pin interface methods
    virtual void begin(const char * inst, const char * term) {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
};
    
#endif
