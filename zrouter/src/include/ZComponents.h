#ifndef ADS_ZCOMPONENTS_H
#define ADS_ZCOMPONENTS_H

#ifndef ADS_H
#include "ads.h"
#endif

///
/// ZINTERFACE - Declare the interface identifier; add to adsInterfaces enum
/// in ZComponents.h
///
#define ZINTERFACE(INTERFACE) ZIID_##INTERFACE

///
/// ZCOMPONENT - Declare the component identifier; add to adsComponents enum
/// in ZComponents.h
///
#define ZCOMPONENT(COMPONENT) ZCID_##COMPONENT

BEGIN_NAMESPACE_ADS

enum ZComponentID
{
    ZCOMPONENT(ZguiDb),
    ZCOMPONENT(ZguiExt),
    ZCOMPONENT(ZguiTmg),
    ZCOMPONENT(Sdb),
    ZCOMPONENT(Zdcr),
    ZCOMPONENT(Dui),
    ZCOMPONENT(Ui),
    ZCOMPONENT(oaWriter),
    ZCOMPONENT(oaReader)
};

enum ZInterfaceID
{
    ZIID_ZObject = 0,

    // Place all entries after this line
    ZINTERFACE(IZgui),
    ZINTERFACE(ISdb),
    ZINTERFACE(IZdcr),
    ZINTERFACE(IDui),
    ZINTERFACE(IUi),
    ZINTERFACE(IoaWriter),
    ZINTERFACE(IoaReader)
};

END_NAMESPACE_ADS

#endif
