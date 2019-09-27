/* Copyright Athena Design Systems 2003 */

#include <stdio.h>

#include <tcl.h>


#define ZROUTER_VERSION "1.0"

void ZrouterInit_zui(Tcl_Interp *interp);

int AthInitZroute	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthReadLef		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthReadLefMacros(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthReadDef		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthReadDefTiles	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthMakeTiles	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthAssignTracks	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthGetTiles		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

//int AthGetChip_zui	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

//int zui_get			(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
//int zui_chipAttr_get(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthLayersPrint	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthPinsAssign	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthBusMake		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthTilesRoute	(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthDbRead		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);
int AthDbWrite		(ClientData cdata, Tcl_Interp *interp, int objc, Tcl_Obj *CONST objv[]);

