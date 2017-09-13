#include "../include/barcode.h"
#include "USBWrapper.h"
#include "FileLog.h"
#include "Config.h"

USBWrapper usb;

WORD WINAPI BAR_Init(char *pIn, char* pOut)
{
	gConfig.ReadConfigFile();

	if (usb.OpenDevice() != 0) {
		char * status = "11111111";
		memcpy(pOut, status, 8);
		return 1;
	}

	usb.StartReadThread();

	char * status = "00000000";
	memcpy(pOut, status, 8);
	return 0;
}

WORD WINAPI BAR_Uninit(char* pIn, char* pOut)
{
	int r = 0;

	// 停止扫扫描线程
	usb.StopReadThread();

	r = usb.CloseDevice();

	if (r < 0) //-1
	{
		char * status = "11111111";
		memcpy(pOut, status, 8);

		r = 1; // 失败
	}
	else
	{
		char * status = "00000000";
		memcpy(pOut, status, 8);

		r = 0; //成功
	}
		

	return r;  //return !r
}

WORD WINAPI BAR_ReadData(char *pIn, char *pOut, int *pOutLen)
{
	int r = 0;

	
	r = usb.ReadData(pIn, pOut, pOutLen);
	

	if (r < 0)
	{ 
		char * status = "11111111";
		*pOutLen = 8;
		memcpy(pOut, status, *pOutLen);

		r = 1;
	}
	else
	{
		r = 0;
	}
		

	return r;
}


WORD WINAPI BAR_GetStatus(char *pIn, char* pOut)
{
	if (usb.GetStatus())
	{
		char * status = "00000000";
		memcpy(pOut, status, 8);
		return 0;
		
	}
	else
	{
		char * status = "11111111";
		memcpy(pOut, status, 8);
		return 1;
	}
}

WORD WINAPI BAR_GetSerial(char * pOut)
{
	char serialNo[30];
	memset(serialNo, 0x00, sizeof(serialNo));

	if (usb.ReadSerialNo(serialNo))
	{
		sprintf(pOut, "%s,%s,%s", serialNo, "新大陆", "EM20");
		return 0;
	}
	else
	{
		sprintf(pOut, "%s,%s,%s", "11111111", "新大陆", "EM20");
		return 1;
	}
}

WORD WINAPI BAR_GetDeviceInfo(char *pIn, char *pOut)
{
	// 0x7E0x00 0x000x05 0x33 0x480x300x330x30 0xB2

	//0x7E0x000x000x050x330x480x300x330x300xB2
	// 新大陆 NLS-EM20

	
	char serialNo[30];
	memset(serialNo, 0x00, sizeof(serialNo));
	
	if (usb.ReadSerialNo(serialNo))
	{
		sprintf(pOut, "%s,%s,%s", serialNo, "新大陆", "EM20");
		return 0;
	}
	else
	{
		sprintf(pOut, "%s,%s,%s", "11111111", "新大陆", "EM20");
		return 1;
	}
}
