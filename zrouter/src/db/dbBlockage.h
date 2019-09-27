#ifndef ADS_DB_BLOCKAGE_H
#define ADS_DB_BLOCKAGE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbInst;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbBlockageFlags
{
    uint _pushed_down : 1;
    uint _spare_bits  : 31;
};

class _dbBlockage : public dbObject
{
  public:
    _dbBlockageFlags   _flags;
    dbId<_dbInst>      _inst;
    dbId<_dbBox>       _bbox;

    _dbBlockage(_dbDatabase * db );
    _dbBlockage(_dbDatabase * db, const _dbBlockage & b );
    ~_dbBlockage();

    _dbInst * getInst();
    _dbBox * getBBox() const;

    int operator==( const _dbBlockage & rhs ) const;
    int operator!=( const _dbBlockage & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbBlockage & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbBlockage & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbBlockage::_dbBlockage( _dbDatabase * )
{
    _flags._pushed_down = 0;
    _flags._spare_bits = 0;
}

inline _dbBlockage::_dbBlockage( _dbDatabase *, const _dbBlockage & b )
        : _flags( b._flags),
          _inst( b._inst ),
          _bbox( b._bbox )
{
}

inline _dbBlockage::~_dbBlockage()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbBlockage & blockage )
{
    uint * bit_field = (uint *) &blockage._flags;
    stream << *bit_field;
    stream << blockage._inst;
    stream << blockage._bbox;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbBlockage & blockage )
{
    uint * bit_field = (uint *) &blockage._flags;
    stream >> *bit_field;
    stream >> blockage._inst;
    stream >> blockage._bbox;
    return stream;
}

END_NAMESPACE_ADS

#endif
