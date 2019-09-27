#ifndef ADS_DB_SWIRE_H
#define ADS_DB_SWIRE_H

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

class _dbSWire;
class _dbNet;
class _dbSBox;
class dbDiff;

struct _dbSWireFlags
{
    dbWireType::Value _wire_type     : 6;
    uint              _spare_bits    : 26;
};

class _dbSWire : public dbObject
{
public:
    _dbSWireFlags     _flags;
    dbId<_dbNet>      _net;
    dbId<_dbNet>      _shield;
    dbId<_dbSBox>     _wires;
    dbId<_dbSWire>    _next_swire;

    _dbSWire( _dbDatabase * ) 
    {
        _flags._wire_type = dbWireType::NONE;
        _flags._spare_bits = 0;
    }

    _dbSWire( _dbDatabase *, const _dbSWire & s )
        : _flags(s._flags),
          _net(s._net),
          _shield(s._shield),
          _wires(s._wires),
          _next_swire(s._next_swire)
    {
    }
    
    ~_dbSWire() 
    {
    }

    int operator==( const _dbSWire & rhs ) const;
    int operator!=( const _dbSWire & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbSWire & rhs ) const;
    
    void differences( dbDiff & diff, const char * field, const _dbSWire & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline dbOStream & operator<<( dbOStream & stream, const _dbSWire & wire )
{
    uint *bit_field = (uint *) &wire._flags;
    stream << *bit_field;
    stream << wire._net;
    stream << wire._shield;
    stream << wire._wires;
    stream << wire._next_swire;

    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbSWire & wire )
{
    uint *bit_field = (uint *) &wire._flags;
    stream >> *bit_field;
    stream >> wire._net;
    stream >> wire._shield;
    stream >> wire._wires;
    stream >> wire._next_swire;

    return stream;
}


END_NAMESPACE_ADS
#endif
