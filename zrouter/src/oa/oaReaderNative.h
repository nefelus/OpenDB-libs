#ifndef ADS_OA_READER_H
#define ADS_OA_READER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_I_OA_READER_H
#include "IoaReader.h"
#endif

#include "dbWireGraph.h"
#include <string>

#ifdef OA_READER_USE_NATIVE_OA
#include <map>
#include <oa/oaDesignDB.h>
using namespace oa;
#endif

BEGIN_NAMESPACE_ADS

class dbLib;
class dbChip;
class dbMaster;
class dbWireGraph;

class oaReader : public ZImplements<oaReader, IoaReader>
{
    std::string    _defs_file;
#ifdef OA_READER_USE_NATIVE_OA
    oaLibDefList * _oa_lib_def_list;
    oaDesign *     _oa_design;
    oaBlock *      _oa_block;
    oaTech *       _oa_tech;
    dbBlock *      _block;
    dbTech *      _tech;
    dbDatabase *   _db;
    oaDefNS        _ns;
    uint           _nm_scale_factor;
    char           _divide_char;
    char           _left_bus_char;
    char           _right_bus_char;

    std::map<std::string, dbMaster *> _master_map;
    std::map<oaInst *, dbInst *>      _inst_map;
    std::map<int, dbTechLayer *>      _layer_map;
    std::map<oaNet *, dbNet *>        _net_map;
    std::map<std::string, dbLib *>    _libs;

    oaConstraintGroup * getRules(const oaObject *object);
    dbTechNonDefaultRule * getNonDefaultRule(const oaObject *object);
    bool openLibDefs();
    bool openTech(const char * lib);
    bool openDesign(const char * lib, const char * cell, const char * view);
    bool mapTech();
    bool createBlock();
    bool createTerms();
    bool createInstances();
    bool createNets();
    bool createRoutes();
    bool createGeoms();
    bool createRoutes(oaNet *, dbNet *);
    bool createGeoms(oaNet *, dbNet *);
    bool createGCells();
    bool createTracks();
    bool createRows();
    bool createBlockages();
    bool createBlockVias();
    bool createDieArea();
    bool createRow( oaRow * row );
    bool createBlockVia( oaViaHeader * viaHeader );
    bool addRect( dbSWire * swire, oaRect * r );
    bool addPath( dbSWire * swire, oaPath * p );
    bool addPathSeg( dbSWire * swire, oaPathSeg * ps );
    bool addPolygon( dbSWire * swire, oaPolygon * p );
    bool addAreaBlockage( oaAreaBlockage * blockage, adsRect & rect, dbInst * inst );
    bool addAreaHaloBlockage( oaAreaHalo * blockage, adsRect & rect, dbInst * inst );
    bool addLayerBlockage( oaLayerBlockage * blockage, adsRect & rect, dbInst * inst );
    bool addLayerHaloBlockage( oaLayerHalo * blockage, adsRect & rect, dbInst * inst );
    bool addPathSeg( oaPathSeg * seg,
                     dbWireGraph::Node * & prev,
                     dbWireType curType,
                     dbTechNonDefaultRule * curRule,
                     dbWireGraph & G );
    bool getFirstViaLayer(oaRouteObjectArray & objects,
                           oaLayerNum & layerNum,
                          oaUInt4 i);
    oaLayerNum getOtherViaLayer(oaVia * via, oaLayerNum layerNum);
    bool isSpecialNet( oaBitNet * net );
    dbMaster * findMaster( const char * lib, const char * name );
    dbSite * findSite( const char * lib );
    void setPlacement( oaInst * oa_inst, dbInst * db_inst );
    void getMasterBoundary( oaInst * oa_inst, oaBox & bbox );
    void getDelimeters();
#endif

    bool runCommand( const std::string & cmd );
    
  public:

    oaReader();
    ~oaReader();
    
    void setDefsFile( const char * file );

    dbLib * readLib( dbDatabase * db, const char * lib );
    
    dbChip * readChip( dbDatabase * db,
                       const char * lib,
                       const char * cell,
                       const char * view,
                       std::vector<dbLib *> &search_libs );
    dbBlock * readBlock( dbBlock * parent,
                         const char * lib,
                         const char * cell,
                         const char * view,
                         std::vector<dbLib *> &search_libs );
#ifdef OA_READER_USE_NATIVE_OA
    int oa2db( int v ) { return v * _nm_scale_factor; } 
#endif
};

END_NAMESPACE_ADS

#endif
