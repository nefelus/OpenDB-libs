#ifndef ADS_I_OA_WRITER_H
#define ADS_I_OA_WRITER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif

BEGIN_NAMESPACE_ADS

class dbLib;
class dbBlock;

class IoaWriter : public ZObject
{
  public:
    virtual void setLibPath( const char * path ) = 0;
    virtual void setDesignPath( const char * path ) = 0;
    virtual void setDefsFile( const char * file ) = 0;
    virtual bool writeTechAndLib( dbLib * lib, const char * lib_name ) = 0;
    virtual bool writeBlock( dbBlock * lib,
                             const char * lib_name,
                             const char * cell_name,
                             const char * view_name,
                             const char * tech_lib_name
                             ) = 0;
};

END_NAMESPACE_ADS

#endif
