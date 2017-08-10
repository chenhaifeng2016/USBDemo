
// USBDemoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "USBDemo.h"
#include "USBDemoDlg.h"
#include "afxdialogex.h"

//#include "barcode.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSBDemoDlg �Ի���



CUSBDemoDlg::CUSBDemoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_USBDEMO_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CUSBDemoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CUSBDemoDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CUSBDemoDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON3, &CUSBDemoDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CUSBDemoDlg::OnBnClickedButton4)
	
	ON_BN_CLICKED(IDC_BUTTON5, &CUSBDemoDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON2, &CUSBDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_READ_DEVICE_INFO, &CUSBDemoDlg::OnBnClickedReadDeviceInfo)
END_MESSAGE_MAP()


// CUSBDemoDlg ��Ϣ�������

BOOL CUSBDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	if (!LoadDLL())
		return FALSE;

	hThread = INVALID_HANDLE_VALUE;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CUSBDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CUSBDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CUSBDemoDlg::LoadDLL()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	hModule = LoadLibrary("Bar_Code.dll");
	if (hModule == NULL)
	{
		AfxMessageBox("load BarCode.dll error.");
		return false;
	}

	BAR_Init = (fpBAR_Init)GetProcAddress(hModule, "BAR_Init");
	if (BAR_Init == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_Init error.");
		return false;
	}

	BAR_Uninit = (fpBAR_Uninit)GetProcAddress(hModule, "BAR_Uninit");
	if (BAR_Uninit == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_Uninit error.");
		return false;
	}

	BAR_GetSerial = (fpBAR_GetSerial)GetProcAddress(hModule, "BAR_GetSerial");
	if (BAR_GetSerial == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_GetSerial error.");
		return false;
	}

	BAR_ReadData = (fpBAR_ReadData)GetProcAddress(hModule, "BAR_ReadData");
	if (BAR_ReadData == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_ReadData error.");
		return false;
	}

	BAR_GetStatus = (fpBAR_GetStatus)GetProcAddress(hModule, "BAR_GetStatus");
	if (BAR_GetStatus == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_GetStatus error.");
		return false;
	}

	BAR_GetDeviceInfo = (fpBAR_GetDeviceInfo)GetProcAddress(hModule, "BAR_GetDeviceInfo");
	if (BAR_GetDeviceInfo == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_GetDeviceInfo error.");
		return false;
	}

	AfxMessageBox("����dll�ɹ�");
	return true;
}


void CUSBDemoDlg::OnBnClickedButton1()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	count = 1;

	char * pIn = "1000";
	char pOut[256] = { 0 };

	if (BAR_Init(pIn, pOut) == 0)
	{
		AfxMessageBox(_T("���豸�ɹ�"));
		
	}
	else
	{
		CString msg = _T("���豸ʧ��");
		AfxMessageBox(msg);
	}	
}


void CUSBDemoDlg::OnBnClickedButton3()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char * pIn = "1000";
	char pOut[256] = { 0 };

	if (BAR_Uninit(pIn, pOut) == 0)
	{
		AfxMessageBox(_T("�ر��豸�ɹ�"));
	}
	else
	{
		AfxMessageBox(_T("�ر��豸ʧ��"));
	}
}

//��ʱ5���ȡ
void CUSBDemoDlg::OnBnClickedButton4()
{
	//char * pIn = "0"; //10��
	char * pIn = "5000"; //5000����
	char pOut[1024] = { 0 };
	int outlen;

	if (BAR_ReadData(pIn, pOut, &outlen) == 0)
	{
		CString msg;
		msg.Format("���%d ����ά��ɹ� %s ���� %d\n", count++, pOut, outlen);
		TRACE(msg);
		AfxMessageBox(msg);
		
	}
	else
	{
		
		CString msg;
		msg.Format("���%d ����ά��ʧ��\n", count++);

		//TRACE(msg);
		AfxMessageBox(msg);
	}
}

//������ѯ�߳�
void CUSBDemoDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (hThread != INVALID_HANDLE_VALUE) {
		AfxMessageBox("ģʽ2�Ķ�ȡ�߳��������У������ظ�����");
		return;
	}

	run = true;
	hThread = CreateThread(NULL, 0, CUSBDemoDlg::MyThreadFunction, this, 0, NULL);
}

//��ѯ�̺߳���
DWORD WINAPI CUSBDemoDlg::MyThreadFunction(LPVOID lpParam)
{
	if (lpParam == NULL)
		return -1;

	CUSBDemoDlg*  pThis = (CUSBDemoDlg*)lpParam;
	char * pIn = "0"; //0����
	char pOut[1024] = { 0 };
	int outlen;
	

	while (pThis->run)
	{
		if (pThis->BAR_ReadData(pIn, pOut, &outlen) == 0)
		{
			CString msg;
			msg.Format("ģʽ2������ά��ɹ� %s ���� %d\n", pOut, outlen);
			TRACE(msg);
			AfxMessageBox(msg);

		}
		else
		{
			CString msg;
			msg.Format("ģʽ2: ����ά��ʧ��\n");

			//TRACE(msg);
			
		}

		//20������ѯһ��
		Sleep(20);
	}

	return 0;
}


void CUSBDemoDlg::OnBnClickedButton2()
{
	CloseThread();
}

void CUSBDemoDlg::CloseThread()
{
	run = false;

	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	hThread = INVALID_HANDLE_VALUE;
	
	TRACE("�����߳�\n");
}


void CUSBDemoDlg::OnBnClickedReadDeviceInfo()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char * pIn = NULL;
	char pOut[200] = { 0 };

	BAR_GetDeviceInfo(pIn, pOut);

	CString msg = pOut;
	AfxMessageBox(msg);
}
