#ifndef ADS_I_OA_READER_H
#define ADS_I_OA_READER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZOBJECT_H
#include "ZObject.h"
#endif

#include <vector>
#include <string>

BEGIN_NAMESPACE_ADS

class dbLib;
class dbChip;
class dbBlock;

class IoaReader : public ZObject
{
  public:
    virtual void setDefsFile( const char * file ) = 0;
    virtual dbLib * readLib( dbDatabase * db, const char * lib ) = 0;
    virtual dbChip * readChip( dbDatabase * db,
                               const char * lib,
                               const char * cell,
                               const char * view,
                               std::vector<dbLib *> &search_libs ) = 0;
    virtual dbBlock * readBlock( dbBlock * parent,
                                 const char * lib,
                                 const char * cell,
                                 const char * view,
                                 std::vector<dbLib *> &search_libs ) = 0;
};

END_NAMESPACE_ADS

#endif
