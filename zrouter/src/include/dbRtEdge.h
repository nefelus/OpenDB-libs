#ifndef ADS_DB_ROUTE_EDGE_H
#define ADS_DB_ROUTE_EDGE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DLIST_H
#include "adsDList.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

BEGIN_NAMESPACE_ADS

class dbVia;
class dbTechVia;
class dbTechLayerRule;
class dbRtTree;
class dbRtNode;

// EndStyle - Defines the end style of a segment
class dbRtEndStyle
{
  public:
    // End-Types:
    //     EXTENDED - segment extended 1/2 path-width
    //     VARIABLE - segment extended variable amount
    enum Type{ EXTENDED, VARIABLE };

  private:
    Type _type;
    int  _ext;

  public:
    dbRtEndStyle() : _type(EXTENDED), _ext(0) {}

    void setExtended() 
    {
        _type = EXTENDED;
        _ext = 0;
    }
   
    void setVariable(int ext)
    {
        _type = VARIABLE;
        _ext = ext;
    }

    Type getType() const { return _type; }
    int getExt() const { return _ext; }

    int operator==( const dbRtEndStyle & s ) const
    {
        return (_type == s._type) && (_ext == s._ext);
    }

    int operator!=( const dbRtEndStyle & s ) const
    {
        return (_type != s._type) || (_ext != s._ext);
    }
    
    friend class dbRtTree;
};

// dbRtEdge - represents an edge in the routing tree.
class dbRtEdge
{
    friend class dbRtTree;
    friend class dbRtNode;
    friend class dbRtNodeEdgeIterator;
    
   public:
    // If new edge types are added, the corresponding allocator/deallocator code must be updated in dbRtTree.cpp
    enum Type { SEGMENT=0, TECH_VIA=1, VIA=2, SHORT=3, VWIRE=4 };

   protected:
    Type                       _type;
    dbRtNode *                 _src;
    dbRtNode *                 _tgt;
    dbWireType::Value          _wire_type;
    dbTechLayerRule *          _non_default_rule;
    double                     _r;
    double                     _c;
    dbRtTree *                 _rt_tree;
    dbRtEdge *                 _next[2];
    dbRtEdge *                 _prev[2];
    adsDListEntry<dbRtEdge>    _rt_edge;
    int                        _shape_id;
    uint                       _property;
    bool                       _visited;

    dbRtEdge( Type type, dbWireType::Value wire_type, dbTechLayerRule * rule )
           : _type(type),
             _src(NULL), 
             _tgt(NULL),
              _wire_type(wire_type), 
             _non_default_rule(rule),
             _r(0.0),
             _c(0.0),
             _rt_tree(NULL),
             _shape_id(-1),
             _property(0),
             _visited(false)
    {
        _next[0] = NULL;
        _next[1] = NULL;
        _prev[0] = NULL;
        _prev[1] = NULL;
    }

    dbRtNode * opposite( dbRtNode * n ) const { return n == _src ? _tgt : _src; }
    dbRtEdge * & prev( dbRtNode * node ) { return node == _src ? _prev[0] : _prev[1]; }
    dbRtEdge * & next( dbRtNode * node ) { return node == _src ? _next[0] : _next[1]; }
     
   public:
    virtual ~dbRtEdge() {}
    virtual void getBBox( adsRect & bbox ) = 0;
    
    // Get the type of this node
    Type getType() const { return _type; }

    // Get the source edge of this node (A source node has NO directional significance)
    dbRtNode * getSource() const { return _src; }

    // Get the target edge of this node (A target node has NO directional significance)
    dbRtNode * getTarget() const { return _tgt; }

    // Get the opposite node of this node, assumes n == src or n == tgt
    dbRtNode * getOpposite( dbRtNode * n ) const { return opposite(n); }

    // Set the wire-type of this edge.
    void setWireType( dbWireType::Value value ) { _wire_type = value; }

    // Get the wire-type of this edge.
    dbWireType::Value getWireType() const { return _wire_type; }

