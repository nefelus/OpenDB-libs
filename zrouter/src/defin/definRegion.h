#ifndef ADS_DEFIN_REGION_H
#define ADS_DEFIN_REGION_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IREGION_H
#include "definIRegion.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbRegion;

class definRegion : public definIRegion, public definBase
{
    dbRegion * _cur_region;
    
  public:
    definRegion();
    virtual ~definRegion();

    void init();

    virtual void begin( const char * name, bool is_group );
    virtual void boundary( int x1, int y1, int x2, int y2 );
    virtual void type( defRegionType type );
    virtual void inst( const char * inst );
    virtual void parent( const char * region );
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();
};
    
END_NAMESPACE_ADS

#endif
