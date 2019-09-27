#ifndef ADS_DEFIN_PIN_H
#define ADS_DEFIN_PIN_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DEFIN_IPIN_H
#include "definIPin.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DEFIN_POLYGON_H
#include "definPolygon.h"
#endif

#include <string>

BEGIN_NAMESPACE_ADS

class dbBTerm;
class dbTechLayer;
 
class definPin : public definIPin, public definBase
{
    struct Rect
    {
        dbTechLayer *            _layer;
        adsRect                  _rect;

        Rect( dbTechLayer * layer, const adsRect & rect )
                : _layer(layer), _rect(rect)
            {}
    };

    struct Polygon : public definPolygon
    {
        dbTechLayer *            _layer;
        
        Polygon( dbTechLayer * layer, const std::vector<adsPoint> & points )
                : definPolygon(points), _layer(layer)
            {}
    };

    struct Pin
    {
        dbBTerm *   _bterm;
        std::string _pin;

        Pin( dbBTerm * bterm, const std::string & pin )
                : _bterm(bterm), _pin(pin)
        {
        }
    };

    
        
  public:
    int        _bterm_cnt;

  private:
    dbBTerm *                _cur_bterm;
    dbPlacementStatus::Value _status;
    dbOrientType::Value      _orient;
    int                      _orig_x;
    int                      _orig_y;
    int                      _min_spacing;
    int                      _effective_width;
    char                     _left_bus;
    char                     _right_bus;
    adsRect                  _rect;
    dbTechLayer *            _layer;
    bool                     _has_min_spacing;
    bool                     _has_effective_width;
    bool                     _has_placement;
    std::vector<Rect>        _rects;
    std::vector<Polygon>     _polygons;
    std::vector<Pin>         _ground_pins;
    std::vector<Pin>         _supply_pins;

    void addRect( Rect & r );
    void addPolygon( Polygon & p );

    // Pin interface methods
    virtual void pinsBegin( int n );
    virtual void pinBegin( const char * name, const char * net );
    virtual void pinSpecial();
    virtual void pinUse( defSigType type );
    virtual void pinDirection( defIoType type );
    virtual void pinPlacement( defPlacement status, int x, int y, defOrient orient );
    virtual void pinMinSpacing( int spacing );
    virtual void pinEffectiveWidth( int width );
    virtual void pinRect( const char * layer, int x1, int y1, int x2, int y2 );
    virtual void pinPolygon( std::vector<defPoint> & points );
    virtual void pinGroundPin( const char * groundPin );
    virtual void pinSupplyPin( const char * supplyPin );
    virtual void pinEnd();
    virtual void pinsEnd();
    
  public:
    definPin();
    virtual ~definPin();
    void init();
};
    
END_NAMESPACE_ADS

#endif
