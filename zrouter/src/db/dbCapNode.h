#ifndef ADS_DB_CAPNODE_H
#define ADS_DB_CAPNODE_H

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

class _dbNet;
class _dbCCSeg;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//struct _dbCapNodeFlags
//{
//    uint             _cnt          : 7;
//    uint             _internal     : 1;
//    uint             _iterm        : 1;
//    uint             _bterm        : 1;
//    uint             _branch       : 1;
//    uint             _foreign      : 1;
//    uint             _childrenCnt  : 3;
//    uint             _select       : 1;
//    union {
//            uint             _spare_bits   : 16;
//            uint             _sort_index   : 16;
//    };
//};

struct _dbCapNodeFlags
{
    uint             _internal     : 1;
    uint             _iterm        : 1;
    uint             _bterm        : 1;
    uint             _branch       : 1;
    uint             _foreign      : 1;
    uint             _childrenCnt  : 5;
    uint             _select       : 1;
    union {
            uint             _spare_bits   : 20;
            uint             _sort_index   : 20;
    };
    uint             _name         : 1;
    //uint             _dest         : 1;
};

class _dbCapNode : public dbObject
{
  public:
    enum Fields // dbJournal field names
    {
        FLAGS,
        NODE_NUM,
        CAPACITANCE,
        ADDCAPNCAPACITANCE,
        SETNET,
        SETNEXT
    };
    
    // PERSISTANT-MEMBERS
    _dbCapNodeFlags   _flags;
    uint              _node_num; // rc-network node-id
    dbId<_dbNet>      _net;
    dbId<_dbCapNode>  _next;
    dbId<_dbCCSeg>    _cc_segs;
    
    _dbCapNode( _dbDatabase *);
    _dbCapNode( _dbDatabase *, const _dbCapNode & n );
    ~_dbCapNode();

    int operator==( const _dbCapNode & rhs ) const;
    int operator!=( const _dbCapNode & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbCapNode & rhs ) const
    {
        _dbCapNode * o1 = (_dbCapNode *) this;
        _dbCapNode * o2 = (_dbCapNode *) &rhs;
        return o1->getOID() < o2->getOID();
    }
    
    void differences( dbDiff & diff, const char * field, const _dbCapNode & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbCapNode::_dbCapNode( _dbDatabase * )
{
    _flags._internal= 0;
    _flags._iterm= 0;
    _flags._bterm= 0;
    _flags._branch= 0;
    _flags._foreign= 0;
    _flags._childrenCnt = 0;
    _flags._select= 0;
    _flags._name= 0;
    _flags._spare_bits = 0;
    _net= 0;
    _node_num= 0;
}

inline _dbCapNode::_dbCapNode( _dbDatabase *, const _dbCapNode & n )
        : _flags( n._flags ),
          _node_num( n._node_num ),
          _net( n._net ),
          _next( n._next )
{
}

inline _dbCapNode::~_dbCapNode()
{
}

inline dbOStream & operator<<( dbOStream & stream, const _dbCapNode & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream << *bit_field;


    stream << seg._node_num;
    stream << seg._net;
    stream << seg._next;
    stream << seg._cc_segs;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbCapNode & seg )
{
    uint *bit_field = (uint *) &seg._flags;
    stream >> *bit_field;
    if (!stream.getDatabase()->isSchema(ADS_DB_INDEPENDENT_EXT_CORNERS))
        *bit_field = (*bit_field)>>7;
    if (!stream.getDatabase()->isSchema(ADS_DB_5BITCAPNODECHILDRENCNT))
    {
        seg._flags._select = (seg._flags._childrenCnt % 16) >> 3;
        seg._flags._childrenCnt = seg._flags._childrenCnt % 8;
    }

    stream >> seg._node_num;
    stream >> seg._net;
    stream >> seg._next;
    stream >> seg._cc_segs;
    return stream;
}

END_NAMESPACE_ADS

#endif
