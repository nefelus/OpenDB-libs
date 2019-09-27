#ifndef _QTAPP_H
#define _QTAPP_H

#include <qapplication.h>
#include <stdio.h>
#include "debug.h"
#include "globals.h"
#include "tagManager.h"
#include "dataset.h"
#include "colormap.h"
#include "renderarea.h"
#include "parse.h"
#include "layerset.h"
#include "image.h"


class QtApp {

    public:
        QApplication *app;
        Tcl_Interp *interp;
        RenderArea *ra[MAX_RA];
        int _racnt;

        tagManager datasets; // for top-level datasets 
        tagManager cmaps; // for colormaps
        tagManager tags;  // for global tags
        //tagManager dlist; // for global display list

        tagManager images; // all images in the system
        //tagManager ilist; // for image display list

        DataSet *curDS;
        ColorMap *curCM;

        LayerSet ils;

        QtApp() ;
        ~QtApp() ;
};

#endif
