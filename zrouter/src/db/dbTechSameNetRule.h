#ifndef ADS_DB_TECH_SAME_NET_RULE_H
#define ADS_DB_TECH_SAME_NET_RULE_H

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
class _dbTechSameNet;
class _dbTechNonDefaultRule;
class _dbTechLayer;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTechSameNetRuleFlags
{
    uint  _stack         : 1;
    uint  _spare_bits    : 31;
};

class _dbTechSameNetRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechSameNetRuleFlags     _flags;
    uint                        _spacing;
    dbId<_dbTechLayer>          _layer_1;
    dbId<_dbTechLayer>          _layer_2;

    _dbTechSameNetRule( _dbDatabase *, const _dbTechSameNetRule & r );
    _dbTechSameNetRule( _dbDatabase * );
    ~_dbTechSameNetRule();

    int operator==( const _dbTechSameNetRule & rhs ) const;
    int operator!=( const _dbTechSameNetRule & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechSameNetRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTechSameNetRule::_dbTechSameNetRule( _dbDatabase *, const _dbTechSameNetRule & r )
        : _flags(r._flags),
          _spacing(r._spacing),
          _layer_1(r._layer_1),
          _layer_2(r._layer_2)
{
}

inline _dbTechSameNetRule::_dbTechSameNetRule( _dbDatabase * )
{
    _flags._stack = 0;
    _flags._spare_bits = 0;
    _spacing  = 0;
}

inline _dbTechSameNetRule::~_dbTechSameNetRule()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbTechSameNetRule & rule )
{
    uint * bit_field = (uint *) &rule._flags;
    stream << *bit_field;
    stream << rule._spacing;
    stream << rule._layer_1;
    stream << rule._layer_2;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbTechSameNetRule & rule )
{
    uint * bit_field = (uint *) &rule._flags;
    stream >> *bit_field;
    stream >> rule._spacing;
    stream >> rule._layer_1;
    stream >> rule._layer_2;
    return stream;
}

END_NAMESPACE_ADS

#endif
