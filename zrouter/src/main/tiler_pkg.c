/* 
 * Copyright (C) 2003 Athena Design Systems
 *
 */
#include <tcl.h>
#include "util_pkg.h"

CFUNCTION_DEFINITION( AthMakeTiles);
CFUNCTION_DEFINITION( AthAssignTracks);
CFUNCTION_DEFINITION( AthBusMake);
CFUNCTION_DEFINITION( AthPinsAssign);
CFUNCTION_DEFINITION( AthTilesRoute);

void ZrouterInit_tiles(Tcl_Interp *interp)
{
	TCLCMD("zroute::tiler::tiles_make_cmd", AthMakeTiles);
	TCLCMD("zroute::tiler::tracks_assign_cmd", AthAssignTracks);
	TCLCMD("zroute::tiler::buses_make_cmd", AthBusMake);
	TCLCMD("zroute::tiler::pins_assign_cmd", AthPinsAssign);
	TCLCMD("zroute::tiler::tiles_route_cmd", AthTilesRoute);
}
