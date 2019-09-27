/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZINTERFACE_H
#define ADS_ZINTERFACE_H

#include <vector>

#ifndef _TCL
#include <tcl.h>
#endif

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

BEGIN_NAMESPACE_ADS

class dbDatabase;
class ZSession;
class ZObject;
class dbObject;

/////////////////////////////////
/// Event value types
/////////////////////////////////
enum ZValueType
{
    Z_CHAR,
    Z_UCHAR,
    Z_SHORT,
    Z_USHORT,
    Z_INT,
    Z_UINT,
    Z_FLOAT,
    Z_DOUBLE,
    Z_STRING,
    Z_BOOL,
    Z_ZOBJECT,
    Z_DBOBJECT
};

///////////////////////////////////////////////////////
// ZEvent - Alternative interface for generating events
///////////////////////////////////////////////////////
class ZEvent
{
    struct Attr
    {
        const char * _name;
        ZValueType   _type;
        union 
        {
            int          _int;
            unsigned int _uint;
            double       _dbl;
            const char * _str;
            dbObject *   _dbobj;
            ZObject *    _zobj;
        } _value;
        
        Attr( const char * name );
        ~Attr();
        void clearValue();
    };
        
    const char *         _name;
    std::vector<Attr *>  _attrs;
    Attr * find( const char * attr );

    ZEvent & operator=( const ZEvent & e );
    ZEvent( const ZEvent & e);

  public:
    ZEvent( const char * name );
    ~ZEvent();

    // Add a new attribute value pair.
    // Return false, if the attr already exists
    void add( const char * attr, bool value );
    void add( const char * attr, char * value );
    void add( const char * attr, char value );
    void add( const char * attr, short value );
    void add( const char * attr, int value );
    void add( const char * attr, unsigned char value );
    void add( const char * attr, unsigned short value );
    void add( const char * attr, unsigned int value );
    void add( const char * attr, float value );
    void add( const char * attr, double value );
    void add( const char * attr, ZObject * value );
    void add( const char * attr, dbObject * value );
    void add( const char * attr, const adsRect & value );
    void add( const char * attr, adsPoint value );

    // Change an existing attribute value pair.
    // Return false, if the attr does not exists
    void change( const char * attr, bool value );
    void change( const char * attr, char * value );
    void change( const char * attr, char value );
    void change( const char * attr, short value );
    void change( const char * attr, int value );
    void change( const char * attr, unsigned char value );
    void change( const char * attr, unsigned short value );
    void change( const char * attr, unsigned int value );
    void change( const char * attr, float value );
    void change( const char * attr, double value );
    void change( const char * attr, ZObject * value );
    void change( const char * attr, dbObject * value );
    void change( const char * attr, const adsRect & value );
    void change( const char * attr, adsPoint value );

    friend class ZInterface;
};

/////////////////////////////////////////////////////
/// ZContext - Context the software is running in.
/////////////////////////////////////////////////////
class ZContext
{
  public:
    
    Tcl_Interp * _interp;
    ZSession *   _session;
};

//////////////////////////////////////////////
/// Interface to zroute software architecture
//////////////////////////////////////////////
class ZInterface
{
public:
    ZInterface();
    virtual ~ZInterface();
    
    //
    // Post an event. This method specifies the event as a series of attribute value pairs. There
    // must be a at least one attribute value pair. Furthermore the series of attribute value pairs must
    // be terminated with a zero. For examples:
    //    int x, y;
    //    ...
    //    z->event("foo", "x", Z_INT, x, "y", Z_INT, y, 0 );
    //
    // Throws ZException
    int event( const char * name, const char * attr1, ZValueType type, ... );

    //
    // Post an event. This method specifies the event as a series of attribute value pairs. There
    // must be a at least one attribute value pair. Furthermore the series of attribute value pairs must
    // be terminated with a zero. For examples:
    //    int x, y;
    //    ...
    //    z->event("foo", "x", "10", "y", "11", 0 );
    // Throws ZException
    int event( const char * name, const char * attr1, const char * val, ... );

    // Post an event.
    // Throws ZException
    int event( const ZEvent & e );

    // idle event
    int idle( int level);

    // ade event
    int ade( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // milos event
    int milos( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // warning message
    int warning( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // informational message
    int info( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // informational message
    int notice( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // verbose/debugging message
    int verbose( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // debug message
    //void debug(const char *mod, const char *tag, const char * fmt, ... ) ADS_FORMAT_PRINTF(4,5);

    // error, Throws a ZException
    void error( int code, const char * fmt, ... ) ADS_FORMAT_PRINTF(3,4);

    // Get the name of this module
    virtual const char * getModuleName() { return ""; }

public:
    ZContext     _context;
};

END_NAMESPACE_ADS

#endif
