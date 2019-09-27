#ifndef ADS_DB_ITERATOR_H
#define ADS_DB_ITERATOR_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class dbObject;
class dbObjectTable;

class dbIterator
{
  public:

    virtual bool reversible() = 0;
    virtual bool orderReversed() = 0;
    virtual void reverse( register dbObject * parent ) = 0;
    virtual uint sequential() = 0;
    virtual uint size( register dbObject * parent ) = 0;
    virtual uint begin( register dbObject * parent ) = 0;
    virtual uint end( register dbObject * parent ) = 0;
    virtual uint next( register uint id, ... ) = 0;
    virtual dbObject * getObject( register uint id, ... ) = 0;
};

END_NAMESPACE_ADS

#endif
