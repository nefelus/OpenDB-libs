#ifndef VIA_H 
#define VIA_H 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "layer.h"
#include "misc_global.h"
#include "box.h"

class Ath__parser;
class Ath__box;
class Ath__layerTable;

class Ath__masterVia
{
private:
	uint _nameId;
	uint _num;
	uint _cutNum;
	uint _width;
	uint _length;
	double _resPerCut;    // per square
	double _res;    // per square
	double _ccCap;  // cap pcoupling per nm
	double _totCap; // cap pcoupling per nm
	Ath__box *_cutBoxList;
	uint _cutCnt;
	Ath__box *_layerBoxList[2];
	uint _layer[2];
	uint _level[2];
	char *foreign;
	uint _signalFlag:1; 
	uint _defFlag:1; 
	uint _defaultFlag:1; 

public:

	void reset();
	void printVia(FILE *fp);
	void writeBoxListDB(Ath__box *list, char *name, FILE *fp);
	Ath__box* readBoxList(FILE *fp);
	void writeDB(FILE *fp);
	void readDB(FILE *fp);

	friend class Ath__masterViaTable;
};


class Ath__masterViaTable
{
private:
	Ath__nameTable *_nameTable;
	AthArray<Ath__masterVia*> *_viaArray;
	AthPool<Ath__masterVia> *_pool;
	Ath__layerTable *_layerTablePtr;
	uint _level2via[1024];
	uint _via2level[1024];

	// for lef reader
	int _lefViaWordCnt;
	int _newViaCnt;


	// int *nameMap; // TODO

	friend class Ath__tech;
	
public:
	
	~Ath__masterViaTable();
	Ath__masterViaTable(uint n);
	void setLayerTablePtr(Ath__layerTable *v);
	uint getViaCnt();
	Ath__masterVia* allocVia(uint nameId, uint num);
	Ath__masterVia* allocVia(char *name);
	uint newVia(uint defaultVia, char *name);
	void writeDB(FILE *fp);
	void readDB(FILE *fp);
	void setLayer(uint viaId, char *layer);
	void checkViaLayers(FILE *fp, uint viaId);
	char *getName(uint id);
	uint getOtherLevel(Ath__masterVia* via, uint level);
	uint switchLevel(uint viaId, char *layerName);
	uint switchLevel(uint viaId, uint level);
	Ath__masterVia *getVia(int id);
	uint getViaId(char *name, uint ignoreFlag=0, uint exitFlag=0);
	Ath__masterVia *getVia(char *name);
	int readVia(Ath__parser *parser, char *endWord, int units);
	void printVias(FILE *fp);
	void checkCounters();

};

#endif
