inline unsigned int hash_string( const char * str )
{
    unsigned int hash = 0;
    int c;

    while((c = *str++) != '\0')
        hash = c + (hash << 6) + (hash << 16) - hash;

    return hash;
}

inline int streq( register const char * s1, register const char * s2 )
{
  do_str_cmp:
    register char c1 = *s1++;
    register char c2 = *s2++;
    
    if ( c1 != c2 )
        return 0;
    
    if ( c1 == 0 )
        return 1;
    
    goto do_str_cmp;
    return 0;
}

inline hashTable::~hashTable()
{
    unsigned int sz = _hash_tbl.size();

    unsigned int i;

    for( i = 0; i < sz; ++i )
        if ( _hash_tbl[i]._key )
            free( (void *) _hash_tbl[i]._key );
}

inline hashTable::hashTable()
{
}

inline hashTable::hashTable(int size)
{
    int i;
    for( i = 1; i < size; i *= 2 );
    _hash_tbl.resize(i);
}

inline void hashTable::setSize(int size)
{
    int i;
    for( i = 1; i < size; i *= 2 );
    _hash_tbl.resize(i);
}

inline void
hashTable::resizeTable()
{
    unsigned int sz = _hash_tbl.size();
    unsigned int new_sz = sz * 2;

    std::vector<hashEntry> old_tbl = _hash_tbl;
    
    _hash_tbl.resize(new_sz);

    std::vector<hashEntry>::iterator itr;

    for( itr = _hash_tbl.begin(); itr != _hash_tbl.end(); ++itr )
    {
        hashEntry & e = *itr;
        e._key = NULL;
        e._value = 0;
    }
    
    for( itr = old_tbl.begin(); itr != old_tbl.end(); ++itr )
    {
        hashEntry & e = *itr;
        insert( e._key, e._value, false );
    }
}

inline void
hashTable::insert( const char * key, int value, bool alloc_key )
{
    unsigned int sz = _hash_tbl.size();

    if ( sz == 0 )
        _hash_tbl.resize(1);

    int hmask = sz - 1;
    unsigned int count;
    unsigned int idx = hash_string(key) & hmask;

    for( count = 0; _hash_tbl[idx]._key != NULL; ++count )
    {
        if ( count == sz ) // table-full
        {
            resizeTable();
            count = 0;
            sz = _hash_tbl.size();
            hmask = sz - 1;
            idx = hash_string(key) & hmask;
        }
        else
        {
            ++idx;

            if ( idx == sz )
                idx = 0;
        }
    }

/*
    if ( z != idx )
        printf("K(%s) v(%d) h(%d) %d\n", key, value, idx, z );
*/
    
    if ( alloc_key )
        _hash_tbl[idx]._key = strdup(key);
    else
        _hash_tbl[idx]._key = key;
    
    _hash_tbl[idx]._value = value;
}

inline bool
hashTable::find( const char * key, int & value ) const
{
    unsigned int sz = _hash_tbl.size();
    unsigned int hmask = sz - 1;
    unsigned int idx = hash_string(key) & hmask;
    unsigned int count;

    for( count = 0; (_hash_tbl[idx]._key != NULL) && (count < sz); ++count )
    {
        const hashEntry & e = _hash_tbl[idx];
        
        if ( streq( e._key, key ) )
        {
            value = e._value;
            return true;
        }
        
        if ( ++idx == sz )
            idx = 0;
    }
    
    return false;
}
