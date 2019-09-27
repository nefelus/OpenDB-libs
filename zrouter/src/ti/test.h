#include "tm.h"

BEGIN_NAMESPACE_ADS

class Test : public ZTechModule<Test>
{
public:
    bool vchange( bool v ) { return true; }
    bool var_bool;
    int var_int;
    double var_float;
    dbObject * var_dbobject;
    ZObject * var_zobject;
    std::string var_string;
    Test( ZArgs * in, ZArgs * out ) : ZTechModule<Test>(in, out) {}
    ~Test() {}
    int m1( ZArgs * in, ZArgs * out ) { return 0; }
    int m2( ZArgs * in, ZArgs * out ) { return 0;}
    int m3( ZArgs * in, ZArgs * out ) { return 0;}
    int m4( ZArgs * in, ZArgs * out ) { return 0;}
    int m5( ZArgs * in, ZArgs * out ) { return 0;}
};

END_NAMESPACE_ADS
