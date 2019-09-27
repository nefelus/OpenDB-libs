#include <stdlib.h>

#include "dpt.h"
#include <time.h>

USING_NAMESPACE_ADS;

int main(int ac,char**av) {
	dpt *d = new dpt();
	int x0,y0,x1,y1;
	int w,h,s,slice,rgb;
	char line[1024];
	int n = 0;
	time_t stime;
	time_t etime;

	time(&stime);

	// process input for the following commands:
	while (fgets(line,sizeof(line),stdin)) {
		switch (line[0]) {
			case 'c': 
				// c width height x0 y0 x1 y1 slices
				if (sscanf(line,"c %d %d %d %d %d %d %d",
					&w,&h,&x0,&y0,&x1,&y1,&s)!=7) {
					assert(0);
				}
				d->configure(w,h,x0,y0,x1,y1,s);
				break;

			case 'C' : 
				// C slice hexcolor
				if (sscanf(line,"C %d %x",&slice,&rgb) != 2) {
					assert(0);
				}
				d->colorize(slice,rgb);
				break;

			case 'b' : 
				// b x0 y0 x1 y1 slice (box)
				if (sscanf(line,"b %d %d %d %d %d",&x0,&y0,&x1,&y1,&s)!=5 ){
					assert(0);
				}
				d->box(x0,y0,x1,y1,s);
				n++;
                //if ( n > 10000) exit (-1);
				break;

			case 'w' :
				// w (write image out)
				d->write_image(stdout);
				break;

			case 'W' :
				// W slice (write slice out)
				if (!sscanf(line,"W %d",&slice)) {
					assert(0);
				}
				d->write_pbm(stdout,slice);
			default:
				fprintf(stderr,"ignoring line: %s\n",line);
			break;
		}
	}
	time(&etime);
	fprintf(stderr, "dtest: rendered %4d objects @ %6.2f o/sec\n",
		n, n / (double)(etime - stime));
}
