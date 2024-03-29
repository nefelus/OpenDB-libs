#ifndef ADS_RTREE_H
#define ADS_RTREE_H

#include <string.h>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_ALLOCATOR_H
#include "adsAllocator.h"
#endif

#ifndef ADS_RTREE_CORE_H
#include "../zlib/adsRTreeCore.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class adsRTree;

template <class T>
class adsRTreeIterator
{
    enum { SP_MAX = 64 };
    adsRTreeBranch *  _root;
    adsRect           _search_bbox;
    adsRTreeNode   *  _stack[SP_MAX];
    int               _sp;
    
    bool findLeaf();
    
  public:
    
    adsRTreeIterator( adsRTree<T> & tree );

    // iterate all items in the tree
    void begin();

    // iterate items within this rectangle.
    void begin( const adsRect & rect );

    // next item found, returns false when no more items are found.
    bool next( T & value );
};

///
/// adsRTree - Implmentation of R*Tree spatial index.
///
/// See: "The R*-tree: An Effiencent and Robust Access Method for Points and Rectangles",
/// Norbert Beckman,Hans-Peter Krigel, Ralf Schnieder, Bernard Seeger.
///
template <class T>
class adsRTree : public adsRTreeCore
{
    adsAllocator< adsRTreeLeaf<T> > _leaf_alloc;
    
    adsRTreeLeaf<T> * findLeaf( const adsRect & rect, const T & value, adsRTreeNode * stack[] );
    adsRTreeLeaf<T> * findLeaf( const adsRect & rect, const T & value, adsRTreeBranch * parent );

    // functions to help debug memory problems using purify
    void destroyTree();
    void destroyTree(adsRTreeNode *);
    
  public:
    typedef adsRTreeIterator<T> iterator;
    
    // Construct a rtree with a node fanout of max_children_per_node.
    adsRTree( unsigned short max_children_per_node = 16 );

    // destructor
    ~adsRTree();

    // Insert a rect/value pair into the table.
    void insert( const adsRect & rect, const T & value );

    // Remove a rect/value pair into the table. If the rect/value
    // pair does not exist, then nothing is done.
    void remove( const adsRect & rect, const T & value );

    // Remove all items from the tree. 
    void clear();

    // Returns true if the tree contains the rect/value pair.
    bool isMember( const adsRect & rect, const T & value );

    bool hasValue( const T & value );

    uint memSizeTotal()  const { return _leaf_alloc.vm_size() + _branch_alloc.size(); }
    uint memSizeLeafs()  const { return _leaf_alloc.vm_size(); }
    uint memSizeBranches()  const { return _branch_alloc.vm_size(); }
    int height() const { return _root->_level; }
    void printStats() { adsRTreeCore::printStats(); }

    friend class adsRTreeIterator<T>;
};

template <class T>
inline adsRTree<T>::adsRTree( unsigned short max_children_per_node ) 
        : adsRTreeCore( max_children_per_node ), _leaf_alloc(512)
{
}

template <class T>
inline adsRTree<T>::~adsRTree()
{
#ifdef ADS_PURIFY
    // We need to delete each node if we are using purify, because
    // this is usally left up to the block allocator...
    destroyTree();
#endif
}

template <class T>
inline void adsRTree<T>::destroyTree()
{
    if ( _root )
    {
        destroyTree(_root);
        _root = NULL;
    }
}

template <class T>
inline void adsRTree<T>::destroyTree( adsRTreeNode * node )
{
    if ( node->_level == 0 )
    {
        _leaf_alloc.free( (adsRTreeLeaf<T> *) node );
    }
    else
    {

        adsRTreeBranch * b = (adsRTreeBranch *) node;
        adsRTreeNode * c;

        for( c = b->_children; c; )
        {   
            adsRTreeNode * n = c->_next;
            destroyTree(c);
            c = n;
       }

        _branch_alloc.free(b);
    }
}

template <class T>
inline void adsRTree<T>::clear()
{
#ifdef ADS_PURIFY
    destroyTree();
    _root = _branch_alloc.create();
    _root->_bbox.mergeInit();
#else    
    adsRTreeCore::clear();
    _leaf_alloc.clear();
#endif
}

template <class T>
inline void adsRTree<T>::insert( const adsRect & rect, const T & value )
{
    adsRTreeLeaf<T> * leaf = _leaf_alloc.create();
    leaf->_value = value;
    leaf->_bbox = rect;

    unsigned char overflow[SP_MAX+2];
    memset( overflow, 0, _root->_level+2 );
    adsRTreeCore::insert( leaf, 1, overflow );
}

template <class T>
inline void adsRTree<T>::remove( const adsRect & rect, const T & value )
{
    adsRTreeNode * path[SP_MAX+1];
    adsRTreeLeaf<T> * leaf = findLeaf( rect, value, path );

    if ( leaf == NULL )
        return;
    
    // unlink from parent
    adsRTreeBranch * parent = (adsRTreeBranch *) path[leaf->_level+1];
    adsRTreeNode * c = parent->_children;;
    adsRTreeNode * p = NULL;

    for( ; c ; p = c, c = c->_next )
    {
        if ( c == leaf )
        {
            if ( p == NULL )
                parent->_children = c->_next;
            else
                p->_next = c->_next;
    
            break;
        }
    }

    parent->_count--;

    if ( parent->_bbox.inside( leaf->_bbox ) == false )
        recompute_bboxes( parent->_level, path );

    if ( parent->_count < _MIN )
        adsRTreeCore::condense_tree( parent->_level, path );

    _leaf_alloc.destroy(leaf);
}

