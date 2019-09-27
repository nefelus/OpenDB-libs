#ifndef ADS_ZFACTORY_H
#define ADS_ZFACTORY_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif

BEGIN_NAMESPACE_ADS

class ZContext;
class ZObject;
class ZFactory;

///
/// adsRegisterZFactory - Register this factory and return the class-id this factory represents.
///
void adsRegisterZFactory( ZFactory * factory, ZComponentID cid );

///
/// ZFactory - Class factory interface
///
class ZFactory
{
  public:

    //
    // Return values:
    //    Z_OK - Object created
    //    Z_ERROR_NO_INTERFACE - Object not created because it does not
    //                           support the specified interface.
    //
    virtual int create( const ZContext & context, ZInterfaceID iid, void ** p ) = 0;
};

///
/// ZFactoryImpl - A singleton class which creates instances of the implementation
/// class.
///
template <class IMPL, class INFC>
class ZFactoryImpl : public ZFactory 
{
  public:
    int create( const ZContext & context, ZInterfaceID iid, void ** p )
    {
        IMPL * o = new IMPL;
        o->_context = context;

        if( o->QueryInterface( iid, p ) == Z_OK )
            return Z_OK;
        
        delete o;
        *p = NULL;
        return Z_ERROR_NO_INTERFACE;
    }
};

///
/// ZREGISTER_FACTORY - This macro is used to register a class implmentation.
///
/// This macro must be added to main/factories.cpp
///
#define REGISTER_ZFACTORY(IMPL,INFC) \
            extern void adsRegisterZFactory_##IMPL##INFC(); \
            adsRegisterZFactory_##IMPL##INFC();


#define DECLARE_ZFACTORY(IMPL,INFC) \
    USING_NAMESPACE_ADS; \
    void adsRegisterZFactory_##IMPL##INFC() \
    { \
        static ZFactoryImpl<IMPL,INFC> * factory = NULL; \
        if ( factory == NULL ) \
        { \
            factory = new ZFactoryImpl<IMPL,INFC>; \
            assert(factory); \
            adsRegisterZFactory( factory, ZCID(IMPL)); \
        } \
    } \

#define DECLARE_ZFACTORY_CID(IMPL,INFC,CID) \
    USING_NAMESPACE_ADS; \
    void adsRegisterZFactory_##IMPL##INFC() \
    { \
        static ZFactoryImpl<IMPL,INFC> * factory = NULL; \
        if ( factory == NULL ) \
        { \
            factory = new ZFactoryImpl<IMPL,INFC>; \
            assert(factory); \
            adsRegisterZFactory(factory, CID); \
        } \
    } \

END_NAMESPACE_ADS

#endif
