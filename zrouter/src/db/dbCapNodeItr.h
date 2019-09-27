#ifndef ADS_DB_CAPNODE_ITR_H
#define ADS_DB_CAPNODE_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbCapNode;
template <class T> class dbTable;

class dbCapNodeItr : public dbIterator
{
    dbTable<_dbCapNode> * _seg_tbl;

public:
    dbCapNodeItr( dbTable<_dbCapNode> * seg_tbl )
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
