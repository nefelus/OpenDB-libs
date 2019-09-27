#ifndef ADS_DEFOUT_IMPL_H
#define ADS_DEFOUT_IMPL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_MAP_H
#include "dbMap.h"
#endif

#include <list>
#include <map>
#include <string>
#include "defout.h"

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbBTerm;
class dbInst;
class dbTechNonDefaultRule;
class dbTechLayerRule;;

class defout_impl
{
    enum ObjType
    {
        COMPONENT,
        COMPONENTPIN,
        DESIGN,
        GROUP,
        NET,
        NONDEFAULTRULE,
        REGION,
        ROW,
        SPECIALNET
    };
    
    double    _dist_factor;
    FILE *    _out;
    bool      _use_net_inst_ids;
    bool      _use_master_ids;
    bool      _use_alias;
    std::list<dbNet*> _select_net_list;
    std::list<dbInst*> _select_inst_list;
    dbMap<dbNet,char> *_select_net_map;
    dbMap<dbInst,char> *_select_inst_map;
    dbTechNonDefaultRule * _non_default_rule;
    int       _version;
    std::map<std::string,bool> _prop_defs[9];

    int defdist( int value ) 
    {
        return (int) (((double) value) * _dist_factor);
    }

    int defdist( uint value ) 
    {
        return (uint) (((double) value) * _dist_factor);
    }

    void writePropertyDefinitions( dbBlock * block );
    void writeRows( dbBlock * block );
    void writeTracks( dbBlock * block );
    void writeGCells( dbBlock * block );
    void writeVias( dbBlock * block );
    void writeVia( dbVia * via );
    void writeInsts( dbBlock * block );
    void writeNonDefaultRules( dbBlock * block );
    void writeNonDefaultRule( dbTechNonDefaultRule * rule );
    void writeLayerRule( dbTechLayerRule * rule );
    void writeInst( dbInst * inst );
    void writeBTerms( dbBlock * block );
    void writeBTerm( dbBTerm * bterm );
    void writeBPin( dbBPin * bpin, int n );
    void writeRegions( dbBlock * block );
    void writeGroups( dbBlock * block );
    void writeBlockages( dbBlock * block );
    void writeNets( dbBlock * block );
    void writeNet( dbNet * net );
    void writeSNet( dbNet * net );
    void writeWire( dbWire * wire );
    void writeSWire( dbSWire * wire );
    void writeSpecialPath( dbSBox * box );
    void writePropValue( dbProperty * prop );
    void writeProperties( dbObject * object );
    void writePinProperties( dbBlock * block );
    bool hasProperties( dbObject * object, ObjType type );
    
  public:

    defout_impl()
    {
        _use_net_inst_ids = false;
        _use_master_ids = false;
        _use_alias = false;
        _select_net_map = NULL;
        _select_inst_map = NULL;
        _version = defout::DEF_5_5;
    }

    ~defout_impl() {}

    void setUseLayerAlias( bool value )
    {
        _use_alias = value;
    }

    void setUseNetInstIds( bool value )
    {
        _use_net_inst_ids = value;
    }
    
    void setUseMasterIds( bool value )
    {
        _use_master_ids = value;
    }
    
    void selectNet( dbNet *net );

    void selectInst( dbInst *inst );
    void setVersion( int v ) { _version = v; } 
    
    bool writeBlock( dbBlock * block, const char * def_file);
};

END_NAMESPACE_ADS

#endif
