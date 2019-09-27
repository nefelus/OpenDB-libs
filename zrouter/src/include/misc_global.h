/* Copyright Athena Design Systems 2003 */

#ifndef MISC_GLOBAL_H
#define MISC_GLOBAL_H

#include <string.h>

void Ath__allocFailure(char *msg);
void Ath__hashError(char *msg, int exitFlag);

void ATH__failMessage(char *msg);

char* ATH__allocCharWord(int n);
void ATH__deallocCharWord(char *a);

FILE* ATH__openFile(const char *name, const char* type);

void ATH__closeFile(FILE *fp);


#include "ads.h"
BEGIN_NAMESPACE_ADS
void start_timer();
void end_timer(char *msg);
void start_htimer();
void end_htimer(char *msg);
char *replace_string(const char *start, char search, const char *replace);
END_NAMESPACE_ADS

#endif
