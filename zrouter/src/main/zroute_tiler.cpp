/* Copyright Athena Design Systems 2003 */

#ifdef _WIN32
#include "direct.h"
#endif

#include "ztop.h"

extern "C" int AthMakeTBlocks(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	int len;
    char *options= Tcl_GetStringFromObj(objv[1], &len);
	const char *list=	Tcl_GetVar2(interp, options, "block", 0);

	if (!Ath__getZtop()->makeTblocks((char *) list))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthMakeTiles(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	char *use= " rows cols ";

	if (!(Ath__check_args(3, use, interp, objc, objv)))
		return TCL_ERROR;

	char *list= Ath__getOption_string(1);

	char *sepH= Ath__getOption_string(4);
	char *sepV= Ath__getOption_string(5);
	int rows= Ath__getOption_int(2);
	int cols= Ath__getOption_int(3);
	if (cols<0)
		cols= 0;

	if (!Ath__getZtop()->makeTiles(list, rows, cols, sepH, sepV))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthAssignTracks(ClientData cdata, Tcl_Interp *interp, int objc,
						   Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " metH metV ", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");

	char *metH= Ath__getOption_string_required("metH");
	char *metV= Ath__getOption_string_required("metV");

	if (!Ath__getZtop()->assign_tracks(block, metH, metV))
		return TCL_ERROR;
	else
		return TCL_OK;
}
extern "C" int AthPinsAssign(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");
	char *tile= Ath__getOption_string_required("tile");
	char *metH= Ath__getOption_string_required("metH");
	char *metV= Ath__getOption_string_required("metV");

	int connType=	Ath__getOption_index("connections", "all", "next", "jumper", "corner");

	int priority=	Ath__getOption_index("priority", "order", "length");

	int style=		Ath__getOption_index("style", "edge", "area", "stagger", "opt");

	int space_units;
	int space= Ath__getOption_int("spacing", 1, "nm", "gp", &space_units);

	int width_units;
	int width= Ath__getOption_int("width", 1, "nm", "gp", &width_units);

	int length_units;
	int length= Ath__getOption_int("length", 5, "nm", "gp", &length_units);

	Ath__getZtop()->assign_pins(block, tile, connType, priority, style, 
		space, space_units, width, width_units, length, length_units,
		metH, metV);

	return TCL_OK;
}
extern "C" int AthBusMake(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *block= Ath__getOption_string_required("block");
	char *tile= Ath__getOption_string_required("tile");

	int opt= Ath__getOption_index("opt", "even", "xtalk");

	int space= 0;
	int width= 0;
	
	if (opt>0) {
		int space_units;
		space= Ath__getOption_int("spacing", 0, "nm", "gp", &space_units);
		int width_units;
		width= Ath__getOption_int("width", 0, "nm", "gp", &width_units);
	}
	Ath__getZtop()->make_buses(block, tile, opt, width, space);

	return TCL_OK;
}
extern "C" int AthTilesRoute(ClientData cdata, Tcl_Interp *interp, int objc,
	       Tcl_Obj *CONST objv[])
{
	if (!(Ath__check_args(1, " name", interp, objc, objv))) {
		return TCL_ERROR;
	}
	char *tile= Ath__getOption_string_required("tile");

	int row, col;
	int n= Ath__getZtop()->get2ints(tile, "_", &row, &col);
	if (n<=0) {
		row= -1;
		col= -1;
	}
//	uint pinCnt= Ath__getZtop()->getBlockById(1)->writeDefQuads("tile.def", row, col);

	return TCL_OK;
}

