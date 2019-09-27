#ifndef ADS_DEFIN_POLYGON_H
#define ADS_DEFIN_POLYGON_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#include <vector>

BEGIN_NAMESPACE_ADS

class definPolygon
{
    std::vector<adsPoint> _points;

  public:

    definPolygon() { }
    definPolygon( const std::vector<adsPoint> & points );
    void decompose( std::vector<adsRect> & rect );
};

END_NAMESPACE_ADS

#endif
