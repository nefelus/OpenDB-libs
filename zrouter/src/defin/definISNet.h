#ifndef ADS_DEFIN_ISNET_H
#define ADS_DEFIN_ISNET_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definISNet
{
  public:
    /// Special Net interface methods
    virtual void begin( const char * name ) {}
    virtual void connection( const char * iname, const char * pname, bool synthesized ) {}
    virtual void use( defSigType type ) {}
    virtual void wire( defWireType type, const char * shield ) {}
    virtual void rect( const char * layer, int x1, int y1, int x2, int y2 ) {}
    virtual void polygon( const char * layer, std::vector<defPoint> & points ) {}
    virtual void path( const char * layer, int width ) {}
    virtual void pathStyle( int style ) {}
    virtual void pathShape( const char * type ) {}
    virtual void pathPoint( int x, int y ) {}
    virtual void pathPoint( int x, int y, int ext ) {}
    virtual void pathVia( const char * via ) {}
    virtual void pathViaArray( const char * via, int numX, int numY, int stepX, int stepY ) {}
    virtual void pathEnd() {}
    virtual void wireEnd() {}
    virtual void source( defSource source ) {}
    virtual void weight( int weight ) {}
    virtual void fixedbump() {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
};
    
#endif