template <class T>
inline bool adsRTree<T>::isMember( const adsRect & rect, const T & value )
{
    adsRTreeNode * path[SP_MAX+1];
    adsRTreeLeaf<T> * leaf = findLeaf( rect, value, path );

    if ( leaf )
        return true;

    return false;
}

template <class T>
inline bool adsRTree<T>::hasValue( const T & value )
{
    adsRTreeIterator<T> itr(*this);
    T v;

    for( itr.begin(); itr.next(v); )
    {
        if ( v == value )
            return true;
    }

    return false;
}

template <class T>
inline adsRTreeLeaf<T> * adsRTree<T>::findLeaf( const adsRect & rect, const T & value, adsRTreeBranch * parent )
{
    if ( parent->_level == 1 )
    {
        adsRTreeLeaf<T> * leaf = (adsRTreeLeaf<T> *) parent->_children;
    
        for( ; leaf != NULL ; leaf = (adsRTreeLeaf<T> *) leaf->_next )
            if ( (leaf->_bbox == rect) && (leaf->_value == value) )
                return leaf;

        return NULL;
    }

    adsRTreeNode * node = parent->_children;
    
    for( ; node != NULL ; node = node->_next )
    {
        if ( node->_bbox.contains(rect) )
        {
            adsRTreeLeaf<T> * leaf = findLeaf( rect, value, (adsRTreeBranch *) node );

            if ( leaf )
                return leaf;
        }
    }

    return NULL;
}

#if 0
template <class T>
inline adsRTreeLeaf<T> * adsRTree<T>::findLeaf( const adsRect & rect, const T & value, adsRTreeNode * stack[] )
{
    register adsRTreeNode ** sp = &stack[_root->_level];
    register adsRTreeNode ** max = sp;
    register adsRTreeNode * node = _root;
    
  find_leaf_next_child:
    while( node != NULL )
    {
        if ( node->_bbox.contains(rect) )
        {
            if ( node->_level == 1 )
            {
                register adsRTreeLeaf<T> * leaf = (adsRTreeLeaf<T> *) ((adsRTreeBranch *) node)->_children;
    
                for( ; leaf != NULL ; leaf = (adsRTreeLeaf<T> *) leaf->_next )
                {
                    if ( (leaf->_value == value) && (leaf->_bbox == rect) )
                    {
                        *sp = node;
                        return leaf;
                    }
                }
            }
            else
            {
                *sp = node;
                --sp;
                node = ((adsRTreeBranch *) node)->_children;
                goto find_leaf_next_child;
            }
        }

        node = node->_next;
    }

    ++sp;

    if ( sp < max )
    {
        node = (*sp)->_next;
        goto find_leaf_next_child;
    }
    
    return NULL;
}
#endif

#if 1
template <class T>
inline adsRTreeLeaf<T> * adsRTree<T>::findLeaf( const adsRect & rect, const T & value, adsRTreeNode * stack[] )
{
    register int sp = _root->_level;
    register int max = _root->_level;
    register adsRTreeNode * node = _root;
    stack[sp] = _root;

    for( ;; )
    {
      descend_node:

        for( ; node != NULL ; node = node->_next )
        {
            if ( node->_bbox.contains(rect) )
            {
                stack[sp] = node;

                if ( node->_level == 1 )
                {
                    adsRTreeLeaf<T> * leaf = (adsRTreeLeaf<T> *) ((adsRTreeBranch *) node)->_children;
    
                    for( ; leaf != NULL ; leaf = (adsRTreeLeaf<T> *) leaf->_next )
                        if ( (leaf->_value == value) && (leaf->_bbox == rect) )
                            return leaf;
                }
                else
                {
                    node = ((adsRTreeBranch *) node)->_children;
                    stack[--sp] = node;
                    goto descend_node;
                }
            }
        }

        ++sp;

        if ( sp > max ) // back to root...
            break;
        
        node = stack[sp]->_next;
    }
    
    return NULL;
}
#endif

template <class T>
inline adsRTreeIterator<T>::adsRTreeIterator( adsRTree<T> & tree )
{
    _root = tree._root;
    _sp = SP_MAX;
}

template <class T>
inline void adsRTreeIterator<T>::begin( const adsRect & rect )
{
    _search_bbox = rect;
    _sp = SP_MAX - 1;
    _stack[_sp] = _root;
    findLeaf();
}

template <class T>
inline void adsRTreeIterator<T>::begin()
{
    _search_bbox = _root->_bbox;
    _sp = SP_MAX - 1;
    _stack[_sp] = _root;
    findLeaf();
}

template <class T>
inline bool adsRTreeIterator<T>::findLeaf()
{
  descend_node:
    adsRTreeNode * child = _stack[_sp];

    for( ; child != NULL ; child = child->_next )
    {
        if ( _search_bbox.intersects(child->_bbox) )
        {
            _stack[_sp] = child->_next;
            _stack[--_sp] = ((adsRTreeBranch *) child)->_children;

            if ( child->_level == 1 )
                return true;

            goto descend_node;
        }
    }

    _stack[_sp] = NULL;
    return false;
}

template <class T>
inline bool adsRTreeIterator<T>::next( T & value )
{
  next_child:
    adsRTreeLeaf<T> * leaf = (adsRTreeLeaf<T> *) _stack[_sp];
    
    for( ; leaf != NULL ; leaf = (adsRTreeLeaf<T> *) leaf->_next )
    {
        if ( _search_bbox.intersects( leaf->_bbox ) )
        {
            _stack[_sp] = leaf->_next;
            value = leaf->_value;
            return true;
        }
    }

    while( ++_sp < SP_MAX )
    {
        if ( findLeaf() )
            goto next_child;
    }

    return false;
}


END_NAMESPACE_ADS

#endif
