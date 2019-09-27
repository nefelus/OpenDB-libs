#include <tcl.h>
#include "tm.h"

extern "C" void ZTCL_InitModules( Tcl_Interp * interp );
void ZTCL_InitModules( Tcl_Interp * interp )
{
    ads::ZTechModuleMgr::initialize(interp);
    ZTCL_REGISTER(ZSession);
    ZTCL_REGISTER(ZDB);
    ZTCL_REGISTER(TMP);
}
