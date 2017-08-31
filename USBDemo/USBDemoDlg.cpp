
// USBDemoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "USBDemo.h"
#include "USBDemoDlg.h"
#include "afxdialogex.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CUSBDemoDlg 对话框



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
	ON_BN_CLICKED(IDC_LOAD_DLL, &CUSBDemoDlg::OnBnClickedLoadDll)
	ON_BN_CLICKED(IDC_GET_STATUS, &CUSBDemoDlg::OnBnClickedGetStatus)
END_MESSAGE_MAP()


// CUSBDemoDlg 消息处理程序

BOOL CUSBDemoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	

	hThread = INVALID_HANDLE_VALUE;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CUSBDemoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CUSBDemoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



bool CUSBDemoDlg::LoadDLL()
{
	// TODO: 在此添加控件通知处理程序代码
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

	AfxMessageBox("加载dll成功");
	return true;
}


void CUSBDemoDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	count = 1;

	char * pIn = "1000";
	char pOut[256] = { 0 };

	if (BAR_Init(pIn, pOut) == 0)
	{
		AfxMessageBox(_T("初始化成功"));
		
	}
	else
	{
		CString msg = _T("初始化失败");
		AfxMessageBox(msg);
	}	
}


void CUSBDemoDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	char * pIn = "1000";
	char pOut[256] = { 0 };

	if (BAR_Uninit(pIn, pOut) == 0)
	{
		AfxMessageBox(_T("释放资源成功"));
	}
	else
	{
		AfxMessageBox(_T("释放资源失败"));
	}
}

//限时5秒读取
void CUSBDemoDlg::OnBnClickedButton4()
{
	//char * pIn = "0"; //10秒
	char * pIn = "5000"; //5000毫秒
	char pOut[1024] = { 0 };
	int outlen;

	if (BAR_ReadData(pIn, pOut, &outlen) == 0)
	{
		CString msg;
		msg.Format("模式1：序号%d 读二维码成功 %s 长度 %d\n", count++, pOut, outlen);
		TRACE(msg);
		AfxMessageBox(msg);
		
	}
	else
	{
		
		CString msg;
		msg.Format("模式1：序号%d 读二维码失败\n", count++);

		//TRACE(msg);
		AfxMessageBox(msg);
	}
}

//启动轮询线程
void CUSBDemoDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (hThread != INVALID_HANDLE_VALUE) {
		AfxMessageBox("模式2的读取线程正在运行，不能重复运行");
		return;
	}

	run = true;
	hThread = CreateThread(NULL, 0, CUSBDemoDlg::MyThreadFunction, this, 0, NULL);
}

//轮询线程函数
DWORD WINAPI CUSBDemoDlg::MyThreadFunction(LPVOID lpParam)
{
	if (lpParam == NULL)
		return -1;

	CUSBDemoDlg*  pThis = (CUSBDemoDlg*)lpParam;
	char * pIn = "0"; //0毫秒
	char pOut[1024] = { 0 };
	int outlen;
	

	while (pThis->run)
	{
		if (pThis->BAR_ReadData(pIn, pOut, &outlen) == 0)
		{
			CString msg;
			msg.Format("模式2：读二维码成功 %s 长度 %d\n", pOut, outlen);
			TRACE(msg);
			AfxMessageBox(msg);

		}
		else
		{
			CString msg;
			msg.Format("模式2: 读二维码失败.............................\n");

			TRACE(msg);
			
		}

		//20毫秒轮询一次
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
	
	TRACE("结束线程\n");
}

void CUSBDemoDlg::OnBnClickedLoadDll()
{
	LoadDLL();
}


void CUSBDemoDlg::OnBnClickedReadDeviceInfo()
{

	char * pIn = NULL;
	char pOut[200] = { 0 };

	int rc = BAR_GetStatus(pIn, pOut);

	CString msg;
	msg.Format("返回码%d, 输出参数%s", rc, pOut);
	AfxMessageBox(msg);
	
}



void CUSBDemoDlg::OnBnClickedGetStatus()
{
	char * pIn = NULL;
	char pOut[200] = { 0 };

	BAR_GetDeviceInfo(pIn, pOut);

	CString msg = pOut;
	AfxMessageBox(msg);
}