    // Get the NonDefaultRule of this edge (returns NULL is there is no non-default-rule);
    void setNonDefaultRule( dbTechLayerRule * rule ) { _non_default_rule = rule; }

    // Get the NonDefaultRule of this edge (returns NULL is there is no non-default-rule);
    dbTechLayerRule * getNonDefaultRule() const { return _non_default_rule; }

    // Set the resistance of this edge
    void setResistance( double r ) { _r = r; }

    // Get the resistance of this edge
    double getResistance() const { return _r; }

    // Set the capacitance of this edge
    void setCapacitance( double c ) { _c = c; }

    // Get the capacitance of this edge
    double getCapacitance() const { return _c; }

    // Get the value of the visited flag.
    bool isVisited() const { return _visited; }

    // Set the value of the visited flag.
    void setVisited(bool value) { _visited = value; }

    // Assign a property to this edge
    void setProperty( uint property ) { _property = property; }

    // Assign a property to this edge
    uint getProperty() { return _property; }
    
    static adsDListEntry<dbRtEdge> * rtEdge( dbRtEdge * edge ) { return &edge->_rt_edge; }
};

// VIA edge
class dbRtVia : public dbRtEdge
{
    dbVia * _via;

    dbRtVia( dbVia * via, dbWireType::Value wire_type, dbTechLayerRule * rule )
        : dbRtEdge(VIA, wire_type, rule), _via(via)
    {
    }

  public:

    virtual void getBBox( adsRect & bbox );
    
    // Get the via of this edge
    dbVia * getVia() const { return _via; }
    void setVia( dbVia * via ) { _via = via; }

    friend class dbRtTree;
};

// TECH-VIA edge
class dbRtTechVia : public dbRtEdge
{
    dbTechVia * _via;

  public:
    dbRtTechVia( dbTechVia * via, dbWireType::Value wire_type, dbTechLayerRule * rule )
        : dbRtEdge(TECH_VIA, wire_type, rule), _via(via)
    {
    }

  public:

    virtual void getBBox( adsRect & bbox );

    // Get the via of this edge
    dbTechVia * getVia() const { return _via; }

    friend class dbRtTree;
};

// Wire segment
class dbRtSegment : public dbRtEdge
{
    dbRtEndStyle _src_style;
    dbRtEndStyle _tgt_style;

    dbRtSegment( dbRtEndStyle src, dbRtEndStyle tgt, dbWireType::Value wire_type, dbTechLayerRule * rule )
        : dbRtEdge(SEGMENT, wire_type, rule), _src_style(src), _tgt_style(tgt)
    {
    }

  public:

    virtual void getBBox( adsRect & bbox );

    // Set the end-syle of the source node of this edge
    void setSourceEndStyle( dbRtEndStyle style ) { _src_style = style; }

    // Get the end-syle of the source node of this edge
    dbRtEndStyle getSourceEndStyle() const { return _src_style; }

    // Set the end-syle of the target node of this edge
    void setTargetEndStyle( dbRtEndStyle style ) { _tgt_style = style; }

    // Get the end-syle of the target node of this edge
    dbRtEndStyle getTargetEndStyle() const { return _tgt_style; }

    int getWidth() const;

    friend class dbRtTree;
};

// Virtual short to node
class dbRtShort : public dbRtEdge
{

    dbRtShort( dbWireType::Value wire_type, dbTechLayerRule * rule )
        : dbRtEdge(SHORT, wire_type, rule)
    {
    }

  public:

    virtual void getBBox( adsRect & bbox );

    friend class dbRtTree;
};

// Virtual wire between nodes
class dbRtVWire : public dbRtEdge
{
  public:

    dbRtVWire( dbWireType::Value wire_type, dbTechLayerRule * rule )
        : dbRtEdge(VWIRE, wire_type, rule)
    {
    }

  public:

    virtual void getBBox( adsRect & bbox );

    friend class dbRtTree;
};

END_NAMESPACE_ADS

#endif
