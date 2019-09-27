#include <stdlib.h>

#include "dpt.h"
#include <time.h>
#include <unistd.h>

USING_NAMESPACE_ADS;

int main(int argc,char *argv[]) {
	dpt *d = new dpt();
	int x0,y0,x1,y1;
	int w,h,s,slice,rgb;
	char line[1024];
	int n = 0;
	time_t stime;
	time_t etime;

	time(&stime);

    FILE *input;
    if( argc <= 1 )
    {
        input = stdin;
    }
    else
    {
        input = fopen(argv[1],"r");
    }

	// process input for the following commands:
    int format = DPT_PPM;
    int enc = 255;
	while (fgets(line,sizeof(line),input)) {
		switch (line[0]) {
            case '#':
                // comment
                continue;
			case 'c': 
				// c width height x0 y0 x1 y1 slices
				if (sscanf(line,"c %d %d %d %d %d %d %d",
					&w,&h,&x0,&y0,&x1,&y1,&s)!=7) {
					assert(0);
				}
                //fprintf(stderr,"Calling configure with: %d, %d, %d, %d, %d, %d, %d\n",w,h,x0,y0,x1,y1,s);
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
				sscanf(line,"w %d %d",&format,&enc);
                //d->create_image(stdout,NULL,DPT_FILE,255,format);
                d->create_image(stdout,NULL,DPT_FILE,enc,format);
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
