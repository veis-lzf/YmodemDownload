
// DownloadToolDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "DownloadTool.h"
#include "DownloadToolDlg.h"
#include "afxdialogex.h"
#include "./Serial/CSerialPort.h"
#include "./Serial/CThreadComm.h"
#include "./Serial/CThread.h"
#include "protocol.h"
#include "Ymodem/ymodem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}



BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDownloadToolDlg 对话框



CDownloadToolDlg::CDownloadToolDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DOWNLOADTOOL_DIALOG, pParent) 
	, m_bOpen(FALSE)
	, m_fileInfo({ 0 })
	, m_pbuf(NULL)
	, bHandOK(FALSE)
	, m_pWriteThread(NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
	m_hIconOpen = AfxGetApp()->LoadIcon(IDI_ICON3);
	m_hIconClose = AfxGetApp()->LoadIcon(IDI_ICON2);
}

void CDownloadToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_ctrlSerialList);
	DDX_Control(pDX, IDC_COMBO2, m_ctrlBaudRateList);
	DDX_Control(pDX, IDC_COMBO3, m_ctrlDownloadMode);
	DDX_Control(pDX, IDC_LIST1, m_ctrlMsgBox);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
	DDX_Control(pDX, IDC_EDIT2, m_ctrlFilePath);
	DDX_Control(pDX, IDC_STATIC2, m_progressStatus);
	DDX_Control(pDX, IDC_STATIC1, m_ctrlOpenCloseStatusIcon);
}

BEGIN_MESSAGE_MAP(CDownloadToolDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDownloadToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDownloadToolDlg::OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTNLOADFILE, &CDownloadToolDlg::OnBnClickedBtnloadfile)
	ON_BN_CLICKED(IDC_BTNWRITE, &CDownloadToolDlg::OnBnClickedBtnwrite)
	ON_MESSAGE(WM_HAND_OK_MSG, OnHandlerUsrMsg)
	ON_MESSAGE(WM_START2WRITE_MSG, OnHandlerUsrMsg)
	ON_MESSAGE(WM_WRITE_FINISH_MSG, OnHandlerUsrMsg)
	ON_MESSAGE(WM_RECEIVE_ACK_MSG, OnHandlerUsrMsg)
	ON_MESSAGE(WM_RECEIVE_NACK_MSG, OnHandlerUsrMsg)
	ON_MESSAGE(WM_WRITE_PACK_FINISH_MSG, OnHandlerUsrMsg)
	ON_CBN_DROPDOWN(IDC_COMBO1, &CDownloadToolDlg::OnCbnDropdownCombo1)
END_MESSAGE_MAP()


// CDownloadToolDlg 消息处理程序

BOOL CDownloadToolDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	OnInitSerialList();
	OnInitBaudRateList();
	OnInitDownloadMode(); 
	OnIntiProgressBar();
	OnSetStatusToDownLoadButton(FALSE);

	m_ctrlOpenCloseStatusIcon.SetIcon(m_hIconClose);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDownloadToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDownloadToolDlg::OnPaint()
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
HCURSOR CDownloadToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LRESULT CDownloadToolDlg::OnHandlerUsrMsg(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case WM_HAND_OK_MSG:
		TRACE(_T("WM_HAND_OK_MSG Message!\r\n"));
		OnAddMsgToListBox(_T("Hand OK! Begin to Write..."));
		OnSetStatusToDownLoadButton(TRUE);
		//OnBnClickedBtnwrite();
		bHandOK = TRUE;
		break;
	case WM_START2WRITE_MSG:
		break;
	case WM_WRITE_FINISH_MSG:
		break;
	case WM_RECEIVE_ACK_MSG:
		break;
	case WM_RECEIVE_NACK_MSG:
		break;
	case WM_WRITE_PACK_FINISH_MSG:
	{
		CString str;
		int cur_size = (*(int*)lParam);

		str.Format(_T("write size:%d"), cur_size);
		OnAddMsgToListBox(str);
		OnUpdateProgress((cur_size + 1) *100 / m_fileInfo.file_size);

		if (cur_size == m_fileInfo.file_size)
		{
			str.Format(_T("Write Successful！"));
			OnAddMsgToListBox(str);
		}
	}
		break;
	default:
		TRACE(_T("Unknow Message!\r\n"));
		break;
	}

	return LRESULT();
}


