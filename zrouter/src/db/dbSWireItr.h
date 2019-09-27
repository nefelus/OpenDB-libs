#ifndef ADS_DB_SWIRE_ITR_H
#define ADS_DB_SWIRE_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbSWire;
template <class T> class dbTable;

class dbSWireItr : public dbIterator
{
    dbTable<_dbSWire> * _swire_tbl;

public:
    dbSWireItr( dbTable<_dbSWire> * swire_tbl )
    {
        _swire_tbl = swire_tbl;
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
