#ifndef ADS_DB_CCSEG_ITR_H
#define ADS_DB_CCSEG_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbCCSeg;
template <class T> class dbTable;

class dbCCSegItr : public dbIterator
{
    dbTable<_dbCCSeg> * _seg_tbl;

public:
    dbCCSegItr( dbTable<_dbCCSeg> * seg_tbl )
    {
        _seg_tbl = seg_tbl;
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
