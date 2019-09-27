#ifndef ADS_DB_INT_HASH_TABLE_H
#define ADS_DB_INT_HASH_TABLE_H

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
///     uint          _id
///     dbId<T>       _next_entry
///
//////////////////////////////////////////////////////////
template <class T>
class dbIntHashTable
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

    dbIntHashTable();
    dbIntHashTable( const dbIntHashTable<T> & t );
    ~dbIntHashTable();
    int operator==( const dbIntHashTable<T> & rhs ) const;
    int operator!=( const dbIntHashTable<T> & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const dbIntHashTable<T> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    void setTable( dbTable<T> * table ) { _obj_tbl = table; }
    
    T * find( register uint id );
    int hasMember( register uint id );
    void insert( T * object );
    void remove( T * object );
};

template <class T> dbOStream & operator<<( dbOStream & stream, const dbIntHashTable<T> & table );
template <class T> dbIStream & operator>>( dbIStream & stream, dbIntHashTable<T> & table );

END_NAMESPACE_ADS

#endif
