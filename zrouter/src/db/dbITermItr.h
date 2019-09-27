#ifndef ADS_DB_ITERM_ITR_H
#define ADS_DB_ITERM_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbITerm;
template <class T> class dbTable;

class dbNetITermItr : public dbIterator
{
    dbTable<_dbITerm> * _iterm_tbl;

public:
    dbNetITermItr( dbTable<_dbITerm> * iterm_tbl )
    {
        _iterm_tbl = iterm_tbl;
    }
    
    bool reversible();
    bool orderReversed();
    void reverse(register dbObject * parent);
    uint sequential();
    uint size( register dbObject * parent );
    uint begin( register dbObject * parent );
    uint end( register dbObject * parent );
    uint next( register uint id, ... );
    dbObject * getObject( register uint id, ... );
};

class dbInstITermItr : public dbIterator
{
    dbTable<_dbITerm> * _iterm_tbl;

public:
    dbInstITermItr( dbTable<_dbITerm> * iterm_tbl )
    {
        _iterm_tbl = iterm_tbl;
    }
    
    bool reversible();
    bool orderReversed();
    void reverse(register dbObject * parent);
    uint sequential();
    uint size( register dbObject * parent );
    uint begin( register dbObject * parent );
    uint end( register dbObject * parent );
    uint next( register uint id, ... );
    dbObject * getObject( register uint id, ... );
};

END_NAMESPACE_ADS

#endif
