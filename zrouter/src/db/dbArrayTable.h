#ifndef ADS_DB_ARRAY_TABLE_H
#define ADS_DB_ARRAY_TABLE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
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

class dbArrayTablePage : public dbObjectPage
{
  public:
    char _objects[1];
};

//
// dbArrayTable - Specialized class to allocate a fixed array of objects.
//                The array is indexing is only in terms of database IDs.
//                Also, this class does not free-pages when they become
//                empty, because an array may cross the page boundary.
//
template <class T>
class dbArrayTable : public dbObjectTable
{
  public:
    // PERSISTANT-DATA
    uint              _page_mask;     // bit-mask to get page-offset
    uint              _page_shift;    // number of bits to shift to determine page-no
    uint              _page_cnt;      // high-water mark of page-table
    uint              _page_tbl_size; // length of the page table
    uint              _alloc_cnt;     // number of object allocated
    uint              _objects_per_alloc; // number of object to allocate per array
    uint              _free_list;  // objects on freelist

    // NON-PERSISTANT-DATA
    dbArrayTablePage ** _pages;      // page-table

    void resizePageTbl();
    void newPage();
    void pushQ( uint & Q, _dbObject * e );
    _dbObject * popQ( uint & Q );
    void unlinkQ( uint & Q, _dbObject * e );

    dbArrayTable( _dbDatabase * db, dbObject * owner, GetObjTbl_t m, dbObjectType type,
                  uint array_size, uint page_size = 128, uint page_shift = 7);

    // Make a copy of a table.
    // The copy is identical including the ordering of all free-lists.
    dbArrayTable(_dbDatabase * db, dbObject * owner, const dbArrayTable<T> & );
    
    virtual ~dbArrayTable();

    // returns the number of instances of "T" allocated
    uint size() const { return _alloc_cnt; }

    // returns the number of objects per array
    uint arraySize() const { return _objects_per_alloc; }

    // Create a "T", calls T( _dbDatabase * )
    dbId<T> createArray();

    // Destroy instance of "T", calls destructor
    void destroyArray( dbId<T> t );

    // clear the table
    void clear();

    // clear the table and set the array-size
    //void reset( int array_size );

    uint page_size() const { return _page_mask + 1; }
    
    // Get the object of this id
    T * getPtr( register uint id ) const
    {
        register uint page = id >> _page_shift;
        register uint offset = id & _page_mask;
        ZASSERT( (id != 0) && (page < _page_cnt) );
        T * p = (T *) &(_pages[page]->_objects[offset*sizeof(T)]);
        ZASSERT( p->_oid & DB_ALLOC_BIT );
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
    // used to get objects on the free-list.
    //
    T * getFreeObj( register uint id )
    {
        register uint page = id >> _page_shift;
        register uint offset = id & _page_mask;
        ZASSERT( (id != 0) && (page < _page_cnt) );
        T * p = (T *) &(_pages[page]->_objects[offset*sizeof(T)]);
        ZASSERT( (p->_oid & DB_ALLOC_BIT) == 0 );
        return  p;
    }

    int operator==( const dbArrayTable<T> & table ) const;
    int operator!=( const dbArrayTable<T> & table ) const;
    void differences( dbDiff & diff, const dbArrayTable<T> & rhs ) const;
    void out( dbDiff & diff, char side ) const;

    void readPage( dbIStream & stream, dbArrayTablePage * page );
    void writePage( dbOStream & stream, const dbArrayTablePage * page ) const;
    void getObjects( std::vector<T *> & objects );
    dbObject * getObject( register uint id, ... ) { return getPtr(id); }
    
  private:
    T * create();
    void destroy( T * t );
    void copy_pages( const dbArrayTable<T> &  );
    void copy_page( uint page_id, dbArrayTablePage * page );
};

template <class T>
dbOStream & operator<<( dbOStream & stream, const dbArrayTable<T> & table );

template <class T>
dbIStream & operator>>( dbIStream & stream, dbArrayTable<T> & table );

END_NAMESPACE_ADS

#endif
