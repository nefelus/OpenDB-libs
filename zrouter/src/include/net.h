/* Copyright Athena Design Systems 2003 */

#ifndef NET_H 
#define NET_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "wire.h"
#include "atypes.h"

#include "dbtable2.h"
#include "inst.h"
#include "array1.h"

char* ATH__allocCharWord(int n);
void ATH__deallocCharWord(char *a);

class Ath__iterm;
class Ath__zui;

class Ath__net
{
private:

	uint _nameId;
	uint _firstTerm;
	uint _termOutCnt:8; // tri-state: 256 max
	uint _termInOutCnt:8;
	uint _termInCnt:16;
	uint _firstBlockTerm:28;
	uint _blockTermCnt:4;

	Ath__box *_headBox;

public:

	void setId(uint n);
	void reset();
	void writeDB(FILE *fp);
	void set(uint n2, uint n3, uint n4, uint n5, uint n6, uint n7);
	void addBtermIndex(uint n);
	void setBterms(uint n, uint cnt);
	uint getBtermCnt();
	uint getBtermFirstIndex();
	uint getNameId();
	uint getTermCnt();
	uint getFirstTerm();
	void addBox(Ath__box *bb);

	friend class Ath__netTable;
};
class Ath__btermTable;
class Ath__instTable;
class Ath__inst;

class Ath__netTable
{
private:

	Ath__dbtable2<Ath__net> *_dbtable;

	AthArray<uint> *_itermTable;

	Ath__array1D<uint> *_itermTmpTable;
	Ath__btermTable *_btermTablePtr;
	Ath__instTable *_instTablePtr;
	Ath__gridTile *_gridSystem;
	AthPool<Ath__box> *_boxPool;
	uint _defUnits;


public:

	Ath__netTable(int nCnt);
	~Ath__netTable();
	Ath__box* allocBox(AthPool<Ath__box> *pool);

	void setDefUnits(uint defUnits) { _defUnits= defUnits; };

	Ath__gridTile* getGridSystem();
	void setGrid(Ath__gridTile *v);
	void resetTmp();
	void setItable(Ath__instTable *itable) { _instTablePtr= itable; }
	void setIOtable(Ath__btermTable *btable) { _btermTablePtr= btable; }
	uint addTmpTerm(uint itermId);
	Ath__net *makeNet(uint nameId);
	uint addNet(char *name, int pinCnt, uint flags, bool itermOrder);
	Ath__net* getNetPtr(uint nameId);
	Ath__net* getNet(char *name);
	Ath__net* getNet(uint id);
	Ath__net* checkNet(char *name);
	char *getName(Ath__net *m);
	char *getName(uint id);
	uint getTermId(uint termIndex);
	Ath__inst* getFirstInstPtr(Ath__net* netPtr);
	Ath__inst* getInstPtr(uint termIndex);
	Ath__mterm* getMtermPtr(uint termIndex);
	Ath__mterm* getMasterTermPtr(Ath__inst *inst, uint masterTermIndex);
	uint orderNetTerms(uint nettag, Ath__termDirection dir, bool itermOrder);
	void orderNetTerms(Ath__net *netPtr, bool itermOrder);
	void printInstTerms(FILE *fp, Ath__net *net);
	void printNetInsts(const char *filename);
	void writeNetsDB(FILE *fp, char *netType, uint globalFlag, bool powerNetFlag);
	void printStats(FILE *fp);
	uint getAllNetBoxes(Ath__zui *zui, Ath__hierType hier, Ath__boxType box);
	uint getNetBoxes(Ath__zui *zui, uint nameId, Ath__hierType hier, Ath__boxType box);
	uint getInstBoxes(Ath__zui *zui, Ath__net *net);

	uint printPoints();
	int readNets(uint nameType, Ath__tech *tech, Ath__parser *parser, char *endWord, int skip, int skipTerms, int skipBoxes, uint minLen);
	uint groupConnections(Ath__tech *techTable, Ath__quadTable *quad);
	uint addObs(Ath__quadTable *quad);
	FILE *openNetFile(int cnt, uint ii);

	void defNets(FILE *fp, Ath__array1D<uint> *netNameIdArray, uint skipShapes);
	uint getInstPtrs(Ath__array1D<uint> *netNameIdArray, Ath__array1D<Ath__inst*> *instPtrTable);
	void readNetsDB(FILE *fp, uint bCnt, Ath__hierType hier, bool consumeOnly=false);
	Ath__box* readBoxList(FILE *fp, uint netId, Ath__hierType hier, bool consumeOnly=false);
	uint getStats(uint level, uint *dir, uint *minWidth, uint *maxWidth, uint minLen);
	void makePowerGrid(Ath__layerTable *layerTable, Ath__box *bb);
	uint add2Grid(Ath__grid *grid);

	uint startNetIterator();
	Ath__net* getNextNet();
	Ath__iterm* getItermPtr(uint termIndex);
	void printInstTerms2(FILE *fp, Ath__net *net);
	uint getInstTermIndex(uint termIndex);
	uint getIterms(Ath__array1D<Ath__iterm*> *itermTable, Ath__net* net);
	void printInstTerms3(FILE *fp, Ath__net *net);
	uint getInstConns(uint netId, Ath__zui *zui);
};

#endif

