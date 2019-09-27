/* Copyright Athena Design Systems 2003 */

#ifndef PARSE_H 
#define PARSE_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "array1.h"

class Ath__parser
{
private :

	char *_line;
	char *_tmpLine;
	char *_wordSeparators;
	char **_wordArray;
	char _commentChar;
	int _maxWordCnt;
	int _wordSize;

	int _lineNum;
	int _currentWordCnt;
	int _lineSize;
	FILE *_inFP;
	FILE *_dbgFP;
	char *_inputFile;

	int _dbg;
	int _progressLineChunk;

public:

	Ath__parser();
	Ath__parser(int lSize, int wCnt, int wSize);
	void init();
	void keepLine();
	void getTmpLine();
	~Ath__parser();
	void openFile(char *name=NULL);
	void setInputFP(FILE *fp);
	void setDbg(int v);
	FILE *getDbgFP();
	int createWords();
	int mkWords(int ii);
	int mkWords(const char *word, char *sep=NULL);
	int makeWords(const char *headSubWord);
	bool startWord(const char *headSubWord);
	int get2Double(const char *word, char *sep, double &v1, double &v2);
	int get2Int(const char *word, char *sep, int &v1, int &v2);
	int skipToEnd(char *endWord);
	int skipToEnd(char *endWord, char *name);
	int readLineAndBreak(int prevWordCnt= -1);
	int readMultipleLineAndBreak(char continuationChar);
	int readLine(const char *headSubWord);
	int parseNextLineUntil(char *endWord, int dbg=0);
	int parseNextLineUntil(int n, char *endWord1, char *endWord2, int *pos12);
    int parseNextLineIfended(int jj);
	int parseNextLine(char continuationChar='\0');
	int parseOneMoreLine(int jj);
	char *get(int ii);
	char* get(int start, char* prefix);
	int getInt(int ii);
	int getInt(int n, int start);
	double getDouble(int ii);
	int getIntFromDouble(int ii);
	void getDoubleArray(Ath__array1D<double> *A, int start, double mult=1.0);
	Ath__array1D<double> *readDoubleArray(char *keyword, int start);
	int getIntFromDouble(int ii, int lefUnits);
	int reportProgress(FILE *fp);
	int reportLines(FILE *fp);
	void printWords(FILE *fp);
	void printWord(int ii, FILE *fp, char *sep);

	int getWordCnt();
	char* getLastWord();
	char* getFirstWord();
	char getFirstChar();

	int getPoint(int ii, char *leftParenth, int *x, int *y, char* rightParenth);
	int getNamePair(int ii, char *leftParenth, int *i1, int *i2, char* rightParenth);
	int getWordCountTo(int start, char rightParenth);

	int getDefCoord(int ii, int prev);
	void syntaxError(char *msg);
	void mkDir(char *word);
	int mkDirTree(char *word, char *sep);
	bool isPlusKeyword(int ii, char *key1);
	char* getRequiredPlusKeyword(int ii, char *key1);
	char* getPlusKeyword(int ii);
	int parseNextUntil(char *endWord);
	bool isSeparator(char a);
	char* getValue(int start, char *key);

	void resetSeparator(char *s);
	void addSeparator(char *s);
	bool isKeyword(int ii, char *key1);
	void resetLineNum(int v);
	int getLineNum();
	bool isDigit(int ii, int jj);
	
	bool setIntVal(const char *key, int n, int &val);
	bool setDoubleVal(const char *key, int n, double &val);
	void printInt(FILE *fp, const char *sep, const char *key, int v, bool pos=false);
	void printDouble(FILE *fp, const char *sep, const char *key, double v, bool pos=false);
	void printString(FILE *fp, const char *sep, const char *key, char* v, bool pos=false);

};

#endif
