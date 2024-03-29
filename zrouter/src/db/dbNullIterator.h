#ifndef ADS_DB_NULL_ITERATOR_H
#define ADS_DB_NULL_ITERATOR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

BEGIN_NAMESPACE_ADS

class dbObject;

class dbNullIterator : public dbIterator
{
  public:
    bool reversible();
    bool orderReversed();
    void reverse( register dbObject * parent );
    uint sequential();
    uint size( register dbObject * parent );
    uint begin( register dbObject * parent );
    uint end( register dbObject * parent );
    uint next( register uint id, ... );
    dbObject * getObject( register uint id, ... );

    static dbNullIterator null_iterator;
};


END_NAMESPACE_ADS

#endif
