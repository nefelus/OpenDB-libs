#ifndef ADS_DEFIN_PIN_PROPS_H
#define ADS_DEFIN_PIN_PROPS_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DEFIN_IPIN_PROPS_H
#include "definIPinProps.h"
#endif

#ifndef ADS_DEFIN_BASE_H
#include "definBase.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

BEGIN_NAMESPACE_ADS

class dbObject;
class dbBTerm;

class definPinProps : public definIPinProps, public definBase
{
  private:
    dbObject * _cur_obj;
    
  public:
    virtual void begin(const char * inst, const char * term);
    virtual void property( const char * name, const char * value );
    virtual void property( const char * name, int value );
    virtual void property( const char * name, double value );
    virtual void end();

    definPinProps();
    virtual ~definPinProps();
};
    
END_NAMESPACE_ADS

#endif
