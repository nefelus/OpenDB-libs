#ifndef TECH_H 
#define TECH_H 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "layer.h"
#include "via.h"
#include "master.h"

class Ath__db;

class Ath__tech
{
public:
	Ath__layerTable *_layerTable;
	Ath__masterViaTable *_viaTable;
	Ath__masterTable *_masterTable;

	uint _units;
	
public:
	
	Ath__tech(uint units, uint layerCnt, uint masterCnt);
	~Ath__tech();
	int readLef(char *fileName);
	int readLefMacros(char *fileName);
	uint writeDB(Ath__db *db);
	uint readLayersDB(FILE *fp);
	FILE* openFP(char *dir, char *name, int format, char *mode);
	uint readDB(Ath__db *db);
	uint readViasDB(FILE *fp);
	uint readMastersDB(FILE *fp);
	uint readDBheader(FILE *fp, char *keyword, char* obj_type);

};

#endif

