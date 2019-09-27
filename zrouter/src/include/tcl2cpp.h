/* Copyright Athena Design Systems 2003 */

#ifndef TCL2CPP_H
#define TCL2CPP_H

#ifdef _WIN32
#include "direct.h"
#endif

#include "tcl.h"
#include "atypes.h"
#include "parse.h"

#include <string.h>
#include <stdlib.h>

#define ZROUTER_VERSION "1.0"

class Ath__tcl2Cpp
{

private:

	char **_cmdTable;
	char **_useTable;
	uint _maxCmdCnt;
	uint _cmdCnt;
	
	char **_args;
	uint _maxWordCnt;
	uint _wordSize;
	uint _argc;

	char *_cmd;

	Ath__parser *_parse;

public:

	Ath__tcl2Cpp(uint m, uint n, uint k);
	~Ath__tcl2Cpp();
	char **allocArray(uint m, uint n);
	uint defineCmd(char *name, char *use);
	char **getCmdTable();

	char *getTcl_string(uint ii);
	int getTcl_int(uint ii);
	bool getTcl_bool(uint ii);
	char** checkArgs(int n, char *usage, Tcl_Interp *interp, 
		int objc, Tcl_Obj *CONST objv[]);

	uint getOptionNum(char *option, char *val0, 
		char *val1=NULL, char *val2=NULL, char *val3=NULL, char *val4=NULL);

	int getInt(char *option, int val, char *units1, char *units2, int *units);
	char* getRequiredName(char *option);
	int getInt(char *word, uint start);
	uint getArgCnt();

};


#endif


