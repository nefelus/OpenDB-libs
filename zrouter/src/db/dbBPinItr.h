#ifndef ADS_DB_BPIN_ITR_H
#define ADS_DB_BPIN_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbBPin;
template <class T> class dbTable;

class dbBPinItr : public dbIterator
{
    dbTable<_dbBPin> * _bpin_tbl;

public:
    dbBPinItr( dbTable<_dbBPin> * bpin_tbl ) 
    {
        _bpin_tbl = bpin_tbl;
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
