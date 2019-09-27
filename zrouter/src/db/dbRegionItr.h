#ifndef ADS_DB_REGION_ITR_H
#define ADS_DB_REGION_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbRegion;
template <class T> class dbTable;

class dbRegionItr : public dbIterator
{
    dbTable<_dbRegion> * _region_tbl;

public:
    dbRegionItr( dbTable<_dbRegion> * region_tbl ) 
    {
        _region_tbl = region_tbl;
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
