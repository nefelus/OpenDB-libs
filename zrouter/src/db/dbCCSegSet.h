#ifndef ADS_DB_CCSEG_SET_H
#define ADS_DB_CCSEG_SET_H

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class dbCCSeg;

template <>
class dbSetIterator<dbCCSeg>
{
    friend class dbSet<dbCCSeg>;
    
    dbIterator * _itr;
    uint         _cur;
    uint         _pid;

    dbSetIterator( dbIterator * itr, uint id, uint pid )
    {
        _itr = itr;
        _cur = id;
        _pid = pid;
    }

  public:
    dbSetIterator()
    {
        _itr = NULL;
        _cur = 0;
        _pid = 0;
    }

    dbSetIterator( const dbSetIterator & it )
    {
        _itr = it._itr;
        _cur = it._cur;
        _pid = it._pid;
    }

    bool operator==( const dbSetIterator<dbCCSeg> & it )
    {
        return (_itr == it._itr) && (_cur == it._cur) && (_pid == it._pid);
    }

    bool operator!=( const dbSetIterator<dbCCSeg> & it )
    {
        return (_itr != it._itr) || (_cur != it._cur) || (_pid != it._pid);
    }
    
    dbCCSeg * operator*()
    {
        return (dbCCSeg *) _itr->getObject(_cur);
    }

    dbCCSeg * operator->()
    {
        return (dbCCSeg *) _itr->getObject(_cur);
    }

    dbSetIterator<dbCCSeg> & operator++()
    {
        _cur = _itr->next(_cur,_pid);
        return *this;
    }

    dbSetIterator<dbCCSeg> operator++(int)
    {
        dbSetIterator it(*this);
        _cur = _itr->next(_cur,_pid);
        return it;
    }
};

template <>
class dbSet<dbCCSeg>
{
    dbIterator * _itr;
    dbObject *   _parent;
    uint         _pid;
    
  public:
    typedef dbSetIterator<dbCCSeg> iterator;
    
    dbSet()
    {
        _itr = NULL;
        _parent = NULL;
        _pid = 0;
    }

    dbSet( dbObject * parent, dbIterator * itr )
    {
        _parent = parent;
        _itr = itr;
        _pid = parent->getId();
    }

    dbSet( const dbSet<dbCCSeg> & c )
    {
        _itr = c._itr;
        _parent = c._parent;
        _pid = c._pid;
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
        return iterator( _itr, _itr->begin(_parent), _pid );
    }

    ///
    /// Return an end() iterator.
    ///
    iterator end()
    {
        return iterator( _itr, _itr->end(_parent), _pid );
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
