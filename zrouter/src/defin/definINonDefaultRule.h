#ifndef ADS_DEFIN_INON_DEFAULT_RULE_H
#define ADS_DEFIN_INON_DEFAULT_RULE_H

class definINonDefaultRule
{
  public:
    // non-default-rule interface methods
    virtual void beginRules(int n) {}
    virtual void beginRule( const char * name ) {}
    virtual void hardSpacing() {}
    virtual void via( const char * name ) {}
    virtual void viaRule( const char * rule ) {}
    virtual void minCuts( const char * layer, int count ) {}
    virtual void beginLayerRule( const char * layer, int width ) {}
    virtual void spacing(int s) {}
    virtual void wireExt(int e) {}
    virtual void endLayerRule() {}
    virtual void property( const char * name, const char * value ) {}
    virtual void property( const char * name, int value ) {}
    virtual void property( const char * name, double value ) {}
    virtual void endRule() {}
    virtual void endRules() {}
};
    
#endif
