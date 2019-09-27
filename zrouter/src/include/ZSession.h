#ifndef ADS_ZSESSION_H
#define ADS_ZSESSION_H

#ifndef ADS_TM_H
#include "tm.h"
#endif

BEGIN_NAMESPACE_ADS

class ZNamespace;

class ZSession : public ZTechModule<ZSession>
{
  public:
    ZNamespace * _ns;
    
    ZSession( ZArgs * in, ZArgs * out );
    ~ZSession();
    friend class ZInterface;
};

END_NAMESPACE_ADS

#endif
