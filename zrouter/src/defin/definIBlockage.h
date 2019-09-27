#ifndef ADS_DEFIN_IBLOCKAGE_H
#define ADS_DEFIN_IBLOCKAGE_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definIBlockage
{
  public:
    // Routing Blockage interface methods
    virtual void blockageRoutingBegin( const char * layer ) {}
    virtual void blockageRoutingComponent( const char * comp ) {}
    virtual void blockageRoutingSlots() {}
    virtual void blockageRoutingFills() {}
    virtual void blockageRoutingPushdown() {}
    virtual void blockageRoutingMinSpacing(int spacing) {}
    virtual void blockageRoutingEffectiveWidth(int width) {}
    virtual void blockageRoutingRect( int x1, int y1, int x2, int y2 ) {}
    virtual void blockageRoutingPolygon( std::vector<defPoint> & points ) {}
    virtual void blockageRoutingEnd() {}

    // Placement Blockage interface methods
    virtual void blockagePlacementBegin() {}
    virtual void blockagePlacementComponent( const char * comp ) {}
    virtual void blockagePlacementPushdown() {}
    virtual void blockagePlacementRect( int x1, int y1, int x2, int y2 ) {}
    virtual void blockagePlacementEnd() {}
};
    
#endif
