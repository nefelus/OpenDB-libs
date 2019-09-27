#ifndef ADS_DEFIN_HASH_H
#define ADS_DEFIN_HASH_H

#include <vector>

struct hashEntry
{
    const char * _key;
    int          _value;
    hashEntry() { _key = NULL; _value = 0; }
};
    
class hashTable
{
    std::vector<hashEntry> _hash_tbl;
    void resizeTable();
    void insert( const char * key, int value, bool alloc_key );
    
public:
    ~hashTable();
    hashTable();
    hashTable(int size);

    void setSize(int size);
    void insert( const char * key, int value ) { insert( key, value, true ); }
    bool find( const char * key, int & value ) const;
};

#include "hash.hpp"

#endif
