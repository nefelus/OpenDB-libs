#ifndef ADS_DB_STREAM_H
#define ADS_DB_STREAM_H

#include <string.h>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;

class dbOStream
{
    _dbDatabase * _db;
    FILE *       _f;
    double       _lef_area_factor;
    double       _lef_dist_factor;

    void write_error()
    {
        throw ZIOError( ferror(_f), "write failed on database stream; system io error: " );
    }
    
  public:

    dbOStream( _dbDatabase * db, FILE * f );
    
    _dbDatabase * getDatabase() { return _db; }
    
    dbOStream & operator<<( bool c )
    {
        unsigned char b = (c == true ? 1 : 0);
        return *this << b;
    }
    
    dbOStream & operator<<( char c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( unsigned char c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( short c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( unsigned short c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( int c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( unsigned int c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( float c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( double c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( long double c )
    {
        int n = fwrite( &c, sizeof(c), 1, _f );
        if ( n != 1 ) write_error();
        return *this;
    }

    dbOStream & operator<<( const char * c )
    {
        if ( c == NULL )
        {
            *this << 0;
        }
        else
        {
            int l = strlen(c) + 1;
            *this << l;
            int n = fwrite( c, l, 1, _f );
            if ( n != 1 ) write_error();
        }
        
        return *this;
    }

    void markStream()
    {
        int marker = ftell(_f);
        int magic = 0xCCCCCCCC;
        *this << magic;
        *this << marker;
    }

    double lefarea( int value )
    {
        return ((double) value * _lef_area_factor);
    }

    double lefdist( int value )
    {
        return ((double) value * _lef_dist_factor);
    }
};

class dbIStream
{
    FILE *        _f;
    _dbDatabase * _db;
    double        _lef_area_factor;
    double        _lef_dist_factor;

    void read_error()
    {
        if ( feof(_f) )
            throw ZException( "read failed on database stream (unexpected end-of-file encounted)." );
        else
            throw ZIOError( ferror(_f), "read failed on database stream; system io error: " );
    }
    
  public:
    dbIStream( _dbDatabase * db, FILE * f );

    _dbDatabase * getDatabase() { return _db; }

    dbIStream & operator>>( bool & c )
    {
        unsigned char b;
        *this >> b;
        c = (b == 1 ? true : false);
        return *this;
    }
    
    dbIStream & operator>>( char & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( unsigned char & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( short & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( unsigned short & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( int & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( unsigned int & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( float & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( double & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( long double & c )
    {
        int n = fread( &c, sizeof(c), 1, _f );
        if ( n != 1 ) read_error();
        return *this;
    }

    dbIStream & operator>>( char * & c )
    {
        int l;
        *this >> l;

        if ( l == 0 )
            c = NULL;
        else
        {
            c = (char *) malloc(l);
            int n = fread( c, l, 1, _f );
            if ( n != 1 ) read_error();
        }
        
        return *this;
    }

    void checkStream()
    {
        int marker = ftell(_f);
        int magic = 0xCCCCCCCC;
        int smarker;
        int smagic;
        *this >> smagic;
        *this >> smarker;
        ZASSERT( (magic == smagic) && (marker == smarker) );
    }

    double lefarea( int value )
    {
        return ((double) value * _lef_area_factor);
    }

    double lefdist( int value )
    {
        return ((double) value * _lef_dist_factor);
    }
};


END_NAMESPACE_ADS

#endif
