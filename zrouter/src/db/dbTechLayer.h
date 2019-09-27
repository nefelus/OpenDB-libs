#ifndef ADS_DB_TECH_LAYER_H
#define ADS_DB_TECH_LAYER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

#ifndef ADS_DB_MATRIX_H
#include "dbMatrix.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;

class _dbDatabase;
class _dbTechLayerSpacingRule;
class _dbTechMinCutRule;
class _dbTechMinEncRule;
class _dbTechV55InfluenceEntry;
class _dbTechLayerAntennaRule;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbTechLayer : public dbObject
{
  public:
    // PERSISTENT-MEMBERS

struct _LyFlags
{
    dbTechLayerType::Value _type        : 4;
    dbTechLayerDir::Value  _direction   : 4;
    uint                   _has_max_width    : 1;
    uint                   _has_min_step     : 1;
    uint                   _has_thickness    : 1;
    uint                   _has_area         : 1;
    uint                   _has_protrusion   : 1;
    uint                   _has_alias   : 1;
    uint                   _spare_bits  : 18;
} _flags;

    uint                   _pitch;
    uint                   _width;
    uint                   _spacing;
    double                 _resistance;
    double                 _capacitance;
    double                 _edge_capacitance;
    uint                   _wire_extension;
    uint                   _number;
    uint                   _rlevel;
    double                 _area;
    uint                   _thickness;
    uint                   _min_step;
    uint                   _max_width;
    struct  {    // Protrusion
      uint _width;
      uint _length;
      uint _from_width;
    }                      _pt;
    char *                 _name;
    char *                 _alias;
    dbId<_dbTechLayer>     _upper;
    dbId<_dbTechLayer>     _lower;
    dbTable<_dbTechLayerSpacingRule> *  _spacing_rules_tbl;
    dbTable<_dbTechMinCutRule> *        _min_cut_rules_tbl;
    dbTable<_dbTechMinEncRule> *        _min_enc_rules_tbl;
    dbTable<_dbTechV55InfluenceEntry> * _v55inf_tbl;
    dbVector<uint>                      _v55sp_length_idx;
    dbVector<uint>                      _v55sp_width_idx;
    dbMatrix<uint>                      _v55sp_spacing;
    dbId<_dbTechLayerAntennaRule>       _oxide1;
    dbId<_dbTechLayerAntennaRule>       _oxide2;

    _dbTechLayer( _dbDatabase * db);
    _dbTechLayer( _dbDatabase * db, const _dbTechLayer & l);
    ~_dbTechLayer();

    int operator==( const _dbTechLayer & rhs ) const;
    int operator!=( const _dbTechLayer & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechLayer & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    int operator<( const _dbTechLayer & rhs ) const
    {
        return _number < rhs._number;
    }

    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbTechLayer & layer );
dbIStream & operator>>( dbIStream & stream, _dbTechLayer & layer );

END_NAMESPACE_ADS

#endif
