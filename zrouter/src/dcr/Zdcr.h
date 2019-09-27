#ifndef ADS_DCR_ZDCR_H
#define ADS_DCR_ZDCR_H

#include "IZdcr.h"  // Why do you need here

#ifndef ADS_ZIMPLEMENTS_H
#include "ZImplements.h"
#endif

#ifndef ADS_GEOM_H
#include "geom.h"
#endif

class Ath__zui; // TODO: put in namespace

BEGIN_NAMESPACE_ADS

class dpt;
class IZgui;

///
/// Zdcr - Implementation for ZRoute Zui object interface
///
/// 

class Zdcr : public ZImplements<Zdcr, IZdcr>
{
private:

	
	ZPtr<IZgui> _gui[128];
	uint _guiCnt;

	Ath__zui *_zui;
	dpt *_dpt;
	char *_name;
	char *_moduleName;
	char **_nameTable;
	uint *_mapIndex;
	uint _memuMult;

	uint _menuId;
	uint _subMenuCnt;
	uint *_subMenuIdTable;
	uint *_subMenuTypeTable;

	char* makeName(const char *name);
	
public:
	Zdcr();
	~Zdcr();
	
	uint getAppGuiCnt();
	int inspect(const char *objectName, const char *selectType, const char *action, const char *chip,
		const char *bb, const char *layer, const char *hier, const char *type, FILE *outFP, bool noclip, bool write2list);
	void inspect();
	void chip_get(const char *chip, bool gridFlag, int maxobjects, FILE *outFP);
	void chipAttr(char *layers, uint n, bool def);
	void addGui(ZPtr<IZgui> z);
	// IZdcr Interface
	
	char *setName(const char *name);
	char *getName();
	uint getIndex(uint menuId, uint boxId);
	void init(ZContext & context, bool dbEnv, const char *name, const char *module, bool noStore);
	uint addSubMenu(uint menuId, char *name, uint boxId);
	uint getMenuId(char *name);
	void setBlockId(uint v);
	uint getBlockId();
	bool isBlockSelected(uint bid);

	uint initDbMenus();
	uint setupModuleMenu(const char *moduleName, const char *onechar, uint id);
	uint setInspectEnv(const char *action, const char *objectName, const char *bb, const char *selectType, bool noclip);
	void setFilters(const char *chip, const char *layer, const char *type, const char *hier);
	bool getPullDownMenu(const char *objectName);
	uint addPullDownMenu(char* menu, char* subMenu, char *db_name, char *zui_action);
	uint addPullDownMenu(uint menuId, uint subMenuId, char *db_name, char *zui_action);

	char* getInspectName();
	uint getSubmenuObjId(uint *id2);
	uint getPullDownActionId();
	bool msgAction();
	bool selectAction();
	bool isSelectedMenu(char *name);
	uint getSelectIds(uint *id1, uint *id2, uint *id3);
	bool isSubmenuType(uint id);
	char *getMsgBuffer(bool setBuffFlag);
	void print_self(char *blk, char *name, char *extra);
	void wireMsg(int x1, int y1, int x2, int y2, uint level, char *msg_buf);

	void getChipAttr(char *bbox, char *layers, char *menus, char *blocks, char *defTypes);
	uint getMenuNames(char *buff);
	void writeToTcl(uint lineLimit, FILE *fp, bool write2list);
	void getBbox(int *x1, int *y1, int *x2, int *y2);
	char *getFirstTclBox(int *x1, int *y1, int *x2, int *y2, uint *layer);
	void resetSearchParams(char *chip, const char *bb, const char *layer, 
						const char *hier, const char *type, bool noclip, bool q);

	int getCoords(const char *bb, int *x1, int *y1, int *x2, int *y2);
	void setMaxArea(int x1, int y1, int x2, int y2);
	void resetMaxArea();

	uint addBox(uint id, uint subMenuId, uint menuId, int layer, 
					  int x1, int y1,int x2, int y2, uint ownId, char *boxType=NULL);
	
	uint addArrow(bool right, uint subMenuId, uint menuId, int layer, 
						int labelCnt, char **label, double *val,
						int x1, int y1,int x2, int y2, uint boxFilter);
	
	uint addBoxAndMsg(uint id, uint subMenuId, uint menuId, int layer, 
		int x1, int y1,int x2, int y2, uint ownId, char *special);


	bool* getExcludeLayerTable();
	bool getSubMenuFlag(uint menuId, uint subMenu);
	bool validSearchBbox();
	void setSearchBox(int x1, int y1, int x2, int y2);
	bool clipBox(int &x1, int &y1, int &x2, int &y2);
	bool invalidateSearchBox();
	bool isInspectMenu(uint subMenuId);
	bool isInspectSubMenu(uint subMenuId);
	bool isInspectSubMenu(uint menuId, uint subMenuId);

	void setContextMarker();
	void setSignalMarker();
	void setInstMarker();
	void resetMarker();

	uint writeToDpt();
	void *getDpt();

};

END_NAMESPACE_ADS

#endif
