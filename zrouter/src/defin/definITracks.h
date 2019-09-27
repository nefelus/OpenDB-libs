#ifndef ADS_DEFIN_ITRACKS_H
#define ADS_DEFIN_ITRACKS_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definITracks
{
  public:
    /// Track interface methods
    virtual void tracksBegin( defDirection dir, int orig, int count, int step ) {}
    virtual void tracksLayer( const char * layer ) {}
    virtual void tracksEnd() {}
};
    
#endif
