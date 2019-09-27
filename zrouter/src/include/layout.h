/* Copyright Athena Design Systems 2003 */

#ifndef LAYOUT_H 
#define LAYOUT_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dbgdefines.h"

#include "tech.h"
#include "net.h"
#include "block.h"
#include "db1.h"


class Ath__layoutTable
{

private:
		uint _defUnits;

public:

	Ath__tech *_techTable;
	Ath__masterTable *_masterTable;
	Ath__btermTable *_btermTable;
	Ath__instTable *_instTable;
	Ath__netTable *_netTable;
	Ath__netTable* _powerNetTable;
	uint _globalPowerFlag;
	Ath__box _designBox;
	Ath__box _maxBox;
	Ath__quadTable *_quad;
	Ath__box *_blockageList;
	AthPool<Ath__box> *_blockagePool;
	uint _num;
	char _designName[1024];

public:

	Ath__layoutTable();
	~Ath__layoutTable();
	Ath__box* allocBox(AthPool<Ath__box> *pool);
	void initBtermTable();
	void writeDesignHeaderDB(FILE *fp);
	void readDesignHeaderDB(FILE *fp);
	void setGlobalPowerTable(Ath__netTable* table);


	void setOrigDefUnits(uint units);
	uint getDefMult();
	uint getOrigDefUnits();

	int readDef(char *fileName, bool skipSpecial, Ath__netTable* powerTable, Ath__trans *coordTrans= NULL);
	uint makeQuads(uint rowCnt, uint colCnt, uint metH=0, uint metV=0);
	uint makeQuadTable(uint rowCnt, uint colCnt, uint metH=0, uint metV=0);
	uint getQuadBoxes(Ath__zui *zui);
	uint assignTracks(uint metH, uint metV);
	bool getDesignBox(int *x1, int *y1, int *x2, int *y2);
	uint getBlockages(Ath__zui *zui, Ath__hierType hier, Ath__boxType box);
	uint getDesignBox(Ath__zui *zui, uint maxFlag);
	bool getMaxDesignBox(Ath__box *bb);

	uint writeDefQuads(char *def, int row, int col);
	uint defQuad(uint ii, uint jj, char *def, Ath__zui *zui);
	Ath__box *readBlockages(Ath__parser *parser, char *endWord, int skip, uint defUnits, Ath__tech *tech);
	uint getQuadCnt(int modSize, int xy1, int xy2);
	uint makeQuadsBySize(uint rowSize, uint colSize, uint metH=0, uint metV=0);
	bool touchDesignBox(Ath__zui *zui);
	
	uint writeDB(Ath__db *db, bool powerNetFlag);
	uint readDB(Ath__db *db, bool readPower, Ath__netTable *powerTable=NULL);
	
	FILE* openFP(char *dir, char *name, int format, char *mode);
	void closeFP(FILE* fp);
	void initSignalTable(uint netCnt);
	uint readSignalsDB(FILE *fp);
	uint readInstsDB(FILE *fp);
	uint readMastersDB(char *dir, uint format);
	uint readFrameDB(FILE *fp);
	uint readDBheader(FILE *fp, char *keyword, char* obj_type);
	Ath__box* readBlockagesDB(FILE *fp, AthPool<Ath__box> *boxPool);
	void makeMaxBox(uint *minWidth, uint *minHeight);
	void initInstTable(uint iCnt);
	void writeFrameDB(FILE *frameFP);
	bool initPowerTable(uint netCnt, uint defUnits);
	uint readPowerDB(FILE *fp, bool consumeOnly=false);

	Ath__wire *getWire(uint id);
	Ath__qPin *getTilePin(uint id);
	bool inspectBus(Ath__zui *zui, uint nameId, const char *action);
	void inspectInst(Ath__zui *zui, uint nameId, const char *action);
	bool inspectSignal(Ath__zui *zui, uint nameId, const char *action);
	void inspectTilePin(Ath__zui *zui, uint nameId, const char *action);

	void bill_getTopPins();
	uint getTilePins(Ath__zui *zui);
	uint getTileBuses(Ath__zui *zui);
	uint getPowerNetWires(Ath__zui *zui);
	uint getBlockages(Ath__zui *zui);
	uint getInstBoxes(Ath__zui *zui);
	uint getTileBoxes(Ath__zui *zui, uint w, uint layer);

	int inspect_0(Ath__zui *vzui, char objType, int boxType, uint nameId, const char *action);

};
#endif


