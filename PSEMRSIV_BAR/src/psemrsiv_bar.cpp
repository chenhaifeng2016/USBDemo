#include "../include/psemrsiv_bar.h"
#include "barcode.h"
#include <vector>
#include <string>

std::vector<std::string> SplitStr(char* str, const char* split) {
	std::vector<std::string> result;

	char * s = strtok(str, split);
	while (s) {
		result.push_back(std::string(s));
		s = strtok(NULL, split);
	}

	return result;
}

int WINAPI BAR_GetModuleInfo(ModuleInfo * p_ModuleInfo) {

	int size = sizeof(ModuleInfo);

	memset(p_ModuleInfo, 0x00, size);

	char * pIn = "1000";
	char pOut[256] = { 0 };
	int r = BAR_Init(pIn, pOut);
	if (r != 0)
		return -1;

	memset(pOut, 0x00, sizeof(pOut));
	r = BAR_GetDeviceInfo(pIn, pOut);
	if (r != 0) {
		BAR_Uninit(pIn, pOut);
		return -1;
	}
	
	std::vector<std::string> result = SplitStr(pOut, ",");
	
	memcpy(p_ModuleInfo[0]->ID, result[0].c_str(), result[0].length());
	//memcpy((*p_ModuleInfo + 1)->ID, result[0].c_str(), result[0].length());

	
	memcpy(p_ModuleInfo[0]->Model, result[2].c_str(), result[2].length());
	//memcpy((*p_ModuleInfo + 1)->Model, result[2].c_str(), result[2].length());

	
	memcpy(p_ModuleInfo[0]->Brand, result[1].c_str(), result[1].length());
	//memcpy((*p_ModuleInfo + 1)->Brand, result[1].c_str(), result[1].length());

	char * version = "1.0";
	memcpy(p_ModuleInfo[0]->Version, version, strlen(version));
	//memcpy((*p_ModuleInfo + 1)->Version, version, strlen(version));

	char * VerBuildTime = "2017-09-15 00:00:00";
	memcpy(p_ModuleInfo[0]->VerBuildTime, VerBuildTime, strlen(VerBuildTime));
	//memcpy((*p_ModuleInfo + 1)->VerBuildTime, VerBuildTime, strlen(VerBuildTime));


	BAR_Uninit(pIn, pOut);

	
	
	

	return 1; //1³É¹¦ -1Ê§°Ü
}
