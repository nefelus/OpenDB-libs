#ifndef ADS_DB_UTIL_H
#define ADS_DB_UTIL_H

#ifndef ADS_H
#include "ads.h"
#endif

#include "../db/dbMatrix.h"
#include "dbTypes.h"
#include "geom.h"
#include <map>
#include <vector>

BEGIN_NAMESPACE_ADS

class dbObject;
class dbInst;
class dbITerm;
class dbNet;
class dbTech;
class dbBTerm;
class dbTechLayer;
class dbTechLayerRule;
class dbTechVia;
class dbBlock;
class dbTechNonDefaultRule;
class dbShape;
class dbSBox;
class dbBox;

//
// This class creates a new net along with a wire.
//
class dbCreateNetUtil
{
    typedef std::map<int, dbTechLayerRule *> RuleMap;
    dbTech *                              _tech;
    dbBlock *                             _block;
    std::vector<RuleMap>                  _rules;
    std::vector<dbTechLayer *>            _routingLayers;
    int                                   _ruleNameHint;
    dbMatrix< std::vector<dbTechVia *> >  _vias;
	bool								  _milosFormat;
	dbNet*								  _currentNet;
	dbNet**								  _mapArray;
	uint									_mapCnt;
    uint                                    _ecoCnt;


    dbTechVia * getVia(int l1, int l2, adsRect & bbox);
    //dbTechLayerRule * getRule(int routingLayer, int width);
    std::pair<dbBTerm *,dbBTerm *> createTerms4SingleNet(dbNet * net, int x1, int y1, int x2, int y2, dbTechLayer *inly);
    
  public:
    dbTechLayerRule * getRule(int routingLayer, int width);
    
        bool                                _skipPowerNets;
        bool                                _useLocation;
		bool								_verbose;

    dbCreateNetUtil();
	~dbCreateNetUtil();
    void setBlock( dbBlock * block, bool skipInit=false);
    dbBlock * getBlock() const { return _block; }
    dbNet * createNetSingleWire(const char * name, int
    x1, int y1, int x2, int y2, int rlevel, bool skipBterms = false, bool
    skipNetExists=false);
    dbNet * createNetSingleWire(const char * name, int x1, int y1, int x2, int y2, int rlevel, dbTechLayerDir dir, bool skipBterms = false);
    dbNet * createNetSingleVia(const char * name, int x1, int y1, int x2, int y2, int lay1, int lay2 );
    bool createSingleVia(dbNet * net, int x1, int y1, int x2, int y2, int lay1, int lay2 );
	
	//2/15/12
	dbBox *createTechVia(int x1, int y1, int x2, int y2, int lay1, int lay2 );

	//7/23/07

	dbBlock *createBlock(dbBlock *blk, bool copyViaTable=false);
	dbInst *createInst(dbInst *ii, bool createInstance, bool destroyInstance);
	dbNet *createNet(dbNet *nn, bool create, bool destroy);

	//8/1/07
	dbInst *updateInst(dbInst *inst0, bool createInstance, bool destroyInstance);
	bool printECO(dbNet *net, const char *termConnect, FILE *fp);
	bool printECO(dbNet *nn, dbBlock *srcBlock, FILE *fp);
	dbNet *updateNet(dbNet *nn, bool create, bool destroy);

	//8/3/07
	dbITerm *updateITerm(dbITerm *iterm, bool disconnect);
	uint printEcoTerms(dbNet *net, const char *termConnect, FILE *fp);
	uint printModifiedNetECO(dbNet *net, const char *termConnect, const char *termDisconnect, FILE *fp);
	uint printEcoNet(dbNet *ecoNet, dbBlock *srcBlock, FILE *fp);
	uint printNewInsts(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp);
	uint printDeletedInsts(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp);
	uint printModifiedInsts(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp);
	uint printEcoInst(dbInst *inst0, dbBlock *srcBlock, FILE *fp);
	bool printEcoInstVerbose(FILE *fp, dbInst *inst, char *header);
	uint printNewNets(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp, int itermCnt);
	uint printDeletedNets(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp, int itermCnt);
	uint printDisconnectedTerms(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp);
	uint printConnectedTerms(dbBlock *ecoBlock, dbBlock *srcBlock, FILE *fp);
	uint printModifiedNets(dbBlock *ecoBlock, bool connectTerm, dbBlock *srcBlock, FILE *fp);
	void writeEco(dbBlock *ecoBlock, dbBlock *srcBlock, const char *fileName, bool debug=false);

	//9/11/07
	dbNet *createNetSingleWire(adsRect & r, uint level, uint netId, uint shapeId);
	dbNet *createSpecialNetSingleWire(adsRect & r, dbTechLayer *layer, dbNet *origNet, uint sboxId);
	uint getFirstShape(dbNet *net, dbShape & s);
	bool setFirstShapeProperty(dbNet *net,uint prop);
	dbSBox *createSpecialWire(dbNet *mainNet, adsRect & r, dbTechLayer *layer, uint sboxId);
	void setCurrentNet(dbNet *net);
	dbNet *createSpecialNet(dbNet *origNet, char *name);
	void allocMapArray(uint n);
	void checkAndSet(uint id);
	dbInst *createInst(dbInst *inst0);
    int createIntTmgProperty(dbObject* obj);
    bool annotateSlack(FILE *fp, dbObject* obj);
    bool printTimeAndSlackProperties(dbObject* obj, FILE *fp);
    void writeDetailedEco(dbBlock *ecoBlock, dbBlock *srcBlock, const char *fileName, bool debug); 
	dbNet *copyNet(dbNet *net, bool copyVias= true, char *name= NULL, bool removeITermsBTerms= true);
	dbNet *getCurrentNet();
};


END_NAMESPACE_ADS

#endif
