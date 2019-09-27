#ifndef ADS_DB_BOX_H
#define ADS_DB_BOX_H

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

class _dbDatabase;
class _dbTechVia;
class _dbTechLayer;
class _dbVia;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbBoxFlags
{
    dbBoxOwner::Value  _owner_type   : 4;
    uint               _visited      : 1;
    uint               _mark         : 1;
    uint               _is_tech_via  : 1;
    uint               _is_block_via : 1;
    uint               _layer_id     : 8;
    uint               _via_id       : 16;
};

class _dbBox : public dbObject
{
  public:

    enum Type { BLOCK_VIA, TECH_VIA, BOX };
    
    // PERSISTANT-MEMBERS
    _dbBoxFlags        _flags;
    adsRect            _rect;
    uint               _owner;
    dbId<_dbBox>       _next_box;

    _dbBox( _dbDatabase * );
    _dbBox( _dbDatabase *, const _dbBox & b );
    ~_dbBox();
    int operator==( const _dbBox & rhs ) const;
    int operator!=( const _dbBox & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbBox & rhs ) const;
    int equal( const _dbBox & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbBox & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    _dbTechLayer * getTechLayer() const;
    _dbTechVia * getTechVia() const;
    _dbVia * getBlockVia() const;

    void getViaXY( int & x, int & y ) const;

    Type getType() const
    {
        if ( _flags._is_tech_via )
            return TECH_VIA;

        if ( _flags._is_block_via )
            return BLOCK_VIA;

        return BOX;
    }
};

inline _dbBox::_dbBox( _dbDatabase * )
{
    _flags._owner_type = dbBoxOwner::UNKNOWN;
    _flags._is_tech_via = 0;
    _flags._is_block_via = 0;
    _flags._layer_id = 0;
    _flags._via_id = 0;
    _flags._visited = 0;
    _flags._mark = 0;
}

inline _dbBox::_dbBox( _dbDatabase *, const _dbBox & b )
        : _flags( b._flags ),
          _rect( b._rect ),
          _owner( b._owner ),
          _next_box( b._next_box )
{
}

inline _dbBox::~_dbBox()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbBox & box )
{
    uint * bit_field = (uint *) & box._flags;
    stream << *bit_field;
    stream << box._rect;
    stream << box._owner;
    stream << box._next_box;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbBox & box )
{
    uint * bit_field = (uint *) & box._flags;
    stream >> *bit_field;
    stream >> box._rect;
    stream >> box._owner;
    stream >> box._next_box;
    return stream;
}

END_NAMESPACE_ADS

#endif
