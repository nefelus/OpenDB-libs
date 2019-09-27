/* 
 * Copyright (C) 2003-2004 Athena Design Systems
 *
 */

#include <tcl.h>
#include "ztm.h"

int zevent(Tcl_Interp *interp,const char *event,const char *ecmd) {
	char cmd[1024];
	sprintf(cmd,"zroute::zevent %s %s",event,ecmd);

    Tcl_SavedResult savedResult;
    Tcl_SaveResult(interp,&savedResult);

    int rc = Tcl_Eval(interp,cmd);

    Tcl_RestoreResult(interp,&savedResult);

	return rc;
}
