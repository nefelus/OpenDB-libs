#ifndef ADS_DEFIN_ROW_H
#define ADS_DEFIN_ROW_H

#include <string.h>
#include <vector>
#include <map>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IROW_H
#include "definIRow.h"
#endif

#ifndef ADS_DEFIN_DEF_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class dbSite;
class dbLib;
class dbRow;

class definRow : public definIRow, public definBase
{
    struct ltstr
    {
        bool operator()(const char * s1, const char * s2) const
        {
            return strcmp(s1,s2) < 0;
        }
    };

    typedef std::map<const char *, dbSite *, ltstr> SiteMap;
    SiteMap              _sites;
    std::vector<dbLib *> _libs;
    dbRow *              _cur_row;

    /// Row interface methods
    virtual void begin( const char * name,
                        const char * site,
                        int origin_x, int origin_y,
                        defOrient orient,
                        defRow direction,
                        int num_sites,
                        int spacing );
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();
    
    dbSite * getSite( const char * name );
    
  public:
    definRow();
    virtual ~definRow();
    void init();
    void setLibs( std::vector<dbLib *> & libs )
    {
        _libs = libs;
    }
    
};
    
END_NAMESPACE_ADS

#endif
