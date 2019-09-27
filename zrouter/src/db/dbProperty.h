#ifndef ADS_DB_PROPERTY_H
#define ADS_DB_PROPERTY_H

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
class _dbDatabase;
class _dbProperty;
class dbPropertyItr;
class dbIStream;
class dbOStream;
class dbDiff;
class _dbNameCache;

enum _PropTypeEnum 
{
    // Do not change the order of this enum.
    DB_STRING_PROP  = 0,
    DB_BOOL_PROP    = 1,
    DB_INT_PROP     = 2,
    DB_DOUBLE_PROP  = 3
};

struct _dbPropertyFlags
{
    _PropTypeEnum  _type         : 4;
    uint           _owner_type   : 8;
    uint           _spare_bits   : 20;
};

class _dbProperty : public dbObject
{
  public:
    _dbPropertyFlags  _flags;
    uint              _name;
    dbId<_dbProperty> _next;
    uint              _owner;
    
    union
    {
        char *       _str_val;
        uint         _bool_val;
        int          _int_val;
        double       _double_val;
    } _value;

    _dbProperty( _dbDatabase * );
    _dbProperty( _dbDatabase *, const _dbProperty & n );
    ~_dbProperty();

    int operator==( const _dbProperty & rhs ) const;
    int operator!=( const _dbProperty & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbProperty & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbProperty & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    static dbTable<_dbProperty> * getPropTable( dbObject * object );
    static _dbNameCache * getNameCache( dbObject * object );
    static dbPropertyItr * getItr( dbObject * object );
    static _dbProperty * createProperty( dbObject * object, const char * name, _PropTypeEnum type );
};

dbOStream & operator<<( dbOStream & stream, const _dbProperty & prop );
dbIStream & operator>>( dbIStream & stream, _dbProperty & prop );

END_NAMESPACE_ADS

#endif
