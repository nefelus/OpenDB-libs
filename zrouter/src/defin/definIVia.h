#ifndef ADS_DEFIN_IVIA_H
#define ADS_DEFIN_IVIA_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definIVia
{
  public:
    // Via interface methods
    virtual void viaBegin( const char * name ) {}

    // generated via
    virtual void viaRule( const char * rule ) {}
    virtual void viaCutSize( int xSize, int ySize ) {}
    virtual void viaLayers(  const char * bottom, const char * cut, const char * top ) {}
    virtual void viaCutSpacing( int xSpacing, int ySpacing ) {}
    virtual void viaEnclosure( int xBot, int yBot, int xTop, int yTop ) {}
    virtual void viaRowCol( int numCutRows, int numCutCols ) {}
    virtual void viaOrigin( int xOffset, int yOffset ) {}
    virtual void viaOffset( int xBot, int yBot, int xTop, int yTop ) {}
    virtual void viaPattern( const char * pattern ) {}

    // fixed VIA
    virtual void viaRect( const char * layer, int x1, int y1, int x2, int y2 ) {}
    virtual void viaPolygon( std::vector<defPoint> & points ) {}
    virtual void viaEnd() {}
};
    
#endif
