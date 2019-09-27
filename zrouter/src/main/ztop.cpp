/* Copyright Athena Design Systems 2003 */

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "z2tcl.h"
#include "tcl2cpp.h"


#include "tech.h"

uint newSchemaFlag= 0;

static Ath__zroute *Z= NULL;

static Ath__tcl2Cpp *tcl2Cpp= NULL;

//-------------------------------------------------------------
Ath__zroute *Ath__getZtop()
//-------------------------------------------------------------
{
	assert(Z);
	return Z;
}
//-------------------------------------------------------------

void Ath__zroute_init(uint layerCnt, uint masterCnt)
{
#ifdef ZUI
	Z= new Ath__zroute(layerCnt, masterCnt, (uint) 200);
	assert(Z);
#endif
	
	tcl2Cpp= new Ath__tcl2Cpp(50, 512, 10000);
}
void Ath__init_new_schema()
{
	newSchemaFlag= 1;
	fprintf(stdout, "initializing new schema\n\n");
	Z->_dbSchema = 1;
}
char** Ath__check_args(int n, char *usage, Tcl_Interp *interp, 
			   int objc, Tcl_Obj *CONST objv[])
{
	return tcl2Cpp->checkArgs(2, usage, interp, objc, objv);
}

char *Ath__getOption_string(uint ii)
{
	return tcl2Cpp->getTcl_string(ii);
}

char* Ath__getOption_string_required(char *option)
{
	return tcl2Cpp->getRequiredName(option);
}
uint Ath__getOption_index(char *option, char *val0, char *val1, char *val2, char *val3, char *val4)
{
	return tcl2Cpp->getOptionNum(option, val0, val1, val2, val3, val4);
}

int Ath__getOption_int(uint ii)
{
	return tcl2Cpp->getTcl_int(ii);
}
int Ath__getOption_int(char *option, int val, char *units1, char *units2, int *units)
{
	return tcl2Cpp->getInt(option, val, units1, units2, units);
}

