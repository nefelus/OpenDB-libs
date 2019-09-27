/* Copyright Athena Design Systems 2003 */

#ifdef _WIN32
#include "direct.h"
#endif

#include "layout.h"
#include "z2tcl.h"


void Ath__zroute_init(uint layerCnt, uint masterCnt);
int Ath__readLef(char *fileName);
int Ath__readLefMacros(char *fileName);
int Ath__readDef(char *fileName);
int Ath__readDefTiles(char *fileName);
int Ath__readDefBlocks(char *fileName);
int Ath__makeTiles(char *list, int rows, int cols, char* topH, char *topV);
int Ath__zui_get(char *filter);
uint Ath__getTclBoxCnt();
char *Ath__getTclBox(int ii);
uint Ath__getTclBoxCntV();
char *Ath__getTclBoxV(uint ii);
int Ath__assign_tracks(char *list, char* topH, char *topV);
int Ath__chip_get(char *bb, char *filter);
int Ath__tiles_get(char *bb);

int Ath__chipAttr_get(int *x1, int *y1, int *x2, int *y2, char *layers,
					  char *objects, char *hierarchy);

int Ath__navigate(char *tileName, char *bb);

uint Ath__print_routing_layers();

uint Ath__pins_assign(char *blkName, char *tile, int connType, int priority, int style, 
		int space, int space_units, int width, int width_units, int length, int length_units,
		char *top_h_layer, char *top_v_layer);

uint Ath__bus_make(char *blkName, char *tile, int opt, int width, int space);
uint Ath__db_write(char *blkName, char *dir, int format);
uint Ath__db_read(char *blkName, char *dir, int format);

uint Ath__tiles_route(char *blkName, char *tile);

