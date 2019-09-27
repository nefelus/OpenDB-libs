#ifndef ADS_DB_CORE_H
#define ADS_DB_CORE_H

///
/// This file contains the core objects that everything is
/// built from.
///
/// The core objects are:
///
///  dbId - included
///  dbObjectTable
///  dbObjectPage
///  dbTablePage
///

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_ATTR_TABLE_H
#include "dbAttrTable.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class _dbProperty;
class dbObjectTable;

#define DB_ALLOC_BIT   0x80000000
#define DB_OFFSET_MASK (~DB_ALLOC_BIT)

typedef dbObjectTable * (dbObject::*GetObjTbl_t)(dbObjectType);

///////////////////////////////////////////////////////////////
/// dbObjectTable definition
///////////////////////////////////////////////////////////////
class dbObjectTable
{
  public:
    // NON-PERSISTANT DATA
    _dbDatabase *     _db;
    dbObject *        _owner;
    dbObjectType      _type;
    uint              _obj_size;
    dbObjectTable * (dbObject::*_getObjectTable)( dbObjectType type );

    // PERSISTANT DATA
    dbAttrTable< dbId<_dbProperty> > _prop_list;

    virtual ~dbObjectTable() {};
    dbObjectTable();
    dbObjectTable( _dbDatabase * db, dbObject * owner,
                   dbObjectTable * (dbObject::*m)(dbObjectType),
                   dbObjectType type, uint size );

    dbId<_dbProperty> getPropList( uint oid )
    {
        return _prop_list.getAttr(oid);
    }
    
    void setPropList( uint oid, dbId<_dbProperty> propList )
    {
       _prop_list.setAttr(oid, propList);
    }

    virtual dbObject * getObject( register uint id, ... ) = 0;

    dbObjectTable * getObjectTable( dbObjectType type )
    {
        return (_owner->*_getObjectTable)(type);
    }
};

///////////////////////////////////////////////////////////////
/// _dbObject definition - free-list object
///////////////////////////////////////////////////////////////

class _dbObject : public dbObject
{
  public:
    uint _next;
    uint _prev;
};

///////////////////////////////////////////////////////////////
/// dbObjectPage definition
///////////////////////////////////////////////////////////////
class dbObjectPage
{
  public:
    // NON-PERSISTANT DATA
    dbObjectTable * _table;
    uint            _page_addr;
    uint            _alloccnt;

    bool valid_page() const { return _alloccnt != 0; }
};

///////////////////////////////////////////////////////////////
/// dbObjectTable implementation
///////////////////////////////////////////////////////////////
inline dbObjectTable::dbObjectTable()
{
    _db = NULL;
    _owner = NULL;
}

inline dbObjectTable::dbObjectTable( _dbDatabase * db, dbObject * owner,
                      dbObjectTable * (dbObject::*m)(dbObjectType),
                      dbObjectType type, uint size )
{
    _db = db;
    _owner = owner;
    _getObjectTable = m;
    _type = type;

    // Objects must be greater than 16-bytes
    assert( size >= sizeof(_dbObject) );
    _obj_size = size;
}

///////////////////////////////////////////////////////////////
/// dbObject inlines
///////////////////////////////////////////////////////////////

inline uint dbObject::getOID() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page->_page_addr | offset / page->_table->_obj_size;
}

inline dbObjectTable * dbObject::getTable() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page->_table;
}

inline _dbDatabase * dbObject::getDatabase() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page->_table->_db;
}

inline dbObject * dbObject::getOwner() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page->_table->_owner;
}

inline dbObjectType dbObject::getType() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page->_table->_type;
}

inline dbObjectPage * dbObject::getObjectPage() const
{
    register uint offset = (_oid & DB_OFFSET_MASK);
    register char * base = (char *) this - offset;
    register dbObjectPage * page = (dbObjectPage *) (base - sizeof(dbObjectPage));
    return page;
}

END_NAMESPACE_ADS

#endif
