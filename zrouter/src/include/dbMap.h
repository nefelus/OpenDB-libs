#ifndef ADS_DB_MAP_H
#define ADS_DB_MAP_H

#include <map>
#include <vector>

#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_SET_H
#include "../db/dbSet.h"
#endif

BEGIN_NAMESPACE_ADS

///
/// This class creates a assocation between
/// the objects of dbSet<T> to a an object of type "D".
/// If "D" is a class object, then "D" must have a public default constructor
/// and a public destructor.
///
/// NOTE: The map becomes invalid if the dbSet membership changes, i.e.,
/// a member is added or deleted from the set.
///
/// If the map becomes invalid, access operations may fail.
///
template <class T, class D>
class dbMap
{
    dbSet<T>            _set;
    std::map<T *, D> *  _map;    // map used if set is not sequential
    std::vector<D> *    _vector; // vector used if set is sequential

    // Map cannot be assigned or copied!
    dbMap( const dbMap & );
    dbMap & operator=( const dbMap & );
    
  public:

    ///
    /// Create a new map from the set. The data-objects are initialized
    /// to the default constructor of the object type.
    ///
    dbMap( const dbSet<T> & set );

    ///
    /// Destructor.
    ///
    ~dbMap();

    ///
    /// D & operator[T *] const
    /// Access to a data object of key "T *".
    ///
    /// Example:
    ///
    /// dbSet<dbNet> nets = block->getNets();
    /// dbMap<dbNet, int> net_weight;
    /// dbSet<dbNet>::iterator itr;
    ///
    /// for( itr = nets.begin(); itr != nets.end(); ++itr )
    /// {
    ///     dbNet * net = *itr;
    ///     net_weight[net] = 1;
    /// }
    /// 
    D & operator[]( T * object );

    ///
    /// const D & operator[T *] const
    /// Const access to a data object of key "T *".
    ///
    /// Example:
    ///
    /// dbSet<dbNet> nets = block->getNets();
    /// dbMap<dbNet, int> net_weight;
    /// dbSet<dbNet>::iterator itr;
    ///
    /// for( itr = nets.begin(); itr != nets.end(); ++itr )
    /// {
    ///     dbNet * net = *itr;
    ///     int weight = net_weight[net];
    ///     printf("net(%d) weight = %d\n", net->getId(), weight );
    /// }
    /// 
    const D & operator[]( T * object ) const;
};

END_NAMESPACE_ADS

#ifndef ADS_DB_MAP_HPP
#include "../db/dbMap.hpp"
#endif

#endif
