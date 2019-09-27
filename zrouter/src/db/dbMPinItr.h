#ifndef ADS_DB_MPIN_ITR_H
#define ADS_DB_MPIN_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbMPin;
template <class T> class dbTable;

class dbMPinItr : public dbIterator
{
    dbTable<_dbMPin> * _mpin_tbl;

public:
    dbMPinItr( dbTable<_dbMPin> * mpin_tbl ) 
    {
        _mpin_tbl = mpin_tbl;
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
