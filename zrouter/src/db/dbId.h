#ifndef ADS_DB_ID_H
#define ADS_DB_ID_H

#ifndef ADS_DB_STREAM_H
#include "dbStream.h"
#endif

BEGIN_NAMESPACE_ADS

class dbIdValidation {
   public:
   static uint invalidId()  { return 0; }
   static bool isId(const char *inid);
};

//
// April 2006 twg -
//
//     I removed the inheritance of empty class dbIdValidation from the
//     inheritance list of dbId. The "C++" standard does not enforce an empty class
//     to have zero bytes. It is up to the compiler to optimize the empty class.
//
//     If the compiler does NOT optimize the empty class, then database index will
//     increase from 32-bits to 64-bits! (the compiler will align the "_id" field
//     to a 32-bit boundary.)
//
template <class T>
class dbId
{
    unsigned int _id;

  public:
    typedef T _type;
    
    dbId() { _id = 0; }
    dbId( const dbId<T> & id ) : _id(id._id) { }
    dbId( register unsigned int id ) { _id = id; }
    
    operator unsigned int () const { return _id; }
    unsigned int & id() { return _id; }

    bool isValid() { return _id > 0; }

    friend dbOStream & operator<<( dbOStream & stream, const dbId<T> & id )
    {
        stream << id._id;
        return stream;
    }
    
    friend dbIStream & operator>>( dbIStream & stream, dbId<T> & id )
    {
        stream >> id._id;
        return stream;
    }
};

END_NAMESPACE_ADS

#endif
