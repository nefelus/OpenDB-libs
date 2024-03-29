#ifndef ADS_ALLOCATOR_H
#define ADS_ALLOCATOR_H

#include <new>

#ifndef ADS_H
#include "ads.h"
#endif

#ifdef ADS_PURIFY
#define ADS_PURIFY_ALLOCATOR
#endif

BEGIN_NAMESPACE_ADS

///
/// Simple memory pool.
///
/// This is a replacement for malloc and free when
/// you need to maintain a large and dynamic pool of objects.
///
/// NOTE: The memory used by this pool is only released when the pool is
///       destroyed.
///
/// There are two usage alternatives when using C++ classes.
/// 
/// 1) You can apply a specific constructor by doing the folowing:
///
///      class Foo {};
///
///      adsAllocator<Foo> alloc;
///      Foo * p = new(alloc.malloc()) Foo;
///      ...
///      p->~Foo();
///      alloc.free(p);
///
/// 2) Or you can use the create() and destroy() methods.
///    The create() method will call the default constructor.
///    The destroy() method will call the destructor. The example above can
///    be recoded as:
/// 
///      adsAllocator<Foo> alloc;
///      Foo * p = alloc.create();
///      alloc.destroy(p);
/// 
/// DO NOT CALL DELETE ON a POINTER RETURNED FROM THE ALLOCATOR
///
///      delete p;    // ILLEGAL
///
template <class T>
class adsAllocator
{
    struct chunk
    {
        chunk * _next;
    };
        
    struct block
    {
        void *  _chunk;
        block * _next;
    };

    block * _block_list;
    int     _block_size;
    uint    _size;
    uint    _vm_size;
    chunk * _free_list;
    void new_block();

  public:

    // construct an allocator, block-size = number of object reserved per block allocation
    adsAllocator(int block_size = 128);

    // destroy the allocator, all memory is free'ed...
    ~adsAllocator();

    // clear the pool, release all memory and resets the pool to it's initial state.
    void clear();

    // malloc a new object
    T * malloc();

    // free an object
    void free( T * );

    // Create a new object and call the default constructor
    T * create();

    // Destroy an object and call the destructor
    void destroy( T * );

    uint size() const { return _size; }

    uint vm_size() const;
};

template <class T>
inline adsAllocator<T>::adsAllocator(int block_size)
{
    _block_size = block_size;
    _free_list = NULL;
    _block_list = NULL;
    _size = 0;
    _vm_size = 0;
}

template <class T>
inline adsAllocator<T>::~adsAllocator()
{
    clear();
}

template <class T>
inline uint adsAllocator<T>::vm_size() const
{
    return _vm_size;
}

template <class T>
inline void adsAllocator<T>::clear()
{
    block * b = _block_list;
    block * next = NULL;

    for( ; b ; b = next )
    {
        next = b->_next;
        ::free( (void *) b->_chunk );
        ::free( (void *) b );
    }

    _free_list = NULL;
    _block_list = NULL;
}

template <class T>
inline T * adsAllocator<T>::malloc()
{
#ifdef ADS_PURIFY_ALLOCATOR
    _size += sizeof(T);
    return (T *) ::malloc(sizeof(T));
#else
    if ( _free_list == NULL )
        new_block();

    chunk * c = _free_list;
    _free_list = c->_next;
    return (T*) c;
#endif
}

template <class T>
inline T * adsAllocator<T>::create()
{
    T * t = adsAllocator<T>::malloc();
    new(t) T;
    return t;
}

template <class T>
inline void adsAllocator<T>::free( T * t )
{
#ifdef ADS_PURIFY_ALLOCATOR
    _size -= sizeof(T);
    ::free((void *) t);
#else
    
    chunk * c = (chunk *) t;
    c->_next = _free_list;
    _free_list = c;
#endif
}

template <class T>
inline void adsAllocator<T>::destroy( T * t )
{
    t->~T();
    adsAllocator<T>::free(t);
}

template <class T>
inline void adsAllocator<T>::new_block()
{
    int obj_size = sizeof(T) < sizeof(chunk) ? sizeof(chunk) : sizeof(T);
    _size += sizeof(block);
    _size += _block_size*obj_size;

    block * b = (block *) ::malloc(sizeof(block));
    assert(b);
    b->_chunk =  ::malloc( obj_size * _block_size );
    assert(b->_chunk);
    _vm_size += sizeof(block);
    _vm_size += obj_size * _block_size;

    char * base = (char *) b->_chunk;
    char * end = base + obj_size * _block_size;
    char * c;
    
    for( c = end - obj_size; c >= base; c -= obj_size )
    {
        chunk * ck = (chunk *) c;
        ck->_next = _free_list;
        _free_list = ck;
    }

    b->_next = _block_list;
    _block_list = b;
}


END_NAMESPACE_ADS

#endif
