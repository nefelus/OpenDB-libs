/* Copyright Athena Design Systems 2003 */

#ifndef ATHARRAY1_H 
#define ATHARRAY1_H 


#include <stdio.h>
#include <malloc.h>
#include <assert.h>

template <class T> class Ath__array1D
{
private:
	T *_array;
	int _size;
	int _chunk;
	int _current;
	int _iterCnt;

	
public:
	Ath__array1D(int chunk=0)
	{
		_chunk= chunk;
		if (_chunk<=0)
			_chunk= 1024;

		_current= 0;
		if (chunk>0) {
			_size= _chunk;
			_array= (T *) realloc(NULL, _size*sizeof(T));
		}
		else {
			_size= 0;
			_array= NULL;
		}
		_iterCnt= 0;
	}
	~Ath__array1D()
	{
		if (_array!=NULL)
			::free(_array);
	}
	int add(T t)
	{
		if (_current>=_size) {
			_size += _chunk;
			_array= (T *) realloc(_array, _size*sizeof(T));
		}
		int n= _current;
		_array[_current ++]= t;

		return n;
	}
	int reSize(int maxSize)
	{
		if (maxSize<_size) 
			return _size;
		
		_size= (maxSize/_chunk +1) * _chunk;
		_array= (T *) realloc(_array, _size*sizeof(T));

		if (_array==NULL)
		{
			fprintf(stderr, "Cannot allocate array of size %d\n", _size);
			assert(0);
		}
		return _size;
	}
	T* getTable()
	{
		return _array;
	}
	T& get(int i)
	{
		assert((i>=0)&&(i<_current));

		return _array[i];
	}
	T& geti(int i)
	{
		if (i>=_size)
			reSize(i+1);

		return _array[i];
	}
	T& getLast()
	{
		assert(_current-1>=0);

		return _array[_current-1];
	}
	void clear(T t)
	{
		for (int ii= 0; ii<_size; ii++)
			_array[ii]= t;
	}
	int findIndex(T t)
	{
		for (int ii= 0; ii<_current; ii++) {
			if (_array[ii]==t) {
				return ii;
                        }
                }
			
		return -1;
	}
	int findNextBiggestIndex(T t, int start=0)
	{
		for (int ii= start; ii<_current; ii++) {
			if (t==_array[ii])
				return ii;
			if (t<_array[ii])
				return ii>0 ? ii-1 : 0;
		}
		return _current;
	}

	bool notEmpty()
	{
		if (_current>0)
			return true;
		else 
			return false;
	}
	bool getNext(T& a)
	{
		if (_iterCnt<_current) {
			a= _array[_iterCnt++];
			return true;
		}
		else
			return false;
	}
	T& pop()
	{
		assert(_current>0);

		_current --;

		return _array[_current];
	}
	unsigned int getSize(void)
	{
		return _size;
	}
	void resetIterator(unsigned int v=0)
	{
		_iterCnt= v;
	}
	void resetCnt(unsigned int v=0)
	{
		_current= v;
		_iterCnt= v;
	}
	unsigned int getCnt(void)
	{
		return _current;
	}
	void set(int ii, T t)
	{
		if (ii>=_size)
			reSize(ii+1);

		_array[ii]= t;
	}

};

#endif