// 打开串口
void CDownloadToolDlg::OnBnClickedOk()
{
	CString port, port_num;
	DWORD dwBaud;

	m_ctrlSerialList.GetWindowText(port);
	port_num = port.Right(port.GetLength() - 3); // 提取串口号数字部分
	if (_ttoi(port_num) > 9)
	{
		port = _T("\\\\.\\") + port;
	}
	m_bOpen = m_Comm.OpenComm(port);

	dwBaud = m_ctrlBaudRateList.GetItemData(m_ctrlBaudRateList.GetCurSel());

	if (m_bOpen)
	{
		// 设置串口参数
		// 默认无校验，8位数据位，1位停止位
		BOOL bRet = m_Comm.SetCommState(dwBaud, NOPARITY, 8, ONESTOPBIT);
		if (!bRet) {
			m_Comm.CloseComm();
			OnAddMsgToListBox(_T("设置串口属性失败！"));
			return;
		}
		// 设置缓冲区大小
		bRet = m_Comm.SetupComm(RX_BUFSIZE, TX_BUFSIZE);
		if (!bRet) {
			m_Comm.CloseComm();
			OnAddMsgToListBox(_T("设置串口输入输出缓冲区失败！"));
			return;
		}
		// 清除错误状态
		bRet = m_Comm.PurgeComm(PURGE_TXABORT | PURGE_RXABORT | PURGE_RXCLEAR | PURGE_TXCLEAR);
		if (!bRet) {
			m_Comm.CloseComm();
			OnAddMsgToListBox(_T("清除串口状态失败！"));
			return;
		}
		// 设置监听事件
		bRet = m_Comm.SetCommMask(EV_RXCHAR);
		if (!bRet) {
			m_Comm.CloseComm();
			OnAddMsgToListBox(_T("设置串口事件失败！"));
			return;
		}
		// 启动接收线程
		m_Comm.StartComm();
		OnSetStatusListCtrl(FALSE);
		OnAddMsgToListBox( _T("Open Serial port successful!"));
		m_ctrlOpenCloseStatusIcon.SetIcon(m_hIconOpen);
		GetDlgItem(IDOK)->SetWindowText(_T("关闭"));
		m_bOpen = FALSE;
	}
	else
	{
		m_Comm.CloseComm();
		OnSetStatusListCtrl(TRUE);
		OnAddMsgToListBox(_T("Close Serial port successful!"));
		m_ctrlOpenCloseStatusIcon.SetIcon(m_hIconClose);
		GetDlgItem(IDOK)->SetWindowText(_T("打开"));
		OnSetStatusToDownLoadButton(FALSE);
		if (m_pWriteThread != NULL)
		{
			if (m_pWriteThread->m_bAutoDelete == FALSE)
			{
				m_pWriteThread->m_bAutoDelete = TRUE;
				m_pWriteThread->PostThreadMessage(WM_QUIT, NULL, NULL);
			}
		}
	}
	OnIntiProgressBar();
}


void CDownloadToolDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnCancel();
}


void CDownloadToolDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialogEx::OnCancel();
	//CDialogEx::OnClose();
}

// 初始化串口号列表控件
BOOL CDownloadToolDlg::OnInitSerialList()
{
	#define MAX_COMNAME_LENGTH	256 
	long i = 0;
	TCHAR portName[MAX_COMNAME_LENGTH] = { 0 }, commName[MAX_COMNAME_LENGTH] = { 0 };
	DWORD dwLong, dwSize;
	HKEY hKey;
	LONG nRetVal = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		_T("Hardware\\DeviceMap\\SerialComm"), NULL,
		KEY_READ, &hKey);

	TRACE(_T("%s,%d\r\n"), __FUNCTION__, __LINE__);
	m_ctrlSerialList.ResetContent();
	if (nRetVal == ERROR_SUCCESS)
	{
		while (1)
		{
			dwLong = dwSize = sizeof(portName)/sizeof(TCHAR) - 1;

			nRetVal = RegEnumValue(hKey, i, portName, &dwLong, NULL, NULL, (PUCHAR)commName, &dwSize);
			if (nRetVal == ERROR_NO_MORE_ITEMS) // 枚举串口
				break;
			CString strCommName;
			strCommName.Format(_T("%s"), commName);
			m_ctrlSerialList.AddString(strCommName); // 添加到列表
			i++;
		}
		if (m_ctrlSerialList.GetCount() == 0)
		{
			OnAddMsgToListBox(_T("[HKEY_LOCAL_MACHINE:Hardware\\DeviceMap\\SerialComm]里无串口!"));
			return FALSE;
		}
		RegCloseKey(hKey);
	}

	m_ctrlSerialList.SetCurSel(0);

	return TRUE;
}


