#define DEBUG_MEM 1
#include "tagManager.h"

main()
{
    for(int i=0 ;i<100; i++)
    {
        tagManager *tm = new tagManager();

        tm->getTagId("signal");
        tm->getTagId("siGnal","swire");
        tm->getTagId("sIgnal","spin");
        tm->dump();
        int idx = (tm->getTagId("power","pwire"))->id;
        int idx2 = (tm->getTagId("Power","ppin"))->id;

        //tm->dump();

        //printf("Erasing: %d!\n",idx);
        //tm.erase("power","ppin");
        //tm.erase("power",idx);

        tm->erase("power");
        tm->dump();

        tm->getTagId("power","Pnew!");
        tm->dump();
        delete tm;
    }
}
