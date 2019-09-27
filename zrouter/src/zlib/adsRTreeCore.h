#ifndef ADS_RTREE_CORE_H
#define ADS_RTREE_CORE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_ALLOCATOR_H
#include "adsAllocator.h"
#endif

BEGIN_NAMESPACE_ADS

class adsRTreeNode
{
  public:
    unsigned char  _level; // leaf = 0, leaf-branch = 1, 1 > internal-branch 
    unsigned short _count; // branch child-count
    adsRect        _bbox;
    adsRTreeNode * _next;

    adsRTreeNode()
    {
        _count = 0;
        _next = NULL;
    }
};

class adsRTreeBranch : public adsRTreeNode
{
  public:
    adsRTreeNode * _children;

    adsRTreeBranch()
    {
        _children = NULL;
        _level = 1;
    }
};

template <class T>
class adsRTreeLeaf : public adsRTreeNode
{
  public:
    T _value;
    adsRTreeLeaf() { _level = 0; }
};

class adsRTreeStats;

class adsRTreeCore
{
  protected:
    enum { SP_MAX = 64 };
    int  _MAX; // maximum number of children per node.
    int  _MIN; // minimum number of children per node.
    int  _REINSERT; // number of children per node to reinsert.
    
    adsAllocator< adsRTreeBranch > _branch_alloc;
    adsRTreeBranch * _root;

    adsRTreeCore( unsigned short max_children_per_node );
    ~adsRTreeCore();
    void clear();
    adsRTreeNode * chooseChild( const adsRect & ir, adsRTreeBranch * node, bool & is_contained );
    adsRTreeNode * chooseSubTree( const adsRect & rect, int level, adsRTreeNode * path[] );
    void update_bboxes( int level, adsRTreeNode * path[] );
    void recompute_bboxes( int level, adsRTreeNode * path[] );
    void adjustTree( adsRTreeBranch * new_child, adsRTreeNode * path[] );
    void reinsert( adsRTreeBranch * node, adsRTreeNode * path[],  unsigned char * overflow );
    uint64 chooseSplitYAxis( adsRTreeBranch * parent, adsRTreeNode * children[] );
    uint64 chooseSplitXAxis( adsRTreeBranch * parent, adsRTreeNode * children[] );
    int chooseSplitDistribution( adsRTreeBranch * parent, adsRTreeNode * children[] );
    adsRTreeBranch * split( adsRTreeBranch * node );
    void insert( adsRTreeNode * child, int level, unsigned char * overflow );
    void condense_tree( int level, adsRTreeNode * path[] );

    void checkBBoxes( adsRTreeNode * node );
    void checkBBoxes();
    void printStats();
    void printStats( adsRTreeNode * node, adsRTreeStats * stats );
};
    
END_NAMESPACE_ADS

#endif
