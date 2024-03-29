#ifndef ADS_DB_TABLE_H
#define ADS_DB_TABLE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

#ifndef ADS_DB_ITERATOR_H
#include "dbIterator.h"
#endif

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

#include <vector>

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;

class dbTablePage : public dbObjectPage
{
  public:
    char _objects[1];
};

template <class T>
class dbTable : public dbObjectTable, public dbIterator
{
  public:
    // PERSISTANT-DATA
    uint              _page_mask;     // bit-mask to get page-offset
    uint              _page_shift;    // number of bits to shift to determine page-no
    uint              _top_idx;       // largest id which has been allocated.
    uint              _bottom_idx;    // smallest id which has been allocated.
    uint              _page_cnt;      // high-water mark of page-table
    uint              _page_tbl_size; // length of the page table
    uint              _alloc_cnt;     // number of object allocated
    uint              _free_list;  // objects on freelist

    // NON-PERSISTANT-DATA
    dbTablePage **    _pages;      // page-table

    void resizePageTbl();
    void newPage();
    void pushQ( uint & Q, _dbObject * e );
    _dbObject * popQ( uint & Q );
    void unlinkQ( uint & Q, _dbObject * e );

    dbTable( _dbDatabase * db, dbObject * owner,
              dbObjectTable * (dbObject::*m)(dbObjectType),
             dbObjectType type,
             uint page_size = 128, uint page_shift = 7);

    // Make a copy of a table.
    // The copy is identical including the ordering of all free-lists.
    //dbTable(_dbDatabase * db, dbObject * owner, const dbTable<T> & T );
    dbTable(_dbDatabase * db, dbObject * owner, const dbTable<T> &  );
    
    virtual ~dbTable();

    // returns the number of instances of "T" allocated
    uint size() const { return _alloc_cnt; }

    // Create a "T", calls T( _dbDatabase * )
    T * create();

    // Duplicate a "T", calls T( _dbDatabase *, const T & )
    T * duplicate( T * t );

    // Destroy instance of "T", calls destructor
    void destroy( T * );

    // clear the table
    void clear();

    uint page_size() const { return _page_mask + 1; }
    
    // Get the object of this id
    T * getPtr( uint id ) const
    {
        uint page = id >> _page_shift;
        uint offset = id & _page_mask;

        assert( (id != 0) && (page < _page_cnt) );
        T * p = (T *) &(_pages[page]->_objects[offset*sizeof(T)]);
        assert( p->_oid & DB_ALLOC_BIT );
        return  p;
    }

    bool validId( register uint id ) const
    {
        register uint page = id >> _page_shift;
        register uint offset = id & _page_mask;

        if( (id != 0) && (page < _page_cnt) )
        {
            T * p = (T *) &(_pages[page]->_objects[offset*sizeof(T)]);
            return (p->_oid & DB_ALLOC_BIT) == DB_ALLOC_BIT;
        }

        return false;
    }
    
    //
    // Get the object of this id
    // This method is the same as getPtr() but is is
    // use to get objects on the free-list.
    //
    T * getFreeObj( register uint id )
    {
        register uint page = id >> _page_shift;
        register uint offset = id & _page_mask;
        assert( (id != 0) && (page < _page_cnt) );
        T * p = (T *) &(_pages[page]->_objects[offset*sizeof(T)]);
        assert( (p->_oid & DB_ALLOC_BIT) == 0 );
        return  p;
    }

    // find the new top_idx...
    void findTop();

    // find the new bottom_idx...
    void findBottom();

    void readPage( dbIStream & stream, dbTablePage * page );
    void writePage( dbOStream & stream, const dbTablePage * page ) const;

    int operator==( const dbTable<T> & table ) const;
    int operator!=( const dbTable<T> & table ) const;
    void differences( dbDiff & diff, const dbTable<T> & rhs ) const;
    void out( dbDiff & diff, char side ) const;

    // dbIterator interface methods
    bool reversible();
    bool orderReversed();
    void reverse( register dbObject * parent );
    uint sequential();
    uint size( register dbObject * parent );
    uint begin( register dbObject * parent );
    uint end( register dbObject * parent );
    uint next( register uint cur, ... );
    dbObject * getObject( register uint cur, ... );
    void getObjects( std::vector<T *> & objects );
    
  private:
    void copy_pages( const dbTable<T> &  );
    void copy_page( uint page_id, dbTablePage * page );
};

template <class T>
dbOStream & operator<<( dbOStream & stream, const dbTable<T> & table );

template <class T>
dbIStream & operator>>( dbIStream & stream, dbTable<T> & table );

END_NAMESPACE_ADS

#endif
