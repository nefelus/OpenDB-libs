#ifndef ADS_DB_TECH_LAYERANTENNARULE_H
#define ADS_DB_TECH_LAYERANTENNARULE_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

#ifndef ADS_DB_TECH_LAYER_H
#include "dbTechLayer.h"
#endif

BEGIN_NAMESPACE_ADS

class _dbDatabase;
class _dbMTerm;
class _dbTechLayer;
class dbTech;
class dbTechLayer;
class dbIStream;
class dbOStream;
class dbDiff;
class lefout;

//
// An antenna multiplier factor is applied to metal. A separate factor may
// be used for diffusion connected metal.
//
class _ARuleFactor
{
 public:
  double    _factor;
  double    _factor_diffuse;
  bool      _explicit;
  bool      _explicit_diffuse;

  _ARuleFactor( const _ARuleFactor & f);
  _ARuleFactor();
  ~_ARuleFactor();
  void setFactors(double factor, double diffuse);
  int operator==( const _ARuleFactor & rhs ) const;
  int operator!=( const _ARuleFactor & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _ARuleFactor & rhs ) const;
  void out( dbDiff & diff, char side, const char * field ) const;
};

inline _ARuleFactor::_ARuleFactor(const _ARuleFactor & f)
        : _factor(f._factor),
          _factor_diffuse(f._factor_diffuse),
          _explicit(f._explicit),
          _explicit_diffuse(f._explicit_diffuse)
{
}

inline _ARuleFactor::_ARuleFactor()
{  _factor = _factor_diffuse = 1.0; _explicit = _explicit_diffuse = false; }

inline _ARuleFactor::~_ARuleFactor()
{}

dbOStream & operator<<( dbOStream & stream, const _ARuleFactor & arf );
dbIStream & operator>>( dbIStream & stream, _ARuleFactor & arf );

//
// An antenna rule ratio is a single ratio for non-diffusion connected segments
// or a piecewise linear function of diffusion area for diffusion connected
// segments.
//
class _ARuleRatio
{
 public:
  double                _ratio;
  dbVector<double>      _diff_idx;
  dbVector<double>      _diff_ratio;

  _ARuleRatio();
  _ARuleRatio(const _ARuleRatio & r);
  ~_ARuleRatio();

  void  setRatios(double ratio, double diff_ratio);
  void  setPWL(const std::vector<double> & diff_idx, const std::vector<double> & ratios);

  int operator==( const _ARuleRatio & rhs ) const;
  int operator!=( const _ARuleRatio & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _ARuleRatio & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

inline _ARuleRatio::_ARuleRatio( const _ARuleRatio & r)
        : _ratio(r._ratio),
          _diff_idx(r._diff_idx),
          _diff_ratio(r._diff_ratio)
{
}

inline _ARuleRatio::_ARuleRatio()
{  _ratio = 0; _diff_idx.clear(); _diff_ratio.clear(); }

inline _ARuleRatio::~_ARuleRatio()
{}

dbOStream & operator<<( dbOStream & stream, const _ARuleRatio & arrt );
dbIStream & operator>>( dbIStream & stream, _ARuleRatio & arrt );

///  An antenna rule comprises a multiplier factor for area and sidearea
///  (perimeter), as well as ratios for the area and sidearea for both
///  a partial (single layer based) and cumulative (all layer) models.

class _dbTechLayerAntennaRule : public dbObject
{
  public:
  dbId<_dbTechLayer> _layer;
  _ARuleFactor        _area_mult;
  _ARuleFactor        _sidearea_mult;
  _ARuleRatio         _par_area_val;
  _ARuleRatio         _cum_area_val;
  _ARuleRatio         _par_sidearea_val;
  _ARuleRatio         _cum_sidearea_val;

  _dbTechLayerAntennaRule( _dbDatabase * ) {}
  _dbTechLayerAntennaRule( _dbDatabase *, const _dbTechLayerAntennaRule & r )
      : _layer( r._layer ),
        _area_mult( r._area_mult ),
        _sidearea_mult( r._sidearea_mult ),
        _par_area_val( r._par_area_val ),
        _cum_area_val( r._cum_area_val ),
        _par_sidearea_val( r._par_sidearea_val ),
        _cum_sidearea_val( r._cum_sidearea_val )
  {
  }

  ~_dbTechLayerAntennaRule() {}
  int operator==( const _dbTechLayerAntennaRule & rhs ) const;
  int operator!=( const _dbTechLayerAntennaRule & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _dbTechLayerAntennaRule & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechLayerAntennaRule & inrule );
dbIStream & operator>>( dbIStream & stream, _dbTechLayerAntennaRule & inrule );

//
// An antenna area element comprises an area value and an optional layer.
// It stores area in squm.
//
class _dbTechAntennaAreaElement
{
 public:
  ~_dbTechAntennaAreaElement() {};

  static void create( dbVector<_dbTechAntennaAreaElement *> & incon, double inarea, dbTechLayer *inly = NULL); // Allocate a new element and add to container.
  void writeLef(const char *header, dbTech *tech, lefout & writer) const;

  friend dbOStream & operator<<( dbOStream & stream, const _dbTechAntennaAreaElement * aae );
  friend dbIStream & operator>>( dbIStream & stream, _dbTechAntennaAreaElement *& aae );

  _dbTechAntennaAreaElement(const _dbTechAntennaAreaElement & e);

  int operator==( const _dbTechAntennaAreaElement & rhs ) const;
  int operator!=( const _dbTechAntennaAreaElement & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _dbTechAntennaAreaElement & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;

 private:
  _dbTechAntennaAreaElement();
  double               _area;
  dbId<_dbTechLayer>   _lyidx;
};

//
// An antenna pin model stores the model specific antenna info for a pin.
//
class _dbTechAntennaPinModel : public dbObject
{
 public:
    dbId<_dbMTerm>                         _mterm;
    dbVector<_dbTechAntennaAreaElement *>  _gate_area;
    dbVector<_dbTechAntennaAreaElement *>  _max_area_car;
    dbVector<_dbTechAntennaAreaElement *>  _max_sidearea_car;
    dbVector<_dbTechAntennaAreaElement *>  _max_cut_car;

  _dbTechAntennaPinModel( _dbDatabase *, const _dbTechAntennaPinModel & m );
  _dbTechAntennaPinModel(_dbDatabase * ) {}
  ~_dbTechAntennaPinModel() {}

  int operator==( const _dbTechAntennaPinModel & rhs ) const;
  int operator!=( const _dbTechAntennaPinModel & rhs ) const { return ! operator==(rhs); }
  void differences( dbDiff & diff, const char * field, const _dbTechAntennaPinModel & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

dbOStream & operator<<( dbOStream & stream, const _dbTechAntennaPinModel & inmod );
dbIStream & operator>>( dbIStream & stream, _dbTechAntennaPinModel & inmod );

END_NAMESPACE_ADS

#endif
