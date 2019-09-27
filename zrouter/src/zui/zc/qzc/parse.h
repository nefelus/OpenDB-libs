#ifndef _PARSE_H
#define _PARSE_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "tcl.h"
#include "globals.h"
#include "debug.h"
#include "tagManager.h"
#include "layerset.h"
#include "dataset.h"
#include "b100.h"


char * skiptokval(char *s);
void readList(char *list, DataSet *ds, tagManager &tm, LayerSet &ls);
void readChannel(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager& tm, LayerSet &ls);
void readStream(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager& tm, LayerSet &ls);
void readStreamz(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager& tm, LayerSet &ls);
void readStream2(char *channel, Tcl_Interp *interp, DataSet *ds, tagManager& tm, LayerSet &ls);
//void addObject(char *line, int len, DataSet *ds, tagManager& tm, LayerSet &ls);
void init_idx(tagManager &tm);
void addObject(char *line, DataSet *ds, tagManager& tm, LayerSet &ls);
void addObject2(char *line, DataSet *ds, tagManager& tm, LayerSet &ls);
void addObjectFixed(char *line, DataSet *ds, tagManager& tm, LayerSet &ls);
void addObjectFixedI(char *line, DataSet *ds, tagManager& tm, LayerSet &ls);
void addObjectFixedtest(char *line, DataSet *ds, tagManager& tm, LayerSet &ls);

int lower(int x0, int x1);

void pool_init();
RenderBox *pool_alloc();
void pool_free(RenderBox *rb);


#endif
