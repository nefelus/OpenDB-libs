/* Copyright Athena Design Systems 2003 */

#ifndef NAME_H 
#define NAME_H 

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#include "atypes.h"
#include "util.h"
#include "misc_global.h"

class Ath__nameBucket
{
private:
	char *_name;
	uint _tag;

public:
	void set(char *name, uint tag);
	void deallocWord();

	friend class Ath__nameTable;
};

class Ath__nameTable
{
private:
	AthHash<int> *_hashTable;
	AthPool<Ath__nameBucket> *_bucketPool;
	// int *nameMap; // TODO
	
	void allocName(char *name, uint nameId, bool hash=false);
	uint addName(char *name, uint dataId);

public:
	
	~Ath__nameTable();
	Ath__nameTable(uint n, char *zero=NULL);

	void writeDB(FILE *fp, char *nameType);
	void readDB(FILE *fp);
	void addData(uint poolId, uint dataId);

	uint addNewName(char *name, uint dataId);
	char *getName(uint poolId);
	uint getDataId(int poolId);
	uint getTagId(char *name);
	uint getDataId(char *name, uint ignoreFlag=0, uint exitFlag=0, int *nn=0);
};

#endif

		
	
