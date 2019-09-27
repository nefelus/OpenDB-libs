//Data Set - holds everything related to a set of objects
#ifndef _DATASET_H
#define _DATASET_H

#include <tcl.h>
#include "debug.h"
#include "globals.h"
#include "renderbox.h"
#include "limits.h"

extern void pool_free(RenderBox *rb);

#define AUTO_DELETE false
class DataSet
{
    private:
        char *_name;

        void init_lists();

        void free_layer(QPtrListIterator<RenderBox> *it);
        void free_lists();

        void init(char *name);

        void checkBBox(RenderBox *rb);

        void checkPolyBBox(RenderBox *rb);

        void appendItems(Tcl_Interp *interp, Tcl_Obj *list, 
                QPtrListIterator<RenderBox> *it, int poly);
        // One per dataset

        long _boxcnt;
        long _swirecnt;
        long _pviacnt;
        long _sviacnt;
        long _polycnt;

        QPtrList<RenderBox> ** _boxv;
        QPtrList<RenderBox> ** _swirev;
        QPtrList<RenderBox> ** _pviav;
        QPtrList<RenderBox> ** _sviav;
        QPtrList<RenderBox> ** _polyv;

        long _x0;
        long _y0;
        long _x1;
        long _y1;
        
        RenderBox _bbox;

        int objectCount;
        int startIdx;
        int endIdx;

    public:

        DataSet(char *name) ;
        DataSet();


        ~DataSet();

        char *getName() { return _name; }

        void incBoxcnt() { _boxcnt++; }
        int getBoxcnt() { return _boxcnt; }

        void incSWirecnt() { _swirecnt++; }
        int getSWirecnt() { return _swirecnt; }

        void incPViacnt() { _pviacnt++; }
        int getPViacnt() { return _pviacnt; }

        void incSViacnt() { _sviacnt++; }
        int getSViacnt() { return _sviacnt; }

        void incPolycnt() { _polycnt++; }
        int getPolycnt() { return _polycnt; }

        void addBox(int layer, RenderBox *rb);
        void addSWire(int layer, RenderBox *rb);
        void addPVia(int layer, RenderBox *rb);
        void addSVia(int layer, RenderBox *rb);
        void addPoly(int layer, RenderBox *rb);

        void prePVia(int layer, RenderBox *rb);
        void preSVia(int layer, RenderBox *rb);

        QPtrListIterator<RenderBox> *getBoxIt(int layer);
        QPtrListIterator<RenderBox> *getSWireIt(int layer);
        QPtrListIterator<RenderBox> *getPViaIt(int layer);
        QPtrListIterator<RenderBox> *getSViaIt(int layer);
        QPtrListIterator<RenderBox> *getPolyIt(int layer);

        RenderBox *getBBox();

        void getBBox(char *buff);

        int getObjectCount();

        Tcl_Obj *getDataSet(Tcl_Interp *interp);

        Tcl_Obj *getDataRange(Tcl_Interp *interp, int start, int end);


};

#endif
