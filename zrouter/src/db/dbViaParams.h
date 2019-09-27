#ifndef ADS_DB_VIA_PARAMS_H
#define ADS_DB_VIA_PARAMS_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbTechLayer;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbViaParams
{
  public:
    int                    _x_cut_size;
    int                    _y_cut_size;
    int                    _x_cut_spacing;
    int                    _y_cut_spacing;
    int                    _x_top_enclosure;
    int                    _y_top_enclosure;
    int                    _x_bot_enclosure;
    int                    _y_bot_enclosure;
    int                    _num_cut_rows;
    int                    _num_cut_cols;
    int                    _x_origin;
    int                    _y_origin;
    int                    _x_top_offset;
    int                    _y_top_offset;
    int                    _x_bot_offset;
    int                    _y_bot_offset;
    dbId<_dbTechLayer>     _top_layer;
    dbId<_dbTechLayer>     _cut_layer;
    dbId<_dbTechLayer>     _bot_layer;
    
    _dbViaParams( const _dbViaParams & v );
    _dbViaParams();
    ~_dbViaParams();

    int operator==( const _dbViaParams & rhs ) const;
    int operator!=( const _dbViaParams & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbViaParams & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    friend dbOStream & operator<<( dbOStream & stream, const _dbViaParams & v );
    friend dbIStream & operator>>( dbIStream & stream, _dbViaParams & v );
};

dbOStream & operator<<( dbOStream & stream, const _dbViaParams & v );
dbIStream & operator>>( dbIStream & stream, _dbViaParams & v );

END_NAMESPACE_ADS

#endif
