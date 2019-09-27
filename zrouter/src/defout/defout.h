#ifndef ADS_DEFOUT_H
#define ADS_DEFOUT_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class defout_impl;
class dbNet;
class dbBlock;

class defout
{
    defout_impl * _writer;

  public:
    enum Version { DEF_5_3, DEF_5_4, DEF_5_5, DEF_5_6 };

    defout();
    ~defout();

    void setUseLayerAlias( bool value );
    void setUseNetInstIds( bool value );
    void setUseMasterIds( bool value );
    void selectNet( dbNet *net );
    void setVersion( Version v ); // default is 5.5

    bool writeBlock( dbBlock * block, const char * def_file );
    
};

END_NAMESPACE_ADS

#endif
