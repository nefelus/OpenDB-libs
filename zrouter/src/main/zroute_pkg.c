/* 
 * Copyright (C) 2003 Athena Design Systems
 *
 */
#include <tcl.h>

void ZrouterInit_db(Tcl_Interp *interp);
void ZrouterInit_zui(Tcl_Interp *interp);
void ZrouterInit_tiles(Tcl_Interp *interp);
// void ZrouterInit_groute(Tcl_Interp *interp);
void ZrouterInit_ext(Tcl_Interp *interp);
void ZTCL_InitModules(Tcl_Interp *interp);
void ZRouter_InitInterfaces();
void ZRouter_InitSta(Tcl_Interp *interp);

//static char* version=ZROUTER_VERSION;
#define ZROUTER_VERSION "1.0"

#ifndef _WIN32
#define _declspec(dllexport)
#endif


_declspec(dllexport)
int
Zrouter_Init(Tcl_Interp *interp)
{

	// the technology modules provide the zroute_api
	// so that zroute is used as the namespace to provide the value
	// added tcl code which uses zroute_tm (tm=technology module)
	if (Tcl_PkgProvideEx(interp, "zroute::tm", ZROUTER_VERSION,
		(ClientData) NULL) != TCL_OK) {
		return TCL_ERROR;
	}
	 ZrouterInit_db(interp);
     Zrouter_InitLogger(interp);
	//ZrouterInit_tiles(interp);
	// ZrouterInit_zui(interp);
	// ZrouterInit_groute(interp);
	//ZrouterInit_ext(interp);
	ZTCL_InitModules(interp);


    // no factories.cpp ZRouter_InitFactories();
    // ZRouter_InitSta(interp);

	return TCL_OK;
}

