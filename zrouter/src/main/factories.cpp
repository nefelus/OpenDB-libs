#include <tcl.h>
#include "ZFactory.h"
#include "ZComponents.h"

extern "C" void ZRouter_InitFactories();

void ZRouter_InitFactories()
{
	REGISTER_ZFACTORY(ZguiDb, IZgui)
	REGISTER_ZFACTORY(ZguiExt, IZgui)
	REGISTER_ZFACTORY(ZguiTmg, IZgui)
	REGISTER_ZFACTORY(Sdb, ISdb)
	REGISTER_ZFACTORY(Zdcr, IZdcr)
	REGISTER_ZFACTORY(duiGUI, IDui)
	REGISTER_ZFACTORY(uiGUI, IUi)
	REGISTER_ZFACTORY(oaWriter, IoaWriter)
	REGISTER_ZFACTORY(oaReader, IoaReader)
}
