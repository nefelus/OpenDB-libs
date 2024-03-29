#include <errno.h>
#include "TMP.h"    // Class defintions of Interface
#include "../TEMPLATE.h"

#include "TMP_Tcl.h" // Generated by the ti utility program
#include <logger.h>

#define TCL_IN_ARGS(name) name * in_args = (name *) in
#define TCL_OUT_ARGS(name) name * out_args = (name *) out

BEGIN_NAMESPACE_ADS

TMP::TMP(ZArgs * in, ZArgs * out) : ZTechModule<TMP>(in, out)
{
	notice(0, "Created an instance of TMP\n");
	_main = new TEMPLATE(_db);
}
TMP::~TMP()
{
	// delete _main;
}
/*************************************** Tcl Test functions *****************/
TCL_METHOD(TMP::test_cmd)
{
	TCL_IN_ARGS(ZIn_TMP_test_cmd);

	const char *name= in_args->hello();

	notice(0, "Hello %s from TMP\n",  name); 

	return TCL_OK;
}
TCL_METHOD(TMP::test_option)
{
	TCL_IN_ARGS(ZIn_TMP_test_option);

	const char *msg= in_args->msg();
	if (msg==NULL) {
		warning(0, "option msg is required\n");
	} else {
		_main->test_option(in_args->msg());
		//_main->test_option(in_args->msg(), in_args->num());
	}
	return TCL_OK;
}
/*************************************** module functions *****************/
TCL_METHOD(TMP::dump_db)
{
	notice(0, "TMP::dump_db\n");
	TCL_IN_ARGS(ZIn_TMP_dump_db);
	_main->writeTopBlock(in_args->file());
/*
	notice(0, "Loging: notice: rpl.cpp: init_place\n");
	warning(0, "Loging: rpl.cpp: init_place");
	info(0, "Loging: info: rpl.cpp: init_place\n");

	// works debug("DBG_PLACE","R","REDO ECO: create dbInstObj %s\n","DBG_PLACE");
	// requires: ::zr.zlog enable_debug_log -module "DBG_PLACE" -only "R"
	

*/
	return TCL_OK;
}
TCL_METHOD(TMP::init)
{
	notice(0, "TMP::init\n");
	TCL_IN_ARGS(ZIn_TMP_init);
	TCL_OUT_ARGS(ZOut_TMP_init);

	//_main->inputDesign();
	notice(0, "TMP::init -- Not implemented!\n");

	return TCL_OK;
}
END_NAMESPACE_ADS
