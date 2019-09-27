#ifndef ADS_DEFIN_IROW_H
#define ADS_DEFIN_IROW_H

#ifndef ADS_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definIRow
{
  public:
    
    /// Row interface methods
    virtual void begin( const char * name,
                        const char * site,
                        int origin_x, int origin_y,
                        defOrient orient,
                        defRow direction,
                        int num_sites,
                        int spacing ) 
        {
        }

    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void end() {}
    
};
    
#endif
