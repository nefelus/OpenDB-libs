#ifndef ADS_DEFIN_BASE_H
#define ADS_DEFIN_BASE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbTech;

class definBase
{
  public:
    dbTech *  _tech;
    dbBlock * _block;
    int       _errors;
    int       _dist_factor;

    definBase();
    void setTech( dbTech * tech );
    void setBlock( dbBlock * block );
    void units( int d );
    virtual void init();
    
    int dbdist( int value ) 
    {
        return (int) (value * _dist_factor);
    }

    int dbdist( double value ) 
    {
        if ( value < 0.0 )
            return (int) (value * _dist_factor - 0.5);
        else
            return (int) (value * _dist_factor + 0.5);
    }

    int round( double value ) 
    {
        if ( value < 0.0 )
            return (int) (value - 0.5);
        else
            return (int) (value + 0.5);
    }
    
    void translate( const std::vector<defPoint> & defpoints, std::vector<adsPoint> & points )
    {    
        points.clear();
        std::vector<defPoint>::const_iterator itr;

        for( itr = defpoints.begin(); itr != defpoints.end(); ++itr )
        {
            const defPoint & p = *itr;
            adsPoint point( dbdist( p._x ), dbdist(p._y) );
            points.push_back(point);
        }
    }
};
    
END_NAMESPACE_ADS

#endif
