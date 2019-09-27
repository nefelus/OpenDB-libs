#ifndef ADS_DB_BLOCK_SET_H
#define ADS_DB_BLOCK_SET_H

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class dbBlock;

template <>
class dbSetIterator<dbBlock>
{
    friend class dbSet<dbBlock>;
    
    dbIterator * _itr;
    uint         _cur;
    dbObject *    _parent;

    dbSetIterator( dbIterator * itr, uint id, dbObject * parent )
    {
        _itr = itr;
        _cur = id;
        _parent = parent;
    }

  public:
    dbSetIterator()
    {
        _itr = NULL;
        _cur = 0;
        _parent = NULL;
    }

    dbSetIterator( const dbSetIterator & it )
    {
        _itr = it._itr;
        _cur = it._cur;
        _parent = it._parent;
    }

    bool operator==( const dbSetIterator<dbBlock> & it )
    {
        return (_itr == it._itr) && (_cur == it._cur) && (_parent == it._parent);
    }

    bool operator!=( const dbSetIterator<dbBlock> & it )
    {
        return (_itr != it._itr) || (_cur != it._cur) || (_parent != it._parent);
    }
    
    dbBlock * operator*()
    {
        return (dbBlock *) _itr->getObject(_cur, _parent);
    }

    dbBlock * operator->()
    {
        return (dbBlock *) _itr->getObject(_cur, _parent);
    }

    dbSetIterator<dbBlock> & operator++()
    {
        _cur = _itr->next(_cur);
        return *this;
    }

    dbSetIterator<dbBlock> operator++(int)
    {
        dbSetIterator it(*this);
        _cur = _itr->next(_cur);
        return it;
    }
};

template <>
class dbSet<dbBlock>
{
    dbIterator * _itr;
    dbObject *   _parent;
    
  public:
    typedef dbSetIterator<dbBlock> iterator;
    
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

    dbSet( const dbSet<dbBlock> & c )
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
        return iterator( _itr, _itr->begin(_parent), _parent );
    }

    ///
    /// Return an end() iterator.
    ///
    iterator end()
    {
        return iterator( _itr, _itr->end(_parent), _parent );
    }

    ///
    /// Returns the maximum number sequential elements the this set
    /// may iterate.
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
};

END_NAMESPACE_ADS

#endif
