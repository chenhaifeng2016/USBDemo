#ifndef USBWRAPPER_H
#define USBWRAPPER_H

#include "libusb.h"
#include <string>

#define OK   0
#define ERR -1

#pragma pack(push)
#pragma pack(1)

typedef struct
{
	unsigned char packetId;// FIXED 0x02.
	unsigned char dataLength;
	unsigned char data[56];
	unsigned char AIMID[3];
	unsigned char reserved[2];
	unsigned char endFlag;// bit0 = 1 indicate next packet.
} ST_HID_POS_IN_PACKECT;


typedef struct
{
	unsigned char packetId; // FIXED 0x04.649640

	unsigned char dataLength;
	unsigned char data[62];
} ST_HID_POS_OUT_PACKECT;

typedef enum
{
	EM_HID_POS_OUT_PACKECT_ID_IN = 0x02,
	EM_HID_POS_OUT_PACKECT_ID_OUT = 0x04,
}
EM_HID_POS_OUT_PACKECT_ID;

#pragma pack(pop)

//typedef void(*CallbackFunc)(char*); //声明回调函数

class USBWrapper
{
public:
	USBWrapper();
	~USBWrapper();

	
	int OpenDevice();//打开设备
	int CloseDevice();//关闭设备
	int ReadData(char *pIn, char *pOut, int *pOutLen);

	bool StartReadThread();//建立读线程
	void StopReadThread();//关闭读线程
	

private:
	static void ScanThread(void * arg);
	int ReadPacket(unsigned char * pdata);
	

private:
	libusb_context * ctx;
	libusb_device * dev;
	libusb_device_handle * devh;
	libusb_config_descriptor * config;
	libusb_endpoint_descriptor * epInput;
	libusb_endpoint_descriptor * epOutput;
	



	int r;
	bool working;
	HANDLE hThread;
	//CallbackFunc mCallbackFunc;
	
	std::string scanner_code;
	std::string return_code;
};

#endif
