#ifndef ADS_DPT_H
#define ADS_DPT_H

#ifndef ADS_H
#include "ads.h"
#endif

BEGIN_NAMESPACE_ADS

typedef char dptPixel;

#define MAXCOLORSLICES 20

class dpt
{
  public:
    dpt();
    ~dpt();

    int configure(int width,int height,
		int x0, int y0, int x1, int y1,int nslices);

	// render a rectangle
	int box(int x0, int y0, int x1, int y1,int slice);

	// write slices as a pbm file
	int write_pbm(FILE*fp,int sl=0);

	// write all slices as a series of PBM files
	int write(FILE*fp);

	// colorize a slice
	int colorize(int slice,int rgb);

	// write all slices as a series of PBM files
	int write_image(FILE*fp);

    void dump_color();
	
  protected:
    dptPixel** slice;            // pixmaps
    int    width;
    int    height;
    int    nslices;              // max number of slices
    int    maxsll;
	int x0,x1,y0,y1;          // bounding box
	double sx,sy;                // scale factors
	int    *color;               // color table
    int     csize;              // size of the color table

    struct rgb
    {
        char out[4];
    };

    struct rgb * cl2;

  protected:
	inline int R_COLOR(unsigned int p) { return 0xff & ((p & 0xff0000)>>16); }
	inline int G_COLOR(unsigned int p) { return 0xff & ((p & 0x00ff00)>>8); }
	inline int B_COLOR(unsigned int p) { return 0xff & ((p & 0x0000ff)   ); }

	// find the highest slice of each pixel
	int top_slice(int x, int y); 
};

END_NAMESPACE_ADS

#endif
