#include <tcl.h>
#include "tm.h"

extern "C" void ZTCL_InitModules( Tcl_Interp * interp );
void ZTCL_InitModules( Tcl_Interp * interp )
{
    ads::ZTechModuleMgr::initialize(interp);
    ZTCL_REGISTER(ZSession);
    ZTCL_REGISTER(ZDB);
 	ZTCL_REGISTER(PL1)
    ZTCL_REGISTER(TMP);
    //ZTCL_REGISTER(GR);
    //ZTCL_REGISTER(GDS);
    //ZTCL_REGISTER(WS);
    //ZTCL_REGISTER(DUI);
    //ZTCL_REGISTER(Planner);
    // ZTCL_REGISTER(TE);
    //ZTCL_REGISTER(ZT);
    //ZTCL_REGISTER(ZPLACE);
    //ZTCL_REGISTER(Ext);
    //ZTCL_REGISTER(Dcr);
    //ZTCL_REGISTER(Tmg);
    //ZTCL_REGISTER(Power); // handling leakage power - payam 12/31/05
    //ZTCL_REGISTER(YR);
    //ZTCL_REGISTER(UI);
    //ZTCL_REGISTER(ECO);
}
