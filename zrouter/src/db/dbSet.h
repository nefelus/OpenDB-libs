#ifndef ADS_DB_SET_H
#define ADS_DB_SET_H

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbSet;

template <class T>
class dbSetIterator
{
    friend class dbSet<T>;
    
    dbIterator * _itr;
    uint         _cur;

    dbSetIterator( dbIterator * itr, uint id );

  public:
    dbSetIterator();
    dbSetIterator( const dbSetIterator & it );

    bool operator==( const dbSetIterator<T> & it );
    bool operator!=( const dbSetIterator<T> & it );
    
    T * operator*();
    T * operator->();
    dbSetIterator<T> & operator++();
    dbSetIterator<T> operator++(int);
};

///
/// The dbSet class provides an abstration of database objects collections.
///
/// There are two types of sets; sequential and non-sequential.
///
/// Sequential set iterators have the following property:
///
///     Let (o1,o2,o3,...,oN) be the objects iterated of a set, where 1..N represents the iteration order.
///
///     Then the following condition holds:
///
///        0 < o1->getId() < o2->getId() < o3->getId() < ... < oN->getId()
///
///     Because of object deletion, there may be gaps between the object identifiers in the iterated set:
///
///        Given: ... < oi->getId() < oj->getId() < ...
///        Then: ... < (oi->getId() + 1) <= oj->getId() < ...
///
/// Non-Sequential set iterators do not have any ordering property.
///
template <class T>
class dbSet
{
    dbIterator * _itr;
    dbObject *   _parent;
    
  public:
    typedef dbSetIterator<T> iterator;
    
    dbSet()
    {
        _itr = NULL;
        _parent = NULL;
    }

    dbSet( dbObject * parent, dbIterator * itr )
    {
        _parent = parent;
        _itr = itr;
    }

    dbSet( const dbSet<T> & c )
    {
        _itr = c._itr;
        _parent = c._parent;
    }

    ///
    /// Returns the number of items in this set.
    ///
    uint size()
    {
        return _itr->size(_parent);
    }

    ///
    /// Return a begin() iterator.
    ///
    iterator begin()
    {
        return iterator( _itr, _itr->begin(_parent) );
    }

    ///
    /// Return an end() iterator.
    ///
    iterator end()
    {
        return iterator( _itr, _itr->end(_parent) );
    }

    ///
    /// If this set is sequential, this function will return the database identifier of the
    /// "Nth" iterated element (see the class description above):
    ///
    ///      returns oN->getId()
    ///
    /// If this set is non-sequential then it returns 0.
    ///
    uint sequential()
    {
        return _itr->sequential();
    }

    ///
    /// Returns true if this set is reversible.
    ///
    bool reversible()
    {
        return _itr->reversible();
    }

    ///
    /// Returns true if the is iterated in the reverse order that
    /// it was created.
    ///
    bool orderReversed()
    {
        return _itr->orderReversed();
    }

    ///
    /// Reverse the order of this set.
    ///
    void reverse()
    {
        _itr->reverse(_parent);
    }

    ///
    /// Returns true if set is empty
    ///
    bool empty() { return begin() == end(); }
};

template <class T>
inline dbSetIterator<T>::dbSetIterator()
{
    _itr = NULL;
    _cur = 0;
}

template <class T>
inline dbSetIterator<T>::dbSetIterator( dbIterator * itr, uint id )
{
    _itr = itr;
    _cur = id;
}

template <class T>
inline dbSetIterator<T>::dbSetIterator( const dbSetIterator & it )
{
    _itr = it._itr;
    _cur = it._cur;
}

template <class T>
inline bool dbSetIterator<T>::operator==( const dbSetIterator & it )
{
    return (_itr == it._itr) && (_cur == it._cur);
}

template <class T>
inline bool dbSetIterator<T>::operator!=( const dbSetIterator & it )
{
    return (_itr != it._itr) || (_cur != it._cur);
}
    
template <class T>
inline T * dbSetIterator<T>::operator*()
{
    return (T *) _itr->getObject(_cur);
}

template <class T>
inline T * dbSetIterator<T>::operator->()
{
    return (T *) _itr->getObject(_cur);
}

template <class T>
inline dbSetIterator<T> & dbSetIterator<T>::operator++()
{
    _cur = _itr->next(_cur);
    return *this;
}
  
template <class T>
inline dbSetIterator<T> dbSetIterator<T>::operator++(int)
{
    dbSetIterator it(*this);
    _cur = _itr->next(_cur);
    return it;
}

END_NAMESPACE_ADS

// Specialization definitions
#include "dbCCSegSet.h"
#include "dbBlockSet.h"

#endif
