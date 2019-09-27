#ifndef ADS_DB_LIB_H
#define ADS_DB_LIB_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

#ifndef ADS_DB_HASH_TABLE_H
#include "dbHashTable.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;
class _dbProperty;
class dbPropertyItr;
class _dbNameCache;
class _dbTech;
class _dbMaster;
class _dbSite;
class _dbDatabase;
class dbString;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbLib : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    int                     _lef_units;
    int                     _dbu_per_micron; // cached value from dbTech
    char                    _hier_delimeter;
    char                    _left_bus_delimeter;
    char                    _right_bus_delimeter;
    char                    _spare;
    char *                  _name;
    dbHashTable<_dbMaster>  _master_hash;
    dbHashTable<_dbSite>    _site_hash;

    // NON-PERSISTANT-MEMBERS
    dbTable<_dbMaster> *    _master_tbl;
    dbTable<_dbSite> *      _site_tbl;
    dbTable<_dbProperty> *  _prop_tbl;
    _dbNameCache *          _name_cache;

    dbPropertyItr *         _prop_itr;

    _dbLib( _dbDatabase * db );
    _dbLib( _dbDatabase * db, const _dbLib & l );
    ~_dbLib();
    int operator==( const _dbLib & rhs ) const;
    int operator!=( const _dbLib & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbLib & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbLib & lib );
dbIStream & operator>>( dbIStream & stream, _dbLib & lib );

END_NAMESPACE_ADS

#endif
