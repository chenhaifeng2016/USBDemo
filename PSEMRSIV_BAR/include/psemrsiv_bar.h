#ifndef PSEMRSIV_BAR_H
#define PSEMRSIV_BAR_H

#include <afx.h>

#ifdef __cplusplus
extern "C" {
#endif
	

	typedef struct {
		char ID[32]; // 模块编号（唯一）;
		char Model[32]; // 模块型号;
		char Brand[32]; // 模块厂商（厂商全称）;
		char Version[32]; //版本号;格式:x.x.x.x;
		char VerBuildTime[80];//版本生成时间，格式:yyyy-mm-dd HH:MM:SS;
	} ModuleInfo[2];

	

	int WINAPI BAR_GetModuleInfo(ModuleInfo * p_ModuleInfo);
	

#ifdef __cplusplus
}
#endif

#endif