// 初始化波特率列表
void CDownloadToolDlg::OnInitBaudRateList()
{
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("1200")), BAUD_1200_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("2400")), BAUD_2400_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("4800")), BAUD_4800_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("9600")), BAUD_9600_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("19200")), BAUD_19200_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("38400")), BAUD_38400_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("115200")), BAUD_115200_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("230400")), BAUD_230400_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("393939")), BAUD_393939_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("460800")), BAUD_460800_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("921600")), BAUD_921600_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("2000000")), BAUD_2000000_RATE);
	m_ctrlBaudRateList.SetItemData(m_ctrlBaudRateList.AddString(_T("3000000")), BAUD_3000000_RATE);

	m_ctrlBaudRateList.SetCurSel(BAUD_115200_INDEX);
}


// 初始化下载方式列表
void CDownloadToolDlg::OnInitDownloadMode()
{
	m_ctrlDownloadMode.SetItemData(m_ctrlDownloadMode.AddString(_T("UART")), UART_MODE);
	m_ctrlDownloadMode.SetItemData(m_ctrlDownloadMode.AddString(_T("I2C")), I2C_MODE);
	m_ctrlDownloadMode.SetItemData(m_ctrlDownloadMode.AddString(_T("SPI")), SPI_MODE);
	m_ctrlDownloadMode.SetItemData(m_ctrlDownloadMode.AddString(_T("GPIO")), GPIO_MODE);

	m_ctrlDownloadMode.SetCurSel(UART_MODE);
}

// 加载文件
void CDownloadToolDlg::OnBnClickedBtnloadfile()
{
	INT_PTR nRet = 0;
	static TCHAR szFilter[] = \
		_T("binary Files (*.bin)|*.bin|hex Files(*.hex)|*.hex")
		_T("|s19 Files(*.s19)|*.s19|mot Files(*.mot)|*.mot")
		_T("|axf Files(*.axf)|*.axf|All Files(*.*)|*.*|| ");

	CFileDialog dlg(
		TRUE, NULL, NULL, 
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		szFilter
		);

	nRet = dlg.DoModal();

	if (nRet == IDOK)
	{
		CString filename = dlg.GetFileName();
#ifdef _UNICODE
		strncpy_s((char *)m_fileInfo.file_name, MAX_FILENAME_SIZE - 1, CW2A(filename), filename.GetLength());
#else
		strncpy_s((char *)m_fileInfo.file_name, MAX_FILENAME_SIZE - 1, filename, filename.GetLength());
#endif
		if (strstr(".bin", (char *)(m_fileInfo.file_name)) == 0)
		{
			m_fileInfo.file_type = BINARY_FILE;
		}
		else if (strstr(".hex", (char*)(m_fileInfo.file_name)) == 0)
		{
			m_fileInfo.file_type = HEX_FILE;
		}
		else if (strstr(".s19", (char*)(m_fileInfo.file_name)) == 0)
		{
			m_fileInfo.file_type = S19_FILE;
		}
		else
		{
			m_fileInfo.file_type = UNKONW_FILE;
		}

		CFileStatus fileStatus;
		CFile::GetStatus(dlg.GetPathName(), fileStatus);
		m_fileInfo.file_size = (uint32_t)fileStatus.m_size;

		CString str;
#ifdef _UNICODE
		str.Format(_T("open file successful!!  file name:%s,size:%d"), CString(m_fileInfo.file_name), m_fileInfo.file_size);
#else
		str.Format(_T("open file successful!!  file name:%s,size:%d"), m_fileInfo.file_name, m_fileInfo.file_size);
#endif
		OnAddMsgToListBox(str);

		m_ctrlFilePath.SetWindowText(dlg.GetPathName());
	}
}

// 写文件进程
UINT __cdecl WriteProgramFileTask(LPVOID pParam)
{
	uint8_t* pbuf = (uint8_t*)pParam;
	uint32_t index = 0;

	CDownloadToolDlg* pDlg = (CDownloadToolDlg *)theApp.GetMainWnd();
	if (pDlg == NULL)
		return 1;

	CSerialPort* pComm = (CSerialPort *)pDlg->OnGetCommPortHandle();
	if (pComm == NULL)
		return 1;

	sFileType fileInfo = pDlg->OnGetFileinfo(); // 通过public接口获取需要下载的文件信息
	if(fileInfo.file_size <= 0)
		return 1;

	if (pDlg->GetHandStatus())
	{
		xymodem_send(&fileInfo, pbuf);
	}
#if 0
	OVERLAPPED m_ov;
	DWORD dwWrite = 0;
	while (index < fileInfo.file_size)
	{
		TRACE(_T("0x%x\r\n"), *(pbuf + index));
		pComm->WriteFile(&(pbuf[index]), 1, &dwWrite, &m_ov);
		index++;
		pDlg->OnUpdateProgress(100*index / fileInfo.file_size);
	}
#endif
	delete[] pbuf;
	return 0;
}

