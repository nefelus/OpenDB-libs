#ifndef ADS_DB_HASH_TABLE_H
#define ADS_DB_HASH_TABLE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_PAGED_VECTOR_H
#include "dbPagedVector.h"
#endif

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;
class dbDiff;
template <class T> class dbTable;

//////////////////////////////////////////////////////////
///
/// dbHashTable - hash table to hash named-objects.
///
/// Each object must have the following "named" fields:
///
///     char *        _name
///     dbId<T>       _next_entry
///
//////////////////////////////////////////////////////////
template <class T>
class dbHashTable
{
  public:
    enum Params { CHAIN_LENGTH=4 };

    // PERSISTANT-MEMBERS
    dbPagedVector< dbId<T>, 256, 8>  _hash_tbl;
    uint                             _num_entries;

    // NON-PERSISTANT-MEMBERS
    dbTable<T> *                     _obj_tbl;

    void growTable();
    void shrinkTable();

    dbHashTable();
    dbHashTable( const dbHashTable<T> & table );
    ~dbHashTable();
    int operator==( const dbHashTable<T> & rhs ) const;
    int operator!=( const dbHashTable<T> & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const dbHashTable<T> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    
    void setTable( dbTable<T> * table ) { _obj_tbl = table; }
    T * find( register const char * name );
    int hasMember( register const char * name );
    void insert( T * object );
    void remove( T * object );
};

template <class T> dbOStream & operator<<( dbOStream & stream, const dbHashTable<T> & table );
template <class T> dbIStream & operator>>( dbIStream & stream, dbHashTable<T> & table );

END_NAMESPACE_ADS

#endif
