#ifndef WORDER_H
#define WORDER_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

class dbBlock;
class dbNet;

void orderWires(dbBlock *b, bool force, int maxLrngth = 0);
void orderWires(dbBlock *b,const char *net_name_or_id,bool force, bool verbose = false, bool quiet = false, int cutLength = 0, int maxLength = 0);
void orderWires(dbNet *net, bool force, bool verbose = false);

// for the tiler
void orderWiresTableInit(dbBlock *block);
void orderWiresTableAdd(dbNet *net,dbWire *wire);
void orderWiresTableRun(dbNet *net = NULL,bool verbose = false);
void orderWiresTableFree();

void findDisconnects(dbBlock *block, bool verbose);

END_NAMESPACE_ADS

#endif

