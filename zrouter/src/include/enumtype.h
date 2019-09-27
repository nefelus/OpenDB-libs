/* Copyright Athena Design Systems 2003 */

#ifndef ENUMTYPE_H 
#define ENUMTYPE_H 


#include <stdio.h>
#include <malloc.h>
#include <assert.h>

#include "atypes.h"


class Ath__associateArray
{
private:
	uint _maxWordCnt;
	char **_nameTable;
	char **_wordTable;
	uint _cnt;
	bool *_flags;
	uint *_map;

	
public:
	Ath__associateArray(uint n, uint minWordSize, bool map=true);
	uint getCnt();
	~Ath__associateArray() ;
	uint add(uint n, char *name, char *name_prefix);
	uint add(uint n, char *name, uint k);
	char* getName(uint n);
	char* getWord(uint n);
	uint getNameIndex(char *name, int start=-1);
	uint getTableIndex(char *word);

	uint getWordMapIndex(char *name_prefix);
	void setFlagByName(char *name);
	void resetFlagByName(char *name);
	void setFlag(uint n);
	void setAllFlag();
	void resetAllFlag();
	bool getFlag(uint n);
	void printAll(char *buff);
	void getNameList(char *buff);
};

#endif


