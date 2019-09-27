#ifndef ADS_DCR_ZGUIDB_H
#define ADS_DCR_ZGUIDB_H

#include "IZgui.h"

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

BEGIN_NAMESPACE_ADS

class dbBlockSearch;

///
/// ZguiDb - Implementation for ZRoute Zui object interface for Db
///
/// 

class ZguiDb : public ZImplements<ZguiDb, IZgui>
{
private:

	ZPtr<IZdcr> _dcr;
	dbBlockSearch *_blockSearch;

	char *_name;

	char* makeName(const char *name);
	void setGuiContext(void *x, void *blk);

	
public:
	ZguiDb();
	~ZguiDb();
	
	// IZgui Interface
	void setDcr(ZPtr<IZdcr> z);
	char *setName(const char *name);
	char *getName();
	void init(const char *);
	uint getBbox(int *x1, int *y1, int *x2, int *y2);
	void chip_get(ZPtr<IZdcr> dcr);
	void inspect(ZPtr<IZdcr> dcr);

};

END_NAMESPACE_ADS

#endif
