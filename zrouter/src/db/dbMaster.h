#ifndef ADS_DB_MASTER_H
#define ADS_DB_MASTER_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_CORE_H
#include "dbCore.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

#ifndef ADS_DB_HASH_TABLE_H
#include "dbHashTable.h"
#endif

BEGIN_NAMESPACE_ADS

template <class T> class dbTable;
class _dbMTerm;
class _dbBox;
class _dbMPin;
class _dbTarget;
class _dbSite;
class _dbDatabase;
class _dbTechAntennaPinModel;
class dbBoxItr;
class dbMPinItr;
class dbTargetItr;
class dbIStream;
class dbOStream;
class dbDiff;

struct dbMasterFlags
{
    uint                _frozen       : 1;
    uint                _x_symmetry   : 1;
    uint                _y_symmetry   : 1;
    uint                _R90_symmetry : 1;
    dbMasterType::Value _type         : 6;
    uint                _mark         : 1;
    uint                _sequential   : 1;
    uint                _special_power   : 1;
    uint                _spare_bits_19   : 19;
};

class _dbMaster : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    dbMasterFlags          _flags;
    int                    _x;
    int                    _y;
    uint                   _height;
    uint                   _width;
    uint                   _mterm_cnt;
    uint                   _id;
    char *                 _name;
    dbId<_dbMaster>        _next_entry;
    dbId<_dbMaster>        _leq;
    dbId<_dbMaster>        _eeq;
    dbId<_dbBox>           _obstructions;
    dbId<_dbSite>          _site;
    dbHashTable<_dbMTerm>  _mterm_hash;
    dbTable<_dbMTerm> *    _mterm_tbl;
    dbTable<_dbMPin> *     _mpin_tbl;
    dbTable<_dbTarget> *   _target_tbl;
    dbTable<_dbBox> *      _box_tbl;
    dbTable<_dbTechAntennaPinModel> * _antenna_pin_model_tbl;
    
    void *		   _sy_cell;
     
    // NON-PERSISTANT-MEMBERS
    dbBoxItr *          _box_itr;
    dbMPinItr *         _mpin_itr;
    dbTargetItr *       _target_itr;
	int                 _clocked_mterm_index;
	int                 _output_mterm_index;

    _dbMaster( _dbDatabase * db );
    _dbMaster( _dbDatabase * db, const _dbMaster & m );
    ~_dbMaster();
    int operator==( const _dbMaster & rhs ) const;
    int operator!=( const _dbMaster & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbMaster & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    dbObjectTable * getObjectTable( dbObjectType type );
};

dbOStream & operator<<( dbOStream & stream, const _dbMaster & master );
dbIStream & operator>>( dbIStream & stream, _dbMaster & master );

END_NAMESPACE_ADS

#endif
