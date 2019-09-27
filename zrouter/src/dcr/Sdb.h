#ifndef ADS_DCR_SDB_H
#define ADS_DCR_SDB_H

#include "ISdb.h"  // Why do you need here
#include "IZdcr.h"

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#include "db.h"
#include "dbShape.h"

#include "array1.h"

class Ath__gridTable;

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbBox;
class dbTech;
class dbNet;
class dbSBox;
//class dbShape;
//class dbWireShapeItr;

///
/// Sdb - Implementation for ZRoute Area Search Infrastructure
///
/// 

class Sdb : public ZImplements<Sdb, ISdb>
{
	Ath__gridTable *_search;

	Ath__array1D<uint> *_idTable;
	//Ath__searchBox areaBb;
	
private:
	uint getViaLevel(dbSBox *s);
	uint getViaLevel(dbShape *s);
	uint addNetSBoxes(dbNet *net, uint wtype, bool wireVia);
	uint addNetShapesOnSearch(dbNet * net, uint wtype, bool wireVia);
	uint addNetShapesOnSearch(dbNet * net, uint wtype, uint vtype);


public:
	Sdb();
	~Sdb();
	
	// ISdb Interface
	
	void removeMarkedNetWires();
	void reMakeSdb(dbTech *tech, dbBlock *block);
	void cleanSdb();
	void dumpTrackCounts(FILE *fp);
	void setExtControl(dbBlock *block, bool useDbSdb, uint adj, uint npsrc, uint nptgt, uint ccUp, bool allNet, uint contextDepth, Ath__array1D<int> **contextArray, uint *contextLength, Ath__array1D<SEQ*> ***dgContextArray, uint *dgContextDepth, uint *dgContextPlanes, uint *dgContextTracks, uint *dgContextBaseLvl, int *dgContextLowLvl, int *dgContextHiLvl, uint *dgContextBaseTrack, int *dgContextLowTrack, int *dgContextHiTrack, int **dgContextTrackBase, AthPool<SEQ> *seqPool);
	void adjustOverlapMakerEnd();
	void adjustMetalFill();

	void setupForBoxes(adsRect maxRect, uint minHeight, uint minWidth);

	void setMaxArea(int x1, int y1, int x2, int y2);
	void resetMaxArea();

	void addBox(dbBox * bb, uint wtype, uint id);
	uint setExtrusionMarker(uint startRow, uint startCol);
	Ath__gridTable* getSearchPtr(); //EXT
	void setDefaultWireType(uint v); //EXT

	void searchBoxIds(int x1, int y1, int x2, int y2);


	uint addBox(int x1, int y1, int x2, int y2, uint level, uint id1, uint id2, uint wtype);
	void getBox(uint wid, int *x1, int *y1, int *x2, int *y2, uint *level, uint *id1, uint *id2, uint *wtype);
	uint addBlockages(dbBlock * block);
	void initSearchForNets(dbTech *tech, dbBlock *block);
	void addPowerNets(dbBlock *block, uint wtype, bool wireVia);
	void addSignalNets(dbBlock *block, uint wtype, bool wireVia);
	void addSignalNets(dbBlock *block, uint wtype, uint vtype);

	void searchWireIds(int x1, int y1, int x2, int y2, bool ignoreLayers, bool *exludeTable);
	uint makeGuiBoxes(ZPtr<IZdcr> dcr, uint hierType, uint boxType, bool useSdbId, uint excludeId1=0);
	uint searchSpread(void *ip, uint spreadTrackDist, std::vector<dbNet *> & inets, char *bbox, void (*doSpread)(void *ip, uint, void *, void *, void *));
	uint couplingCaps(ZPtr<ISdb> ccSdb, uint couplingDist, ZInterface *Interface, void (*extCompute)(int *, void *), void *);
	uint startIterator();
	uint getNextWireId();
	bool getNextBox(int &x1, int &y1, int &x2, int &y2, uint &level, uint &netId, uint &shapeId);
	void getCCdist(uint wid, uint *width, uint *level, uint *id1, uint *id2);
	void getIds(uint wid, uint *id1, uint *id2, uint *wtype);


/*
	ZPtr<Sdb_r> getReadInterface()
	{
		ZPtr<Sdb_r> p;
		adsNewComponent( _context, p );
		Sdb_r * s = (Sdb_r *) p.getPtr();
		s->_sdb = ZPtr<ISdb>(this);
		return p;
	}
	
	ZPtr<Sdb_w> getWriteInterface()
	{
		ZPtr<Sdb_w> p;
		adsNewComponent( _context, p );
		Sdb_w * s = (Sdb_w *) p.getPtr();
		s->_sdb = ZPtr<ISdb>(this);
		return p;
	}
*/
};

END_NAMESPACE_ADS

#endif
