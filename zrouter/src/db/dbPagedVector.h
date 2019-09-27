#ifndef ADS_DB_PAGED_VECTOR_H
#define ADS_DB_PAGED_VECTOR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZEXCEPTION_H
#include "ZException.h"
#endif

#ifndef ADS_DB_STREAM_H
#include "dbStream.h"
#endif

#ifndef ADS_DB_DIFF_H
#include "dbDiff.h"
#endif

#include "logger.h"
BEGIN_NAMESPACE_ADS

template <class T, const uint P, const uint S> class dbPagedVector;
class dbDiff;

//
// Vector - Creates a vector of type T. However, the vector is created
// as a series of pages of type T. The size of the page is specified 
// by the template params.
//
// PAGE_SIZE = number of objects per page (MUST BE POWER OF TWO)
// PAGE_SHIFT = log2(PAGE_SIZE)
//
template <class T, const uint PAGE_SIZE = 128, const uint PAGE_SHIFT = 7>
class dbPagedVector
{
  private:
    T **              _pages;
    unsigned int      _page_cnt;
    unsigned int      _page_tbl_size;
    unsigned int      _next_idx;

    int _freedIdxHead; // DKF - to delete
    int _freedIdxTail; // DKF - to delete

    void resizePageTbl();
    void newPage();

  public:

    dbPagedVector();
    dbPagedVector( const dbPagedVector<T,PAGE_SIZE,PAGE_SHIFT> & V );
    ~dbPagedVector();

    void push_back( const T & item );

    uint push_back( int cnt, const T & item )
    {
        uint id = _next_idx;
        uint i;
        for ( i = 0; i < cnt; ++i )
            push_back(item);
        return id;
    }
      
    unsigned int size() const { return _next_idx; }
    unsigned int getIdx(uint chunkSize, const T & ival); // DKF - to delete
    void freeIdx (uint idx); // DKF - to delete
    void clear();
    
    T & operator[]( register unsigned int id )
    {
        ZASSERT( id < _next_idx );
        register unsigned int page = (id & ~(PAGE_SIZE-1)) >> PAGE_SHIFT;
        register unsigned int offset = id & (PAGE_SIZE-1);
        return _pages[page][offset];
    }
    const T & operator[]( register unsigned int id ) const
    {
        ZASSERT( id < _next_idx );
        register unsigned int page = (id & ~(PAGE_SIZE-1)) >> PAGE_SHIFT;
        register unsigned int offset = id & (PAGE_SIZE-1);
        return _pages[page][offset];
    }

