#ifndef ADS_POLY_DECOMP_H
#define ADS_POLY_DECOMP_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

BEGIN_NAMESPACE_ADS

//
// Decompose a simple rectilinear polygon into rectangles.
// The polygon points are assumed to be in clockwise order.
//
void decompose_polygon( const std::vector<adsPoint> & points, std::vector<adsRect> & rects );

// Returns true if the verticies of this polygon are clockwise orderd.
bool polygon_is_clockwise( const std::vector<adsPoint> & points );

END_NAMESPACE_ADS

#endif
