#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <chrono>

#include "USBWrapper.h"
#include "FileLog.h"
#include "Config.h"
#include <assert.h>

#define SERIAL_NO_LEN 8

//�ٽ���
CRITICAL_SECTION gCriticalSec;




USBWrapper::USBWrapper()
{
	ctx = NULL;
	dev = NULL;
	devh = NULL;
	config = NULL;
	epInput = NULL;
	epOutput = NULL;
	r = -1;
	working = false;
	hThread = INVALID_HANDLE_VALUE;
	//mCallbackFunc = NULL;
	return_code = "";

	InitializeCriticalSection(&gCriticalSec);
}


USBWrapper::~USBWrapper()
{
	DeleteCriticalSection(&gCriticalSec);
}

int USBWrapper::OpenDevice()
{
	status = 0;

	if (ctx != NULL)
		return ERROR;

	r = libusb_init(&ctx);
	if (r < 0)
	{
		return r;
	}
	
	devh = libusb_open_device_with_vid_pid(ctx, 0x1EAB, 0x1310);
	if (devh == NULL)
	{
		return ERROR;
	}

	if (libusb_kernel_driver_active(devh, 0) == 1)
	{
		//TRACE("libusb_kernel_driver_active\n");

		if (libusb_detach_kernel_driver(devh, 0) == 0)
		{
			//TRACE("libusb_detach_kernel_driver == 0\n");
		}
		else
		{
			//TRACE("libusb_detach_kernel_driver != 0\n");
		}
	}

	r = libusb_claim_interface(devh, 0);
	if (r < 0)
	{
		//libusb_close(devh);

		return r;
	}

	dev = libusb_get_device(devh);
	if (dev == NULL)
	{
		return ERROR;
	}

	struct libusb_device_descriptor desc;
	r = libusb_get_device_descriptor(dev, &desc);
	if (r != 0)
	{
		libusb_release_interface(devh, 0);
		libusb_close(devh);

		return r;
	}

	r = libusb_get_active_config_descriptor(dev, &config);
	if (r != 0)
	{
		libusb_release_interface(devh, 0);
		libusb_close(devh);
		return r;
	}

	libusb_interface_descriptor * interface = (libusb_interface_descriptor*) config->interface->altsetting;
	for (int i = 0; i < interface->bNumEndpoints && interface->endpoint != NULL; i++)
	{
		libusb_endpoint_descriptor * endpoint = (libusb_endpoint_descriptor*) (interface->endpoint + i);

		const char * tran_type_name;
		if (endpoint->bmAttributes == 0x00)
		{
			tran_type_name = "CONTROL";
		}
		else if (endpoint->bmAttributes == 0x01)
		{
			tran_type_name = "SIO";
		}
		else if (endpoint->bmAttributes == 0x02)
		{
			tran_type_name = "BULK";
		}
		else if (endpoint->bmAttributes == 0x03)
		{
			tran_type_name = "INTER";
		}
		else
		{
			tran_type_name = "NULL";
		}

		const char * port_type_name = ((endpoint->bEndpointAddress & 0x80) == 0) ? "OUT" : "IN";

		if ((endpoint->bEndpointAddress & 0x80) != 0)
		{
			epInput = endpoint;
		}
		else
		{
			epOutput = endpoint;
		}
	}//end for

	if (epInput == NULL)
	{
		r = libusb_release_interface(devh, 0);
		libusb_close(devh);
		return ERROR;
	}

	status = 1;
	return OK;
}

int USBWrapper::CloseDevice()
{
	status = 0;

	if (devh != NULL)
	{
		libusb_release_interface(devh, 0);
		libusb_close(devh);

		devh = NULL;
		dev = NULL;
	}

	if (ctx != NULL)
	{
		libusb_exit(ctx);
		ctx = NULL;
	}

	return OK;
}



