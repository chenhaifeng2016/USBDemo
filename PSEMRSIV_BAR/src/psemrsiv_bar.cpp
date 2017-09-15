#include "../include/psemrsiv_bar.h"

int WINAPI BAR_GetModuleInfo(ModuleInfo * p_ModuleInfo) {

	int size = sizeof(ModuleInfo);

	memset(p_ModuleInfo, 0x00, size);

	char * id = "qr";
	memcpy(p_ModuleInfo[0]->ID, id, strlen(id));

	char * ID = "ID";
	memcpy((*p_ModuleInfo+1)->ID, ID, strlen(ID));

	char * model = "model";
	memcpy(p_ModuleInfo[0]->Model, model, strlen(model));
	memcpy((*p_ModuleInfo + 1)->Model, model, strlen(model));

	char * brand = "NLS";
	memcpy(p_ModuleInfo[0]->Brand, brand, strlen(brand));
	memcpy((*p_ModuleInfo + 1)->Brand, brand, strlen(brand));

	char * version = "1.0";
	memcpy(p_ModuleInfo[0]->Version, version, strlen(version));
	memcpy((*p_ModuleInfo + 1)->Version, version, strlen(version));

	char * VerBuildTime = "2017-09-15 00:00:00";
	memcpy(p_ModuleInfo[0]->VerBuildTime, VerBuildTime, strlen(VerBuildTime));
	memcpy((*p_ModuleInfo + 1)->VerBuildTime, VerBuildTime, strlen(VerBuildTime));

	//p_ModuleInfo[1] = p_ModuleInfo[0];

	return 1; //1³É¹¦ -1Ê§°Ü
}
