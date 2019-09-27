#ifndef ADS_DB_ROW_H
#define ADS_DB_ROW_H

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

template <class T> class dbTable;
class dbIStream;
class dbOStream;
class dbSite;
class dbLib;
class dbDiff;

struct dbRowFlags
{
    dbOrientType::Value _orient    : 4;
    dbRowDir::Value     _dir       : 2;
    uint                _spare_bits : 26;
};

class _dbRow : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    dbRowFlags    _flags;
    char *        _name;
    dbId<dbLib>   _lib;
    dbId<dbSite>  _site;
    int           _x;
    int           _y;
    int           _site_cnt;
    int           _spacing;

    _dbRow( _dbDatabase *, const _dbRow & r );
    _dbRow( _dbDatabase * );
    ~_dbRow();

    int operator==( const _dbRow & rhs ) const;
    int operator!=( const _dbRow & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbRow & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbRow & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbRow::_dbRow( _dbDatabase *, const _dbRow & r )
        : _flags(r._flags),
          _name(NULL),
          _lib(r._lib),
          _site(r._site),
          _x(r._x),
          _y(r._y),
          _site_cnt(r._site_cnt),
          _spacing(r._spacing)
{
    if ( r._name )
    {
        _name = strdup(r._name);
        ZALLOCATED(_name);
    }
}

inline _dbRow::_dbRow( _dbDatabase * )
{
    _flags._orient = dbOrientType::R0;
    _flags._dir = dbRowDir::HORIZONTAL;
    _flags._spare_bits = 0;
    _name = NULL;
    _x = 0;
    _y = 0;
    _site_cnt = 0;
    _spacing = 0;
}

inline _dbRow::~_dbRow()
{
    if ( _name )
        free( (void *) _name );
}

inline dbOStream & operator<<( dbOStream & stream, const _dbRow & row )
{
    uint *bit_field = (uint *) &row._flags;
    stream << *bit_field;
    stream << row._name;
    stream << row._lib;
    stream << row._site;
    stream << row._x;
    stream << row._y;
    stream << row._site_cnt;
    stream << row._spacing;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbRow & row )
{
    uint *bit_field = (uint *) &row._flags;
    stream >> *bit_field;
    stream >> row._name;
    stream >> row._lib;
    stream >> row._site;
    stream >> row._x;
    stream >> row._y;
    stream >> row._site_cnt;
    stream >> row._spacing;
    return stream;
}

END_NAMESPACE_ADS

#endif
