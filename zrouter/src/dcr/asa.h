#ifndef ADS_dcrASA_H
#define ADS_dcrASA_H

#include "tm.h"

#ifndef ADS_H
#include "ads.h"
#endif

#include "tcl.h"
#include "logger.h"
#include "b100.h"

//BEGIN_NAMESPACE_ADS

#define ASA_LAYER 17
#define ASA_HIER 18
#define ASA_TYPE 19
#define ASA_EQ 20

class dcrASA
{
    public:
        dcrASA();
        ~dcrASA();

        void setOutput(FILE *fp);
        void setOutput(Tcl_Interp *in);

        void append(char *x);
        void appendRecord(char *id, int layer, char *hierarchy, char *type, 
            int x0, int y0, int x1, int y1, char *special);
        void appendRecord(char *id, int x0, int y0, int x1, int y1, char *special);
        void output();

        void setList(bool type);

        void clear();
        void reset();

        void setLayer(int layer);
        void setHier(char *hier);
        void setType(char *type);

    private:

        typedef enum { UNKNOWN, INTERP, CHANNEL } DEST_TYPE;
        char fbuff[65536];
        char pbuff[65536];

        DEST_TYPE _dest;

        Tcl_Interp *_interp;
        FILE *_fp;
        bool _ls; // list style

        char *toList(char *x);
        void appendChar(char *x);
};

//END_NAMESPACE_ADS

#endif
