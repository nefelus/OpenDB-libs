#include "qtapp.h"


QtApp::QtApp() 
{ 

    counter *c;

    //for Debugging only..
    //int x=1; char *y="-sync";

    int x=0; char *y=NULL;
    app = new QApplication(x,&y);

    _racnt = -1;

    //dlist.setAuto(0);

    images.setAuto(0);
    //ilist.setAuto(0);

    datasets.setAuto(0);
    curDS = new DataSet("default");
    c = datasets.getTagId("default");
    c->data = (void*)curDS;

    cmaps.setAuto(0);
    curCM = new ColorMap("default");
    c = cmaps.getTagId("default");
    c->data = (void*)curCM;

    // initialize default tags for uncommon values
    tags.getTagId("tags","")->status = 1;
    //tags.getTagId("marks","")->status = 1;

    for(int i=0; i<MAX_RA; i++)
    {
        ra[i] = NULL;
    }

};

QtApp::~QtApp() 
{ 
#ifdef DEBUG_MEM
    fprintf(stderr,"QtApp destructor!\n");
#endif
    // get rid of tag entries in datasets so destructor works

    tmEraser<DataSet> ter;
    ter.eraseTM(datasets);
    //ter.eraseTM(dlist);

    tmEraser<ColorMap> tec;
    tec.eraseTM(cmaps);

    tmEraser<Image> tei;
    tei.eraseTM(images);
    //tei.eraseTM(ilist);

    for(int i=0; i<MAX_RA; i++)
    {
        if( ra[i] != NULL )
        {
            delete ra[i];
        }
    }
};

