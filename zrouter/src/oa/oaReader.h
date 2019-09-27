#ifndef ADS_OA_READER_H
#define ADS_OA_READER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_I_OA_READER_H
#include "IoaReader.h"
#endif

#include <string>

BEGIN_NAMESPACE_ADS

class dbLib;
class dbChip;

class oaReader : public ZImplements<oaReader, IoaReader>
{
    std::string _defs_file;

    bool runCommand( const std::string & cmd );
    
  public:

    oaReader();
    ~oaReader();
    
    void setDefsFile( const char * file );

    dbLib * readLib( dbDatabase * db, const char * lib );
    
    dbChip * readChip( dbDatabase * db,
                       const char * lib,
                       const char * cell,
                       const char * view,
                       std::vector<dbLib *> &search_libs );
    dbBlock * readBlock( dbBlock * parent,
                         const char * lib,
                         const char * cell,
                         const char * view,
                         std::vector<dbLib *> &search_libs );
};

END_NAMESPACE_ADS

#endif
