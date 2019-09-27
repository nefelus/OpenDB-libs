#ifndef ADS_DB_METRICS_H
#define ADS_DB_METRICS_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_ID_H
#include "dbId.h"
#endif

#ifndef ADS_DB_OBJECT_H
#include "dbObject.h"
#endif

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;
class dbDiff;

class _dbMetrics : public dbObject
{
  public:
    int          _worst_slack;
    int          _total_slack;
    
    _dbMetrics( _dbDatabase * );
    _dbMetrics( _dbDatabase *, const _dbMetrics & i );
    ~_dbMetrics();
    int operator==( const _dbMetrics & rhs ) const;
    int operator!=( const _dbMetrics & rhs ) const { return ! operator==(rhs); }
    int operator<( const _dbMetrics & rhs ) const;
    void differences( dbDiff & diff, const char * field, const _dbMetrics & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
    static void merge( _dbMetrics & dst, _dbMetrics & src, int slew_op, int slack_op);
};

inline _dbMetrics::_dbMetrics( _dbDatabase * )
        : _worst_slack(2),
          _total_slack(2)
{
}

inline _dbMetrics::_dbMetrics( _dbDatabase *, const _dbMetrics & i )
        : _worst_slack(i._worst_slack),
          _total_slack(i._total_slack)
{
}

inline _dbMetrics::~_dbMetrics()
{
}

inline int _dbMetrics::operator==( const _dbMetrics & rhs ) const
{
    if ( _worst_slack != rhs._worst_slack )
        return false;

    if ( _total_slack != rhs._total_slack )
        return false;

    return true;
}

inline int _dbMetrics::operator<( const _dbMetrics & rhs ) const
{
    return getOID() < rhs.getOID();
}

inline dbOStream & operator<<( dbOStream & stream, const _dbMetrics & tmg )
{
    stream << tmg._worst_slack;
    stream << tmg._total_slack;
    return stream;
}

inline dbIStream & operator>>( dbIStream & stream, _dbMetrics & tmg )
{
    stream >> tmg._worst_slack;
    stream >> tmg._total_slack;
    return stream;
}

END_NAMESPACE_ADS

#endif
