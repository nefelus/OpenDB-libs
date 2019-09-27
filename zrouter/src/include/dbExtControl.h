#ifndef ADS_DB_EXTCONTROL_H
#define ADS_DB_EXTCONTROL_H

BEGIN_NAMESPACE_ADS
    
class dbExtControl : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    bool                       _independentExtCorners;
    bool                       _foreign;
    bool                       _wireStamped;
    bool                       _rsegCoord;
    bool                       _overCell;
    bool                       _extracted;
    bool                       _lefRC;
    uint                       _cornerCnt;
    uint                       _ccPreseveGeom;
    uint                       _ccUp;
    uint                       _couplingFlag;
    double                     _coupleThreshold;
    double                     _mergeResBound;
    bool                       _mergeViaRes;
    bool                       _mergeParallelCC;
    bool                       _exttreePreMerg;
    double                     _exttreeMaxcap;
    bool                       _useDbSdb;
    uint                       _CCnoPowerSource;
    uint                       _CCnoPowerTarget;
    bool                       _usingMetalPlanes;
    char *                     _ruleFileName;
	char *						_extractedCornerList;
    char *						_derivedCornerList;
    char *						_cornerIndexList;
    char *						_resFactorList;
    char *						_gndcFactorList;
    char *						_ccFactorList;


    dbExtControl();
    ~dbExtControl();
};

dbOStream & operator<<( dbOStream & stream, const dbExtControl & extControl );
dbIStream & operator>>( dbIStream & stream, dbExtControl & extControl );

END_NAMESPACE_ADS

#endif
