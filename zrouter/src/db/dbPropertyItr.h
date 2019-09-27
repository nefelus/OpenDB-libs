#ifndef ADS_DB_PROPERTY_ITR_H
#define ADS_DB_PROPERTY_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbProperty;
template <class T> class dbTable;

class dbPropertyItr : public dbIterator
{
    dbTable<_dbProperty> * _prop_tbl;

public:
    dbPropertyItr( dbTable<_dbProperty> * prop_tbl )
    {
        _prop_tbl = prop_tbl;
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
