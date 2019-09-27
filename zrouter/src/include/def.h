/* Copyright Athena Design Systems 2003 */

#ifndef DEF_H 
#define DEF_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "atypes.h"
#include "util.h"
#include "array1.h"
#include "parse.h"

class Ath__tech;
class Ath__box;

class Ath__defPoint
{
public:
	int _xy[2];
	uint _width;
	uint _layer:8;
	uint _via:18;
	uint _new:1;
	uint _prevXY:1;

public:

	void initValues();
	void setCoords(int x, int y);
	void setMidCoords(char *x, char *y);
	void setLayer(uint viaFlag, uint num, uint w);
	void print(FILE *fp, char *header="");

};

class Ath__defPath
{
private:
	Ath__array1D<Ath__defPoint*> *_pointArray;
	AthPool<Ath__defPoint> *_memPool;
	Ath__box *_headBox;
	AthPool<Ath__box> *_boxPoolPtr;
	FILE *_dbgFP;

public:

	Ath__defPath(int n=0);
	~Ath__defPath();
	uint getPointCnt() { return _pointArray->getCnt(); };
	void setDbgFile(FILE *fp);
	int addPoint(Ath__defPoint *p);
	int addFirstPoint(Ath__defPoint *p);
	Ath__defPoint* createViaPoint(uint num, uint w);
	void reuse(bool resetHead=true);
	Ath__defPoint* createPoint(char *x, char *y);
	Ath__defPoint* allocPoint(); // DEBUG
	Ath__defPoint* createFirstPoint(char *x, char *y, uint level, uint width);
	Ath__defPoint *getPoint(int ii);
	void printPoint(FILE *fp, int idx=-1);
	void print();
	int getDefPathPoints(Ath__tech *tech, Ath__parser *parser, int start,
		int makeSegs= 0, uint units= 1, uint minLength= 0, uint nameId= 0, uint nameType= 0);
	int getNextDefPath(Ath__tech *tech, Ath__parser *parser, int *layerNum);
	void setBoxPool(AthPool<Ath__box> *boxPool);
	void setHeadBox(Ath__box *box);
	Ath__box *getHeadBox();
	int getSegment(Ath__tech *tech, uint ii, int *xy1, int *xy2, int *layerNum);
	void makeCoords(uint dir, uint wdir,  int *xy1, int *xy2, Ath__defPoint* p1, Ath__defPoint* p2);
	Ath__box *makeSegments(Ath__tech *tech, uint minLength, uint units, uint nameId, uint nameType);

};

#endif
