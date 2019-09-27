/* Copyright Athena Design Systems 2003 */

#include "ztop.h"

extern "C" int AthInitZroute(ClientData cdata, Tcl_Interp *interp, 
						   int objc, Tcl_Obj *CONST objv[])
{
	Ath__zroute_init(1000, 512);

	return TCL_OK;
}
extern "C" int AthLayersPrint(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}

	return TCL_OK;
}
extern "C" int AthDbSchema(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	int version=	atoi(Tcl_GetVar2(interp, options, "v", 0));

	if (version>0)
		Ath__init_new_schema();

	return TCL_OK;
}

extern "C" int AthMakeSearch(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{	
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	int force=	atoi(Tcl_GetVar2(interp, options, "force", 0));

	bool skipTopBlock= true;
	bool topOnly= false;
	if (atoi(Tcl_GetVar2(interp, options, "top", 0))>0) {
		topOnly= true;
		skipTopBlock= false;
	}
	bool isdb= false;
	if (atoi(Tcl_GetVar2(interp, options, "isdb", 0))>0) {
		isdb= true;
	}
	// TODO Block
#ifdef ZUI
	Ath__getZtop()->mkSearchDb(force, topOnly, skipTopBlock, isdb);
#endif
	return TCL_OK;
}
