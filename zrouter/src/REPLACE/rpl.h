#ifndef ADS_RPL_H
#define ADS_RPL_H

#ifndef ADS_TM_H
#include <tm.h>
#endif
#include <util.h>
// #include "extRCap.h"
// #include "exttree.h"

#include <logger.h>

#include "replaceMain.h"

#define TCL_METHOD(name) int name( ZArgs * in, ZArgs * out )

BEGIN_NAMESPACE_ADS

class RPL : public ZTechModule<RPL>
{
public:
	RPL(ZArgs* in, ZArgs* out);
	~RPL();

	TCL_METHOD( test_cmd );
	TCL_METHOD( test_init );
	TCL_METHOD( dump_init );
/*************************************** place functions *************/
	TCL_METHOD( init_place );
	TCL_METHOD( read_db );
	TCL_METHOD( dump );
	
	

private:
	replaceMain* _main;

};

END_NAMESPACE_ADS
#endif
