#ifndef ADS_DEFIN_IPROP_DEFS_H
#define ADS_DEFIN_IPROP_DEFS_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definIPropDefs
{
  public:
    virtual void beginDefinitions() { }
    virtual void begin( defObjectType, const char *, defPropType ) {}
    virtual void value( const char *  ) {}
    virtual void value( int ) {}
    virtual void value( double ) {}
    virtual void range( int, int ) {}
    virtual void range( double, double ) {}
    virtual void end() {}
    virtual void endDefinitions() { }
};

#endif
