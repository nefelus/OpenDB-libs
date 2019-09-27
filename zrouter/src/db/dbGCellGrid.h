#ifndef ADS_DB_GCELL_GRID_H
#define ADS_DB_GCELL_GRID_H

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

class _dbDatabase;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbGCellGrid : public dbObject
{
  public:
    dbVector<int> _x_origin;
    dbVector<int> _x_count;
    dbVector<int> _x_step;
    dbVector<int> _y_origin;
    dbVector<int> _y_count;
    dbVector<int> _y_step;
    
    _dbGCellGrid(_dbDatabase * );
    _dbGCellGrid(_dbDatabase *, const _dbGCellGrid & g );
    ~_dbGCellGrid();

    int operator==( const _dbGCellGrid & rhs ) const;
    int operator!=( const _dbGCellGrid & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbGCellGrid & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

    int operator<( const _dbGCellGrid & rhs ) const
    {
        _dbGCellGrid * o1 = (_dbGCellGrid *) this;
        _dbGCellGrid * o2 = (_dbGCellGrid *) &rhs;
        return o1->getOID() < o2->getOID();
    }
};

inline _dbGCellGrid::_dbGCellGrid(_dbDatabase * )
{
}

inline _dbGCellGrid::_dbGCellGrid(_dbDatabase *, const _dbGCellGrid & g )
        : _x_origin(g._x_origin),
          _x_count(g._x_count),
          _x_step(g._x_step),
          _y_origin(g._y_origin),
          _y_count(g._y_count),
          _y_step(g._y_step)
{
}

inline _dbGCellGrid::~_dbGCellGrid()
{
}

inline dbOStream & operator<<( dbOStream & stream,  const _dbGCellGrid & grid )
{
    stream << grid._x_origin;
    stream << grid._x_count;
    stream << grid._x_step;
    stream << grid._y_origin;
    stream << grid._y_count;
    stream << grid._y_step;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbGCellGrid & grid )
{
    stream >> grid._x_origin;
    stream >> grid._x_count;
    stream >> grid._x_step;
    stream >> grid._y_origin;
    stream >> grid._y_count;
    stream >> grid._y_step;
    return stream;
}

END_NAMESPACE_ADS

#endif
