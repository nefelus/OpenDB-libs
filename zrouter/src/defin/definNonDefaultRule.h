#ifndef ADS_DEFIN_NON_DEFAULT_RULE_H
#define ADS_DEFIN_NON_DEFAULT_RULE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DEFIN_INON_DEFAULT_RULE_H
#include "definINonDefaultRule.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbTechNonDefaultRule;
class dbTechLayerRule;
 
class definNonDefaultRule : public definINonDefaultRule, public definBase
{
    dbTechNonDefaultRule * _cur_rule;
    dbTechLayerRule *      _cur_layer_rule;
    
    virtual void beginRules(int n);
    virtual void beginRule( const char * name );
    virtual void hardSpacing();
    virtual void via( const char * name );
    virtual void viaRule( const char * rule );
    virtual void minCuts( const char * layer, int count );
    virtual void beginLayerRule( const char * layer, int width );
    virtual void spacing(int s);
    virtual void wireExt(int e);
    virtual void endLayerRule();
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void endRule();
    virtual void endRules();

  public:
    definNonDefaultRule();
    virtual ~definNonDefaultRule();
    void init();
};
    
END_NAMESPACE_ADS

#endif
