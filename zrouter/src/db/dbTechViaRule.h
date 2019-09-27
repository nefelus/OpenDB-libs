#ifndef ADS_DB_VIA_RULE_H
#define ADS_DB_VIA_RULE_H

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

struct _dbTechViaRuleFlags
{
    uint   _spare_bits      : 32;
};
    
class _dbTechViaRule : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbTechViaRuleFlags  _flags;
    char *               _name;
    dbVector<uint>       _layer_rules;
    dbVector<uint>       _vias;

    _dbTechViaRule( _dbDatabase *, const _dbTechViaRule & v );
    _dbTechViaRule( _dbDatabase * );
    ~_dbTechViaRule();

    int operator==( const _dbTechViaRule & rhs ) const;
    int operator!=( const _dbTechViaRule & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbTechViaRule & rhs ) const
    {
        return strcmp( _name, rhs._name ) < 0;
    }
    
    void differences( dbDiff & diff, const char * field, const _dbTechViaRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechViaRule & v );
dbIStream & operator>>( dbIStream & stream, _dbTechViaRule & v );

END_NAMESPACE_ADS

#endif
