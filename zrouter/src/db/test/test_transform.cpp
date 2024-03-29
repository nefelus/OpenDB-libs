#include <stdio.h>
#include "../../include/geom.h"
#include "../../include/dbTransform.h"

USING_NAMESPACE_ADS;

int main( int argc, char **argv )
{
    adsPoint p(30, 30);
    adsPoint origin(0, 0);

    adsRect r(10,10,15,20);
    dbOrientType orient_R0(dbOrientType::R0);
    dbTransform r0( orient_R0, origin, 20, 40 );
    r0.transformInstanceBBox( p, r );
    printf("R0 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_R90(dbOrientType::R90);
    dbTransform r90( orient_R90, origin, 20, 40 );
    r90.transformInstanceBBox( p, r );
    printf("R90 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_R180(dbOrientType::R180);
    dbTransform r180( orient_R180, origin, 20, 40 );
    r180.transformInstanceBBox( p, r );
    printf("R180 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_R270(dbOrientType::R270);
    dbTransform r270( orient_R270, origin, 20, 40 );
    r270.transformInstanceBBox( p, r );
    printf("R270 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_MY(dbOrientType::MY);
    dbTransform my( orient_MY, origin, 20, 40 );
    my.transformInstanceBBox( p, r );
    printf("MY (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_MYR90(dbOrientType::MYR90);
    dbTransform myr90( orient_MYR90, origin, 20, 40 );
    myr90.transformInstanceBBox( p, r );
    printf("MYR90 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_MX(dbOrientType::MX);
    dbTransform mx( orient_MX, origin, 20, 40 );
    mx.transformInstanceBBox( p, r );
    printf("MX (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );

    r = adsRect(10,10,15,20);
    dbOrientType orient_MXR90(dbOrientType::MXR90);
    dbTransform mxr90( orient_MXR90, origin, 20, 40 );
    mxr90.transformInstanceBBox( p, r );
    printf("MXR90 (%d %d) (%d %d)\n", r.xMin(), r.yMin(), r.xMax(), r.yMax() );
}
