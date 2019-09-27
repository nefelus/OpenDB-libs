#ifndef ADS_DEFIN_COMPONENT_H
#define ADS_DEFIN_COMPONENT_H

#include <string.h>
#include <vector>
#include <map>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_ICOMPONENT_H
#include "definIComponent.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbInst;
class dbMaster;
class dbLib;
class dbSite;
 

class definComponent : public definIComponent, public definBase
{
    struct ltstr
    {
        bool operator()(const char * s1, const char * s2) const
        {
            return strcmp(s1,s2) < 0;
        }
    };

    typedef std::map<const char *, dbMaster *, ltstr> MasterMap;
    typedef std::map<const char *, dbSite *, ltstr> SiteMap;
    std::vector<dbLib *> _libs;
    MasterMap            _masters;
    SiteMap              _sites;
    dbInst *             _cur_inst;

  public:
    int                  _inst_cnt;
    int                  _iterm_cnt;

    /// Component interface methods
    virtual void begin( const char * name , const char * cell );
    virtual void placement( defPlacement status, int x, int y, defOrient orient );
    virtual void region( const char * region );
    virtual void halo( int left, int bottom, int right, int top );
    virtual void source( defSource source );
    virtual void weight( int weight );
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();

    dbMaster * getMaster( const char * name );
    dbSite * getSite( const char * name );

  public:
    definComponent();
    virtual ~definComponent();

    void init();

    void setLibs( std::vector<dbLib *> & libs )
    {
        _libs = libs;
    }
};
    
END_NAMESPACE_ADS

#endif
