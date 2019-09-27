/* Copyright Athena Design Systems 2004 */

#ifndef ADS_TM_H
#define ADS_TM_H

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

#ifndef ADS_ZINTERFACE_H
#include "ZInterface.h"
#endif

#include <vector>
#include <list>
#include <string>

#define ZTCL_REGISTER( MODULE ) \
            void ZTCL_RegisterInterface_##MODULE( Tcl_Interp * ); \
            ZTCL_RegisterInterface_##MODULE(interp);

BEGIN_NAMESPACE_ADS

class dbDatabase;
class dbObject;
class ZObject;
struct ZArg;

template <class M> class ZBindings;
class ZSession;

/////////////////////////////////
/// Argument types
/////////////////////////////////
enum ZArgType
{
    ZARG_UNKNOWN=0,
    ZARG_INT,
    ZARG_FLOAT,
    ZARG_STRING,
    ZARG_BOOL,
    ZARG_ZOBJECT,
    ZARG_DBOBJECT,
    ZARG_ICHANNEL,
    ZARG_OCHANNEL,
    ZARG_RECT,
    ZARG_POINT
};
    
/////////////////////////////////
/// Method Arguments (in/out)
/////////////////////////////////
class ZArgs
{
  protected:
    struct Arg
    {
        const char * _name;
        const char * _value;
        ZArgType     _type;
        bool         _set;
        Arg();
        ~Arg();
    };
        
    std::vector<Arg *> _args;

  public:
    ZContext           _context;

    ~ZArgs();
    ZArgs();
    ZArgs( ZContext & context );
    ZArgs( const std::vector<ZArg> & args, ZContext & context );
    ZArgs( const std::vector<ZArg> & args, ZContext & context, const char * tcl_vname, const char * tcl_sname = NULL );
    ZArgs( const ZArgs & args );
    ZArgs & operator=( const ZArgs & args );
    
    void addArg( const char * name, const char * value, ZArgType type, bool is_set );

    int getCount() const { return _args.size(); }
    const char * getName( int i ) const { return _args[i]->_name; }
    const char * getValue( int i ) const { return _args[i]->_value; }
    ZArgType getType( int i ) const { return _args[i]->_type; }
    void setValue( int i, const char * s );
    void setValue( int i, int n );
    void setValue( int i, double n );
    void setValue( int i, bool b );
    void setValue( int i, const adsRect & r );
    void setValue( int i, adsPoint p );
    bool isSet( int i ) const;
    
    // Type specific routines
    std::string getString( int i ) const;
    bool getBool( int i ) const;
    int getInt( int i ) const;
    double getFloat( int i ) const;
    ZObject * getZObject( int i ) const;
    dbObject * getDbObject( int i ) const;
    FILE * getIChannel( int i ) const;
    FILE * getOChannel( int i ) const;
    const adsRect *getRect( int i ) const;
    const adsPoint *getPoint( int i ) const;

    void getString( int i, std::list<std::string> & l ) const;
    void getBool( int i, std::list<bool> & l ) const;
    void getInt( int i, std::list<int> & l ) const;
    void getFloat( int i, std::list<double> & l ) const;
    void getZObject( int i, std::list<ZObject *> & l ) const;
    void getDbObject( int i, std::list<dbObject *> & l ) const;
    void getIChannel( int i, std::list<FILE *> & l ) const;
    void getOChannel( int i, std::list<FILE *> & l ) const;
    void getRect( int i, std::list<adsRect> & l ) const;
    void getPoint( int i, std::list<adsPoint> & l ) const;

    void setString( int i, const char * s );
    void setString( int i, const std::string & s );
    void setBool( int i, bool b );
    void setInt( int i, int n );
    void setFloat( int i, double n );
    void setZObject( int i, ZObject * z );
    void setDbObject( int i, dbObject * d );
    void setRect( int i, const adsRect & r );
    void setPoint( int i, adsPoint p );

    void setString( int i, const std::list<std::string> & l );
    void setBool( int i, const std::list<bool> & l );
    void setInt( int i, const std::list<int> & l );
    void setFloat( int i, const std::list<double> & l );
    void setZObject( int i, const std::list<ZObject *> & l );
    void setDbObject( int i, const std::list<dbObject *> & l );
    void setRect( int i, const std::list<adsRect> & l );
    void setPoint( int i, const std::list<adsPoint> & l );
};

class ZTechModuleMgr
{
  public:
    static void initialize( Tcl_Interp * interp );
    static void module_doc( const char * module, const char * doc );
    static void method_doc( const char * module, const char * method, const char * doc );
    static void variable_doc( const char * module, const char * method, const char * doc );
    static int man( ClientData clientData, Tcl_Interp * interp, int argc, const char * argv[] );
};

////////////////////////////////////
/// Tcl Technology Module Interface
////////////////////////////////////
template <class M>
class ZTechModule : public ZInterface
{
public:
    dbDatabase * _db;

    typedef int (M::*Method)( ZArgs * in, ZArgs * out );

    // Throws ZException
    ZTechModule( ZArgs * in, ZArgs * out );
    ~ZTechModule();

    // Set the configuration variable.
    // Throws ZException
    int configure( ZArgs * in, ZArgs * out );

    // Get the configuration variable.
    // Throws ZException
    int cget( ZArgs * in, ZArgs * out );

    // Return the name of this module
    const char * getModuleName();

    // construct an instance of this module in TCL
    static int usage( ClientData clientData, Tcl_Interp * interp, int argc, const char * argv[] );

    // construct an instance of this module in TCL
    static int construct( ClientData clientData, Tcl_Interp * interp, int argc, const char * argv[] );

    // execute a method on this module from TCL
    static int dispatch( ClientData clientData, Tcl_Interp * interp, int argc, const char * argv[] );

    // method bindings to TCL
    static ZBindings<M> * _bindings;

    // module name
    static const char * _module;
};

END_NAMESPACE_ADS

#endif
