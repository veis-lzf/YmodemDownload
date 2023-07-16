
// DownloadToolDlg.h: 头文件
//

#pragma once
#include "protocol.h"
#include "./Serial/CSerialPort.h"

/* 用户消息 */
typedef enum
{
	WM_HAND_OK_MSG = (WM_USER + 100),
	WM_START2WRITE_MSG,
	WM_WRITE_FINISH_MSG,
	WM_RECEIVE_ACK_MSG,
	WM_RECEIVE_NACK_MSG,
	WM_WRITE_PACK_FINISH_MSG,
} UsrMsg;

// CDownloadToolDlg 对话框
class CDownloadToolDlg : public CDialogEx
{
// 构造
public:
	CDownloadToolDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DOWNLOADTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;
	HICON m_hIconOpen;
	HICON m_hIconClose;
	BOOL m_bOpen;
	sFileType m_fileInfo;
	CSerialPort m_Comm;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT  OnHandlerUsrMsg(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
		
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedBtnloadfile();
	afx_msg void OnBnClickedBtnwrite();
	afx_msg void OnClose();
private:
	CComboBox m_ctrlSerialList;
	CComboBox m_ctrlBaudRateList;
	CComboBox m_ctrlDownloadMode;
	CListBox m_ctrlMsgBox;
	CProgressCtrl m_ctrlProgress;
	CEdit m_ctrlFilePath;
	CStatic m_progressStatus;
	CStatic m_ctrlOpenCloseStatusIcon;
	uint8_t* m_pbuf;
	BOOL bHandOK;
	CWinThread* m_pWriteThread;
private:
	// 初始化串口号列表控件
	BOOL OnInitSerialList(void);
	// 初始化波特率列表
	void OnInitBaudRateList(void);
	// 初始化下载方式列表
	void OnInitDownloadMode(void);
	// 设置下载相关按钮可操作与不可操作性
	void OnSetStatusToDownLoadButton(BOOL nCmdShow);
	// 初始化进度条
	void OnIntiProgressBar();
public:
	// 获取打开的文件信息
	const sFileType OnGetFileinfo(void);
	// 添加需要显示的字符串到消息列表
	void OnAddMsgToListBox(CString szMsg);
	// 设置进度百分比
	void OnUpdateProgress(DWORD dProgress);
	// 获取串口句柄
	const CSerialPort* OnGetCommPortHandle(void);
	afx_msg void OnCbnDropdownCombo1();
	// 获取握手状态
	BOOL GetHandStatus();
	// 设置串口列表，波特率列表和下载模式列表状态
	void OnSetStatusListCtrl(BOOL nCmdShow);
};
