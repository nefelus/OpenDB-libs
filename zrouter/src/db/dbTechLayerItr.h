#ifndef ADS_DB_TECH_LAYER_ITR_H
#define ADS_DB_TECH_LAYER_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTechLayer;
template <class T> class dbTable;

class dbTechLayerItr : public dbIterator
{
    dbTable<_dbTechLayer> * _layer_tbl;

public:
    dbTechLayerItr( dbTable<_dbTechLayer> * layer_tbl )
    {
        _layer_tbl = layer_tbl;
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
