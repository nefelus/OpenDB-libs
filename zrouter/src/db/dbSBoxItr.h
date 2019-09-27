#ifndef ADS_DB_SBOX_ITR_H
#define ADS_DB_SBOX_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbSBox;
template <class T> class dbTable;

class dbSBoxItr : public dbIterator
{
protected:
    dbTable<_dbSBox> * _box_tbl;

public:
    dbSBoxItr( dbTable<_dbSBox> * box_tbl )
    {
        _box_tbl = box_tbl;
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
