#ifndef ADS_DB_NAME_CACHE_H
#define ADS_DB_NAME_CACHE_H

#ifndef ADS_DB_HASH_TABLE_H
#include "dbHashTable.h"
#endif

BEGIN_NAMESPACE_ADS

class dbOStream;
class dbIStream;
class dbDiff;
class _dbName;
class _dbDatabase;

class _dbNameCache
{
  public:
    dbTable<_dbName> *   _name_tbl;
    dbHashTable<_dbName> _name_hash;
    
    _dbNameCache( _dbDatabase * db, dbObject * owner, dbObjectTable * (dbObject::*m)(dbObjectType) );
    _dbNameCache( _dbDatabase * db, dbObject * owner, const _dbNameCache & cache );
    ~_dbNameCache();

    int operator==( const _dbNameCache & rhs ) const;
    int operator!=( const _dbNameCache & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbNameCache & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    // Find the name, returns 0 if the name does not exists.
    uint findName( const char * name );

    // Add this name to the table if it does not exists.
    // increment the reference count to this name
    uint addName( const char * name );

    // Remove this name to the table if the ref-cnt is 0
    void removeName( uint id );

    // Remove the string this id represents
    const char * getName( uint id );
};

dbOStream & operator<<( dbOStream & stream, const _dbNameCache & net );
dbIStream & operator>>( dbIStream & stream, _dbNameCache & net );

END_NAMESPACE_ADS
#endif
