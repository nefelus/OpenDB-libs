#ifndef ADS_DEFIN_DEF_ICOMPONENT_H
#define ADS_DEFIN_DEF_ICOMPONENT_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definIComponent
{
  public:
    /// Component interface methods
    virtual void begin( const char * name , const char * cell ) {}
    virtual void placement( defPlacement status, int x, int y, defOrient orient ) {}
    virtual void region( const char * region ) {}
    virtual void halo( int left, int bottom, int right, int top ) {}
    virtual void source( defSource source ) {}
    virtual void weight( int weight ) {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
};
    
#endif
