#ifndef ADS_DB_BTERM_ITR_H
#define ADS_DB_BTERM_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbBTerm;
template <class T> class dbTable;

class dbNetBTermItr : public dbIterator
{
    dbTable<_dbBTerm> * _bterm_tbl;

public:
    dbNetBTermItr( dbTable<_dbBTerm> * bterm_tbl ) 
    {
        _bterm_tbl = bterm_tbl;
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
