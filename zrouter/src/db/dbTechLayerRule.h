#ifndef ADS_DB_TECH_LAYERRULE_H
#define ADS_DB_TECH_LAYERRULE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class _dbTech;
class _dbBlock;
class _dbTechLayer;
class _dbTechNonDefaultRule;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTechLayerRuleFlags
{
    uint  _block_rule    : 1;
    uint  _spare_bits    : 31;
};

class _dbTechLayerRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechLayerRuleFlags       _flags;
    uint                        _width;
    uint                        _spacing;
    double                      _resistance;
    double                      _capacitance;
    double                      _edge_capacitance;
    uint                        _wire_extension;
    dbId<_dbTechNonDefaultRule> _non_default_rule;
    dbId<_dbTechLayer>          _layer;

    _dbTechLayerRule( _dbDatabase * );
    _dbTechLayerRule( _dbDatabase *, const _dbTechLayerRule & r );
    ~_dbTechLayerRule();

    _dbTech * getTech();
    _dbBlock * getBlock();

    int operator==( const _dbTechLayerRule & rhs ) const;
    int operator!=( const _dbTechLayerRule & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbTechLayerRule & rhs ) const
    {
        if  ( _layer < rhs._layer )
            return true;

        if  ( _layer > rhs._layer )
            return false;
        
        return _non_default_rule < rhs._non_default_rule;
    }
    
    void differences( dbDiff & diff, const char * field, const _dbTechLayerRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechLayerRule & rule );
dbIStream & operator>>( dbIStream & stream, _dbTechLayerRule & rule );

END_NAMESPACE_ADS

#endif
