
/* Copyright NEFELUS INC 2010 */

#ifndef CHARBUFFER_H
#define CHARBUFFER_H

#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>

class charStore {

private:
	int _buf_cnt;
	char **_buffer_storage;
	int _bufAlloCnt;

public:
	charStore(int n);
	~charStore();
	void resetLineCnt();
	void allocate(char **old, int start, int total);
	bool storeLine(char *buf);
	int getLineCnt();
	char *getStoredLine(int ii);
};


#endif
