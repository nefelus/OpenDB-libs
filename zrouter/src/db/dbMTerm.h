#ifndef ADS_DB_MTERM_H
#define ADS_DB_MTERM_H

#ifndef ADS_H
#include "ads.h"
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

class _dbMPin;
class _dbTarget;
class _dbDatabase;
class _dbTechAntennaAreaElement;
class _dbTechAntennaPinModel;
class dbIStream;
class dbOStream;
class dbDiff;

struct _dbMTermFlags
{
  dbIoType::Value  _io_type    : 4;
  dbSigType::Value _sig_type   : 4;
  uint             _mark       : 1;
  uint             _spare_bits : 23;
};

class _dbMTerm : public dbObject
{
  public:
    // PERSISTANT-MEMBERS
    _dbMTermFlags                _flags;
    uint                         _order_id;
    char *                       _name;
    dbId<_dbMTerm>               _next_entry;
    dbId<_dbMTerm>               _next_mterm;
    dbId<_dbMPin>                _pins;
    dbId<_dbTarget>              _targets;
    dbId<_dbTechAntennaPinModel> _oxide1;
    dbId<_dbTechAntennaPinModel> _oxide2;

    dbVector<_dbTechAntennaAreaElement *>  _par_met_area;
    dbVector<_dbTechAntennaAreaElement *>  _par_met_sidearea;
    dbVector<_dbTechAntennaAreaElement *>  _par_cut_area;
    dbVector<_dbTechAntennaAreaElement *>  _diffarea;

    void *           _timing_data;

    friend dbOStream & operator<<( dbOStream & stream, const _dbMTerm & mterm );
    friend dbIStream & operator>>( dbIStream & stream, _dbMTerm & mterm );

    _dbMTerm( _dbDatabase * db );
    _dbMTerm( _dbDatabase * db, const _dbMTerm & m );
    ~_dbMTerm();

    int operator==( const _dbMTerm & rhs ) const;
    int operator!=( const _dbMTerm & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbMTerm & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbMTerm::_dbMTerm( _dbDatabase * )
{
    _flags._io_type = dbIoType::INPUT;
    _flags._sig_type = dbSigType::SIGNAL;
    _flags._spare_bits = 0;
    _order_id = 0;
    _name = 0;
    _par_met_area.clear();
    _par_met_sidearea.clear();
    _par_cut_area.clear();
    _diffarea.clear();
    _timing_data = 0;
}
inline _dbMTerm::~_dbMTerm()
{
    if ( _name )
        free( (void *) _name );


	/************************************ dimitri_note ***********************************
         The following 4 vfields should change to look like     dbId<_dbTechAntennaPinModel> _oxide1;

	   	dbVector<_dbTechAntennaAreaElement *>  _par_met_area;
    		dbVector<_dbTechAntennaAreaElement *>  _par_met_sidearea;
    		dbVector<_dbTechAntennaAreaElement *>  _par_cut_area;
    		dbVector<_dbTechAntennaAreaElement *>  _diffarea;	
	************************************************************************************************/

	/* dimitri_fix : cooment out delete loops because of the copiler warning ***************************
		dbMTerm.h:97:15: warning: possible problem detected in invocation of delete operator: [-Wdelete-incomplete]
       			delete *antitr;
	****************************************************************************************************/

/********************************************************************************** dimitri_fix ********

    dbVector<_dbTechAntennaAreaElement *>::iterator  antitr;
    for (antitr = _par_met_area.begin(); antitr != _par_met_area.end(); antitr++)
      delete *antitr;
    _par_met_area.clear();

    for (antitr = _par_met_sidearea.begin(); antitr != _par_met_sidearea.end(); antitr++)
      delete *antitr;
    _par_met_sidearea.clear();

    for (antitr = _par_cut_area.begin(); antitr != _par_cut_area.end(); antitr++)
      delete *antitr;
    _par_cut_area.clear();

    for (antitr = _diffarea.begin(); antitr != _diffarea.end(); antitr++)
      delete *antitr;
    _diffarea.clear();
***********************************************************************************************************/
}
END_NAMESPACE_ADS

#endif
