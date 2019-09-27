#ifndef ADS_H
#include "ads.h"
#endif

#ifndef ADS_DB_H
#include "db.h"
#endif

BEGIN_NAMESPACE_ADS

void create_box( dbSWire * wire, 
                 dbWireShapeType type,
                 dbTechLayer * layer,
                 int prev_x, int prev_y, int prev_ext, bool has_prev_ext,
                 int cur_x, int cur_y, int cur_ext, bool has_cur_ext,
                 int width );

dbTechLayer * create_via_array( dbSWire * wire,
                                dbWireShapeType type,
                                dbTechLayer *layer,
                                dbTechVia * via,
                                int orig_x, int orig_y,
                                int numX, int numY, int stepX, int stepY );

dbTechLayer * create_via_array( dbSWire * wire,
                                dbWireShapeType type,
                                dbTechLayer *layer,
                                dbVia * via,
                                int orig_x, int orig_y,
                                int numX, int numY, int stepX, int stepY );

END_NAMESPACE_ADS
