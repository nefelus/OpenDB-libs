#ifndef ADS_DB_TARGET_ITR_H
#define ADS_DB_TARGET_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTarget;
template <class T> class dbTable;

class dbTargetItr : public dbIterator
{
    dbTable<_dbTarget> * _target_tbl;

public:
    dbTargetItr( dbTable<_dbTarget> * target_tbl ) 
    {
        _target_tbl = target_tbl;
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
