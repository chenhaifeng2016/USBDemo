#include "psemrsiv_bar.h"

int WINAPI BAR_GetModuleInfo(ModuleInfo * p_ModuleInfo) {

	memset(p_ModuleInfo, 0x00, sizeof(ModuleInfo));

	char * id = "qr";
	memcpy(id, p_ModuleInfo[0]->ID, strlen(id));
	memcpy(id, p_ModuleInfo[1]->ID, strlen(id));

	char * model = "model";
	memcpy(model, p_ModuleInfo[0]->Model, strlen(model));
	memcpy(model, p_ModuleInfo[1]->Model, strlen(model));

	char * brand = "NLS";
	memcpy(brand, p_ModuleInfo[0]->Brand, strlen(brand));
	memcpy(brand, p_ModuleInfo[1]->Brand, strlen(brand));

	char * version = "1.0";
	memcpy(version, p_ModuleInfo[0]->Version, strlen(version));
	memcpy(version, p_ModuleInfo[1]->Version, strlen(version));

	char * VerBuildTime = "2017-09-15 00:00:00";
	memcpy(VerBuildTime, p_ModuleInfo[0]->VerBuildTime, strlen(VerBuildTime));
	memcpy(VerBuildTime, p_ModuleInfo[1]->VerBuildTime, strlen(VerBuildTime));

	//p_ModuleInfo[1] = p_ModuleInfo[0]
	return 1; //1³É¹¦ -1Ê§°Ü
}
