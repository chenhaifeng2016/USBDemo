#ifndef PSEMRSIV_BAR_H
#define PSEMRSIV_BAR_H

#include <afx.h>

#ifdef __cplusplus
extern "C" {
#endif
	

	typedef struct {
		char ID[32]; // ģ���ţ�Ψһ��;
		char Model[32]; // ģ���ͺ�;
		char Brand[32]; // ģ�鳧�̣�����ȫ�ƣ�;
		char Version[32]; //�汾��;��ʽ:x.x.x.x;
		char VerBuildTime[80];//�汾����ʱ�䣬��ʽ:yyyy-mm-dd HH:MM:SS;
	} ModuleInfo[2];

	

	int WINAPI BAR_GetModuleInfo(ModuleInfo * p_ModuleInfo);
	

#ifdef __cplusplus
}
#endif

#endif
