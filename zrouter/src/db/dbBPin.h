#ifndef ADS_DB_BPIN_H
#define ADS_DB_BPIN_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbBTerm;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbBPinFlags
{
    dbPlacementStatus::Value   _status              : 4;
    uint                       _has_min_spacing     : 1;
    uint                       _has_effective_width : 1;
    uint                       _spare_bits : 26;
};

class _dbBPin : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbBPinFlags   _flags;
    dbId<_dbBTerm> _bterm;
    dbId<_dbBox>   _bbox;
    dbId<_dbBPin>  _next_bpin;
    uint           _min_spacing;      // 5.6 DEF
    uint           _effective_width;  // 5.6 DEF

    _dbBPin( _dbDatabase *, const _dbBPin & p );
    _dbBPin( _dbDatabase * );
    ~_dbBPin();

    int operator==( const _dbBPin & rhs ) const;
    int operator!=( const _dbBPin & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbBPin & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbIStream & operator>>( dbIStream & stream, _dbBPin & bpin );
dbOStream & operator<<( dbOStream & stream, const _dbBPin & bpin );


END_NAMESPACE_ADS

#endif
