/* Copyright Athena Design Systems 2003 */

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "z2tcl.h"
#include "tcl2cpp.h"
#include "tech.h"


Ath__zroute *Ath__getZtop();

void Ath__zroute_init(uint layerCnt, uint masterCnt);
void Ath__init_new_schema();

char** Ath__check_args(int n, char *usage, Tcl_Interp *interp, 
			   int objc, Tcl_Obj *CONST objv[]);

char *Ath__getOption_string(uint ii);
int Ath__getOption_int(uint ii);

char* Ath__getOption_string_required(char *option);
uint Ath__getOption_index(char *option, char *val0, 
						  char *val1=NULL, char *val2=NULL, char *val3=NULL, char *val4=NULL);

int Ath__getOption_int(char *option, int val, char *units1, char *units2, int *units);
