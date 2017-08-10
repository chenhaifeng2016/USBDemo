#ifndef BARCODE_H
#define BARCODE_H

#include <windows.h>

#ifdef __cplusplus
extern "C" {
#endif

WORD WINAPI BAR_Init(char *pIn, char* pOut);
WORD WINAPI BAR_Uninit(char* pIn, char* pOut);
WORD WINAPI BAR_ReadData(char *pIn, char *pOut, int *pOutLen);
WORD WINAPI BAR_GetSerial(char * cSerialNo);
WORD WINAPI BAR_GetStatus(char *pIn, char* pOut);
WORD WINAPI BAR_GetDeviceInfo(char *pIn, char *pOut);

#ifdef __cplusplus
}
#endif

#endif
