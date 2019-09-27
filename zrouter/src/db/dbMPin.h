#ifndef ADS_DB_MPIN_H
#define ADS_DB_MPIN_H

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

class _dbMTerm;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbMPin : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    dbId<_dbMTerm>  _mterm;
    dbId<_dbBox>    _geoms;
    dbId<_dbMPin>   _next_mpin;

    _dbMPin( _dbDatabase *, const _dbMPin & p );
    _dbMPin( _dbDatabase * );
    ~_dbMPin();

    int operator==( const _dbMPin & rhs ) const;
    int operator!=( const _dbMPin & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbMPin & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbMPin & mpin );
dbIStream & operator>>( dbIStream & stream, _dbMPin & mpin );

END_NAMESPACE_ADS

#endif