int USBWrapper::ReadData(char *pIn, char *pData, int *pOutLen)
{
	if (devh == NULL)
	{
		return ERR;
	}
		


	if (pIn == NULL || pData == NULL)
	{
		return ERR;
	}
		
	
	int timeout = 0;
	try
	{
		timeout = atoi(pIn);
	}
	catch (...)
	{
		return ERR;
	}
	
	
	EnterCriticalSection(&gCriticalSec);

	if (timeout == 0)
	{
		//һ���ǲ�֪��ʲôʱ���Ʊ��Ҫ��0,ÿ��20�������һ�Σ���֤�ܼ�ʱ��ö�ά��

		if (scanner_code.length() == 0)
		{
			LeaveCriticalSection(&gCriticalSec);
			return ERR;
		}
		else
		{
			return_code = scanner_code;
			memcpy(pData, return_code.c_str(), return_code.length());
			*pOutLen = return_code.length();
			scanner_code = "";

			gLog.info("ͨ����ѯ��ʽ��ȡ" + return_code);

			LeaveCriticalSection(&gCriticalSec);
			return OK;
		}
	}

	//��һ�����Ѿ���������֤������Ҫ�����ȡ��ά�룬���㴫����ʱʱ�䣬Ҫ�������ʱ���ڶ���ά�룬��������ǰ���أ��������ȵ���ʱʱ�䵽�󷵻ض�ȡʧ��
	scanner_code = "";
	
	//std::chrono::high_resolution_clock::time_point beginTime = std::chrono::high_resolution_clock::now();
	clock_t beginTime;
	beginTime = clock();
	
	while (true)
	{
		//std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();
		//std::chrono::milliseconds timeInterval = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime);

		clock_t endTime;
		endTime = clock();
		
		int timeInterval = endTime - beginTime;
		
		
		
		if (timeInterval >= timeout)
		{
			return ERR;
			
		}
			

		if (scanner_code.length() == 0)
		{
			// û�ж�ά�룬�ȴ���ʱ
			Sleep(1);
			continue;
		}
		else
		{
			//�ж�ά�룬�жϺ��ϴη��ص��ǲ���ͬһ��Ʊ�Ķ�ά��
				return_code = scanner_code;

				memcpy(pData, return_code.c_str(), return_code.length());
				*pOutLen = return_code.length();

				scanner_code = "";
				
				gLog.info("ͨ����ʱ��ʽ��ȡ" + return_code);

				return OK;
		}			
	} // end while
	
	return ERR;
}



int USBWrapper::ReadPacket(unsigned char * pdata)
{
	int rlen;
	int ilen = 0;
	

	int count = 0;
	char msg[50] = { 0 };

	while (true)
	{
		ST_HID_POS_IN_PACKECT packet;
		memset(&packet, 0, sizeof(ST_HID_POS_IN_PACKECT));


		sprintf(msg, "count=%d\n", count++);
		OutputDebugString(msg);

		r = libusb_interrupt_transfer(devh, epInput->bEndpointAddress, (unsigned char*)&packet, sizeof(ST_HID_POS_IN_PACKECT), &rlen, 0); //block
		if (r < 0)
		{
			gLog.error("ReadPacket libusb_interrupt_transfer");
			return r;
		}

		if (rlen == 0)
		{
			packet.packetId = EM_HID_POS_OUT_PACKECT_ID_IN;
			packet.dataLength = 0;
			packet.endFlag = 0;
			rlen = sizeof(packet);
		}

		if (rlen != sizeof(packet)) // not valid packet.
		{
			gLog.error("ReadPacket -101");
			return -101;
		}
			

		if (packet.packetId != EM_HID_POS_OUT_PACKECT_ID_IN)
		{
			gLog.error("ReadPacket -102");
			return -102;
		}

		if (packet.dataLength > sizeof(packet.data))
		{
			gLog.error("ReadPacket -103");
			return -103;
		}

		if (packet.dataLength > 0)
		{
			char buf[50] = { 0 };
			sprintf(buf, "len = %d\n", packet.dataLength);
			OutputDebugString(buf);

			if (packet.dataLength == 0x12) //18���ֽ�
			{
				//prefix2(0x02 0x00)    ����2�ֽ�(data + LRC)   type(0x34)   data   LRC
				if (packet.data[0] == 0x02 && packet.data[1] == 0x00 && packet.data[4] == 0x34)
				{
					char data[9] = { 0 };
					memcpy(data, packet.data + 9, SERIAL_NO_LEN);
					serialNo = data;
					
					//queue.push(serialNo);

					return 0; // �̶�����0������ǰ�˴���
				}
			}

			memcpy(pdata + ilen, packet.data, packet.dataLength);
			ilen += packet.dataLength;
		}

		if (packet.dataLength == 0 || (packet.endFlag & 1) == 0)
			break;
	}//end while

	return ilen; // �����ܳ���
}

