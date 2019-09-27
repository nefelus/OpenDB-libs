#ifndef ADS_DB_TECH_MINCUT_AREA_H
#define ADS_DB_TECH_MINCUT_AREA_H

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

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class _dbTechLayer;
class dbIStream;
class dbOStream;
class dbDiff;

class _dbTechMinCutRule : public dbObject
{
  public:
    // PERSISTENT-MEMBERS
  TechMinCutRule::_Flword  _flags;
  uint                     _cuts;
  uint                     _width;
  uint                     _length;
  uint                     _within;

    _dbTechMinCutRule( _dbDatabase * db, const _dbTechMinCutRule & r );
    _dbTechMinCutRule( _dbDatabase * db );
    ~_dbTechMinCutRule();

    int operator==( const _dbTechMinCutRule & rhs ) const;
    int operator!=( const _dbTechMinCutRule & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechMinCutRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTechMinCutRule::_dbTechMinCutRule( _dbDatabase * db, const _dbTechMinCutRule & r)
        : _flags(r._flags),
          _cuts(r._cuts),
          _width(r._width),
          _length(r._length),
          _within(r._within)
{
}

inline _dbTechMinCutRule::_dbTechMinCutRule( _dbDatabase * db )
{
  _flags._rule = TechMinCutRule::NONE;
  _flags._cuts_length = 0;
  _flags._spare_bits = 0;
  _cuts  = 0;
  _width = _length = _within = 0;
}

inline _dbTechMinCutRule::~_dbTechMinCutRule()
{}

dbOStream & operator<<( dbOStream & stream, const _dbTechMinCutRule &rule );
dbIStream & operator>>( dbIStream & stream, _dbTechMinCutRule & rule );

class _dbTechMinEncRule : public dbObject
{
  public:
    // PERSISTENT-MEMBERS

  struct _Flword
  {
    uint _has_width      :  1;
    uint  _spare_bits    : 31;
  } _flags;

  uint                        _area;
  uint                        _width;

    _dbTechMinEncRule( _dbDatabase * db );
    _dbTechMinEncRule( _dbDatabase * db, const _dbTechMinEncRule & r);
    ~_dbTechMinEncRule();
    int operator==( const _dbTechMinEncRule & rhs ) const;
    int operator!=( const _dbTechMinEncRule & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const _dbTechMinEncRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _dbTechMinEncRule::_dbTechMinEncRule(  _dbDatabase * db, const _dbTechMinEncRule & r)
        : _flags(r._flags),
          _area(r._area),
          _width(r._width)
{
}

inline _dbTechMinEncRule::_dbTechMinEncRule( _dbDatabase * db )
{
  _flags._has_width = 0;
  _flags._spare_bits = 0;
  _area  = 0;
  _width = 0;
}

inline _dbTechMinEncRule::~_dbTechMinEncRule()
{
}

dbOStream & operator<<( dbOStream & stream, const _dbTechMinEncRule & rule );
dbIStream & operator>>( dbIStream & stream, _dbTechMinEncRule & rule );

END_NAMESPACE_ADS

#endif