// 写入文件
void CDownloadToolDlg::OnBnClickedBtnwrite()
{
	CFile file;
	CString strPath, strMsg;
	BOOL bRet = TRUE;
	m_ctrlFilePath.GetWindowText(strPath);

	if (strPath.IsEmpty())
	{
		strMsg = _T("请先加载需要下载的文件再进行下载操作！");
		OnAddMsgToListBox(strMsg);
		MessageBox(strMsg, _T("警告"), MB_ICONWARNING);
		return;
	}
	bRet = file.Open(strPath, CFile::modeRead);

	if (FALSE == bRet)
	{
		strMsg = _T("打开文件失败！");
		OnAddMsgToListBox(strMsg);
		MessageBox(strMsg, _T("错误"), MB_ICONERROR);
		return;
	}
	m_pbuf = new uint8_t[m_fileInfo.file_size];
	if(m_pbuf == NULL)
	{
		strMsg = _T("内存分配失败！");
		OnAddMsgToListBox(strMsg);
		MessageBox(strMsg, _T("错误"), MB_ICONERROR);
		file.Close();
		return;
	}
	file.Read(m_pbuf, m_fileInfo.file_size);
	file.Close();

	// 创建写文件进程，传入程序文件缓冲区首地址
	m_pWriteThread = AfxBeginThread(WriteProgramFileTask, (LPVOID)m_pbuf);
	m_pWriteThread->m_bAutoDelete = FALSE;
}


// 设置下载相关按钮可操作与不可操作性
void CDownloadToolDlg::OnSetStatusToDownLoadButton(BOOL nCmdShow)
{
	// 默认不使能下载按钮功能
//	m_ctrlFilePath.EnableWindow(nCmdShow);
//	GetDlgItem(IDC_BTNLOADFILE)->EnableWindow(nCmdShow);
	GetDlgItem(IDC_BTNWRITE)->EnableWindow(nCmdShow);
}


// 添加需要显示的字符串到消息列表
void CDownloadToolDlg::OnAddMsgToListBox(CString szMsg)
{
	int nCount;
	m_ctrlMsgBox.InsertString(m_ctrlMsgBox.GetCount(), szMsg);
	nCount = m_ctrlMsgBox.GetCount() - 1;
	m_ctrlMsgBox.SetTopIndex(nCount);
	m_ctrlMsgBox.SetScrollPos(SB_VERT, nCount);
	m_ctrlMsgBox.SetCurSel(nCount);
}


// 获取打开的文件信息
const sFileType CDownloadToolDlg::OnGetFileinfo()
{
	return m_fileInfo;
}


// 初始化进度条
void CDownloadToolDlg::OnIntiProgressBar()
{
	m_ctrlProgress.SetBarColor(RGB(6, 176, 37)); // green
	m_ctrlProgress.SetRange32(0, 100);
	OnUpdateProgress(0);
}


// 设置进度百分比
void CDownloadToolDlg::OnUpdateProgress(DWORD dProgress)
{
	if (dProgress > 100)
		dProgress = 100;
	CString szStr;
	szStr.Format(_T("%d%%"), dProgress);
	m_progressStatus.SetWindowText(szStr);

	m_ctrlProgress.SetPos(dProgress);
}

// 获取窗口句柄
const CSerialPort* CDownloadToolDlg::OnGetCommPortHandle(void)
{
	return (&m_Comm);
}


void CDownloadToolDlg::OnCbnDropdownCombo1()
{
	OnInitSerialList();
}


// 获取握手状态
BOOL CDownloadToolDlg::GetHandStatus()
{
	return bHandOK;
}


// 设置串口列表，波特率列表和下载模式列表状态
void CDownloadToolDlg::OnSetStatusListCtrl(BOOL nCmdShow)
{
	GetDlgItem(IDC_COMBO1)->EnableWindow(nCmdShow);
	GetDlgItem(IDC_COMBO2)->EnableWindow(nCmdShow);
	GetDlgItem(IDC_COMBO3)->EnableWindow(nCmdShow);
}
