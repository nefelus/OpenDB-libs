#ifndef ADS_H
#define ADS_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <math.h>

#ifdef WIN32
#pragma warning (disable : 4786)
#pragma warning (disable : 4660)
#endif

#define ADS_NAMESPACE ads
#define BEGIN_NAMESPACE_ADS namespace ADS_NAMESPACE {
#define END_NAMESPACE_ADS }
#define USING_NAMESPACE_ADS using namespace ADS_NAMESPACE

typedef unsigned int uint;
typedef unsigned char uchar;

#ifdef WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#else
typedef long long int64;
typedef unsigned long long uint64;
#endif

#ifdef WIN32

#ifndef strcasecmp
#define strcasecmp _stricoll
#endif

#ifndef snprintf
#define snprintf _snprintf
#endif

#ifndef vsnprintf
#define vsnprintf _vsnprintf
#endif

#endif

#ifndef ZSUBMENUID
#define ZSUBMENUID(x,y) x*10+y
#endif

#endif
