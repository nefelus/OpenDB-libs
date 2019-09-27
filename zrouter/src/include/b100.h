/* Utility to convert numbers to base-100 for efficient transfer across
** the network (for zui)
** Author: Mattias Hembruch
** Copyright: Athena Design Systems, 2006
*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>

#define B100_ESCAPE 27
#define B100_START 28
#define B100_END 127
#define B100_TERM B100_START+128
#define B100_ZERO '0'
#define B100_MINUS 26

void b100(int num, FILE *fp);
char *unb100(char *enc, int *resout);

