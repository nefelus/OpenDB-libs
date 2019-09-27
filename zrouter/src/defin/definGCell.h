#ifndef ADS_DEFIN_GCELL_H
#define ADS_DEFIN_GCELL_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DEFIN_IGCELL_H
#include "definIGCell.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

BEGIN_NAMESPACE_ADS

class definGCell : public definIGCell, public definBase
{
    /// GCell interface methods
    virtual void gcell( defDirection dir, int orig, int count, int step );

  public:
    definGCell();
    virtual ~definGCell();
    void init();
};
    
END_NAMESPACE_ADS

#endif
