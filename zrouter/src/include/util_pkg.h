/* 
 * Copyright (C) 2003 Athena Design Systems
 *
 */
#ifndef UTIL_PKG_H 
#define UTIL_PKG_H 

#define TCLCMD(tclcmd,cppcmd) \
	Tcl_CreateObjCommand(interp,tclcmd, cppcmd, \
		(ClientData) NULL, (Tcl_CmdDeleteProc *)NULL)

#define CFUNCTION_DEFINITION(cppcmd) \
int cppcmd	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

#endif
