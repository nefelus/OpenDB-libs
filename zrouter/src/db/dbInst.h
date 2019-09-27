#ifndef ADS_DB_INST_H
#define ADS_DB_INST_H

#ifndef ADS_H
#include "ads.h"
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

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"                // disconnect the child-iterm

#endif

#ifndef ADS_DB_DATABASE_H
#include "dbDatabase.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbBox;
class _dbInstHdr;
class _dbHier;
class _dbITerm;
class _dbRegion;
class _dbDatabase;
class dbInst;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbInstFlags
{
    dbOrientType::Value      _orient      : 4;
    dbPlacementStatus::Value _status      : 4;
    uint                     _user_flag_1 : 1;
    uint                     _user_flag_2 : 1;
    uint                     _user_flag_3 : 1;
    uint                     _size_only   : 1;
    uint                     _dont_touch  : 1;
    uint                     _dont_size   : 1;
    dbSourceType::Value      _source      : 4;
    uint                     _eco_create  : 1;
    uint                     _eco_destroy : 1;
    uint                     _eco_modify  : 1;
    uint                     _input_cone  : 1;
    uint                     _inside_cone  : 1;
    uint                     _level        : 9;
    //uint                     _spare_bits  : 11;
};

class _dbInst : public dbObject
{
  public:

    enum Field  // dbJournalField name
    {
        FLAGS,
        ORIGIN,
        INVALIDATETIMING
    };

    _dbInstFlags     _flags;
    char *           _name;
    int              _x;
    int              _y;
    int              _weight;
    dbId<_dbInst>    _next_entry;
    dbId<_dbInstHdr> _inst_hdr;
    dbId<_dbBox>     _bbox;
    dbId<_dbRegion>  _region;
    dbId<_dbInst>    _region_next;
    dbId<_dbInst>    _region_prev;
    dbId<_dbHier>    _hierarchy;
    dbVector< uint>  _iterms;
    dbId<_dbBox>     _halo;

    _dbInst( _dbDatabase * );
    _dbInst( _dbDatabase *, const _dbInst & i );
    ~_dbInst();

    int operator==( const _dbInst & rhs ) const;
    int operator!=( const _dbInst & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbInst & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbInst & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream,  const _dbInst & inst );
dbIStream & operator>>( dbIStream & stream, _dbInst & inst );

END_NAMESPACE_ADS

#endif
