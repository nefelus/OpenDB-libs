#ifndef LAYER_H 
#define LAYER_H 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "name.h"
#include "misc_global.h"

class Ath__parser;

class Ath__layer
{
private:
	uint _nameId;
	uint _num;
	uint _level;
	int _routeFlag; // 1=routing, 0=cut, -1=masterslice
	uint _width;
	uint _spacing;
	uint _pitch;
	uint _direction;// 0=vertical, 1=horiz.
	double _res;    // per square
	double _ccCap;  // cap pcoupling per nm
	double _totCap; // cap pcoupling per nm

public:

	void writeDB(FILE *fp);
	void readDB(FILE *fp);

	friend class Ath__grid;
	friend class Ath__gridStack;
	friend class Ath__gridTile;
	
	friend class Ath__layerTable;
};


class Ath__layerTable
{
private:
	Ath__nameTable *_nameTable;
	AthArray<Ath__layer*> *_layerArray;
	AthPool<Ath__layer> *_pool;

	uint _level2layerMap[1024];
	uint _layerNum;
	uint _layerCnt;
	uint _levelCnt;
	uint _cutCnt;

	// for lef reader
	int _lefLayerWordCnt;
	int _lefNewLayerCnt;


	// int *nameMap; // TODO

	friend class Ath__tech;
	
public:
	
	~Ath__layerTable();
	Ath__layerTable(int n);
	void writeDB(FILE *fp);
	void readDB(FILE *fp);
	uint getLevelCnt();
	Ath__layer* allocLayer(uint nameId, uint num);
	Ath__layer* allocLayer(char *name);
	int newLayer(char *name, int f1, int d, int w, int s, int p, double r, double cc, double tc);
	Ath__layer *getLayer(int id);
	Ath__layer *getLayer(char *name);
	char *getLayerNameByLevel(uint ii);
	char *getLayerName(uint id);
	uint getLevel(char *name);
	uint getLevel(char *name, uint *num, int *cutFlag);
	uint printRoutingLayers(FILE *fp);
	uint getLastLevel();
	uint getLayers_0(char *layerBuff);

	Ath__layer *getLayerBylevel(uint level) ;
	uint getDir(uint level)   ;
	uint getWidth(uint level) ;
	uint getPitch(uint level) ;
	uint getHighPitch(int level) ;
	uint getLayerNum(uint level) ;
	
	void initLefLayerCounters();
	int readLefLayers(Ath__parser *parser, char *endWord, int units);
	void printStats(FILE *fp);
	void checkCounters();

};

#endif
