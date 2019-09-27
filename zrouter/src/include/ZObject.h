/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZOBJECT_H
#define ADS_ZOBJECT_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

#ifndef ADS_ZCOMPONENTS_H
#include "ZComponents.h"
#endif

///
/// This file contains an implmentation of a component based software
/// architecture. This implementation is strictly for C++ interfaces. There is
/// no support for external language bindings. And there is no support for
/// multiple interfaces (QueryInterface: one interface is supported per implementation).
///

BEGIN_NAMESPACE_ADS

template <class T> class ZPtr;
class ZContext;
class ZObject;

#define Z_SUCCEEDED(r) (r == Z_OK)
#define Z_FAILED(r) (r != Z_OK)
#define Z_OK 0
#define Z_ERROR_NO_INTERFACE  1
#define Z_ERROR_OUT_OF_MEMORY 2
#define Z_ERROR_NO_COMPONENT  3

///
/// ZINTERFACE_ID -
/// Associate the interface identifier this the interface. This macro
/// should be added to the public section of the interface class.
///
#define ZINTERFACE_ID(INTERFACE) enum { ZIID = ads::ZIID_##INTERFACE };

///
/// ZCID - Returns the component interface identifier. You must include
/// ZComponents.h to use this macro.
///
#define ZCID(COMPONENT) (ads::ZCID_##COMPONENT)

///
/// ZIID - Returns the component interface identifier. You must include
/// ZComponents.h to use this macro.
///
#define ZIID(INTERFACE) (ads::ZIID_##INTERFACE)

///
/// adsCreateComponent - Create a new instance of this compomnent, return a pointer
/// to the specified interface.
///
/// Returns: Z_OK, Z_ERROR_NO_COMPONENT, Z_ERROR_NO_INTERFACE
///
/// result is set to the interface pointer on success, otherwise it is set to NULL.
///
int adsCreateComponent( const ZContext & context, ZComponentID cid, ZInterfaceID iid, void ** result );

///
/// adsNewComponent - Creates a component specified by the component-identifier.
/// The ZPtr is assigned the specified interface of this component. A run-time error
/// will occur if this component does not support the specified interface.
///
template <class INTERFACE>
inline int adsNewComponent( const ZContext & context, ZComponentID cid, ZPtr<INTERFACE> & ptr )
{
    INTERFACE * p;
    ZInterfaceID iid = (ZInterfaceID) INTERFACE::ZIID;
    int r = adsCreateComponent( context, cid, iid, (void **) &p );

    ptr = (INTERFACE *) p;

    if ( r == Z_OK )
        p->Release();
    
    return r;
}

///
/// ZObject - Base class of all interfaces.
///
class ZObject
{
  public:
    typedef ZObject _zobject_traits;
    enum { ZIID = 0 };
    ZObject();
    virtual ~ZObject();
    virtual uint AddRef() = 0;
    virtual uint Release() = 0;
    virtual int QueryInterface( ZInterfaceID iid, void ** ref ) = 0;
};

///
/// ZPtr - Smart pointer class to access methods of interface.
///
template <class T>
class ZPtr
{
    T * _p;

    void setPtr( T * p )
    {
        if ( _p )
            _p->Release();
        
        _p  = p;
    }

    T & operator*(); // Do not allow derefencing.

  public:

    ZPtr()
    {
        _p = NULL;
    }
    
    ZPtr( ZObject * p )
    {
        if ( p == NULL )
            _p = NULL;
        else
        {
            void * v;
            int r = p->QueryInterface( (ZInterfaceID) T::ZIID, &v );
    
            if(r != Z_OK)
            {
                assert(r == Z_OK); // throw the assert in debug mode
                throw ZException("ZPtr: Invalid interface assignment");
            }
    
            _p = (T *) v;
        }
    }

    ZPtr( const ZPtr<T> & p )
    {
        _p = p._p;

        if ( _p )
            _p->AddRef();
    }

    ~ZPtr()
    {
        if ( _p )
            _p->Release();
    }

    ZPtr<T> & operator=( ZObject * p )
    {
        if ( p == NULL )
            setPtr( NULL );
        else
        {
            void *v;
            int r = p->QueryInterface( (ZInterfaceID) T::ZIID, &v );

            if(r != Z_OK)
            {
                assert(r == Z_OK); // throw the assert in debug mode
                throw ZException("ZPtr: Invalid interface assignment");
            }
    
            setPtr( (T *) v );
        }
        
        return *this;
    }

    ZPtr<T> & operator=( ZPtr<T> & p )
    {
        if ( this != &p )
        {
            setPtr( p._p );

            if ( _p )
                _p->AddRef();
        }
        
        return *this;
    }

    int operator==( const ZPtr<T> & p ) const { return _p == p._p; }
    int operator!=( const ZPtr<T> & p ) const { return _p != p._p; }
    int operator==( const T * p ) const { return _p == p; }
    int operator!=( const T * p ) const { return _p != p; }

    // pointer operator
    T * operator->() { return _p; }

    // get the pointer
    T * getPtr() { return _p; }

    // cast operator to a ZObject
    operator ZObject *() const { return (ZObject *) _p; }
};

END_NAMESPACE_ADS

#endif
