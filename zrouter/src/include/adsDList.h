#ifndef ADS_DLIST_H
#define ADS_DLIST_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

///
/// This class implements an instrusive double-linked list.
///
/// See sample test program below.
///

template <class T>
class adsDListEntry
{
  public:
    T * _next;
    T * _prev;
};

template <class T, adsDListEntry<T> * ( T * )> class adsDList;

template <class T, adsDListEntry<T> * ENTRY( T * )>
class adsDListIterator
{
    T * _cur;

    void incr() { _cur = NEXT(_cur); }
    T * &  NEXT( T * n ) { return ENTRY(n)->_next; }
    
public:
    adsDListIterator() { _cur = NULL; }
    adsDListIterator( T * cur ) { _cur = cur; }
    adsDListIterator( const adsDListIterator & i ) { _cur = i._cur; }
    adsDListIterator & operator=( const adsDListIterator & i ) { _cur = i._cur; return *this; }
                
    int operator==( const adsDListIterator & i ) const { return _cur == i._cur; } 
    int operator!=( const adsDListIterator & i ) const { return _cur != i._cur; } 
    T * operator*() { return _cur; }
    adsDListIterator<T,ENTRY> & operator++() { incr(); return *this; }
    adsDListIterator<T,ENTRY> operator++(int) { adsDListIterator<T,ENTRY> i = *this; incr(); return i; }

    friend class adsDList<T,ENTRY>;
};
    
template <class T, adsDListEntry<T> * ENTRY(T * )>
class adsDList
{
/*
  public:
    typedef adsDListIterator<T,ENTRY> iterator;
*/
  private:
    T * _head;
    T * _tail;

    bool lessthan( const T & p1, const T & p2 ) { return *p1 < *p2; }

    T * & NEXT( T * n )
    {
        return ENTRY(n)->_next;
    }

    T * & PREV( T * n )
    {
        return ENTRY(n)->_prev;
    }
    
  public:
    typedef adsDListIterator<T,ENTRY> iterator;

    adsDList() { _head = NULL; _tail = NULL; }

    T * front() { return _head; }
    T * back() { return _tail; }

    void push_front( T * p )
    {
        if ( _head == NULL )
        {
            _head = p;
            _tail = p;
            NEXT(p) = NULL;
            PREV(p) = NULL;
        }
        else
        {
            PREV(_head) = p;
            NEXT(p) = _head;
            PREV(p) = NULL;
            _head = p;
        }
    }

    void push_back( T * p )
    {
        if ( _head == NULL )
        {
            _head = p;
            _tail = p;
            NEXT(p) = NULL;
            PREV(p) = NULL;
        }
        else
        {
            NEXT(_tail) = p;
            PREV(p) = _tail;
            NEXT(p) = NULL;
            _tail = p;
        }
    }

    void clear() { _head = _tail = NULL; }
    bool empty() const { return _head == NULL; }
    iterator begin() { return iterator(_head); }
    iterator end() { return iterator(NULL); }

    void swap( adsDList & l )
    {
        T * head = l._head;
        l._head = _head;
        _head = head;
        T * tail = l._tail;
        l._tail = _tail;
        _tail = tail;
    }

    iterator remove( T * p ) { return remove( iterator(p) ); }

    void move( typename adsDList<T,ENTRY>::iterator itr1,
               typename adsDList<T,ENTRY>::iterator itr2 )
    {
        if ( *itr1 == _head )
        {
            if ( _head == NULL )
            {
                _head = *itr2;
                _tail = *itr2;
                NEXT(*itr2) = NULL;
                PREV(*itr2) = NULL;
            }
            else
            {
                PREV(_head) = *itr2;
                NEXT(*itr2) = _head;
                PREV(*itr2) = NULL;
                _head = *itr2;
            }
        }
        else
        {
            NEXT(PREV(*itr1)) = *itr2;
            PREV(*itr2) = PREV(*itr1);
            NEXT(*itr2) = *itr1;
            PREV(*itr1) = *itr2;
        }
    }
    
    adsDListIterator<T,ENTRY> remove( iterator cur )
    {
        if ( *cur == _head )
        {
            if ( *cur == _tail )
            {
                _head = NULL;
                _tail = NULL;
            }
            
            else
            {
                _head = NEXT(*cur);
                PREV(_head) = NULL;
            }
        }
    
        else if ( *cur == _tail )
        {
            _tail = PREV(*cur);
            NEXT(_tail) = NULL;
        }
        
        else
        {
            NEXT(PREV(*cur)) = NEXT(*cur);
            PREV(NEXT(*cur)) = PREV(*cur);
        }
        
        return iterator(NEXT(*cur));
    }
    
    void reverse()
    {
        if ( _tail == _head )
            return;
        
        T * c = _head;
        T * n = NEXT(c);
    
        while( n )
        {
            T * tmp = NEXT(n);
            PREV(c) = n;
            NEXT(n) = c;
            c = n;
            n = tmp;
        }
    
        T * tmp = _head;
        _head = _tail;
        _tail = tmp;
    
        PREV(_head) = NULL;
        NEXT(_tail) = NULL;
    }
    
