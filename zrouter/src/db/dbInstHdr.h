#ifndef ADS_DB_INST_HDR_H
#define ADS_DB_INST_HDR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbLib;
class dbMaster;
class _dbLib;
class _dbMaster;
class _dbMTerm;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

class dbInstHdr : public dbObject
{
 public:
    dbBlock * getBlock();
    dbLib * getLib();
    dbMaster * getMaster();
    
    static dbInstHdr * create( dbBlock * block, dbMaster * master );
    static void destroy( dbInstHdr * hdr );
};

class _dbInstHdr : public dbObject
{
  public:
    int                        _mterm_cnt;
    uint                       _id;
    dbId<_dbInstHdr>           _next_entry;
    dbId<_dbLib>               _lib;
    dbId<_dbMaster>            _master;
    dbVector< dbId<_dbMTerm> > _mterms;
    int                        _inst_cnt; // number of instances of this InstHdr

    _dbInstHdr(_dbDatabase * db);
    _dbInstHdr(_dbDatabase * db, const _dbInstHdr & i);
    ~_dbInstHdr();
    int operator==( const _dbInstHdr & rhs ) const;
    int operator!=( const _dbInstHdr & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbInstHdr & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbInstHdr & inst_hdr );
dbIStream & operator>>( dbIStream & stream, _dbInstHdr & inst_hdr );

END_NAMESPACE_ADS

#endif
