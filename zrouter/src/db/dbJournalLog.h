#ifndef ADS_DB_JOURNAL_LOG_H
#define ADS_DB_JOURNAL_LOG_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_PAGED_VECTOR_H
#include "dbPagedVector.h"
#endif

BEGIN_NAMESPACE_ADS

class dbIStream;
class dbOStream;

class dbJournalLog
{
    dbPagedVector<unsigned char> _data;
    int                          _idx;
    int                          _debug;

    unsigned char next() { return _data[_idx++]; }

public:

    dbJournalLog();
    ~dbJournalLog();
    
    void clear()
    {
        _data.clear();
        _idx = 0;
    }

    bool empty() { return _data.size() == 0; }
    
    uint idx() { return _idx; }
    uint size() { return _data.size(); }
    void push( bool value );
    void push( char value );
    void push( unsigned char value );
    void push( int value );
    void push( unsigned int value );
    void push( float value );
    void push( double value );
    void push( const char * value );

    void begin() { _idx = 0; }
    bool end() { return _idx == (int)_data.size(); }
    void set( uint idx ) { _idx = idx; }

    void pop( bool & value );
    void pop( char & value );
    void pop( unsigned char & value );
    void pop( int & value );
    void pop( unsigned int & value );
    void pop( float & value );
    void pop( double & value );
    void pop( char * & value );
    void pop( std::string & value );
    friend dbIStream & operator>>( dbIStream & stream, dbJournalLog & log );
    friend dbOStream & operator<<( dbOStream & stream, const dbJournalLog & log );
};

END_NAMESPACE_ADS    


#endif
