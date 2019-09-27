#ifndef ADS_DEFIN_IFILL_H
#define ADS_DEFIN_IFILL_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

#include <vector>

class definIFill
{
  public:
    // Fill interface methods
    virtual void fillBegin( const char * layer ) {}
    virtual void fillRect( int x1, int y1, int x2, int y2 ) {}
    virtual void fillPolygon( std::vector<defPoint> & points ) {}
    virtual void fillEnd() {}
};
    
#endif
