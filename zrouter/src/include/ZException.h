/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZEXCEPTION_H
#define ADS_ZEXCEPTION_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifdef __GNUC__
#define ADS_FORMAT_PRINTF(F,A) __attribute__((format(printf,F,A)))
#else
#define ADS_FORMAT_PRINTF(F,A)
#endif

BEGIN_NAMESPACE_ADS

/////////////////////////////////
// Base exception class
/////////////////////////////////
class ZException
{
  public:
    const char * _msg;
	bool _free_msg;

    ZException();
    ZException( const char * fmt, ... ) ADS_FORMAT_PRINTF(2,3);
    ZException( const ZException & ex );
    ~ZException();

};

class ZOutOfMemory : public ZException
{
  public:
    ZOutOfMemory()
    {
	_free_msg = false;
        _msg = "Out of memory";
    }
};

class ZIOError : public ZException
{
  public:
	  ZIOError( int err );
      ZIOError( int err, const char * msg );
};

class ZAssert : public ZException
{
  public:
    ZAssert( const char * expr, const char * file, int line );
};

#define ZASSERT( expr ) assert( expr )

#if 0
#if defined(DEBUG) || ! defined(NDEBUG)
#define ZASSERT( expr ) ((expr) ?  0 : throw ZAssert(  #expr, __FILE__, __LINE__ ))
#else
#define ZASSERT( expr )
#endif
#endif

#define ZALLOCATED( expr ) do { if ( (expr) == NULL ) throw ZOutOfMemory(); } while(0);

END_NAMESPACE_ADS

#endif
