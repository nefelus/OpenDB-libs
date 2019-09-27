#ifndef ADS_DB_TARGET_H
#define ADS_DB_TARGET_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTechLayer;
class _dbTarget;
class _dbDatabase;
class _dbMTerm;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbTargetFlags
{
    uint             _spare_bits : 32;
};

class _dbTarget : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTargetFlags     _flags;
    adsPoint           _point;
    dbId<_dbMTerm>     _mterm;
    dbId<_dbTechLayer> _layer;
    dbId<_dbTarget>    _next;
    
    _dbTarget( _dbDatabase *, const _dbTarget & t );
    _dbTarget( _dbDatabase * );
    ~_dbTarget();

    int operator==( const _dbTarget & rhs ) const;
    int operator!=( const _dbTarget & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTarget & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTarget::_dbTarget( _dbDatabase *, const _dbTarget & t )
        : _flags(t._flags),
          _point(t._point),
          _mterm(t._mterm),
          _layer(t._layer),
          _next(t._next)
{
}

inline _dbTarget::_dbTarget( _dbDatabase * )
{
    _flags._spare_bits = 0;
}

inline _dbTarget::~_dbTarget()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbTarget & target )
{
    uint * bit_field = (uint *) &target._flags;
    stream << *bit_field;
    stream << target._point;
    stream << target._mterm;
    stream << target._layer;
    stream << target._next;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbTarget & target )
{
    uint * bit_field = (uint *) &target._flags;
    stream >> *bit_field;
    stream >> target._point;
    stream >> target._mterm;
    stream >> target._layer;
    stream >> target._next;
    return stream;
}

END_NAMESPACE_ADS

#endif
