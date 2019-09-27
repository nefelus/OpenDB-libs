#ifndef ADS_DEFIN_H
#define ADS_DEFIN_H

#include <vector>

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class definReader;
class dbDatabase;
class dbLib;
class dbBlock;
class dbChip;
class dbTech;

class defin
{
    definReader * _reader;

  public:
    defin(dbDatabase *db);
    ~defin();

    void skipWires();
    void skipConnections();
    void skipSpecialWires();
    void skipShields();
    void skipBlockWires();
    void skipFillWires();
    void namesAreDBIDs();
    void setAssemblyMode();
    void useBlockName( const char * name );

    /// Create a new chip
    dbChip * createChip( std::vector<dbLib *> & search_libs, const char * def_file );

    /// Create a new hierachical block
    dbBlock * createBlock( dbBlock * parent, std::vector<dbLib *> & search_libs, const char * def_file );

    /// Replace the wires of this block.
    bool replaceWires( dbBlock * block, const char * def_file );
};
    
END_NAMESPACE_ADS

#endif
