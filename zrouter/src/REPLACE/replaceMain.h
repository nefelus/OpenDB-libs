#ifndef ADS_REPLACEMAIN_H
#define ADS_REPLACEMAIN_H

#ifndef ADS_H
//#include "ads.h"
#include <ads.h>
#endif

#include "db.h"
BEGIN_NAMESPACE_ADS
class replaceMain
{
private:
	const char * _test_input;
	int _test_val;

	dbDatabase *_db;
	
public:
	replaceMain(dbDatabase *db);
	dbBlock* GetTopBlock();

	void test_init(const char *);
	void init_val(int v);
	void dump_init();

	int place();
	bool initPlace();
	bool inputDesign();
	bool Dump(FILE *);
	void init();
	void initArgument(int, char **);
	void calcTSVweight();
	void initialPlacement_main();
};

END_NAMESPACE_ADS
#endif
