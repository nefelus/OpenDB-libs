#ifndef ADS_DB_DATABASE_H
#define ADS_DB_DATABASE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

BEGIN_NAMESPACE_ADS

//
// When changing the database schema please add a #define to refer to the schema changes.
// Use the define statement along with the isSchema(rev) method:
//
// GOOD:
//
//    if ( db->isSchema(ADS_DB_REGION_SCHEMA) )
//    {
//     ....
//    }
//
// Don't use a revision number in the code, because it is hard to read:
//
// BAD:
//
//    if ( db->_schema_minor > 33 )
//    {
//     ....
//    }
//

//
// Schema Revisions
//
#define ADS_DB_SCHEMA_MAJOR                 0  // Not used...
#define ADS_DB_REGION_SCHEMA                33
#define ADS_DB_HIER_REGION_SCHEMA           34
#define ADS_DB_HIER_INST_SCHEMA             35
#define ADS_DB_EXT_CORNERS_SCHEMA           36
#define ADS_DB_EXT_CORNERS_SCHEMA           36
#define ADS_DB_EXT_CONTROL_CORNERS_SCHEMA   37
#define ADS_DB_DEF_5_6                      38
#define ADS_DB_DEF_ATTRS                    39
#define ADS_DB_EXT_RSEG_COORDS              40
#define ADS_DB_PROPERTIES                   41
#define ADS_DB_TRANSFORM_FIX                42
#define ADS_DB_TRANSFORM_FIX2               43
#define ADS_DB_ITERM_tmgTmpD                44
#define ADS_DB_BLOCK_STREAM_PROPERTIES      45
#define ADS_DB_BLOCK_CHILDREN_V1            46
#define ADS_DB_TMG_SCENARIO                 47
#define ADS_DB_INDEPENDENT_EXT_CORNERS      48
#define ADS_DB_ADJUSTCC                     49
#define ADS_DB_5BITCAPNODECHILDRENCNT       50
#define ADS_DB_EXT_CONTROL_STAMPWIRE        51
#define ADS_DB_SCHEMA_MINOR                 51 // Current revision number

template <class T> class dbTable;
class _dbProperty;
class dbPropertyItr;
class _dbNameCache;
class _dbTech;
class _dbChip;
class _dbLib;
class dbOStream;
class dbIStream;
class dbDiff;

class _dbDatabase : public dbObject
{
  public:
    // PERSISTANT_MEMBERS
    uint                _magic1;
    uint                _magic2;
    uint                _schema_major;
    uint                _schema_minor;
    uint                _master_id;    // for a unique id across all libraries
    dbId<_dbChip>       _chip;
    dbId<_dbTech>       _tech;

    // NON_PERSISTANT_MEMBERS
    dbTable<_dbTech> *     _tech_tbl;
    dbTable<_dbLib> *      _lib_tbl;
    dbTable<_dbChip> *     _chip_tbl;
    dbTable<_dbProperty> * _prop_tbl;
    _dbNameCache *         _name_cache;
    dbPropertyItr *        _prop_itr;
    int                    _unique_id;

    char *            _file;
    
    _dbDatabase( _dbDatabase * db );
    _dbDatabase( _dbDatabase * db, int id );
    _dbDatabase( _dbDatabase * db, const _dbDatabase & d );
    ~_dbDatabase();

    int operator==( const _dbDatabase & rhs ) const;
    int operator!=( const _dbDatabase & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbDatabase & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    bool isSchema( uint rev ) { return _schema_minor >= rev; }
    bool isLessThanSchema( uint rev ) { return _schema_minor < rev; }
    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbDatabase & db );
dbIStream & operator>>( dbIStream & stream, _dbDatabase & db );

END_NAMESPACE_ADS

#endif
