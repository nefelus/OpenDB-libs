#ifndef ADS_DB_MATRIX_H
#define ADS_DB_MATRIX_H

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
class dbDiff;

template <class T>
class dbMatrix
{
  public:
    uint _n;
    uint _m;
    std::vector< std::vector<T> > _matrix;
    
    dbMatrix();
    dbMatrix(uint n, uint m);
    void resize(uint n, uint m);
    uint numRows() const { return _n; }
    uint numCols() const { return _m; }
    void clear();
    uint numElems() const;
   
    dbMatrix(const dbMatrix<T> & incpy)
        : _n(incpy._n),
          _m(incpy._m),
          _matrix(incpy._matrix)
    {
    }
    
    dbMatrix<T> & operator=( const dbMatrix<T> & incpy )
    {
        _n = incpy._n;
        _m = incpy._m;
        _matrix.clear();
        _matrix = incpy._matrix;
	return *this;
    }
    const T & operator()(uint i, uint j) const;
    T & operator()(uint i, uint j);

    int operator==( const dbMatrix<T> & rhs ) const;
    int operator!=( const dbMatrix<T> & rhs ) const { return ! operator==(rhs); }
    void differences( dbDiff & diff, const char * field, const dbMatrix<T> & rhs ) const;
    void out( dbDiff & diff, char side, const char * field ) const;
};

template <class T>
inline dbOStream & operator<<( dbOStream & stream, const dbMatrix<T> & mat )
{
    stream << mat._n;
    stream << mat._m;
    
    uint i, j;

    for( i = 0; i < mat._n; ++i )
    {
        for( j = 0; j < mat._m; ++j )
        {
            const T & x = mat(i,j);
            stream << x;
        }
    }
    
    return stream;
}

template <class T>
inline dbIStream & operator>>( dbIStream & stream, dbMatrix<T> & mat )
{
    uint n;
    uint m;

    stream >> n;
    stream >> m;
    mat.resize(n,m);

    uint i, j;

    for( i = 0; i < mat._n; ++i )
    {
        for( j = 0; j < mat._m; ++j )
        {
            T & x = mat(i,j);
            stream >> x;
        }
    }
    
    return stream;
}

template <class T>
inline dbMatrix<T>::dbMatrix()
{
    _n = 0;
    _m = 0;
}

template <class T>
inline dbMatrix<T>::dbMatrix(uint n, uint m)
{
    _n = n;
    _m = m;
    resize(n,m);
}

template <class T>
inline void dbMatrix<T>::clear()
{
    _n = _m = 0;
    _matrix.clear();
}

template <class T>
inline uint dbMatrix<T>::numElems() const
{
    return (_n * _m);
}

template <class T>
inline void dbMatrix<T>::resize(uint n, uint m)
{
    uint i;

    _n = n;
    _m = m;
    _matrix.resize(n);
    
    for( i = 0; i < _n; ++i )
        _matrix[i].resize(m);
}

template <class T>
inline const T & dbMatrix<T>::operator()(uint i, uint j) const
{
    assert( (i >= 0) && (i < _n) && (j >= 0) && (j < _m) );
    const std::vector<T> & v = _matrix[i];
    return v[j];
}

template <class T>
inline T & dbMatrix<T>::operator()(uint i, uint j)
{
    assert( (i >= 0) && (i < _n) && (j >= 0) && (j < _m) );
    std::vector<T> & v = _matrix[i];
    return v[j];
}

template <class T>
inline int dbMatrix<T>::operator==( const dbMatrix<T> & rhs ) const
{
    return _matrix == rhs._matrix;
}

template <class T>
inline void dbMatrix<T>::differences( dbDiff & diff, const char * field, const dbMatrix<T> & rhs ) const
{
    uint i, j;

    for( i = 0; i < _n && i < rhs._n; ++i )
    {
        for( j = 0; j < _m && j < rhs._m; ++j )
        {
            const T & o1 = (*this)(i,j);
            const T & o2 = rhs(i,j);

            if ( o1 != o2 )
            {
                diff.report("< %s[%d][%d] = ", field, i, j );
                diff << o1;
                diff << "\n";
                diff.report("> %s[%d][%d] = ", field, i, j );
                diff << o2;
                diff << "\n";
            }
        }

        if ( j < _m )
        {
            for( j = 0; j < _m; ++j )
            {
                const T & o1 = (*this)(i,j);

                diff.report("< %s[%d][%d] = ", field, i, j );
                diff << o1;
                diff << "\n";
            }
        }

        if ( j < rhs._m )
        {
            for( j = 0; j < rhs._m; ++j )
            {
                const T & o2 = rhs(i,j);
                diff.report("> %s[%d][%d] = ", field, i, j );
                diff << o2;
                diff << "\n";
            }
        }
    }

    if ( i < _n )
    {
        for( ; i < _n; ++i )
        {
            for( j = 0; j < _m; ++j )
            {
                const T & o1 = (*this)(i,j);
                diff.report("< %s[%d][%d] = ", field, i, j );
                diff << o1;
                diff << "\n";
            }
        }
    }
    
    if ( i < rhs._n )
    {
        for( ; i < rhs._n; ++i )
        {
            for( j = 0; j < rhs._m; ++j )
            {
                const T & o2 = rhs(i,j);
                diff.report("> %s[%d][%d] = ", field, i, j );
                diff << o2;
                diff << "\n";
            }
        }
    }
}

template <class T>
inline void dbMatrix<T>::out( dbDiff & diff, char side, const char * field ) const
{
    uint i, j;

    for( i = 0; i < _n; ++i )
    {
        for( j = 0; j < _m; ++j )
        {
            const T & o1 = (*this)(i,j);
            diff.report("%c %s[%d][%d] = ", side, field, i, j );
            diff << o1;
            diff << "\n";
        }
    }
}

END_NAMESPACE_ADS

#endif
