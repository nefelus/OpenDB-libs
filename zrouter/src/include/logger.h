#ifndef LOGGER_H 
#define LOGGER_H
#include <stdarg.h>
#include <stdio.h>
#ifndef _WIN32
#include <sys/times.h>
#endif

#include "tm.h"
#include "ads.h"

BEGIN_NAMESPACE_ADS

#define _ATH_LOGBUFLEN 1024*8
#define LAST_TIMER -1
#define NUM_TIMERS 64

class Logger : public ZInterface
{
    public:
        static Logger *getLogger();
        static void initLogger(Tcl_Interp *interp);

    private:
        static Logger *_logger;
        Logger(Tcl_Interp *interp) { _context._interp = interp; }
};

int idle(int =0);
int milos(int code, const char*msg, ...);
int ade(int code, const char*msg, ...);

int notice(int code, const char *msg, ...);
int verbose(int code, const char *msg, ...);

int info(int code, const char *msg, ...);

void dumpWarn();
int checkWarning(const char *msg);
void resetWarningCount(const char *msg, int max, int cnt);

int warning(int code, const char*msg, ...);

void error(int code, const char*msg, ...);

int fnotice(FILE* file, int code, const char *msg, ...);

int fwarning(FILE* file, int code, const char*msg, ...);

int debug(const char *mod, const char *tag, const char*msg, ...);
int isDebug(const char *mod, const char *tag);

int _new_htimer(void);
void _reset_htimer(int idx);
void _get_htimer(int idx, int& sdiff, int& ndiff);
void _show_htimer(char *msg, char *mod, char *tag, int idx);

END_NAMESPACE_ADS

#endif
