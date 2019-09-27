#ifndef ADS_DB_REGION_H
#define ADS_DB_REGION_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class _dbInst;
class _dbBox;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbRegionFlags
{
    dbRegionType::Value  _type : 4;
    uint                 _invalid : 1;
    uint                 _spare_bits : 28;
};

class _dbRegion : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbRegionFlags   _flags;
    char *           _name;
    dbId<_dbInst>    _insts;
    dbId<_dbBox>     _boxes;
    dbId<_dbRegion>  _parent;
    dbId<_dbRegion>  _children;
    dbId<_dbRegion>  _next_child;

    _dbRegion( _dbDatabase * );
    _dbRegion( _dbDatabase *, const _dbRegion & b );
    ~_dbRegion();

    int operator==( const _dbRegion & rhs ) const;
    int operator!=( const _dbRegion & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbRegion & rhs ) const;

    void differences( dbDiff & diff, const char * field, const _dbRegion & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};


dbOStream & operator<<( dbOStream & stream, const _dbRegion & r );
dbIStream & operator>>( dbIStream & stream, _dbRegion & r );

END_NAMESPACE_ADS

#endif
