#ifndef ADS_DEFIN_PROP_DEFS_H
#define ADS_DEFIN_PROP_DEFS_H

#ifndef ADS_DEFIN_IPROP_DEFS_H
#include "definIPropDefs.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#include <string>

BEGIN_NAMESPACE_ADS

/**********************************************************
 *
 * DEF PROPERTY DEFINITIONS are stored as hierachical
 * properties using the following structure:
 *
 *                       [__ADS_DEF_PROPERTY_DEFINITIONS__]
 *                                    +
 *                                    |
 *                                    |
 *                           +--------+------+  
 *                      [COMPONENT] [NET] [GROUP] .... 
 *                           +        |      |
 *                           |       ...    ...
 *                           |
 *                    +------+
 *                 [NAME] (type encoded as property-type)
 *                    +
 *                    |
 *                +---+--+-----+
 *             [VALUE] [MIN] [MAX] (Optional properties)
 *
 **********************************************************/

class dbProperty;

class definPropDefs : public definIPropDefs, public definBase
{
    dbProperty * _defs;
    dbProperty * _prop;
    
  public:
    virtual void beginDefinitions();
    virtual void begin( defObjectType, const char *, defPropType );
    virtual void value( const char * );
    virtual void value( int );
    virtual void value( double );
    virtual void range( int, int );
    virtual void range( double, double );
    virtual void end();
    virtual void endDefinitions();
};

END_NAMESPACE_ADS
    
#endif