    int size()
    {
        T * c;
        int i = 0;
        for( c = _head; c != NULL; c = NEXT(c) ) ++i;
        return i;
    }
    
    void merge( adsDList<T,ENTRY> & l )
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = l.begin();
        iterator last2 = l.end();
    
        while (first1 != last1 && first2 != last2)
        {
            if ( lessthan( **first2, **first1 ) )
            {
                iterator next = first2;
                ++next;
                move( first1, first2 );
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }
        
        if (first2 != last2)
        {
            if ( _head == NULL )
            {
                _head = *first2;
                _tail = l._tail;
            }
            else
            {
                NEXT(_tail) = *first2;
                PREV(*first2) = _tail;
                _tail = l._tail;
            }
        }
    
        l._head = NULL;
        l._tail = NULL;
    }
    
    template <class CMP>
    void merge( adsDList<T,ENTRY> & l, CMP cmp )
    {
        iterator first1 = begin();
        iterator last1 = end();
        iterator first2 = l.begin();
        iterator last2 = l.end();
    
        while (first1 != last1 && first2 != last2)
        {
            if ( cmp( **first2, **first1 ) )
            {
                iterator next = first2;
                ++next;
                move( first1, first2 );
                first2 = next;
            }
            else
            {
                ++first1;
            }
        }
        
        if (first2 != last2)
        {
            if ( _head == NULL )
            {
                _head = *first2;
                _tail = l._tail;
            }
            else
            {
                NEXT(_tail) = *first2;
                PREV(*first2) = _tail;
                _tail = l._tail;
            }
        }
    
        l._head = NULL;
        l._tail = NULL;
    }
    
    void sort()
    {
        if ( (_head != NULL) && (NEXT(_head) != NULL) )
        {
            adsDList<T, ENTRY> carry;
            adsDList<T, ENTRY> counter[64];
            int fill = 0;
    
            while ( ! empty() )
            {
                T *head = NEXT(_head);
                carry.move(carry.begin(), begin());
                _head = head;
    
                int i = 0;
                while(i < fill && !counter[i].empty())
                {
                  counter[i].merge(carry);
                  carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);         
                if (i == fill) ++fill;
            } 
    
            for (int i = 1; i < fill; ++i)
                counter[i].merge(counter[i-1]);
    
            swap(counter[fill-1]);
        }
    }
    
    template <class CMP>
    void sort( CMP cmp )
    {
        if ( (_head != NULL) && (NEXT(_head) != NULL) )
        {
            adsDList<T, ENTRY> carry;
            adsDList<T, ENTRY> counter[64];
            int fill = 0;
    
            while ( ! empty() )
            {
                T *head = NEXT(_head);
                carry.move(carry.begin(), begin());
                _head = head;
    
                int i = 0;
                while(i < fill && !counter[i].empty())
                {
                  counter[i].merge(carry, cmp);
                  carry.swap(counter[i++]);
                }
                carry.swap(counter[i]);         
                if (i == fill) ++fill;
            } 
    
            for (int i = 1; i < fill; ++i)
                counter[i].merge(counter[i-1], cmp);
    
            swap(counter[fill-1]);
        }
    }
};

END_NAMESPACE_ADS

#if 0
#include <stdio.h>
#include <assert.h>
#include <vector>

USING_NAMESPACE_ADS;

struct elem
{
    int a;
    adsDListEntry<elem> entry;
    elem * next;

    elem(int n )
    {
        a = n;
        next = NULL;
    }

    int operator<( const elem & e ) { return a < e.a; }
    
    static adsDListEntry<elem> * getEntry( elem * e ) 
    {
        return &e->entry;
    }
};

struct cmp
{
    bool operator()( const elem & e1, const elem & e2 )
    {
        return e1.a < e2.a;
    }
};
    

#define M 100000

main()
{
    int i;

    std::vector<elem *> elems;


    for( i = 0; i < M; ++i )
    {
        elem * e = new elem(i);
        elems.push_back(e);
    }
    
    for( i = 0; i < M; ++i )
    {
        int r1 = random() % M;
        int r2 = random() % M;
        elem * e = elems[r1];
        elems[r1] = elems[r2];
        elems[r2] = e;
    }
    
    ads::adsDList<elem, elem::getEntry> l1;

    for( i = 0; i < M; ++i )
    {
        elem * e = elems[i];
        l1.push_back(e);
    }
    
    l1.sort(cmp());
    
    ads::adsDList<elem, elem::getEntry>::iterator itr;

    for( i = 0, itr = l1.begin(); itr != l1.end(); ++itr, i++ )
    {
        elem * e = *itr;
        printf("l1: %d\n", e->a );
        assert( e->a == i );
    }

    l1.reverse();

    for( i = 1, itr = l1.begin(); itr != l1.end(); ++itr, i++ )
    {
        elem * e = *itr;
        printf("l1: %d\n", e->a );
        assert( e->a == (M-i) );
    }
    
}
#endif

#endif
