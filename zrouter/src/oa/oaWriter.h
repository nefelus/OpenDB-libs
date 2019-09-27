#ifndef ADS_OA_WRITER_H
#define ADS_OA_WRITER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_I_OA_WRITER_H
#include "IoaWriter.h"
#endif

#include <string>

BEGIN_NAMESPACE_ADS

class dbLib;
class dbBlock;

class oaWriter : public ZImplements<oaWriter, IoaWriter>
{
    std::string _lib_path;
    std::string _design_path;
    std::string _defs_file;

    bool runCommand( const std::string & cmd );
    
  public:

    oaWriter();
    ~oaWriter();
    
    void setLibPath( const char * path );
    void setDesignPath( const  char * path );
    void setDefsFile( const  char * file );
    bool writeTechAndLib( dbLib * lib, const char * lib_name );
    bool writeBlock( dbBlock * lib,
                     const char * lib_name,
                     const char * cell_name,
                     const char * view_name,
                     const char * tech_lib_name
                     );
};

END_NAMESPACE_ADS

#endif
