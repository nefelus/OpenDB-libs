//Layer Set 
#ifndef _LAYERSET_H
#define _LAYERSET_H

#include "debug.h"
#include "globals.h"

#include "renderbox.h"


class LayerSet
{
    private:
        char *_name;

        void init_layers()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                _layerstatus[i] = 1;
            }
        }

        void init(char *name)
        {
            _name = strdup(name);
            //fprintf(stderr,"New LayerSet: %s!\n",name);
            _maxlayer = 0;

            init_layers();
        }

        int _maxlayer;

        int _layerstatus[MAX_LAYER];

    public:

        LayerSet(char *name) 
        {
            if( name != NULL )
            {
                init(name);
            }
            else
            {
                init("none");
            }
        }
        LayerSet()
        {
            init("none");
        }


        ~LayerSet()
        {
#ifdef DEBUG_MEM
            fprintf(stderr,"LayerSet Destructor!\n");
#endif
            free(_name);
        };

        char *getName() { return _name; }

        void setLayerStatus(int layer, int status)
        {
            //qDebug("Status for Layer Before %d = %d\n",layer, _layerstatus[layer]);
            if( layer < MAX_LAYER )
            {
                 _layerstatus[layer] = status; 
                 setMaxLayer(layer);
            }

            //qDebug("Status for Layer After %d = %d\n",layer, _layerstatus[layer]);
        }

        int getLayerStatus(int layer)
        {
            if( layer < MAX_LAYER)
            {
                return _layerstatus[layer];
            }
            return 0;
        }

        int getMaxLayer()
        {
            return _maxlayer;
        }

        void setMaxLayer(int layer)
        {
            if( layer > _maxlayer )
            {
                _maxlayer = layer;
            }
        }

        void showLayerStatus()
        {
            for(int i=0; i<MAX_LAYER; i++)
            {
                qDebug("Layer %d: %d\n",i,getLayerStatus(i));
            }
        }
};

#endif
