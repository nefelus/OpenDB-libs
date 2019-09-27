#ifndef ADS_DCR_ISDB_H
#define ADS_DCR_ISDB_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif
#include "ZInterface.h"
#include "IZdcr.h"
#include "array1.h"
#include "util.h"
// #include "gseq.h"

// class duiShape;
// typedef duiShape * duiShapeId;

class Ath__gridTable;

BEGIN_NAMESPACE_ADS

class dbBox;
class dbBlock;
class dbTech;
class dbNet;


///
/// ISdb - Internal User Interface for ZRoute Area Search Infrastructure
///
/// 

class ISdb : public ZObject
{
  public:

    ZINTERFACE_ID(ISdb);
    

	virtual void removeMarkedNetWires()=0;
	virtual void reMakeSdb(dbTech *tech, dbBlock *block)=0;
	virtual void cleanSdb()=0;
	virtual void dumpTrackCounts(FILE *fp)=0;
	// virtual void setExtControl(dbBlock *block, bool useDbSdb, uint adj, uint npsrc, uint nptgt, uint ccUp, bool allNet, uint contextDepth, Ath__array1D<int> **contextArray, uint *contextLength, Ath__array1D<SEQ*> ***dgContextArray, uint *dgContextDepth, uint *dgContextPlanes, uint *dgContextTracks, uint *dgContextBaseLvl, int *dgContextLowLvl, int *dgContextHiLvl, uint *dgContextBaseTrack, int *dgContextLowTrack, int *dgContextHiTrack, int **dgContextTrackBase, AthPool<SEQ> *seqPool)=0;
	virtual void adjustOverlapMakerEnd()=0;
	virtual void adjustMetalFill()=0;
	virtual void setupForBoxes(adsRect maxRect, uint minHeight, uint minWidth)=0;
	virtual void addBox(ads::dbBox * bb, uint wtype, uint id)=0;
	virtual uint setExtrusionMarker(uint startRow, uint startCol)=0;
	virtual Ath__gridTable* getSearchPtr()=0;
	virtual void setDefaultWireType(uint v)=0;

	virtual uint addBox(int x1, int y1, int x2, int y2, uint level, uint id1, uint id2, uint wtype)=0;
	virtual void getBox(uint wid, int *x1, int *y1, int *x2, int *y2, uint *level, uint *id1, uint *id2, uint *wtype)=0;

	virtual uint addBlockages(dbBlock * block)=0;
	virtual void initSearchForNets(dbTech *tech, dbBlock *block)=0;
	virtual uint addNetSBoxes(dbNet *net, uint wtype, bool skipVias)=0;
	virtual void addPowerNets(dbBlock *block, uint wtype, bool skipVias)=0;
	virtual void addSignalNets(dbBlock *block, uint wtype, uint vtype)=0;
	virtual void addSignalNets(dbBlock *block, uint wtype, bool wireVia)=0;
	virtual void searchWireIds(int x1, int y1, int x2, int y2, bool ignoreLayers, bool *exludeTable)=0;
	virtual void searchBoxIds(int x1, int y1, int x2, int y2)=0;

	virtual void setMaxArea(int x1, int y1, int x2, int y2)=0;
	virtual void resetMaxArea()=0;

	virtual uint makeGuiBoxes(ZPtr<IZdcr> dcr, uint menuId, uint subMenuId, bool useSdbId, uint excludeId1=0)=0;
	virtual uint searchSpread(void *ip, uint spreadTrackDist, std::vector<dbNet *> & inets, char *bbox, void (*doSpread)(void *, uint, void *, void *, void *))=0;
	virtual uint couplingCaps(ZPtr<ISdb> ccSdb, uint couplingDist, ZInterface *Interface,
		void (*coupleAndCompute)(int *, void *), void* compPtr)=0;
	virtual uint startIterator()=0;
	virtual uint getNextWireId()=0;
	virtual bool getNextBox(int &x1, int &y1, int &x2, int &y2, uint &level, uint &netId, uint &shapeId)=0;
	virtual void getCCdist(uint wid, uint *width, uint *level, uint *id1, uint *id2)=0;
	virtual void getIds(uint wid, uint *id1, uint *id2, uint *wtype)=0;

};
END_NAMESPACE_ADS

#endif
