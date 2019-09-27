#ifndef ADS_DEFIN_BLOCKAGE_H
#define ADS_DEFIN_BLOCKAGE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IBLOCKAGE_H
#include "definIBlockage.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbTechLayer;
class dbInst;

class definBlockage : public definIBlockage, public definBase
{
    dbTechLayer * _layer;
    dbInst *      _inst;
    bool          _slots;
    bool          _fills;
    bool          _pushdown;
    bool          _has_min_spacing;
    bool          _has_effective_width;
    int           _min_spacing;
    int           _effective_width;


    // Routing Blockage interface methods
    virtual void blockageRoutingBegin( const char * layer );
    virtual void blockageRoutingComponent( const char * comp );
    virtual void blockageRoutingSlots();
    virtual void blockageRoutingFills();
    virtual void blockageRoutingPushdown();
    virtual void blockageRoutingMinSpacing(int spacing);
    virtual void blockageRoutingEffectiveWidth(int width);
    virtual void blockageRoutingRect( int x1, int y1, int x2, int y2 );
    virtual void blockageRoutingPolygon( std::vector<defPoint> & points );
    virtual void blockageRoutingEnd();

    // Placement Blockage interface methods
    virtual void blockagePlacementBegin();
    virtual void blockagePlacementComponent( const char * comp );
    virtual void blockagePlacementPushdown();
    virtual void blockagePlacementRect( int x1, int y1, int x2, int y2 );
    virtual void blockagePlacementEnd();

  public:
    definBlockage();
    virtual ~definBlockage();
    void init();
};
    
END_NAMESPACE_ADS

#endif
