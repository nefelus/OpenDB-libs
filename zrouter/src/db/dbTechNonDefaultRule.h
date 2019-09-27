#ifndef ADS_DB_TECH_NONDEFAULT_RULE_H
#define ADS_DB_TECH_NONDEFAULT_RULE_H

#ifndef ADS_H
#include "ads.h"
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

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTech;
class _dbBlock;
class _dbTechLayerRule;
class _dbTechVia;
class _dbTechLayer;
class _dbTechViaGenerateRule;
class _dbTechSameNetRule;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTechNonDefaultRuleFlags
{
    uint _hard_spacing  : 1 ;
    uint _block_rule    : 1 ;
    uint _spare_bits    : 30;
};

class _dbTechNonDefaultRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechNonDefaultRuleFlags               _flags;
    char *                                   _name;
    dbVector< dbId<_dbTechLayerRule> >       _layer_rules;
    dbVector< dbId<_dbTechVia> >             _vias;
    dbVector< dbId<_dbTechSameNetRule> >     _samenet_rules;
    dbMatrix< dbId<_dbTechSameNetRule> >     _samenet_matrix;
    dbVector< dbId<_dbTechVia> >             _use_vias;
    dbVector< dbId<_dbTechViaGenerateRule> > _use_rules;
    dbVector< dbId<_dbTechLayer> >           _cut_layers;
    dbVector<int>                            _min_cuts;
    
    _dbTechNonDefaultRule( _dbDatabase * );
    _dbTechNonDefaultRule( _dbDatabase *, const _dbTechNonDefaultRule & r );
    ~_dbTechNonDefaultRule();

    _dbTech * getTech();
    _dbBlock * getBlock();

    int operator==( const _dbTechNonDefaultRule & rhs ) const;
    int operator!=( const _dbTechNonDefaultRule & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbTechNonDefaultRule & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbTechNonDefaultRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechNonDefaultRule & rule );
dbIStream & operator>>( dbIStream & stream, _dbTechNonDefaultRule & rule );

END_NAMESPACE_ADS

#endif
