#ifndef ADS_DEFIN_SNET_H
#define ADS_DEFIN_SNET_H

#include <vector>
#include <map>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_H
#include "db.h"
#endif

#ifndef ADS_DEFIN_ISNET_H
#include "definISNet.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbSWire;
class dbNet;
class dbTechLayer;
 
class definSNet : public definISNet, public definBase
{
    bool                 _skip_special_wires;
    bool                 _skip_shields;
    bool		         _skip_block_wires;
    bool	        	 _skip_fill_wires;
    bool	        	 _replace_wires;
    bool                 _names_are_ids;
    dbNet *              _cur_net;
    dbTechLayer *        _cur_layer;
    dbSWire *            _swire;
    dbWireType           _wire_type;
    dbWireShapeType      _wire_shape_type;
    dbNet *              _shield_net;
    int                  _prev_x;
    int                  _prev_y;
    int                  _prev_ext;
    bool                 _has_prev_ext;
    int                  _width;
    int                  _point_cnt;

  public:
    int                  _snet_cnt;
    int                  _snet_iterm_cnt;

    /// Special Net interface methods
    virtual void begin( const char * name );
    virtual void connection( const char * iname, const char * pname, bool synthesized );
    virtual void use( defSigType type );
    virtual void rect( const char * layer, int x1, int y1, int x2, int y2 );
    virtual void polygon( const char * layer, std::vector<defPoint> & points );
    virtual void wire( defWireType type, const char * shield );
    virtual void path( const char * layer, int width );
    virtual void pathStyle( int style );
    virtual void pathShape( const char * type );
    virtual void pathPoint( int x, int y );
    virtual void pathPoint( int x, int y, int ext );
    virtual void pathVia( const char * via );
    virtual void pathViaArray( const char * via, int numX, int numY, int stepX, int stepY );
    virtual void pathEnd();
    virtual void wireEnd();
    virtual void source( defSource source );
    virtual void weight( int weight );
    virtual void fixedbump();
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();

    void connect_all( dbNet *, const char * term );

  public:
    definSNet();
    virtual ~definSNet();
    void init();
    
    void skipSpecialWires() { _skip_special_wires = true; }
    void skipShields()      { _skip_shields = true; }
    void skipBlockWires()   { _skip_block_wires = true; }
    void skipFillWires()    { _skip_fill_wires = true; }
    void replaceWires()     { _replace_wires = true; }
    void namesAreDBIDs()   { _names_are_ids = true; }
};
    
END_NAMESPACE_ADS

#endif
