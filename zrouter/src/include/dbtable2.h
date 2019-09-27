/* Copyright Athena Design Systems 2003 */
#ifndef DBTABLE2_H 
#define DBTABLE2_H 

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "atypes.h"
#include "itype.h"
#include "util.h"
#include "name.h"
#include "array2d.h"


template <class T> class Ath__dbtable2
{
private:


	Ath__nameTable *_nameTable;
	AthPool<T> *_pool;
	uint _current;
	uint _tmpCnt;

public:

	Ath__dbtable2(uint cnt, bool memDbg=false)
	{
		_nameTable= new Ath__nameTable(cnt);
		_pool= new AthPool<T>(memDbg, cnt);
		_pool->alloc();
	}
	Ath__dbtable2()
	{
		delete _nameTable;
		delete _pool;
	}
private:
	T* getPtrByAddrId(uint id)
	{
		return _pool->get(id);
	}
	uint addNewName(char *name, uint tag)
	{
		return _nameTable->addNewName(name, tag);
	}

public:
	T *makePtr(uint id)
	{	
		// to read DB functions which read the names first!

		uint n;
		T *a= _pool->alloc(NULL, &n);
		if (n!=id) {
			fprintf(stderr, "ERROR:db makePtr n= %u tag=%u", n, id);
		}

		_nameTable->addData(id, n);
		return a;
	}
	T *makePtr(char *name, uint *nameId, uint skipName=0)
	{	
		uint addr=0;
		T* a= _pool->alloc(NULL, &addr);

		if (skipName==0) {
			*nameId= _nameTable->addNewName(name, addr);
		}
		return a;
	}
	char *getName(uint id)	
	{ 
		return _nameTable->getName(id); 
	}

	uint getCnt() 
	{ 
		return _pool->getCnt(); 
	}
	uint getTotalCnt()
	{
		return _pool->getCnt();
	}
	void writeNamesDB(FILE *fp, char *nameType)
	{
		_nameTable->writeDB(fp, nameType);
	}
	uint getId(char *name)
	{
		return _nameTable->getDataId(name);
	}
	T *getPtr(char *name)
	{
		return getPtrByAddrId(_nameTable->getDataId(name));
	}
	T *getPtr(uint nameId)
	{
		return getPtrByAddrId(_nameTable->getDataId(nameId));
	}
	T *checkPtr(char *name)
	{
		uint num= _nameTable->getDataId(name, 1, 1);
		if (num==0)
			return NULL;

		return getPtrByAddrId(num);
	}
	T *getPtr(char *name, uint *tag)
	{
		*tag= _nameTable->getDataId(name);
		return getPtrByAddrId(*tag); // TO VERIFY
	}
	void readNamesDB(FILE *fp)
	{
		_nameTable->readDB(fp);
	}
	bool writeDBtable(FILE *fp, char *keyword, char* obj_type)
	{
		uint cnt= getTotalCnt()-1;
		if (cnt<=0)
			return false;
		
		fprintf(fp, "%s %s %d\n", keyword, obj_type, cnt);
		
		writeNamesDB(fp, keyword);
	
		return true;
	}
	uint readDBheader(FILE *fp, char *keyword, char* obj_type, uint format)
	{
		int cnt;
		
		fprintf(fp, "%s %s %d\n", keyword, obj_type, &cnt);
		
		return cnt;
	}
	uint startIterator(uint n=1)
	{
		_current= n;
		_tmpCnt= _pool->getCnt();

		return _tmpCnt-n;
	}
	T* getNext()
	{
		if (_current<_tmpCnt)
			return _pool->get(_current++);
		else
			return NULL;
	}
};

#endif


