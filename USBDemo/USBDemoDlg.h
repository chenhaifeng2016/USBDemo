
// USBDemoDlg.h : ͷ�ļ�
//

#pragma once

typedef WORD (WINAPI *fpBAR_Init) (char *, char *);
typedef WORD (WINAPI *fpBAR_Uninit) (char *, char *);
typedef WORD (WINAPI *fpBAR_GetSerial) (char *);
typedef WORD (WINAPI *fpBAR_ReadData) (char *, char *, int *);
typedef WORD (WINAPI *fpBAR_GetStatus) (char *, char *);

// CUSBDemoDlg �Ի���
class CUSBDemoDlg : public CDialogEx
{
// ����
public:
	CUSBDemoDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	
	int count;
	afx_msg void OnBnClickedButton2();

	HMODULE hModule;
	fpBAR_Init  BAR_Init;
	fpBAR_Uninit  BAR_Uninit;
	fpBAR_GetSerial  BAR_GetSerial;
	fpBAR_ReadData  BAR_ReadData;
	fpBAR_GetStatus  BAR_GetStatus;
	afx_msg void OnBnClickedButton5();
};
