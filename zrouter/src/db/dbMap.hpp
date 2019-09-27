#ifndef ADS_DB_MAP_HPP
#define ADS_DB_MAP_HPP

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T, class D>
inline dbMap< T, D >::dbMap( const dbSet<T> & set )
{
    _set = set;

    // Use a vector if this set represents a sequential iterator
    if ( _set.sequential() )
    {
        _vector = new std::vector<D>( _set.sequential() + 1 );
        _map = NULL;
    }

    // Use a map if this set represents random iterator
    else
    {
        _vector = NULL;
        _map = new std::map<T *, D>;
        typename dbSet<T>::iterator itr;

        for( itr = _set.begin(); itr != _set.end(); ++itr )
        {
            T * object = *itr;
            (*_map)[object] = D();
        }
    }
}

template <class T, class D>
inline dbMap<T,D>::~dbMap()
{
    if ( _map )
        delete _map;

    if ( _vector )
        delete _vector;
}

template <class T, class D>
inline const D & dbMap<T,D>::operator[]( T * object ) const
{
    if ( _map )
    {
        typename std::map<T *,D>::const_iterator itr = _map->find(object);
        ZASSERT( itr != _map->end() );
        return (*itr).second;
    }

    uint idx = object->getId();
    ZASSERT(idx && (idx < _vector->size()));
    return (*_vector)[idx];
}

template <class T, class D>
inline D & dbMap<T,D>::operator[]( T * object )
{
    if ( _map )
    {
        typename std::map<T *,D>::iterator itr = _map->find(object);
        ZASSERT( itr != _map->end() );
        return (*itr).second;
    }

    uint idx = object->getId();
    ZASSERT(idx && (idx < _vector->size()));
    return (*_vector)[idx];
}

END_NAMESPACE_ADS

#endif
