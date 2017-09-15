
// USBDemoDlg.h : 头文件
//

#pragma once

typedef WORD (WINAPI *fpBAR_Init) (char *, char *);
typedef WORD (WINAPI *fpBAR_Uninit) (char *, char *);
typedef WORD (WINAPI *fpBAR_GetSerial) (char *);
typedef WORD (WINAPI *fpBAR_ReadData) (char *, char *, int *);
typedef WORD (WINAPI *fpBAR_GetStatus) (char *, char *);
typedef WORD(WINAPI *fpBAR_GetDeviceInfo) (char*, char*);

// CUSBDemoDlg 对话框
class CUSBDemoDlg : public CDialogEx
{
// 构造
public:
	CUSBDemoDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_USBDEMO_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();

	
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	
	int count;
	bool LoadDLL();

	HMODULE hModule;
	fpBAR_Init  BAR_Init;
	fpBAR_Uninit  BAR_Uninit;
	fpBAR_GetSerial  BAR_GetSerial;
	fpBAR_ReadData  BAR_ReadData;
	fpBAR_GetStatus  BAR_GetStatus;
	fpBAR_GetDeviceInfo BAR_GetDeviceInfo;

	afx_msg void OnBnClickedButton5();

	HANDLE hThread;

	static DWORD WINAPI MyThreadFunction(LPVOID lpParam);
	afx_msg void OnBnClickedButton2();

	void CloseThread();
	bool run;
	afx_msg void OnBnClickedReadDeviceInfo();
	afx_msg void OnBnClickedLoadDll();
	afx_msg void OnBnClickedGetStatus();
	afx_msg void OnBnClickedButton6();
};
