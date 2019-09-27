#include "dataset.h"
#include "parse.h"
#include "tcl.h"

main(int argc, char *argv[])
{

    DataSet *ds = new DataSet("default");
    tagManager tm;
    LayerSet ls;

    FILE *fp = fopen("set1.cmd","r");

    char buff[1024];
    char *line;
    line = buff+1;
    fgets(buff,1023,fp);
    int lc=0;
    while(!feof(fp) )
    {
        if( strlen(buff) <= 1 )
            break;
        buff[strlen(buff)-1]='\0';
        //printf("Read line: %s\n",buff);
        addObject(line, ds, tm, ls);
        lc++;
        fgets(buff,1023,fp);
    }
    fclose(fp);
    printf("Parsed %d items!\n",lc);

    delete ds;


}
