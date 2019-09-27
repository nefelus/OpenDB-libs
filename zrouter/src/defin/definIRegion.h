#ifndef ADS_DEFIN_DEF_IREGION_H
#define ADS_DEFIN_DEF_IREGION_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definIRegion
{
  public:
    /// Region interface methods
    virtual void begin( const char * name, bool is_group = false ) {}
    virtual void boundary( int x1, int y1, int x2, int y2 ) {}
    virtual void type( defRegionType type ) {}
    virtual void inst( const char * inst ) {}
    virtual void parent( const char * region ) {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
};
    
#endif
