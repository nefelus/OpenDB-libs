#ifndef ADS_DB_VIA_GENERATE_RULE_H
#define ADS_DB_VIA_GENERATE_RULE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_VECTOR_H
#include "dbVector.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTechLayer;
class _dbBox;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

//
// These flags keep track of the variations between difference LEF versions
//
struct _dbTechViaGenerateRuleFlags
{
    uint   _default                : 1; 
    uint   _spare_bits             : 31;
};
    
class _dbTechViaGenerateRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechViaGenerateRuleFlags _flags; 
    char *                      _name;
    dbVector<uint>              _layer_rules;

    _dbTechViaGenerateRule( _dbDatabase *, const _dbTechViaGenerateRule & v );
    _dbTechViaGenerateRule( _dbDatabase * );
    ~_dbTechViaGenerateRule();

    int operator==( const _dbTechViaGenerateRule & rhs ) const;
    int operator!=( const _dbTechViaGenerateRule & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbTechViaGenerateRule & rhs ) const
    {
        return strcmp( _name, rhs._name ) < 0;
    }
    
    void differences( dbDiff & diff, const char * field, const _dbTechViaGenerateRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechViaGenerateRule & v );
dbIStream & operator>>( dbIStream & stream, _dbTechViaGenerateRule & v );

END_NAMESPACE_ADS

#endif
