#include "dbIntHashTable.h"

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

BEGIN_NAMESPACE_ADS

inline uint hash_int( register uint id )
{
    register int key = (int) id;
    key += ~(key << 15);
    key ^=  (key >> 10);
    key +=  (key << 3);
    key ^=  (key >> 6);
    key += ~(key << 11);
    key ^=  (key >> 16);
    return (uint) key;
}

template <class T>
dbIntHashTable<T>::dbIntHashTable()
{
    _obj_tbl = NULL;
    _num_entries = 0;
}

template <class T>
dbIntHashTable<T>::dbIntHashTable( const dbIntHashTable<T> & t )
        : _hash_tbl( t._hash_tbl ),
          _num_entries( t._num_entries ),
          _obj_tbl( t._obj_tbl )
{
}

template <class T>
dbIntHashTable<T>::~dbIntHashTable()
{
}

template <class T>
int dbIntHashTable<T>::operator==( const dbIntHashTable<T> & rhs ) const
{
    if ( _num_entries != rhs._num_entries )
        return false;

    if ( _hash_tbl != rhs._hash_tbl )
        return false;

    return true;
}

template <class T>
void dbIntHashTable<T>::growTable()
{
    uint sz = _hash_tbl.size();
    dbId<T> entries;
    uint i;

    for( i = 0; i < sz; ++i )
    {
        dbId<T> cur = _hash_tbl[i];
    
        while( cur != 0 )
        {
	    T * entry = _obj_tbl->getPtr(cur);
            dbId<T> next = entry->_next_entry;
            entry->_next_entry = entries;
            entries = entry->getOID();
            cur = next;
        }

        _hash_tbl[i] = 0;
    }

    // double the size of the hash-table
    dbId<T> nullId;
    for( i = 0; i < sz; ++i )
        _hash_tbl.push_back( nullId );

    // reinsert the entries
    sz = _hash_tbl.size() - 1;
    dbId<T> cur = entries;
    
    while( cur != 0 )
    {
        T * entry = _obj_tbl->getPtr(cur);
        dbId<T> next = entry->_next_entry;
        uint hid = hash_int(entry->_id) & sz;
        dbId<T> & e = _hash_tbl[hid];
        entry->_next_entry = e;
        e = entry->getOID();
        cur = next;
    }
}

template <class T>
void dbIntHashTable<T>::shrinkTable()
{
    uint sz = _hash_tbl.size();
    dbId<T> entries;
    uint i;

    for( i = 0; i < sz; ++i )
    {
        dbId<T> cur = _hash_tbl[i];
    
        while( cur != 0 )
        {
	    T * entry = _obj_tbl->getPtr(cur);
            dbId<T> next = entry->_next_entry;
            entry->_next_entry = entries;
            entries = entry->getOID();
            cur = next;
        }

        _hash_tbl[i] = 0;
    }

    //TODO: add method to dbPagedVector to resize the table
    _hash_tbl.clear();
    sz >>= 1;
    
    // halve the size of the hash-table
    dbId<T> nullId;
    for( i = 0; i < sz; ++i )
        _hash_tbl.push_back( nullId );

    sz -= 1;
    // reinsert the entries
    dbId<T> cur = entries;
    
    while( cur != 0 )
    {
        T * entry = _obj_tbl->getPtr(cur);
        dbId<T> next = entry->_next_entry;
        uint hid = hash_int(entry->_id) & sz;
        dbId<T> & e = _hash_tbl[hid];
        entry->_next_entry = e;
        e = entry->getOID();
        cur = next;
    }
}

template <class T>
void dbIntHashTable<T>::insert( T * object )
{
    ++_num_entries;
    uint sz = _hash_tbl.size();

    if ( sz == 0 )
    {
        dbId<T> nullId;
        _hash_tbl.push_back( nullId );
        sz = 1;
    }
    else
    {
        uint r = _num_entries / sz;
    
        if ( r > CHAIN_LENGTH )
        {
            growTable();
            sz = _hash_tbl.size();
        }
    }

    uint hid = hash_int(object->_id) & (sz-1);
    dbId<T> & e = _hash_tbl[hid];
    object->_next_entry = e;
    e = object->getOID();
}

template <class T>
T * dbIntHashTable<T>::find( register uint id )
{
    uint sz = _hash_tbl.size();

    if ( sz == 0 )
        return 0;

    uint hid = hash_int(id) & (sz-1);
    dbId<T> cur = _hash_tbl[hid];
    
    while( cur != 0 )
    {
        T * entry = _obj_tbl->getPtr(cur);

        if ( entry->_id == id )
            return entry;
        
        cur = entry->_next_entry;
    }

    return NULL;
}

template <class T>
int dbIntHashTable<T>::hasMember( register uint id )
{
    uint sz = _hash_tbl.size();

    if ( sz == 0 )
        return false;

    uint hid = hash_int(id) & (sz-1);
    dbId<T> cur = _hash_tbl[hid];
    
    while( cur != 0 )
    {
        T * entry = _obj_tbl->getPtr(cur);

        if ( entry->_id == id )
            return true;
        
        cur = entry->_next_entry;
    }

    return false;
}

template <class T>
void dbIntHashTable<T>::remove( T * object )
{
    uint sz = _hash_tbl.size();
    uint hid = hash_int(object->_id) & (sz-1);
    dbId<T> cur = _hash_tbl[hid];
    dbId<T> prev;
    
    while( cur != 0 )
    {
        T * entry = _obj_tbl->getPtr(cur);

        if ( entry == object )
        {
            if ( prev == 0 )
                _hash_tbl[hid] = entry->_next_entry;
            else
            {
                T * p = _obj_tbl->getPtr(prev);
                p->_next_entry = entry->_next_entry;
            }

            --_num_entries;

            uint r = (_num_entries + _num_entries/10) / sz;

            if ( (r < (CHAIN_LENGTH >> 1)) && (sz > 1) )
                shrinkTable();
            
            return;
        }
        
        prev = cur;
        cur = entry->_next_entry;
    }
}

template <class T>
dbOStream & operator<<( dbOStream & stream, const dbIntHashTable<T> & table )
{
    stream << table._hash_tbl;
    stream << table._num_entries;
    return stream;
}

template <class T>
dbIStream & operator>>( dbIStream & stream, dbIntHashTable<T> & table )
{
    stream >> table._hash_tbl;
    stream >> table._num_entries;
    return stream;
}

template <class T>
dbDiff & operator<<( dbDiff & diff, const dbIntHashTable<T>  & table )
{
    return diff;
}

template <class T>
void dbIntHashTable<T>::differences( dbDiff & diff, const char * field, const dbIntHashTable<T> & rhs ) const
{
    diff.report("<> %s", field );
    diff.increment();
    DIFF_FIELD(_num_entries)
    DIFF_VECTOR(_hash_tbl);
    diff.decrement();
}

template <class T>
void dbIntHashTable<T>::out( dbDiff & diff, char side, const char * field ) const
{
    diff.report("%c %s", side, field );
    diff.increment();
    DIFF_OUT_FIELD(_num_entries)
    DIFF_OUT_VECTOR(_hash_tbl);
    diff.decrement();
}

END_NAMESPACE_ADS
