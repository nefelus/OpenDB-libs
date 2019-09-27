#ifndef ADS_DEFIN_READER_H
#define ADS_DEFIN_READER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IREADER_H
#include "definIReader.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class definBlockage;
class definComponent;
class definFill;
class definGCell;
class definNet;
class definPin;
class definRow;
class definSNet;
class definTracks;
class definVia;
class definRegion;
class definNonDefaultRule;
class definPropDefs;
class definPinProps;

class definReader : public definIReader, public definBase
{
    dbDatabase *             _db;
    definBlockage *          _blockageR;
    definComponent *         _componentR;
    definFill *              _fillR;
    definGCell *             _gcellR;
    definNet *               _netR;
    definPin *               _pinR;
    definRow *               _rowR;
    definSNet *              _snetR;
    definTracks *            _tracksR;
    definVia *               _viaR;
    definRegion *            _regionR;
    definNonDefaultRule *    _non_default_ruleR;
    definPropDefs *          _prop_defsR;
    definPinProps *          _pin_propsR;
    std::vector<definBase *> _interfaces;
    bool             _update;
    const char *     _block_name;

    void init();
    void setLibs( std::vector<dbLib *> & lib_names );
    
    virtual void dieArea( const std::vector<defPoint> & points );
    virtual void error( const char * msg );
    virtual void units( int d );
    virtual void line( int line_num );

    void setTech( dbTech * tech );
    void setBlock( dbBlock * block );

    bool createBlock( const char * file );
    bool replaceWires( const char * file );
    void replaceWires();
    int errors();
    
  public:
    definReader( dbDatabase * db );
    virtual ~definReader();
    
    void skipConnections();
    void skipWires();
    void skipSpecialWires();
    void skipShields();
    void skipBlockWires();
    void skipFillWires();
    void useBlockName( const char * name );
    void namesAreDBIDs();
    void setAssemblyMode();
    
    dbChip * createChip( std::vector<dbLib *> & search_libs, const char * def_file );
    dbBlock * createBlock( dbBlock * parent, std::vector<dbLib *> & search_libs, const char * def_file );
    bool replaceWires( dbBlock * block, const char * def_file );
};
    
END_NAMESPACE_ADS

#endif
