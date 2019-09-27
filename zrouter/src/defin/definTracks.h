#ifndef ADS_DEFIN_IMPL_H
#define ADS_DEFIN_IMPL_H

#include <vector>
#include <map>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_ITRACKS_H
#include "definITracks.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

struct Track
{
    int  _dir;
    int  _orig;
    int  _step;
    int  _count;
};

class definTracks : public definITracks, public definBase
{
    Track  _track;

    /// Track interface methods
    virtual void tracksBegin( defDirection dir, int orig, int count, int step );
    virtual void tracksLayer( const char * layer );
    virtual void tracksEnd();

  public:
    definTracks();
    virtual ~definTracks();
    void init();
};
    
END_NAMESPACE_ADS

#endif
