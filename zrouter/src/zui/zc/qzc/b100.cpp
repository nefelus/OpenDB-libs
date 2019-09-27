/* Utility to convert numbers to base-100 for efficient transfer across
** the network (for zui)
** Author: Mattias Hembruch
** Copyright: Athena Design Systems, 2006
*/

#include "b100.h"

void b100(int num, FILE *fp)
{
    int st = num;
    int d1;

    if( num == INT_MIN )
    {
        fputs("L@LK1",fp);
        return;
    }

    if ( num < 0 )
    {
        fputc(B100_MINUS,fp);
        st = -num;
    }

    while(1)
    {
        d1 =  st % 100;
        fputc(B100_START+d1,fp);
        if( (st /= 100) == 0) break;
    } 
}

char *unb100(char *enc, int *resout)
{
    char *ptr = enc;
    register int res = 0;
    int flag = 0;
    register int factor=1;
    if ( *ptr == B100_MINUS )
    {
        flag = 1;
        *ptr++;
    }
    while( *ptr >= B100_START )
    {
        res = res + (*ptr++-B100_START)*factor;
        factor *= 100;
    }
    if( flag )
    {
        res = -res;
    }
    *resout = res;

    // next position to read
    return ptr;
}

