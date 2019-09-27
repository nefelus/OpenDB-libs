/* Copyright Athena Design Systems 2003 */

#ifndef BLOCK_H 
#define BLOCK_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dbtable2.h"
#include "wire.h"
#include "master.h"
#include "net.h"
#include "parse.h"

class Ath__netTable;
class Ath__techTable;

class Ath__bterm
{
private:

	int _x;
	int _y;
	uint _num;
	uint _netId;
	uint _nameId;
	Ath__box *_box;
	uint _headCnt:10; // limit 1024 logical pins per net
	uint _ioType:5;
	uint _type:5;
	uint _placed:1;
	char _orient[3];
	Ath__bterm *_next;

public:

	void setId(uint n);
	void setNetId(uint n);
	void setPosition(int x, int y, const char *orient, char *type);
	void setBox(Ath__box *bb);
	int getX();
	int getY();
	bool getBbox(Ath__box *bb);
	void writeDB(FILE *fp);
	Ath__box* allocBox(AthPool<Ath__box> *pool);
	void readDB(FILE *fp, AthPool<Ath__box> *pool);
	
	friend class Ath__btermTable;
};
class Ath__btermTable
{
private:
	
	Ath__dbtable2<Ath__bterm> *_dbtable;
	
	AthPool<Ath__box> *_btermBoxPool;

	Ath__netTable *_netTablePtr;
	Ath__netTable *_tmpNetTable;

public:
	Ath__btermTable(Ath__netTable *nTable);
	uint setAll(Ath__bterm* m, uint num);
	Ath__bterm* allocBterm(uint num);
	Ath__bterm* addBterm(char *name, uint num) ;
	~Ath__btermTable();
	void deallocTmpNetTable();
	char *getName(uint id);
	uint startBtermIterator();
	Ath__bterm* getNextBterm();

	Ath__bterm* addBterm(char *bname, char *netname);
	int readPins(Ath__parser *parser, char *endWord, int skip, uint defUnits, Ath__tech *tech, uint blockId);
	uint getBterms(char *netname, uint *btermIndex);
	void resetNet(uint newNetId, uint btermIndex);
	uint getBtermBoxes(Ath__zui *zui);
	void writeDB(FILE *fp);
	void readDB(FILE *fp);
	uint readBtermDB(FILE *fp);

	void printStats(FILE *fp);
};

#endif


