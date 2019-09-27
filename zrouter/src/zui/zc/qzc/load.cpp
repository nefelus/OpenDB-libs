#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
main(int argc, char *argv[])
{
    void *handle;
    //handle = dlopen("./tkqt.so", RTLD_NOW);
    handle = dlopen(argv[1], RTLD_NOW);
    if (!handle)
    {
        fprintf(stderr,"Error: %s\n",dlerror());
        exit(1);
    }
}
