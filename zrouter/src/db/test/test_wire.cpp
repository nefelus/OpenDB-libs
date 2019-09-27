#include <stdio.h>
#include "db.h"
#include "dbWireCodec.h"
#include "dbShape.h"

USING_NAMESPACE_ADS;

extern void print_db( dbDatabase * db );

dbTechLayer * m1;
dbTechLayer * m2;
dbTechLayer * m3;
dbTechVia * v12;
dbTechVia * v23;
dbTech * tech;
dbDatabase * db;

void create_tech()
{
    tech = dbTech::create(db, "tech" );

    m1 = dbTechLayer::create(tech, "M1", ( dbTechLayerType::ROUTING ) );
    m1->setWidth(2000);
    m2 = dbTechLayer::create(tech, "M2", ( dbTechLayerType::ROUTING ) );
    m2->setWidth(2000);
    m3 = dbTechLayer::create(tech, "M3", ( dbTechLayerType::ROUTING ) );
    m3->setWidth(2000);

    v12 = dbTechVia::create(tech, "VIA12" );
    dbBox::create(v12, m1, 0, 0, 2000, 2000 );
    dbBox::create(v12, m2, 0, 0, 2000, 2000 );
    
    v23 = dbTechVia::create(tech, "VIA23" );
    dbBox::create(v23, m2, 0, 0, 2000, 2000 );
    dbBox::create(v23, m3, 0, 0, 2000, 2000 );
}

void print_shape( dbShape & shape )
{
    if ( shape.isVia() )
    {
        dbTechVia * tech_via = shape.getTechVia();
        dbString vname = tech_via ->getName();
        printf("VIA %s ( %d %d ) ( %d %d )\n",
               vname.c_str(),
               shape.xMin(),
               shape.yMin(),
               shape.xMax(),
               shape.yMax() );
    }
    else
    {
        dbTechLayer * layer = shape.getTechLayer();
        dbString lname = layer->getName();
        printf("RECT %s ( %d %d ) ( %d %d )\n",
               lname.c_str(),
               shape.xMin(),
               shape.yMin(),
               shape.xMax(),
               shape.yMax() );
    }
}

int main()
{
    db = dbDatabase::open( "db", dbCreate );
    create_tech();
    dbChip * chip = dbChip::create( db, tech, "chip" );
    dbBlock * block = dbBlock::create( chip, "chip" );
    dbNet * net = dbNet::create( block, "net" );
    dbWire * wire = dbWire::create( net );
    dbWireEncoder encoder;
    encoder.begin(wire);
    encoder.newPath( m1, dbWireType::ROUTED );
    encoder.addPoint( 2000, 2000 );
    int j1 = encoder.addPoint( 10000, 2000 );
    encoder.addPoint( 18000, 2000 );
    encoder.newPath(j1);
    encoder.addTechVia(v12);
    int j2 = encoder.addPoint( 10000, 10000 );
    encoder.addPoint( 10000, 18000 );
    encoder.newPath(j2);
    int j3 = encoder.addTechVia(v12);
    encoder.addPoint( 23000, 10000, 4000 );
    encoder.newPath(j3);
    encoder.addPoint( 3000, 10000 );
    encoder.addTechVia(v12);
    encoder.addTechVia(v23);
    encoder.addPoint( 3000, 10000, 4000 );
    encoder.addPoint( 3000, 18000, 6000 );
    encoder.end();

    dbShape shape;
    dbWireShapeItr sitr;
    std::vector<int> shape_id;

    for( sitr.begin(wire); sitr.next(shape); )
    {
        print_shape(shape);
        shape_id.push_back(sitr.getShapeId());
    }
    
    printf("------------------------------\n");
    
    std::vector<int>::iterator itr;

    for( itr = shape_id.begin(); itr != shape_id.end(); ++itr )
    {
        int id = *itr;
        dbShape shape;
        sitr.getShape( id, shape );
        print_shape(shape);
    }

    return 0;
}

