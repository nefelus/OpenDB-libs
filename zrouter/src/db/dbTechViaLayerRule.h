#ifndef ADS_DB_VIA_LAYER_RULE_H
#define ADS_DB_VIA_LAYER_RULE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
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

class _dbTechLayer;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//
// These flags keep track of the variations between difference LEF versions
//
struct _dbTechViaLayerRuleFlags
{
    uint   _direction          : 2;
    uint   _has_enclosure      : 1; 
    uint   _has_width          : 1; 
    uint   _has_overhang       : 1; 
    uint   _has_metal_overhang : 1; 
    uint   _has_resistance     : 1; 
    uint   _has_spacing        : 1; 
    uint   _has_rect           : 1; 
    uint   _spare_bits         : 23;
};
    
class _dbTechViaLayerRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechViaLayerRuleFlags _flags; 
    int                      _overhang1;
    int                      _overhang2;
    int                      _min_width;
    int                      _max_width;
    int                      _overhang;
    int                      _metal_overhang;
    int                      _spacing_x;
    int                      _spacing_y;
    double                   _resistance;
    adsRect                  _rect;
    dbId<_dbTechLayer>       _layer;

    _dbTechViaLayerRule( _dbDatabase *, const _dbTechViaLayerRule & v );
    _dbTechViaLayerRule( _dbDatabase * );
    ~_dbTechViaLayerRule();

    int operator==( const _dbTechViaLayerRule & rhs ) const;
    int operator!=( const _dbTechViaLayerRule & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechViaLayerRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechViaLayerRule & v );
dbIStream & operator>>( dbIStream & stream, _dbTechViaLayerRule & v );

END_NAMESPACE_ADS

#endif
