/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZIMPLEMENTS_HPP
#define ADS_ZIMPLEMENTS_HPP

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZSESSION_H
#include "ZSession.h"
#endif

#ifndef ADS_ZNAMESPACE_H
#include "ZNamespace.h"
#endif

BEGIN_NAMESPACE_ADS

template <class CLASS, class INTERFACE>
ZImplements<CLASS,INTERFACE>::~ZImplements()
{
}

template <class CLASS, class INTERFACE>
uint ZImplements<CLASS,INTERFACE>::AddRef()
{
    return _ref_cnt.inc();
}

template <class CLASS, class INTERFACE>
uint ZImplements<CLASS,INTERFACE>::Release()
{
    int cnt = _ref_cnt.dec();
    
    if ( cnt == 0 )
    {
        _context._session->_ns->removeZObject((ZObject *) this);
        delete this;
    }

    return cnt;
}

template <class CLASS, class INTERFACE>
int ZImplements<CLASS,INTERFACE>::QueryInterface( ZInterfaceID iid, void ** p )
{ 
    if ( iid == (ZInterfaceID) ZObject::ZIID )
    {
        ZObject * o = (ZObject *) this;
        o->AddRef();
        *p = o; 
        return Z_OK;
    }
    
    else if ( iid == (ZInterfaceID) INTERFACE::ZIID )
    {
        INTERFACE * o = (INTERFACE *) this;
        o->AddRef();
        *p = o; 
        return Z_OK;
    }
    
    *p = NULL;
    return Z_ERROR_NO_INTERFACE;
}

END_NAMESPACE_ADS

#endif
