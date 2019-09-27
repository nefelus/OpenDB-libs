#ifndef ADS_DEFIN_IREADER_H
#define ADS_DEFIN_IREADER_H

#ifndef ADS_DB_H
#include "db.h"
#endif

#ifndef ADS_DB_DEFIN_TYPES_H
#include "definTypes.h"
#endif

class definIReader
{
  public:
    /// die area
    virtual void dieArea( const std::vector<defPoint> & points ) {}
    
    // error interface method
    virtual void error( const char * msg ) { printf("%s\n", msg ); }

    // units interface method
    virtual void units( int d ) {}

    virtual void line( int line ) {}
};
    
#endif
