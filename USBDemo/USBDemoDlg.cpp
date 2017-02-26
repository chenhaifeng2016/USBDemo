
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
	ON_BN_CLICKED(IDC_BUTTON2, &CUSBDemoDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON5, &CUSBDemoDlg::OnBnClickedButton5)
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

void CUSBDemoDlg::OnBnClickedButton4()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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



void CUSBDemoDlg::OnBnClickedButton2()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	hModule = LoadLibrary("Bar_Code.dll");
	if (hModule == NULL)
	{
		AfxMessageBox("load BarCode.dll error.");
		return;
	}

	BAR_Init = (fpBAR_Init)GetProcAddress(hModule, "BAR_Init");
	if (BAR_Init == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_Init error.");
		return;
	}

	BAR_Uninit = (fpBAR_Uninit)GetProcAddress(hModule, "BAR_Uninit");
	if (BAR_Uninit == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_Uninit error.");
		return;
	}

	BAR_GetSerial = (fpBAR_GetSerial)GetProcAddress(hModule, "BAR_GetSerial");
	if (BAR_GetSerial == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_GetSerial error.");
		return;
	}

	BAR_ReadData = (fpBAR_ReadData)GetProcAddress(hModule, "BAR_ReadData");
	if (BAR_ReadData == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_ReadData error.");
		return;
	}

	BAR_GetStatus = (fpBAR_GetStatus)GetProcAddress(hModule, "BAR_GetStatus");
	if (BAR_GetStatus == NULL)
	{
		AfxMessageBox("GetProcAddress BAR_GetStatus error.");
		return;
	}

	AfxMessageBox("����dll�ɹ�");
}


void CUSBDemoDlg::OnBnClickedButton5()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	char * pIn = "1000";
	char pOut[1024] = { 0 };

	BAR_GetStatus(pIn, pOut);

	AfxMessageBox(pOut);
}
