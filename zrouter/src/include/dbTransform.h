// Athena --- Transformation utilities

#ifndef ADS_DB_TRANSFORM_H
#define ADS_DB_TRANSFORM_H

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

#ifndef ADS_DB_TYPES_H
#include "dbTypes.h"
#endif

BEGIN_NAMESPACE_ADS

class dbOStream;
class dbIStream;
class dbDiff;

//
// Database Transform - Transform points by rotation and translation.
//
class dbTransform
{
    friend class _dbBlock;
    dbOrientType::Value _orient;
    adsPoint            _offset;
    
  public:

    // T = <R0, (0,0)>
    dbTransform()
        : _orient(dbOrientType::R0), _offset(0,0)
    {
    }

    //  T = <R0, offset>
    dbTransform( adsPoint offset )
        : _orient(dbOrientType::R0), _offset(offset)
    {
    }

    //  T = <orient, (0,0)>
    dbTransform( dbOrientType orient )
        : _orient(orient), _offset(0,0)
    {
    }

    //  T = <orient, offset>
    dbTransform( dbOrientType orient, adsPoint offset )
        : _orient(orient), _offset(offset)
    {
    }

    bool operator==( const dbTransform & t ) const
    {
        return (_orient == t._orient) && (_offset == t._offset);
    }

    bool operator!=( const dbTransform & t ) const
    {
        return ! operator==(t);
    }
    
    void setOrient( dbOrientType orient )
    {
        _orient = orient;
    }

    void setOffset( adsPoint offset )
    {
        _offset = offset;
    }

    void setTransform( dbOrientType orient, adsPoint offset )
    {
        _orient = orient;
        _offset = offset;
    }
    
    // Apply transform to this point
    void apply( adsPoint & p ) const;

    // Apply transform to this point
    void apply( adsRect & r ) const;

    // Post multiply transform.
    void concat( const dbTransform & t );

    // Post multiply transform
    void concat( const dbTransform & t, dbTransform & result );

    // Compute inverse transform
    void invert( dbTransform & result ) const;

    // Compute inverse transform
    void invert();

    dbOrientType getOrient() const { return _orient; }
    adsPoint getOffset() const { return _offset; }

    friend dbOStream & operator<<( dbOStream & stream, const dbTransform & t );
    friend dbIStream & operator>>( dbIStream & stream, dbTransform & t );
    friend dbDiff & operator<<( dbDiff & diff, const dbTransform & t );
};

dbOStream & operator<<( dbOStream & stream, const dbTransform & t );
dbIStream & operator>>( dbIStream & stream, dbTransform & t );
dbDiff & operator<<( dbDiff & diff, const dbTransform & t );

inline void dbTransform::concat( const dbTransform & t )
{
    dbTransform result;
    concat(t,result);
    *this = result;
}

inline void dbTransform::invert()
{
    dbTransform result;
    invert(result);
    *this = result;
}

END_NAMESPACE_ADS

#endif
