#include "asa.h"

using namespace ads;

BEGIN_NAMESPACE_ADS


static char _ath_logbuffer[1024];

void error(int code, const char *msg, ...)
{
    va_list args;
    va_start(args,msg);

    vsnprintf(_ath_logbuffer,sizeof(_ath_logbuffer),msg,args);
    va_end(args);

    fprintf(stderr,"Error: error %d: %s\n",code,_ath_logbuffer);
}


END_NAMESPACE_ADS

int main(int argc, char *argv[])
{

    dcrASA asa;

    FILE *f = fopen("asa.out","w");
    asa.setOutput(f);

    char buff[128];
    for(int i=0;i <100; i++)
    {
        sprintf(buff,"%d",i);
        asa.append(buff);
    }


    asa.setList(true);
    for(int i=100;i <200; i++)
    {
        sprintf(buff,"{%d} {%d} {%d %d}\n",i,i,i,i);
        asa.append(buff);
    }

    fclose(f);

    return 0;

}
