#ifndef ADS_DCR_IZGUI_H
#define ADS_DCR_IZGUI_H

#include "IZdcr.h"
#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif


BEGIN_NAMESPACE_ADS

///
/// IZgui
///
/// 

class IZgui : public ZObject
{
  public:
	
	ZINTERFACE_ID(IZgui);

	virtual void init(const char *name)=0; 
    virtual char *setName(const char *name)=0;
    virtual char *getName()=0;
	virtual void chip_get(ZPtr<IZdcr> dcr)=0;
	virtual void setDcr(ZPtr<IZdcr> dcr)=0;
	virtual uint getBbox(int *x1, int *y1, int *x2, int *y2)=0;
	virtual void setGuiContext(void *x, void *blk=NULL)=0;
	virtual void inspect(ZPtr<IZdcr> dcr)=0;

};

END_NAMESPACE_ADS

#endif
