#ifndef ADS_DB_SITE_H
#define ADS_DB_SITE_H

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

#ifndef ADS_DB_HASH_TABLE_H
#include "dbHashTable.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;
class dbIStream;
class dbOStream;
class dbDiff;

struct dbSiteFlags
{
    uint  _x_symmetry   : 1;
    uint  _y_symmetry   : 1;
    uint  _R90_symmetry : 1;
    dbSiteClass::Value _class : 4;
    uint  _spare_bits   : 25;
};

class _dbSite : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    dbSiteFlags            _flags;
    char *                 _name;
    uint                   _height;
    uint                   _width;
    dbId<_dbSite>          _next_entry;

    _dbSite(_dbDatabase *, const _dbSite & s );
    _dbSite(_dbDatabase * );
    ~_dbSite();

    int operator==( const _dbSite & rhs ) const;
    int operator!=( const _dbSite & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbSite & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline dbOStream & operator<<( dbOStream & stream, const _dbSite & site )
{
    uint *bit_field = (uint *) &site._flags;
    stream << *bit_field;
    stream << site._name;
    stream << site._height;
    stream << site._width;
    stream << site._next_entry;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbSite & site )
{
    uint *bit_field = (uint *) &site._flags;
    stream >> *bit_field;
    stream >> site._name;
    stream >> site._height;
    stream >> site._width;
    stream >> site._next_entry;
    return stream;
}

END_NAMESPACE_ADS

#endif
