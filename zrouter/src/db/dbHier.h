#ifndef ADS_DB_HIER_H
#define ADS_DB_HIER_H

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

class _dbInst;
class _dbBlock;
class _dbBTerm;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//
// Hidden proxy object hierarchy connectivity.
//
class _dbHier : public dbObject
{
  public:
    dbId<_dbInst>              _inst;
    dbId<_dbBlock>             _child_block;
    dbVector< dbId<_dbBTerm> > _child_bterms;

    _dbHier(_dbDatabase * db);
    _dbHier(_dbDatabase * db, const _dbHier & h);
    ~_dbHier();
    int operator==( const _dbHier & rhs ) const;
    int operator!=( const _dbHier & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbHier & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    static _dbHier * create( dbInst * inst, dbBlock * child );
    static void destroy( _dbHier * hdr );
};

dbOStream & operator<<( dbOStream & stream, const _dbHier & inst_hdr );
dbIStream & operator>>( dbIStream & stream, _dbHier & inst_hdr );

END_NAMESPACE_ADS

#endif
