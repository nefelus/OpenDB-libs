/* Copyright Athena Design Systems 2003 */

#ifndef ITYPE_H
#define ITYPE_H

#include <stdio.h>
#include <malloc.h>


class Ath__itype
{
protected:	
	
	union {
		unsigned int _type:5;
		
		struct {
			unsigned int _type:5; // 1-15
			unsigned int _index:4;
			unsigned int _num:23;
		} _t16 ; // array index = 1-15 : _index
		
		struct {
			unsigned int _type:5; // 16-24
			unsigned int _index:5;
			unsigned int _num:22;
		} _t32 ;
		
		struct {
			unsigned int _type:5; // = 25
			unsigned int _index:7;
			unsigned int _num:20;
		} _t128 ;// 
		
		struct {
			unsigned int _type:5; // = 26
			unsigned int _index:8;
			unsigned int _num:19;
		}  _t256 ;// array index = 31
		
		struct {
			unsigned int _type:5; // = 27
			unsigned int _index:9;
			unsigned int _num:18;
		}  _t512 ;// array index = 31
		
		struct {
			unsigned int _type:5; // = 28
			unsigned int _index:10;
			unsigned int _num:17;
		}  _t1k ;// array index = 31
		
		struct {
			unsigned int _type:5; // = 29
			unsigned int _index:11;
			unsigned int _num:16;
		} _t2k ;// array index = 
		
		struct {
			unsigned int _type:5; // = 30
			unsigned int _index:12;
			unsigned int _num:15;
		} _t4k ;// array index = 30
		
		struct {
			unsigned int _type:5; // = 31
			unsigned int _index:13;
			unsigned int _num:14;
		} _big ; // array index = 31
		
		unsigned int _all;
	};
	
public:
	Ath__itype(unsigned int ii=0, unsigned int n=0);
	Ath__itype(unsigned int type, unsigned int index, unsigned int num);
	unsigned int getAll();
	unsigned int getType();
	void setAll(unsigned int n);
	void setType(unsigned int type);
	void setIndex(unsigned int ii);
	void set(unsigned int ii, unsigned int n);
	unsigned int get(unsigned int *index, unsigned int *num, unsigned int type=32);
	unsigned int cnt2type(unsigned int cnt);
};
		
#endif
	
	
