#include <stdio.h>
#include "db.h"

USING_NAMESPACE_ADS;

extern void print_db( dbDatabase * db );

int main( int argc, char ** argv )
{
    if ( argc != 2 )
    {
        printf("usage: dump db\n");
        return 1;
    }

    // Create a database
    dbDatabase * db = dbDatabase::open( argv[1], dbRestore );
    print_db(db);
    dbDatabase::close(db);
    return 0;
}

