#ifndef ADS_DB_TRACK_GRID_H
#define ADS_DB_TRACK_GRID_H

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

class _dbTechLayer;
class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbTrackGrid : public dbObject
{
  public:
    dbId<_dbTechLayer> _layer;
    dbVector<int>      _x_origin;
    dbVector<int>      _x_count;
    dbVector<int>      _x_step;
    dbVector<int>      _y_origin;
    dbVector<int>      _y_count;
    dbVector<int>      _y_step;
    dbId<_dbTechLayer> _next_grid;
    
    _dbTrackGrid( _dbDatabase *, const _dbTrackGrid & g );
    _dbTrackGrid( _dbDatabase * );
    ~_dbTrackGrid();

    int operator==( const _dbTrackGrid & rhs ) const;
    int operator!=( const _dbTrackGrid & rhs ) const { return ! operator==(rhs); }

    int operator<( const _dbTrackGrid & rhs ) const
    {
        if ( _layer < rhs._layer )
            return true;

        if ( _layer > rhs._layer )
            return false;

        return false;
    }
    
    void differences( dbDiff & diff, const char * field, const _dbTrackGrid & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTrackGrid::_dbTrackGrid( _dbDatabase *, const _dbTrackGrid & g )
        : _layer(g._layer),
          _x_origin(g._x_origin),
          _x_count(g._x_count),
          _x_step(g._x_step),
          _y_origin(g._y_origin),
          _y_count(g._y_count),
          _y_step(g._y_step),
          _next_grid(g._next_grid)
{
}

inline _dbTrackGrid::_dbTrackGrid( _dbDatabase * )
{
}

inline _dbTrackGrid::~_dbTrackGrid()
{
}

inline dbOStream & operator<<( dbOStream & stream,  const _dbTrackGrid & grid )
{
    stream << grid._layer;
    stream << grid._x_origin;
    stream << grid._x_count;
    stream << grid._x_step;
    stream << grid._y_origin;
    stream << grid._y_count;
    stream << grid._y_step;
    stream << grid._next_grid;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbTrackGrid & grid )
{
    stream >> grid._layer;
    stream >> grid._x_origin;
    stream >> grid._x_count;
    stream >> grid._x_step;
    stream >> grid._y_origin;
    stream >> grid._y_count;
    stream >> grid._y_step;
    stream >> grid._next_grid;
    return stream;
}

END_NAMESPACE_ADS

#endif
