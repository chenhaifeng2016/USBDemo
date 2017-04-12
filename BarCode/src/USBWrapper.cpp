#include <stdio.h>
#include <stdlib.h>
#include <process.h>
#include <chrono>

#include "USBWrapper.h"
#include "FileLog.h"



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
}


USBWrapper::~USBWrapper()
{
}

int USBWrapper::OpenDevice()
{
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
		libusb_close(devh);

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
			
	}

	if (epInput == NULL)
	{
		r = libusb_release_interface(devh, 0);
		libusb_close(devh);
		return ERROR;
	}

	return OK;
}

int USBWrapper::CloseDevice()
{
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
		timeout = 0;
	}
	
	//һ���ǲ�֪��ʲôʱ���Ʊ��Ҫ��0,ÿ��20�������һ�Σ���֤�ܼ�ʱ��ö�ά��
	if (timeout == 0)
	{
		if (scanner_code.length() == 0)
		{
			return ERR;
		}
		else
		{
			return_code = scanner_code;
			memcpy(pData, return_code.c_str(), return_code.length());
			*pOutLen = return_code.length();
			scanner_code = "";

			gLog.info(return_code);

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

			//if (scanner_code.compare(return_code) != 0)
			{
				

				return_code = scanner_code;

				memcpy(pData, return_code.c_str(), return_code.length());
				*pOutLen = return_code.length();

				scanner_code = "";
				

				return 0;
			}
			/*
			else
			{
				scanner_code = "";
				continue;
			}
			*/
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

		r = libusb_interrupt_transfer(devh, epInput->bEndpointAddress, (unsigned char*)&packet, sizeof(packet), &rlen, 0); //block
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

			memcpy(pdata + ilen, packet.data, packet.dataLength);
			ilen += packet.dataLength;
		}

		if (packet.dataLength == 0 || (packet.endFlag & 1) == 0)
			break;
	}//end while

	return ilen; // �����ܳ���
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

	
	char msg[256] = { 0 };


	while (pThis->working)
	{
		memset(szRxBuf, 0x00, sizeof(szRxBuf));

		rlen = pThis->ReadPacket((unsigned char *)szRxBuf);
		//libusb_interrupt_transfer ������������޷�ֹͣ�߳�

		if (rlen < 0)
		{
			gLog.error("ɨ���߳�ɨ��ʧ���˳��߳� if (rlen < 0)");
			return;
		}

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

		sprintf(msg, "��ά��%s\n", szRxBuf);
		OutputDebugString(msg);
		
		

		pThis->scanner_code = szRxBuf;

		Sleep(1*1000);
	}
	
	//TRACE(".................���߳��˳�.................\n");
}
