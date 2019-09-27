/* 
 * Copyright (C) 2003-2004 Athena Design Systems
 *
 */

#include <tcl.h>
#include "util_pkg.h"

CFUNCTION_DEFINITION(zui_get)
CFUNCTION_DEFINITION(AthGetTiles)
CFUNCTION_DEFINITION(AthGetChip_zui)
CFUNCTION_DEFINITION(AthNavigate_zui)
CFUNCTION_DEFINITION(zui_chipAttr_get)
CFUNCTION_DEFINITION(pull_down_menus_get)

void ZrouterInit_zui(Tcl_Interp *interp)
{
//	TCLCMD("zroute::db::get_cmd", zui_get);
	TCLCMD("zroute::db::tiles_get_cmd", AthGetTiles);
	TCLCMD("zroute::db::chip_get_cmd", AthGetChip_zui);
	TCLCMD("zroute::db::inspect_cmd", AthNavigate_zui);
	TCLCMD("zroute::db::chip_attr_get_cmd", zui_chipAttr_get);
	TCLCMD("zroute::db::menu_define_cmd", pull_down_menus_get);
}
