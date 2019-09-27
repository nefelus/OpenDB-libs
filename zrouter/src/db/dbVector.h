#ifndef ADS_DB_VECTOR_H
#define ADS_DB_VECTOR_H

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_STREAM_H
#include "dbStream.h"
#endif

#ifndef ADS_DB_DIFF_H
#include "dbDiff.h"
#endif

#include <vector>

BEGIN_NAMESPACE_ADS

template <class T>
class dbVector : public std::vector<T> 
{
  public:
    typedef typename std::vector<T>::iterator iterator;
    typedef typename std::vector<T>::const_iterator const_iterator;
    typedef std::vector<T> _base;

    dbVector<T> & operator=( const std::vector<T> & v )
    {
        if ( this != &v )
            *(std::vector<T> *) this = v;

        return *this;
    }

    dbVector() {}

    dbVector( const dbVector<T> & v ) : std::vector<T>(v)
    {
    }

    ~dbVector() {}

    iterator begin() { return _base::begin(); }
    iterator end() { return _base::end(); }
    const_iterator begin() const { return _base::begin(); }
    const_iterator end() const { return _base::end(); }
    void differences( dbDiff & diff, const char * field, const dbVector<T> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

#ifndef WIN32
template <class T>
class dbVector<T *> : public std::vector<T *> 
{
  public:
    typedef typename std::vector<T*>::iterator iterator;
    typedef typename std::vector<T*>::const_iterator const_iterator;
    typedef std::vector<T*> _base;

    dbVector<T*> & operator=( const std::vector<T*> & v )
    {
        if ( this != &v )
            *(std::vector<T> *) this = v;

        return *this;
    }

    dbVector() {}

    dbVector( const dbVector<T*> & v ) : std::vector<T*>(v)
    {
    }

    ~dbVector() {}

    iterator begin() { return _base::begin(); }
    iterator end() { return _base::end(); }
    const_iterator begin() const { return _base::begin(); }
    const_iterator end() const { return _base::end(); }

    int operator==( const dbVector<T*> & rhs ) const 
    { 
        if ( _base::size() != rhs.size() )
            return false;

        typename dbVector<T *>::const_iterator i1 = begin();
        typename dbVector<T *>::const_iterator i2 = rhs.begin();
    
        for( ; i1 != end(); ++i1, ++i2 )
            if ( **i1 != **i2 )
                return false;

        return true;
    }

    int operator!=( const dbVector<T*> & rhs ) const { return ! this->operator==(rhs); }

    void differences( dbDiff & diff, const char * field, const dbVector<T *> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};
#endif

template <class T>
inline dbOStream & operator<<( dbOStream & stream, const dbVector<T> & v )
{
    unsigned int sz = v.size();
    stream << sz;

    typename dbVector<T>::const_iterator itr;

    for( itr = v.begin(); itr != v.end(); ++itr )
    {
        const T & value = *itr;
        stream << value;
    }

    return stream;
}

template <class T>
inline dbIStream & operator>>( dbIStream & stream, dbVector<T> & v )
{
    v.clear();
    unsigned int sz;
    stream >> sz;
    v.reserve(sz);

    T t;
    unsigned int i;
    for( i = 0; i < sz; ++i )
    {
        stream >> t;
        v.push_back(t);
    }

    return stream;
}

template <class T>
inline void dbVector<T>::differences( dbDiff & diff, const char * field, const dbVector<T> & rhs ) const
{
    const_iterator i1 = begin();
    const_iterator i2 = rhs.begin();
    unsigned int i = 0;

    for( ; i1 != end() && i2 != rhs.end(); ++i1, ++i2, ++i )
    {
        if ( *i1 != *i2 )
        {
            diff.report("< %s[%d] = ", field, i );
            diff << *i1;
            diff << "\n";
            diff.report("> %s[%d] = ", field, i );
            diff << *i2;
            diff << "\n";
        }
    }

    for( ; i1 != end() ; ++i1, ++i )
    {
        diff.report("< %s[%d] = ", field, i );
        diff << *i1;
        diff << "\n";
    }
    
    for( ; i2 != rhs.end(); ++i2, ++i )
    {
        diff.report("> %s[%d] = ", field, i );
        diff << *i2;
        diff << "\n";
    }
}


template <class T>
inline void dbVector<T>::out( dbDiff & diff, char side, const char * field ) const
{
    const_iterator i1 = begin();
    unsigned int i = 0;

    for( ; i1 != end() ; ++i1, ++i )
    {
        diff.report("%c %s[%d] = ", side, field, i );
        diff << *i1;
        diff << "\n";
    }
}

#ifndef WIN32
template <class T>
inline void dbVector<T*>::differences( dbDiff & diff, const char * field, const dbVector<T *> & rhs ) const
{
    const_iterator i1 = begin();
    const_iterator i2 = rhs.begin();
    unsigned int i = 0;

    for( ; i1 != end() && i2 != rhs.end(); ++i1, ++i2, ++i )
    {
        if ( *i1 != *i2 )
        {
            diff.report("<> %s[%d]:\n", field, i );
            (*i1)->differences( diff, NULL, *(*i2) );
        }
    }

    for( ; i1 != end() ; ++i1, ++i )
    {
        diff.report("< %s[%d]:\n", field, i );
        (*i1)->out( diff, dbDiff::LEFT, NULL );
    }
    
    for( ; i2 != rhs.end(); ++i2, ++i )
    {
        diff.report("> %s[%d]:\n", field, i );
        (*i1)->out( diff, dbDiff::RIGHT, NULL );
    }
}

template <class T>
inline void dbVector<T*>::out( dbDiff & diff, char side, const char * field ) const
{
    const_iterator i1 = begin();
    unsigned int i = 0;

    for( ; i1 != end() ; ++i1, ++i )
    {
        diff.report("%c %s[%d]:\n", side, field, i );
        (*i1)->out(diff,side, NULL);
    }
}

#endif

END_NAMESPACE_ADS

#endif
