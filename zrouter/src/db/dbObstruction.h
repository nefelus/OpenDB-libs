#ifndef ADS_DB_OBSTRUCTION_H
#define ADS_DB_OBSTRUCTION_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbInst;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbObstructionFlags
{
    uint _slot_obs            : 1;
    uint _fill_obs            : 1;
    uint _pushed_down         : 1;
    uint _has_min_spacing     : 1;
    uint _has_effective_width : 1;
    uint _spare_bits          : 27;
};

class _dbObstruction : public dbObject
{
  public:
    _dbObstructionFlags   _flags;
    dbId<_dbInst>         _inst;
    dbId<_dbBox>          _bbox;
    int                   _min_spacing;
    int                   _effective_width;

    _dbObstruction( _dbDatabase *, const _dbObstruction & o );
    _dbObstruction( _dbDatabase * );
    ~_dbObstruction();

    int operator==( const _dbObstruction & rhs ) const;
    int operator!=( const _dbObstruction & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbObstruction & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbObstruction & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbObstruction & obs );
dbIStream & operator>>( dbIStream & stream, _dbObstruction & obs );

END_NAMESPACE_ADS

#endif