bool USBWrapper::ReadSerialNo(char * pOut)
{
	//char cmd[] = {0x7E0x000x000x050x330x480x300x330x300xB2};
	serialNo = "";
	int rlen = 0;

	ST_HID_POS_OUT_PACKECT packet;
	memset(&packet, 0, sizeof(ST_HID_POS_OUT_PACKECT));

	packet.packetId = 0x04;
	packet.dataLength = 0x0a;
	
	packet.data[0] = 0x7E; // prefix
	packet.data[1] = 0x00;

	packet.data[2] = 0x00; // len
	packet.data[3] = 0x05;

	packet.data[4] = 0x33; //type

	packet.data[5] = 0x48; // data
	packet.data[6] = 0x30;
	packet.data[7] = 0x33;
	packet.data[8] = 0x30;

	packet.data[9] = 0xB2; //lrc
	
	
	int rc = 0;
	

	rc = libusb_interrupt_transfer(devh, epOutput->bEndpointAddress, (unsigned char*)&packet, sizeof(packet), &rlen, 5000); //block
	
	if (rc != 0)
	{
		return false;
	}
		

	// �ȴ����ؽ��������mutex���ź���
	//serialNo = queue.pop();
	
	//�ȴ����ؽ��
	while (true)
	{
		if (serialNo.length() != SERIAL_NO_LEN) // ����жϳ���
			continue;

		memcpy(pOut, serialNo.c_str(), serialNo.length());
		serialNo = "";
		break;
	}
	

	return true;
}

bool USBWrapper::StartReadThread()
{
	if (dev == NULL)
	{
		return false;
	}

	if (working)
		return false;

	working = true;
	
	hThread = (HANDLE)_beginthread(&USBWrapper::ScanThread, 0, this);

	return true;
}

void USBWrapper::StopReadThread()
{
	working = false;

	if (hThread != INVALID_HANDLE_VALUE)
	{
		TerminateThread(hThread, 0);
		hThread  = INVALID_HANDLE_VALUE;
	}
	

	//if (hThread != INVALID_HANDLE_VALUE)
	//{
	//CloseHandle(hThread);
	//hThread = INVALID_HANDLE_VALUE;
	//}
}

void USBWrapper::ScanThread(void * arg)
{
	//TRACE("���߳̿�ʼ����...\n");

	char szRxBuf[1024];
	int rlen;

	USBWrapper * pThis = (USBWrapper*) arg;

	
	std::string prevcode = "";

	clock_t beginTime;
	beginTime = clock();

	clock_t endTime;
	endTime = clock();

	int timeInterval = endTime - beginTime;
	int timeout = gConfig.timeout;//ms


		
	while (pThis->working)
	{
		memset(szRxBuf, 0x00, sizeof(szRxBuf));

		rlen = pThis->ReadPacket((unsigned char *)szRxBuf);
		//libusb_interrupt_transfer ������������޷�ֹͣ�߳�

		if (rlen < 0)
		{
			gLog.error("ɨ���߳�ɨ��ʧ���˳��߳� if (rlen < 0)");
			pThis->status = 0;
			return;
		}

		pThis->status = 1;
		if (rlen == 0)
		{
			Sleep(1);
			continue;
		}

		for (int i = rlen - 1; i > 0; i--)
		{
			if (szRxBuf[i] == 0 || szRxBuf[i] == 0x0d || szRxBuf[i] == 0x0a)
			{
				szRxBuf[i] = '\0';
				rlen -= 1;
				continue;
			}

			break;
		}
		
		endTime = clock();
		timeInterval = endTime - beginTime;

		std::string code = szRxBuf;
		if (code.compare(prevcode) != 0)
		{
			//��ͬƱ��ֱ�ӷ���
			prevcode = code;
			pThis->scanner_code = szRxBuf;
			beginTime = endTime;
			gLog.info2(szRxBuf);
			OutputDebugString("��ͬƱ����...\n");
		}
		else
		{
			//ͬƱ��ɨ����

			if (timeInterval < timeout)
			{
				// ����ʱ
				OutputDebugString("ͬƱû�г�ʱ��������...\n");
			}
			else
			{
				//��ʱ
				prevcode = code;
				pThis->scanner_code = szRxBuf;
				beginTime = endTime;
				gLog.info2(szRxBuf);
				OutputDebugString("ͬƱ��ʱ����...\n");
				//prevcode = "";

			}
		}
		

		Sleep(5);
	}
	
	//TRACE(".................���߳��˳�.................\n");
}
