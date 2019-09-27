#ifndef ADS_DEFIN_NET_H
#define ADS_DEFIN_NET_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_WIRE_CODEC_H
#include "dbWireCodec.h"
#endif

#ifndef ADS_DEFIN_INET_H
#include "definINet.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#include <map>
#include <string>

BEGIN_NAMESPACE_ADS

class dbWire;
class dbSWire;
class dbNet;
class dbVia;
class dbTechLayer;
class dbTechLayerRule;
class dbTechNonDefaultRule;
 
class definNet : public definINet, public definBase
{
    bool                   _skip_signal_connections;
    bool                   _skip_wires;
    bool                   _replace_wires;
    bool                   _names_are_ids;
    bool                   _assembly_mode;
    bool                   _found_new_routing;
    dbNet *                _cur_net;
    dbTechLayer *          _cur_layer;
    dbWireEncoder          _wire_encoder;
    dbWire *               _wire;
    dbWireType             _wire_type;
    dbWireShapeType        _wire_shape_type;
    int                    _prev_x;
    int                    _prev_y;
    int                    _width;
    int                    _point_cnt;
    dbTechLayerRule *      _taper_rule;
    dbTechNonDefaultRule * _non_default_rule;
    dbTechNonDefaultRule * _rule_for_path;
    std::map<std::string, dbVia *> _rotated_vias;

    void getUniqueViaName( std::string & viaName );
    dbVia * getRotatedVia(const char * via_name, int orient );
    dbTechNonDefaultRule * findNonDefaultRule( const char * name );
    
  public:
    int                    _net_cnt;
    int                    _net_iterm_cnt;

    /// Net interface methods
    virtual void begin( const char * name );
    virtual void beginMustjoin( const char * iname, const char * pname );
    virtual void connection( const char * iname, const char * pname );
    virtual void nonDefaultRule( const char * rule );
    virtual void use( defSigType type );
    virtual void wire( defWireType type );
    virtual void path( const char * layer );
    virtual void pathStyle( int style );
    virtual void pathTaper( const char * layer );
    virtual void pathTaperRule( const char * layer, const char * rule );
    virtual void pathPoint( int x, int y );
    virtual void pathPoint( int x, int y, int ext );
    virtual void pathVia( const char * via );
    virtual void pathVia( const char * via, int def_orient );
    virtual void pathEnd();
    virtual void wireEnd();
    virtual void source( defSource source );
    virtual void weight( int weight );
    virtual void fixedbump();
    virtual void xtalk(int value);
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();
    
    void pathBegin( const char * layer );
    //void netBeginCreate( const char * name );
    //void netBeginReplace( const char * name );

  public:
    definNet();
    virtual ~definNet();
    void init();
    void skipWires()       { _skip_wires = true; }
    void skipConnections()       { _skip_signal_connections = true; }
    void replaceWires()    { _replace_wires = true; }
    void setAssemblyMode() { _assembly_mode = true; }
    void namesAreDBIDs()   { _names_are_ids = true; }
};
    
END_NAMESPACE_ADS

#endif
