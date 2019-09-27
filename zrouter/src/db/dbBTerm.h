#ifndef ADS_DB_BTERM_H
#define ADS_DB_BTERM_H

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

#ifndef ADS_DB_DATABASE_H
#include "dbDatabase.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbNet;
class _dbBox;
class _dbTmg;
class _dbBlock;
class _dbBPin;
class _dbITerm;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbBTermFlags
{
    dbIoType::Value            _io_type    : 4;
    dbSigType::Value           _sig_type   : 4;
    uint                       _orient     : 4; // This field is not used anymore. Replaced by bpin...
    uint                       _status     : 4; // This field is not used anymore. Replaced by bpin...
    uint                       _spef       : 1;
    uint                       _special    : 1;
    uint                       _mark       : 1;
    uint                       _tmgTmpA    : 1;
    uint                       _tmgTmpB    : 1;
    uint                       _tmgTmpC    : 1; // payam
    uint                       _tmgTmpD    : 1; // payam
    uint                       _spare_bits : 11;
};

//
// block terminal
//
class _dbBTerm : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbBTermFlags              _flags;
    uint                       _ext_id;
    char *                     _name;
    dbId<_dbBTerm>             _next_entry;
    dbId<_dbNet>               _net;
    dbId<_dbTmg>               _tmg;
    dbId<_dbBTerm>             _next_bterm;
    dbId<_dbBTerm>             _prev_bterm;
    dbId<_dbBlock>             _parent_block; // Up hierarchy: TWG
    dbId<_dbITerm>             _parent_iterm; // Up hierarchy: TWG
    dbId<_dbBPin>              _bpins;        // Up hierarchy: TWG
    dbId<_dbBPin>              _ground_pin;
    dbId<_dbBPin>              _supply_pin;

    _dbBTerm( _dbDatabase * );
    _dbBTerm( _dbDatabase *, const _dbBTerm & b );
    ~_dbBTerm();

    int operator==( const _dbBTerm & rhs ) const;
    int operator!=( const _dbBTerm & rhs ) const { return ! operator==(rhs); }
    int operator<(const _dbBTerm & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbBTerm & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbBTerm & bterm );
dbIStream & operator>>( dbIStream & stream, _dbBTerm & bterm );

END_NAMESPACE_ADS

#endif
