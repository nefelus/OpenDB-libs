#ifndef ADS_DB_WIRE_H
#define ADS_DB_WIRE_H

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

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbWire;
class _dbNet;
class dbDiff;

struct _dbWireFlags
{
    uint _is_global : 1;
    uint _spare_bits : 31;
};

class _dbWire : public dbObject
{
public:
    _dbWireFlags             _flags;
    dbVector<int>            _data;
    dbVector<unsigned char>  _opcodes;
    dbId<_dbNet>             _net;

    _dbWire( _dbDatabase * ) { _flags._is_global = 0; _flags._spare_bits = 0; }

    _dbWire( _dbDatabase *, const _dbWire & w )
        : _flags(w._flags),
          _data(w._data),
          _opcodes(w._opcodes),
          _net(w._net)
        {
        }
        
    ~_dbWire() {}
    
    uint length() { return _opcodes.size(); }

    int operator==( const _dbWire & rhs ) const;
    int operator!=( const _dbWire & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbWire & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbWire & wire );
dbIStream & operator>>( dbIStream & stream, _dbWire & wire );

END_NAMESPACE_ADS

#endif
