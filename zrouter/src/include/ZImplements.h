/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZIMPLEMENTS_H
#define ADS_ZIMPLEMENTS_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZINTERFACE_H
#include "ZInterface.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif

///
/// This file contains an implmentation of a component based software
/// architecture. This implementation is strictly for C++ interfaces. There is
/// no support for external language bindings. And there is no support for
/// multiple interfaces (QueryInterface: one interface is supported per implementation).
///

BEGIN_NAMESPACE_ADS

///
/// ZPtrCnt - Reference count implmentation
///
class ZPtrCnt
{
    int _ref_cnt;

  public:
    ZPtrCnt() 
    { 
        _ref_cnt = 0; 
    }
    uint inc() 
    { 
        return ++_ref_cnt; 
    }
    uint dec() 
    { 
        assert( _ref_cnt > 0); 
        return --_ref_cnt; 
    }
};

///
/// Implements this interface. An object can only implement one interface using ZImplements.
///
template <class CLASS, class INTERFACE>
class ZImplements : public ZInterface, public INTERFACE
{
    ZPtrCnt  _ref_cnt;
    
  public:
    virtual ~ZImplements();

    typedef typename INTERFACE::_zobject_traits _zobject_traits; // Compilation check: this INTERFACE implements a ZObject
    virtual uint AddRef();
    virtual uint Release();
    virtual int QueryInterface( ZInterfaceID iid, void ** ref );
};

///
/// Declare this macro in the ".cpp" file of the implementation class.
#define DECLARE_IMPLEMENTATION(CLASS,INFC) \
           template class ZImplements<CLASS, INFC>;

END_NAMESPACE_ADS

#endif
