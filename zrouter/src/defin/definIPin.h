#ifndef ADS_DEFIN_IPIN_H
#define ADS_DEFIN_IPIN_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definIPin
{
  public:
    // Pin interface methods
    virtual void pinsBegin(int n) {}
    virtual void pinBegin( const char * name, const char * net ) {}
    virtual void pinSpecial() {}
    virtual void pinUse( defSigType type ) {}
    virtual void pinDirection( defIoType type ) {}
    virtual void pinPlacement( defPlacement status, int x, int y, defOrient orient ) {}
    virtual void pinMinSpacing( int spacing ) {}
    virtual void pinEffectiveWidth( int width ) {}
    virtual void pinRect( const char * layer, int x1, int y1, int x2, int y2 ) {}
    virtual void pinPolygon( std::vector<defPoint> & points ) {}
    virtual void pinGroundPin( const char * groundPin ) {}
    virtual void pinSupplyPin( const char * supplyPin ) {}
    virtual void pinEnd() {}
    virtual void pinsEnd() {}
};
    
#endif
