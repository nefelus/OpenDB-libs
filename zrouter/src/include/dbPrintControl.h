#ifndef ADS_DB_PRINTCONTROL_H
#define ADS_DB_PRINTCONTROL_H

#include "array1.h"

BEGIN_NAMESPACE_ADS
    
class dbNet;
class dbInst;

class dbPrintControl
{
  public:
   enum Type
    {
        EXTTREE,
        EXTRACT  //  keep this as the last in enum !!!!
    } ;
    // NON-PERSISTANT-MEMBERS
    uint _groupCnt;
    uint _printCnt;
    Ath__array1D<dbNet *> ** _targetNet;
    Ath__array1D<dbInst *> ** _targetInst;


    dbPrintControl();
    ~dbPrintControl();
    void setPrintControl(dbBlock *block, const char *name, const char *netn, const char *instn);
    uint getPrintCnt(dbPrintControl::Type ptype, dbNet *net);
    uint getPrintCnt(dbPrintControl::Type ptype, dbInst *inst);
};

END_NAMESPACE_ADS

#endif
