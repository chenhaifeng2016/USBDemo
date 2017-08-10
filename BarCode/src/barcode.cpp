#include "../include/barcode.h"
#include "USBWrapper.h"
#include "FileLog.h"
#include "Config.h"

USBWrapper usb;

WORD WINAPI BAR_Init(char *pIn, char* pOut)
{
	int r = 0;
	
	gConfig.ReadConfigFile();

	r = usb.OpenDevice();

	if (r < 0)
		r = 1;
	else
		r = 0;

	if (!usb.StartReadThread())
		r = 1;

	return r;
}

WORD WINAPI BAR_Uninit(char* pIn, char* pOut)
{
	int r = 0;

	//ǿ��ɱ���߳�
	usb.StopReadThread();

	r = usb.CloseDevice();

	if (r < 0)
		r = 1;
	else
		r = 0;

	return r;
}

WORD WINAPI BAR_ReadData(char *pIn, char *pOut, int *pOutLen)
{
	int r = 0;

	
	r = usb.ReadData(pIn, pOut, pOutLen);
	

	if (r < 0)
	{ 
		r = 1;
	}
	else
	{
		r = 0;
	}
		

	return r;
}

WORD WINAPI BAR_GetSerial(char * cSerialNo)
{
	char * serialno = "00000000";
	memcpy(cSerialNo, serialno, 8);

	return 0;
}

WORD WINAPI BAR_GetStatus(char *pIn, char* pOut)
{
	char * status = "00000000";
	memcpy(pOut, status, 7);

	return 0;
}

WORD WINAPI BAR_GetDeviceInfo(char *pIn, char *pOut)
{
	// 0x7E0x00 0x000x05 0x33 0x480x300x330x30 0xB2

	//0x7E0x000x000x050x330x480x300x330x300xB2
	// �´�½ NLS-EM20

	
	char serialNo[50] = { 0 };
	
	if (usb.ReadSerialNo(serialNo))
	{
		sprintf(pOut, "%s,%s,%s", serialNo, "�´�½", "NLS-EM20");
		return 1;
	}
	else
	{
		sprintf(pOut, "%s,%s,%s", "000000000000", "�´�½", "NLS-EM20");
		return 0;
	}
}
