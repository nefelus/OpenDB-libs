/* Copyright Athena Design Systems 2007 */

#ifndef ADS_SDCDB_H
#define ADS_SDCDB_H

#ifndef ADS_H
#include "ads.h"
#endif

#include "db.h"
#include "dbShape.h"
#include "parse.h"
#include "darr.h"

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif



BEGIN_NAMESPACE_ADS


class Sdc_db
{

private:

	char *_inFile;
	char *_outFile;
	FILE *_inFP;
	FILE *_outFP;
	dbBlock *_block;
	char* _idName;

	char* _commandName;
	uint _currentLine;

	Ath__parser *_parser;

public:
	Sdc_db();
	~Sdc_db();
	char* name2id(char *name, char idPrefix);
	bool getPins(Ath__parser *listParser, Ath__parser *itermParser, char *keyword, char idPrefix, bool debug);
	bool parseNameList(Ath__parser *wordParser, char *keyword, char idPrefix, bool virtualFlag, bool debug);
	void translate(dbBlock * block, const char *infile, const char *outfile);
	
};

END_NAMESPACE_ADS

#endif
