/* Copyright Athena Design Systems 2003-2004 */

#ifndef DB1_H 
#define DB1_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "util.h"
#include "atypes.h"
#include "parse.h"

class Ath__db
{
private:
	uint _format;
	uint _charCnt;
	uint _blkCnt;

	char _mode[3];
	char *_userDBdir;
	char *_tmpCurrentDir;

	char *_dbInfo;

	char *_topDir;
	char *_libDir;
	char **_blockDirTable;
	Ath__parser _parse;

	FILE *_dbfp;
	FILE *_dbInfoFP;

	bool _singleDBfile;
	bool _closeFileFlag;
	

public:
	
	Ath__db(char *dir, uint format, char *mode, FILE *fp, bool singleFile=NULL);
	~Ath__db();
	void setStreamMode(char *mode);
	bool isMultipleFiles(uint blockCnt);

	FILE* openFile(char *name, char *mode);
	void writeDesignName(char *filename);
	void writeBlockCnt(uint cnt);
	uint readBlockCnt();
	void readDesignName(char *filename);
	FILE* openNextFP();
	FILE* openDesignFP(char *name);
	void closeDesignFP();
	void closeInfoFP();
	void setLibDir() ;
	void setTopDir(char *defBlockName=NULL) ;
	char *initFileName(char *name, uint n);
	char *initFileName2(char *name1, char *name2, uint n);
	void initBlockNames(char *name, uint m, uint n);
	void deallocBlockNames(uint m);
};

#endif

