#ifndef ADS_DB_CCSEG_H
#define ADS_DB_CCSEG_H

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

#include "dbDatabase.h"

BEGIN_NAMESPACE_ADS

class _dbCapNode;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//struct _dbCCSegFlags
//{
//    uint             _cnt          : 8;
//    uint             _spef_mark_1  : 1;
//    uint             _mark         : 1;
//    uint             _inFileCnt    : 4;
//    uint             _spare_bits   : 18;
//};

struct _dbCCSegFlags
{
    uint             _spef_mark_1  : 1;
    uint             _mark         : 1;
    uint             _inFileCnt    : 4;
    uint             _spare_bits   : 26;
};

class _dbCCSeg : public dbObject
{
  public:
    enum Fields  // dbJournal field names
    {
        FLAGS,
        CAPACITANCE,
        ADDCCCAPACITANCE,
        SWAPCAPNODE,
        LINKCCSEG,
        UNLINKCCSEG,
        SETALLCCCAP
    };
    
    // PERSISTANT-MEMBERS
    _dbCCSegFlags    _flags;
    dbId<_dbCapNode> _cap_node[2];
    dbId<_dbCCSeg>   _next[2];
    
    _dbCCSeg( _dbDatabase * db );
    _dbCCSeg( _dbDatabase * db, const _dbCCSeg & s );
    ~_dbCCSeg();

    const int idx( dbId<_dbCapNode> n ) { return n == _cap_node[0] ? 0 : 1; }
    dbId<_dbCCSeg> & next( dbId<_dbCapNode> n ) { return _next[idx(n)]; }

    int operator==( const _dbCCSeg & rhs ) const;
    int operator!=( const _dbCCSeg & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbCCSeg & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    int operator<( const _dbCCSeg & rhs ) const
    {
        _dbCCSeg * o1 = (_dbCCSeg *) this;
        _dbCCSeg * o2 = (_dbCCSeg *) &rhs;
        return o1->getOID() < o2->getOID();
    }
};

inline _dbCCSeg::_dbCCSeg( _dbDatabase * )
{
    _flags._inFileCnt = 0;
    _flags._spef_mark_1 = 0;
    _flags._mark = 0;
    _flags._spare_bits = 0;
}

inline _dbCCSeg::_dbCCSeg( _dbDatabase *, const _dbCCSeg & s )
        : _flags(s._flags)
{
    _cap_node[0] = s._cap_node[0];
    _cap_node[1] = s._cap_node[1];
    _next[0] = s._next[0];
    _next[1] = s._next[1];
    
}

inline _dbCCSeg::~_dbCCSeg()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbCCSeg & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream << *bit_field;
    stream << seg._cap_node[0];
    stream << seg._cap_node[1];
    stream << seg._next[0];
    stream << seg._next[1];
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbCCSeg & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream >> *bit_field;
    if (!stream.getDatabase()->isSchema(ADS_DB_INDEPENDENT_EXT_CORNERS))
        *bit_field = (*bit_field)>>8;
    stream >> seg._cap_node[0];
    stream >> seg._cap_node[1];
    stream >> seg._next[0];
    stream >> seg._next[1];
    return stream;
}

END_NAMESPACE_ADS

#endif
