#ifndef ADS_DB_REGION_INST_ITR_H
#define ADS_DB_REGION_INST_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbInst;
template <class T> class dbTable;

class dbRegionInstItr : public dbIterator
{
    dbTable<_dbInst> * _inst_tbl;

public:
    dbRegionInstItr( dbTable<_dbInst> * inst_tbl )
    {
        _inst_tbl = inst_tbl;
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
