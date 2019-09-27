#ifndef ADS_CRAWLER_H
#define ADS_CRAWLER_H

#ifndef ADS_TM_H
#include "tm.h"
#endif

#include "IZdcr.h"
#include "IZgui.h"

#include "dpt.h"

#define TCL_METHOD(name) int name( ZArgs * in, ZArgs * out )

BEGIN_NAMESPACE_ADS

class Dcr : public ZTechModule<Dcr>
{
	private:
		
		ZPtr<IZdcr> _dcr;
        	dpt *_zptPtr;

		
	public:
		Dcr( ZArgs * in, ZArgs * out );
		~Dcr();
		
		TCL_METHOD ( init ) ;
		TCL_METHOD ( get ) ;
		TCL_METHOD ( attach ) ;
		TCL_METHOD ( chip_get ) ;
		TCL_METHOD ( tiles_get ) ;
		TCL_METHOD ( chip_attr_get ) ;
		TCL_METHOD ( inspect ) ;
		TCL_METHOD ( add_menu ) ;
		TCL_METHOD ( add_sub_menu ) ;
		TCL_METHOD ( config ) ;
		TCL_METHOD ( clear ) ;
		TCL_METHOD ( paint ) ;
		TCL_METHOD ( image ) ;
		TCL_METHOD ( colors ) ;
		TCL_METHOD ( bc_get ) ;
		TCL_METHOD ( test_milos ) ;
		TCL_METHOD ( test_ade ) ;

		ZPtr<IZdcr> getDcr() { return _dcr; }
//		void setGui(ZPtr<IZgui> z) { _gui= z; }

	private:
		// put some here
		
};

END_NAMESPACE_ADS
#endif
