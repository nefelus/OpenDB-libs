#include "colormap.h"

main(int argc, char *argv[])
{
    ColorMap *cm = new ColorMap("default");

    for( int i=0; i<1000; i++)
    {
        cm->setBrush(0,"mediumseagreen");
        cm->setPen(1,"lawngreen");

        cm->setBrush(MAX_LAYER-1,"mediumseagreen");
        cm->setPen(MAX_LAYER-1,"lawngreen");
    }

    delete cm;


}
