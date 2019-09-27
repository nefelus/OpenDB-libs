#ifndef ADS_DB_CHIP_H
#define ADS_DB_CHIP_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;
class _dbProperty;
class dbPropertyItr;
class _dbNameCache;
class _dbTech;
class _dbBlock;
class _dbDatabase;
class dbBlockItr;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbChip : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    dbId<_dbBlock>         _top;

    // NON-PERSISTANT-MEMBERS
    dbTable<_dbBlock> *    _block_tbl;
    dbTable<_dbProperty> * _prop_tbl;
    _dbNameCache *         _name_cache;
    dbBlockItr *           _block_itr;
    dbPropertyItr *        _prop_itr;
    
    _dbChip( _dbDatabase * db );
    _dbChip( _dbDatabase * db, const _dbChip & c );
    ~_dbChip();

    int operator==( const _dbChip & rhs ) const;
    int operator!=( const _dbChip & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbChip & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbChip & chip );
dbIStream & operator>>( dbIStream & stream, _dbChip & chip );

END_NAMESPACE_ADS

#endif
