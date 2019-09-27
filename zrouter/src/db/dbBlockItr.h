#ifndef ADS_DB_BLOCK_ITR_H
#define ADS_DB_BLOCK_ITR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbBlock;
template <class T> class dbTable;

class dbBlockItr : public dbIterator
{
    dbTable<_dbBlock> * _block_tbl;

  public:
    
    dbBlockItr( dbTable<_dbBlock> * block_tbl )
    {
        _block_tbl = block_tbl;
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