    int operator==( const dbPagedVector<T,PAGE_SIZE,PAGE_SHIFT> & rhs ) const;
    int operator!=( const dbPagedVector<T,PAGE_SIZE,PAGE_SHIFT> & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const dbPagedVector<T,PAGE_SIZE,PAGE_SHIFT> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

template <class T, const uint P, const uint S>
unsigned int dbPagedVector<T,P,S>::getIdx(uint chunkSize, const T & ival)
{ 
    uint idx;
    if ( _freedIdxHead == -1)
    {
        idx = _next_idx;
        _next_idx += chunkSize;
        register unsigned int page = ((_next_idx-1) & ~(P-1)) >> S;
        if ( page == _page_cnt )
            newPage();
        //return idx;
    }
    else
    {
        idx = (uint) _freedIdxHead;
        if (idx == (uint) _freedIdxTail)
            _freedIdxHead = -1;
        else
        {
            register unsigned int page = (idx & ~(P-1)) >> S;
            register unsigned int offset = idx & (P-1);
            uint *fidxp = (uint *)(&_pages[page][offset]);
            _freedIdxHead = (int) *fidxp;
        }
    }
    for (uint dd = 0; dd < chunkSize; dd++)
    {
        uint id = idx + dd;
        ZASSERT( id < _next_idx );
        register unsigned int page = (id & ~(P-1)) >> S;
        register unsigned int offset = id & (P-1);
        _pages[page][offset] = ival;
    }
    return idx;
}

template <class T, const uint P, const uint S>
void dbPagedVector<T,P,S>::freeIdx (uint idx)
{
    if (_freedIdxHead == -1)
        _freedIdxHead = _freedIdxTail = idx;
    else
    {
        register unsigned int page = (_freedIdxTail & ~(P-1)) >> S;
        register unsigned int offset = _freedIdxTail & (P-1);
        uint *fidxp = (uint *)(&_pages[page][offset]);
        *fidxp = _freedIdxTail = idx;
    }
}

template <class T, const uint P, const uint S>
dbPagedVector<T,P,S>::dbPagedVector()
{
    _pages = NULL;
    _page_cnt = 0;
    _page_tbl_size = 0;
    _next_idx = 0;
    _freedIdxHead = -1;
}

template <class T, const uint P, const uint S>
dbPagedVector<T,P,S>::dbPagedVector( const dbPagedVector<T,P,S> & V )
{
    _pages = NULL;
    _page_cnt = 0;
    _page_tbl_size = 0;
    _next_idx = 0;
    _freedIdxHead = -1;
    uint sz = V.size();
    
    uint i;
    for( i = 0; i < sz; ++i )
    {
        push_back(V[i]);
    }
}

template <class T, const uint P, const uint S>
void dbPagedVector<T,P,S>::clear()
{
    if ( _pages )
    {
        unsigned int i;

        for( i = 0; i < _page_cnt; ++i )
            delete[] _pages[i];

        delete[] _pages;
    }

    _pages = NULL;
    _page_cnt = 0;
    _page_tbl_size = 0;
    _next_idx = 0;
    _freedIdxHead = -1;
}

template <class T, const uint P, const uint S>
    dbPagedVector<T,P,S>::~dbPagedVector()
{
    clear();
}

template <class T, const uint P, const uint S>
void dbPagedVector<T,P,S>::resizePageTbl()
{
    T ** old_tbl = _pages;
    unsigned int old_tbl_size = _page_tbl_size;

    if ( _page_tbl_size == 1 )
        ++_page_tbl_size;
    else
        _page_tbl_size += (unsigned int) ((float) _page_tbl_size * (0.5));

    _pages = new T *[_page_tbl_size];
    ZALLOCATED(_pages);

    unsigned int i;

    for( i = 0; i < old_tbl_size; ++i )
        _pages[i] = old_tbl[i];

    for( ; i < _page_tbl_size; ++i )
        _pages[i] = NULL;

    delete[] old_tbl;
}

template <class T, const uint P, const uint S>
void dbPagedVector<T,P,S>::newPage()
{
    T * page = new T[P];
    ZALLOCATED(page);

    if ( _page_tbl_size == 0 )
    {
        _pages = new T *[1];
        ZALLOCATED(_pages);
        _page_tbl_size = 1;
    }
    else if ( _page_tbl_size == _page_cnt )
    {
        resizePageTbl();
    }

    _pages[_page_cnt] = page;
    ++_page_cnt;
}

template <class T, const uint P, const uint S>
void dbPagedVector<T,P,S>::push_back( const T & item )
{
    register unsigned int page = (_next_idx & ~(P-1)) >> S;

    if ( page == _page_cnt )
        newPage();

    register unsigned int offset = _next_idx & (P-1);
    ++_next_idx;

    T * objects = _pages[page];
    objects[offset] = item;
}

template <class T, const uint P, const uint S>
inline int dbPagedVector<T,P,S>::operator==( const dbPagedVector<T,P,S> & rhs ) const
{
    uint sz = size();

    if ( sz != rhs.size() )
        return false;

    uint i;
    for( i = 0; i < sz; ++i )
    {
        const T & l = (*this)[i];
        const T & r = rhs[i];

        if (l != r)
            return false;
    }

    return true;
}

template <class T, const uint P, const uint S>
inline void dbPagedVector<T,P,S>::differences( dbDiff & diff, const char * field, const dbPagedVector<T,P,S> & rhs ) const
{
    uint sz1 = size();
    uint sz2 = rhs.size();
    unsigned int i = 0;

    for( ; i < sz1 && i < sz2 ; ++i )
    {
        const T & o1 = (*this)[i];
        const T & o2 = rhs[i];
        
        if ( o1 != o2 )
        {
            diff.report("< %s[%d] = ", field, i );
            diff << o1;
            diff << "\n";
            diff.report("> %s[%d] = ", field, i );
            diff << o2;
            diff << "\n";
        }
    }

    if ( i < sz1 )
    {
        for( ; i < sz1; ++i )
        {
            const T & o1 = (*this)[i];
            diff.report("< %s[%d] = ", field, i );
            diff << o1;
            diff << "\n";
        }
    }
    
    
    if ( i < sz2 )
    {
        for( ; i < sz2; ++i )
        {
            const T & o2 = rhs[i];
            diff.report("> %s[%d] = ", field, i );
            diff << o2;
            diff << "\n";
        }
    }
}

template <class T, const uint P, const uint S>
inline void dbPagedVector<T,P,S>::out( dbDiff & diff, char side, const char * field ) const
{
    uint sz1 = size();
    unsigned int i = 0;

    for( ; i < sz1; ++i )
    {
        const T & o1 = (*this)[i];
        diff.report("%c %s[%d] = ", side, field, i );
        diff << o1;
        diff << "\n";
    }
}

template <class T, const uint P, const uint S>
inline dbOStream & operator<<( dbOStream & stream, const dbPagedVector<T,P,S> & v )
{
    uint sz = v.size();
    stream << sz;
    
    uint i;
    for( i = 0; i < sz; ++i )
    {
        const T & t = v[i];
        stream << t;
    }

    return stream;
}

template <class T, const uint P, const uint S>
inline dbIStream & operator>>( dbIStream & stream, dbPagedVector<T,P,S> & v )
{
    v.clear();
    
    uint sz;
    stream >> sz;
    T t;
    uint i;

    for( i = 0; i < sz; ++i )
    {
        stream >> t;
        v.push_back(t);
    }

    return stream;
}

END_NAMESPACE_ADS

#endif