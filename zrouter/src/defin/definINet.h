#ifndef ADS_DEFIN_INET_H
#define ADS_DEFIN_INET_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definINet
{
  public:
    /// Net interface methods
    virtual void begin( const char * name ) {}
    virtual void beginMustjoin( const char * iname, const char * pname ) {}
    virtual void connection( const char * iname, const char * pname ) {}
    virtual void nonDefaultRule( const char * rulename ) {}
    virtual void use( defSigType type ) {}
    virtual void wire( defWireType type ) {}
    virtual void path( const char * layer ) {}
    virtual void pathStyle( int style ) {}
    virtual void pathTaper( const char * layer ) {}
    virtual void pathTaperRule( const char * layer, const char * rule ) {}
    virtual void pathPoint( int x, int y ) {}
    virtual void pathPoint( int x, int y, int ext ) {}
    virtual void pathVia( const char * via ) {}
    virtual void pathVia( const char * via, int def_orient ) {}
    virtual void pathEnd() {}
    virtual void wireEnd() {}
    virtual void source( defSource source ) {}
    virtual void weight( int weight ) {}
    virtual void fixedbump() {}
    virtual void xtalk(int value) {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
};
    
#endif
