#ifndef ADS_DB_TECH_LAYERSPACINGRULE_H
#define ADS_DB_TECH_LAYERSPACINGRULE_H

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
class _dbTechLayer;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbTechLayerSpacingRule : public dbObject
{
  public:
     // PERSISTENT-MEMBERS
  TechLayerSpacingRule::_Flword  _flags;
  uint                        _spacing;
  uint                        _length_or_influence;
  uint                        _r1min;
  uint                        _r1max;
  uint                        _r2min;
  uint                        _r2max;
  dbId<_dbTechLayer>          _layer;
  dbId<_dbTechLayer>          _cut_layer_below;

    _dbTechLayerSpacingRule( _dbDatabase *, const _dbTechLayerSpacingRule & r );
    _dbTechLayerSpacingRule( _dbDatabase * );
    ~_dbTechLayerSpacingRule();

    int operator==( const _dbTechLayerSpacingRule & rhs ) const;
    int operator!=( const _dbTechLayerSpacingRule & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechLayerSpacingRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTechLayerSpacingRule::_dbTechLayerSpacingRule( _dbDatabase *, const _dbTechLayerSpacingRule & r )
        : _flags(r._flags),
          _spacing(r._spacing),
          _length_or_influence(r._length_or_influence),
          _r1min(r._r1min),
          _r1max(r._r1max),
          _r2min(r._r2min),
          _r2max(r._r2max),
          _layer(r._layer),
          _cut_layer_below(r._cut_layer_below)
{
}

inline _dbTechLayerSpacingRule::_dbTechLayerSpacingRule( _dbDatabase * )
{
  _flags._rule = TechLayerSpacingRule::DEFAULT;
  _flags._spare_bits = 0;
  _spacing  = 0;
  _length_or_influence = _r1min = _r1max = _r2min = _r2max = 0;
}

inline _dbTechLayerSpacingRule::~_dbTechLayerSpacingRule()
{}

dbOStream & operator<<( dbOStream & stream, const _dbTechLayerSpacingRule &rule );
dbIStream & operator>>( dbIStream & stream, _dbTechLayerSpacingRule & rule );

///  This structure defines entries in the V5.5 influence spacing rule table.

class _dbTechV55InfluenceEntry : public dbObject
{
  public:
  uint                        _width;
  uint                        _within;
  uint                        _spacing;

  _dbTechV55InfluenceEntry( _dbDatabase * db, const _dbTechV55InfluenceEntry & e );
  _dbTechV55InfluenceEntry(_dbDatabase * db );
  ~_dbTechV55InfluenceEntry();
  int operator==( const _dbTechV55InfluenceEntry & rhs ) const;
  int operator!=( const _dbTechV55InfluenceEntry & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _dbTechV55InfluenceEntry & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTechV55InfluenceEntry::_dbTechV55InfluenceEntry( _dbDatabase * db, const _dbTechV55InfluenceEntry & e )
        : _width(e._width),
          _within(e._within),
          _spacing(e._spacing)
{
}

inline _dbTechV55InfluenceEntry::_dbTechV55InfluenceEntry( _dbDatabase * db )
{  _width = _within = _spacing = 0;  }

inline _dbTechV55InfluenceEntry::~_dbTechV55InfluenceEntry()
{}

dbOStream & operator<<( dbOStream & stream, const _dbTechV55InfluenceEntry & infitem );
dbIStream & operator>>( dbIStream & stream, _dbTechV55InfluenceEntry & infitem );

END_NAMESPACE_ADS

#endif
