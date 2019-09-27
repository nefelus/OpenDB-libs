#ifndef ADS_DB_SBOX_H
#define ADS_DB_SBOX_H

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

#ifndef ADS_DB_BOX_H
#include "dbBox.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbSBoxFlags
{
    dbWireShapeType::Value  _wire_type   : 6;
    uint                    _direction   : 2; // 0 = undefiend, 1 = horizontal, 2 = vertical
    uint                    _spare_bits  : 24;
};

class _dbSBox : public _dbBox
{
  public:
    // PERSISTANT-MEMBERS
    _dbSBoxFlags       _sflags;

    _dbSBox( _dbDatabase *, const _dbSBox & b );
    _dbSBox( _dbDatabase * );
    ~_dbSBox();

    int operator==( const _dbSBox & rhs ) const;
    int operator!=( const _dbSBox & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbSBox & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbSBox & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    int equal( const _dbSBox & rhs ) const;
};

inline _dbSBox::_dbSBox( _dbDatabase *  db, const _dbSBox & b )
        : _dbBox( db, b ),
          _sflags(b._sflags)
{
}

inline _dbSBox::_dbSBox( _dbDatabase *  db ) : _dbBox( db )
{
    _sflags._wire_type = dbWireShapeType::COREWIRE;
    _sflags._direction = 0;
    _sflags._spare_bits = 0;
}

inline _dbSBox::~_dbSBox()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbSBox & box )
{
    stream << (_dbBox &) box;
    uint * bit_field = (uint *) & box._sflags;
    stream << *bit_field;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbSBox & box )
{
    stream >> (_dbBox &) box;
    uint * bit_field = (uint *) & box._sflags;
    stream >> *bit_field;
    return stream;
}

END_NAMESPACE_ADS

#endif
