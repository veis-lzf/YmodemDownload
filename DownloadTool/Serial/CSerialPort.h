#pragma once

#include "CThreadComm.h"

// 缓冲区大小
#define TX_BUFSIZE (4*1024)
#define RX_BUFSIZE (4*1024)

class CSerialPort
{
	HANDLE m_hComm;
	DCB m_dcb;
	CThreadComm m_ThreadComm; // 创建线程使用，重写runTask函数实现串口任务处理

	void ShowError(DWORD dwError);
protected:
	
public:
	CSerialPort();
	~CSerialPort(); // 析构函数

	BOOL isOpen();
	// 打开串口
	BOOL OpenComm(CString szComm); 
	// 设置串口参数
	BOOL SetCommState(DWORD dwBaudrate, BYTE byParity, BYTE byByteSize, BYTE byStopBits); 
	// 设置发送和接收缓冲区大小
	BOOL SetupComm(DWORD dwInQueue, DWORD dwOutQueue); 
	// 清除串口标志
	BOOL PurgeComm(DWORD dwFlags); 
	// 设置串口事件类型
	BOOL SetCommMask(DWORD dwEvtMask); 
	// 写文件操作
	BOOL WriteFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	// 读文件操作
	BOOL ReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped); 
	// 获取重叠操作状态
	BOOL GetOverlappedResult(LPOVERLAPPED lpOverlaped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait);
	// 清除串口通信错误
	BOOL ClearCommError(LPDWORD lpErrors, LPCOMSTAT lpStat);
	// 获取串口参数
	DCB GetCommState(void);
	// 设置流控
	BOOL SetFlowCtrl(void);
	// 获取线路状态
	int GetLineStatus(void);
	// 设置DTR状态
	BOOL SetDTR(BOOL enable);
	// 设置RTS状态
	BOOL SetRTS(BOOL enable);
	// 关闭串口
	BOOL CloseComm(void);

public:
	// 启动串口接收线程
	void StartComm(void);

	uint8_t UART_ReceivePacket(uint8_t* data, uint16_t length, uint32_t timeout);
	void UART_SendByte(uint8_t c);
	void UART_SendPacket(uint8_t* data, uint16_t length);
	uint8_t UART_ReceiveByte(uint8_t* c, uint32_t timeout);
};

