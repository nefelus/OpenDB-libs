/* Copyright Athena Design Systems 2004 */

#ifndef ADS_ZNAMESPACE_H
#define ADS_ZNAMESPACE_H

#include <string.h>
#include<map>
#include<vector>

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class ZObject;
class dbDatabase;
class dbObject;

////////////////////////////////////
/// ZNamespace
////////////////////////////////////
class ZNamespace
{
    struct ltstr
    {
        bool operator()(const char * s1, const char * s2) const
        {
            return strcmp(s1,s2) < 0;
        }
    };

    class ZEntry;
    class dbEntry;
    
    std::map<ZObject *, ZEntry *>            _zobj_entries;
    std::map<dbDatabase *, dbEntry *>        _db_entries;
    std::map<const char *, ZEntry *, ltstr>  _zobj_names;
    std::map<const char *, dbEntry *, ltstr> _db_names;
    uint                                     _unique_id;
    
public:
    ZNamespace();
    ~ZNamespace();

    // These functions may throw a ZException
    const char * addZObject( ZObject * obj );
    void removeZObject( ZObject * obj );
    ZObject * resolveZObject( const char * zname );
    void registerDb( dbDatabase * db );
    void unregisterDb( dbDatabase * db );
    dbObject * resolveDB( const char * dbname );
};

END_NAMESPACE_ADS

#endif
