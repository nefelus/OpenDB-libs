#ifndef ADS_DEFIN_FILL_H
#define ADS_DEFIN_FILL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#ifndef ADS_DEFIN_IFILL_H
#include "definIFill.h"
#endif

BEGIN_NAMESPACE_ADS

class dbTechLayer ;

class definFill : public definIFill, public definBase
{
    dbTechLayer * _cur_layer;
    
    // Fill interface methods
    virtual void fillBegin( const char * layer );
    virtual void fillRect( int x1, int y1, int x2, int y2 );
    virtual void fillPolygon( std::vector<defPoint> & points );
    virtual void fillEnd();

  public:
    definFill();
    virtual ~definFill();
    void init();
};
    
END_NAMESPACE_ADS

#endif
