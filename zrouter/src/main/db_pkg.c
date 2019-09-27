/* 
 * Copyright (C) 2003 Athena Design Systems
 *
 */
#include <tcl.h>
#include <util_pkg.h>


CFUNCTION_DEFINITION( AthInitZroute);

CFUNCTION_DEFINITION( AthLayersPrint)
CFUNCTION_DEFINITION( AthDbSchema)
CFUNCTION_DEFINITION( AthMakeTBlocks)
CFUNCTION_DEFINITION( AthMakeSearch)

void ZrouterInit_db(Tcl_Interp *interp)
{
	TCLCMD("zroute::db::init_cmd", AthInitZroute);
	TCLCMD("zroute::db::layers_print_cmd", AthLayersPrint);
	TCLCMD("zroute::db::db_schema_cmd", AthDbSchema);
	// TCLCMD("zroute::db::tblocks_make_cmd", AthMakeTBlocks);
	TCLCMD("zroute::db::search_make_cmd", AthMakeSearch);
}
