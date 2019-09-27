#ifndef ADS_DB_TECH_H
#define ADS_DB_TECH_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

#ifndef ADS_DB_MATRIX_H
#include "dbMatrix.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;

class _dbProperty;
class dbPropertyItr;
class _dbNameCache;
class _dbTechLayer;
class _dbTechLayerRule;
class _dbTechVia;
class _dbTechNonDefaultRule;
class _dbTechSameNetRule;
class _dbTechLayerAntennaRule;
class _dbTechViaRule;
class _dbTechViaLayerRule;
class _dbTechViaGenerateRule;
class _dbBox;
class _dbDatabase;
class dbString;
class dbTechLayerItr;
class dbBoxItr;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTechFlags
{
  dbOnOffType::Value  _namecase     : 1;
  dbOnOffType::Value  _haswireext   : 1;
  dbOnOffType::Value  _nowireext    : 1;
  dbOnOffType::Value  _hasclmeas    : 1;
  dbClMeasureType::Value  _clmeas   : 2;
  dbOnOffType::Value  _hasminspobs  : 1;
  dbOnOffType::Value  _minspobs     : 1;
  dbOnOffType::Value  _hasminsppin  : 1;
  dbOnOffType::Value  _minsppin     : 1;
  uint                _spare_bits   : 22;
};

class _dbTech : public dbObject
{
 private:
    double               _version;
    char                 _version_buf[10];  // Tmp for outputting string
  public:
    // PERSISTANT-MEMBERS
    int                  _via_cnt;
    int                  _layer_cnt;
    int                  _rlayer_cnt;
    int                  _lef_units;
    int                  _dbu_per_micron;
    int                  _mfgrid;
    _dbTechFlags         _flags;
    dbId<_dbTechLayer>   _bottom;
    dbId<_dbTechLayer>   _top;
    dbId<_dbTechNonDefaultRule>         _non_default_rules;
    dbVector< dbId<_dbTechSameNetRule> > _samenet_rules;
    dbMatrix< dbId<_dbTechSameNetRule> > _samenet_matrix;

    // NON-PERSISTANT-STREAMED-MEMBERS
    dbTable<_dbTechLayer> *          _layer_tbl;
    dbTable<_dbTechVia> *            _via_tbl;
    dbTable<_dbTechNonDefaultRule> * _non_default_rule_tbl;
    dbTable<_dbTechLayerRule> *      _layer_rule_tbl;
    dbTable<_dbBox> *                _box_tbl;
    dbTable<_dbTechSameNetRule> *    _samenet_rule_tbl;
    dbTable<_dbTechLayerAntennaRule> * _antenna_rule_tbl;
    dbTable<_dbTechViaRule> *         _via_rule_tbl;
    dbTable<_dbTechViaLayerRule> *    _via_layer_rule_tbl;
    dbTable<_dbTechViaGenerateRule> * _via_generate_rule_tbl;
    dbTable<_dbProperty> *            _prop_tbl;
    _dbNameCache *                    _name_cache;

    // NON-PERSISTANT-NON-STREAMED-MEMBERS
    dbTechLayerItr *                 _layer_itr;
    dbBoxItr *                       _box_itr;
    dbPropertyItr *                  _prop_itr;

    double               _getLefVersion() const;
    const char *         _getLefVersionStr() const;    
    void                 _setLefVersion(double inver);

    _dbTech( _dbDatabase * db );
    _dbTech( _dbDatabase * db, const _dbTech & t );
    ~_dbTech();

    int operator==( const _dbTech & rhs ) const;
    int operator!=( const _dbTech & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTech & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbTech & tech );
dbIStream & operator>>( dbIStream & stream, _dbTech & tech );

END_NAMESPACE_ADS

#endif
