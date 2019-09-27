#include "replaceMain.h"

//#include "logger.h"
#include <logger.h>

BEGIN_NAMESPACE_ADS

replaceMain::replaceMain(dbDatabase *db)
{
	_db= db;
//db->getChip()->getBlock();
};
dbBlock* replaceMain::GetTopBlock()
{
	return _db->getChip()->getBlock();
};
void replaceMain::test_init(const char *v)
{
	_test_input= strdup(v);
	printf("Init: %s\n", _test_input);
};
void replaceMain::init_val(int v)
{
	_test_val= v;
	printf("Init val: %d\n", _test_val);
};
void replaceMain::dump_init()
{
	printf("dump_init_msg: %s\n", _test_input);
	printf("dump_init_val: %d\n", _test_val);
};
END_NAMESPACE_ADS
