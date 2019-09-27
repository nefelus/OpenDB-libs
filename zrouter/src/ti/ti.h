#include <list>
#include <string>

struct Module;
struct Method;
struct Arg;
struct Var;

struct Group;

enum ArgType
{
    INT_ARG,
    FLOAT_ARG,
    STRING_ARG,
    BOOL_ARG,
    DBOBJECT_ARG,
    ZOBJECT_ARG,
    ICHANNEL_ARG,
    OCHANNEL_ARG,
    RECT_ARG,
    POINT_ARG
};

struct Module
{
    const char *        name;
    const char *        zname;
    std::list<Method *> methods;
    std::list<Var *>    vars;
    Method *            constructor;
    std::list<std::string> doc;

    Module( const char * n, const char * zn )
    {
        name = n;
        zname = zn;
        constructor = NULL;
    }
};

struct Member
{
    std::list<std::string> doc;
};

struct Method : public Member
{
    const char *       name;
    const char *       zcmd;
    std::list<Arg *>   args;
    Method( const char * n, const char * c ) { name = n; zcmd = c; }
};

struct Arg
{
    bool               list;
    bool               input;
    ArgType            type;
    const char *       name;
    const char *       value;
    const char *       usage;

    Arg( bool i, bool l, ArgType t, const char * n )
    {
        list = l;
        input = i;
        type = t;
        name = n;
        value = NULL;
        usage = NULL;
    }
};

struct Var : public Member
{
    bool               list;
    ArgType            type;
    const char *       name;
    const char *       usage;
    const char *       notify;

    Var( bool l, ArgType t, const char * n )
    {
        list = l;
        type = t;
        name = n;
        usage = NULL;
        notify = NULL;
    }
};

struct ArgValue
{
    int          type;
    const char * value;
};

extern std::list<Module *> modules;
extern std::list<const char *> ccode;
extern std::list<std::string> doc;

