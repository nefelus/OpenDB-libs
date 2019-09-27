#ifndef ADS_DB_NAME_H
#define ADS_DB_NAME_H

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

class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//
// dbName - This class is used to cache strings that are repeated frequently. For example,
// propery names are repeated frequently.
//
// Net and Instances names are unique and should not use the dbName cache.
//
class _dbName : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    char *        _name;
    dbId<_dbName> _next_entry;
    uint          _ref_cnt;
    
    _dbName( _dbDatabase * );
    _dbName( _dbDatabase *, const _dbName & n );
    ~_dbName();

    int operator==( const _dbName & rhs ) const;
    int operator!=( const _dbName & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbName & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbName & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbName & n );
dbIStream & operator>>( dbIStream & stream, _dbName & n );

END_NAMESPACE_ADS

#endif
