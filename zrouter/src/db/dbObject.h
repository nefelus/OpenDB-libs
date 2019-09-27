#ifndef ADS_DB_OBJECT_H
#define ADS_DB_OBJECT_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

#ifndef DB_PRIVATE
#define DB_PRIVATE private
#endif

///
/// When adding a new database object, you must add a dbObjectType enumerator and
/// edit dbObject.cpp and assign an unique "character" code for its database-name.
/// See the methods:
///    void getDbName( char name[256] );
///    static dbObject * resolveDbName( dbDatabase * db, const char * name );
/// in dbObject.cpp
///
class _dbDatabase;
class dbOStream;
class dbIStream;
class dbObjectPage;
class dbObjectTable;
class dbDiff;

///
/// Steps to add new objects -
///      - add the "name_tbl" entry in dbObject.cpp
///      - add the table entry to the getObjectTable method of the container object.
///
enum dbObjectType
{
    dbDatabaseObj,

    // Design Objects
    dbChipObj,
    dbBlockObj,
    dbInstHdrObj,
    dbInstObj,
    dbNetObj,
    dbBTermObj,
    dbITermObj,
    dbBoxObj,
    dbViaObj,
    dbGCellGridObj,
    dbTrackGridObj,
    dbObstructionObj,
    dbBlockageObj,
    dbWireObj,
    dbSWireObj,
    dbSBoxObj,
    dbCapNodeObj,
    dbRSegObj,
    dbCCSegObj,
    dbRowObj,
    dbTmgObj,
    dbMetricsObj,
    dbRegionObj,
    dbHierObj,
    dbBPinObj,

    // Lib Objects
    dbLibObj,
    dbSiteObj,
    dbMasterObj,
    dbMPinObj,
    dbMTermObj,
    dbTargetObj,
    dbTechAntennaPinModelObj,

    // Tech Objects
    dbTechObj,
    dbTechLayerObj,
    dbTechViaObj,
    dbTechNonDefaultRuleObj,        // also a design object
    dbTechLayerRuleObj,             // also a design object
    dbTechSameNetRuleObj,
    dbTechLayerSpacingRuleObj,
    dbTechMinCutRuleObj,
    dbTechMinEncRuleObj,
    dbTechV55InfluenceEntryObj,
    dbTechLayerAntennaRuleObj,
    dbTechViaRuleObj,
    dbTechViaGenerateRuleObj,
    dbTechViaLayerRuleObj,

    // Property
    dbPropertyObj,
    dbNameObj
};

class dbDatabase;

class dbObject
{
  DB_PRIVATE:
    uint _oid;
    
    // These functions are all inlines, so the cannot be called public
    _dbDatabase * getDatabase() const;
    dbObjectTable * getTable() const;
    dbObjectPage * getObjectPage() const;
    dbObject * getOwner() const;
    dbObjectType getType() const;
    uint getOID() const;

  public:
    dbObject() {}
    ~dbObject() {}
    dbObjectType getObjectType() const;
    dbDatabase * getDb() const;
    uint getId() const;
    void getDbName( char name[256] ) const;
    const char * getObjName() const;
    static dbObject * resolveDbName( dbDatabase * db, const char * name );
    static const char * getObjName( dbObjectType type );
    friend class dbObjectTable;
};

#endif

END_NAMESPACE_ADS
