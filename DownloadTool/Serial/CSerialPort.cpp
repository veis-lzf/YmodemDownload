#include "../pch.h"
#include "../framework.h"
#include "CSerialPort.h"
#include "../DownloadTool.h"
#include "../DownloadToolDlg.h"
#include "../Ymodem/Ymodem.h"

#define DEBUG_MODE 1

#if DEBUG_MODE
// 如果不用于界面程序开发，把MessageBox更换为相应的日志输出函数即可
void CSerialPort::ShowError(DWORD dwError) // 打印显示错误信息
{
    LPVOID lpMsgBuf;
    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dwError,
        0, // Default language
        (LPTSTR)&lpMsgBuf,
        0,
        NULL
    );
    // Display the string.
    MessageBox(NULL, (LPCTSTR)lpMsgBuf, _T("Error"), MB_OK | MB_ICONINFORMATION);
    // Free the buffer.
    LocalFree(lpMsgBuf);
}
#else
void CSerialPort::ShowError(DWORD dwError)
{
}
#endif

CSerialPort::CSerialPort()
{
	m_hComm = NULL;
    memset(&m_dcb, 0, sizeof(m_dcb));
}

CSerialPort::~CSerialPort()
{
    CloseComm();
}

BOOL CSerialPort::isOpen()
{
    if (m_hComm == NULL)
        return FALSE;

    return TRUE;
}

// 打开串口
BOOL CSerialPort::OpenComm(CString szComm)
{
	if (m_hComm == NULL) {
		m_hComm = ::CreateFile(szComm, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
		if (INVALID_HANDLE_VALUE == m_hComm) {
            ShowError(GetLastError());
            m_hComm = NULL;
            return FALSE;
		}
        return TRUE;
	}
	return FALSE;
}

// 设置串口参数
BOOL CSerialPort::SetCommState(DWORD dwBaudrate, BYTE byParity, BYTE byByteSize, BYTE byStopBits)
{
    if (NULL == m_hComm) return FALSE;
    
    // 1.获取串口参数
    BOOL bRet = ::GetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }

    // 2.修改串口参数
    m_dcb.BaudRate = dwBaudrate;
    m_dcb.ByteSize = byByteSize;
    m_dcb.StopBits = byStopBits;
    m_dcb.Parity = byParity;
    bRet = ::SetCommState(m_hComm, &m_dcb);
    if (!bRet) {
        if (m_hComm) {
            ShowError(GetLastError());
            CloseHandle(m_hComm);
            m_hComm = NULL;
        }
        return FALSE;
    }

    return TRUE;
}

// 设置缓冲区大小
BOOL CSerialPort::SetupComm(DWORD dwInQueue, DWORD dwOutQueue)
{
    if (NULL == m_hComm) return FALSE;

    return ::SetupComm(m_hComm, dwInQueue, dwOutQueue);
}

// 清除串口标志位
BOOL CSerialPort::PurgeComm(DWORD dwFlags)
{
    if (NULL == m_hComm) return FALSE;

    return ::PurgeComm(m_hComm, dwFlags);
}

// 设置串口事件类型
BOOL CSerialPort::SetCommMask(DWORD dwEvtMask)
{
    if (NULL == m_hComm) return FALSE;

    return ::SetCommMask(m_hComm, dwEvtMask);
}

// 写文件操作
BOOL CSerialPort::WriteFile(LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
    if (NULL == m_hComm) return FALSE;

    return ::WriteFile(m_hComm, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
}

// 读文件操作
BOOL CSerialPort::ReadFile(LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
    if (NULL == m_hComm) return FALSE;

    return ::ReadFile(m_hComm, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
}

// 返回重叠操作结果
BOOL CSerialPort::GetOverlappedResult(LPOVERLAPPED lpOverlaped, LPDWORD lpNumberOfBytesTransferred, BOOL bWait)
{
    if (NULL == m_hComm) return FALSE;

    return ::GetOverlappedResult(m_hComm, lpOverlaped, lpNumberOfBytesTransferred, bWait);
}

// 清除串口通信错误
BOOL CSerialPort::ClearCommError(LPDWORD lpErrors, LPCOMSTAT lpStat)
{
    if (NULL == m_hComm) return FALSE;

    return ::ClearCommError(m_hComm, lpErrors, lpStat);
}

// 获取串口参数
DCB CSerialPort::GetCommState()
{
    return m_dcb;
}

// 设置硬件握手，流量控制
// 不允许在串口打开之后还可设置
int CSerialPort::SetFlowCtrl()
{
    if (m_hComm != NULL) return FALSE;

    m_dcb.fOutxCtsFlow = TRUE;
    m_dcb.fRtsControl = TRUE;
    return TRUE;
}

// 获取线路状态
int CSerialPort::GetLineStatus()
{
    DWORD dwEvtmask;
    int ret = 0;
    if (m_hComm == NULL) return FALSE;

    if (GetCommModemStatus(m_hComm, &dwEvtmask) == 0) {
        return FALSE;
    }
    ret = dwEvtmask;
    return ret;
}

// 设置DTR线状态
BOOL CSerialPort::SetDTR(BOOL enable)
{
    if (m_hComm == NULL) return FALSE;

    DWORD data = CLRDTR;
    if (enable) data = SETDTR;
    if (EscapeCommFunction(m_hComm, data)) return TRUE;
    return FALSE;
}

// 设置RTS线状态
BOOL CSerialPort::SetRTS(BOOL enable)
{
    if (m_hComm == NULL) return FALSE;

    DWORD data = CLRRTS;
    if (enable) data = SETRTS;
    if (EscapeCommFunction(m_hComm, data)) return TRUE;
    return FALSE;
}

// 关闭串口
BOOL CSerialPort::CloseComm()
{
    // 回收资源
    if (m_hComm == NULL) return FALSE;
    BOOL bRet = CloseHandle(m_hComm);
    m_hComm = NULL;

	// 关闭线程
	m_ThreadComm.StopThread();

    return bRet;
}

// 启动接收线程
void CSerialPort::StartComm(void)
{
    m_ThreadComm.SetThreadData((DWORD)this); // 传入串口类的this指针
    m_ThreadComm.StartThread(); // 启动线程
}

/*
*********************************************************************************************************
*	函 数 名: UART_ReceivePacket
*	功能说明: 接收发送端发来的字符
*	形    参：data  数据
*             timeout  溢出时间
*	返 回 值: 0 接收成功， -1 接收失败
*********************************************************************************************************
*/
uint8_t CSerialPort::UART_ReceivePacket(uint8_t* data, uint16_t length, uint32_t timeout)
{
	BOOL  bRead = TRUE;
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	int cnt = 0;    
    OVERLAPPED m_ov;
    COMSTAT comstat;
	timeout = timeout;

	while (1)
	{
        memset(&comstat, 0, sizeof(COMSTAT));
		bResult = ClearCommError(NULL, &comstat);

		if (comstat.cbInQue == 0)
		{
			Sleep(10);
			if ((timeout--) < 100)
			{
				TRACE(_T("Receive_Byte failed\r\n"));
				return -1;
			}
			continue;
		}

		memset(&m_ov, 0, sizeof(OVERLAPPED));
		bResult = ReadFile(
			data,       /* RX Buffer Pointer */
			length,     /* Read one byte */
			&BytesRead, /* Stores number of bytes read */
			&m_ov);     /* pointer to the m_ov structure */

		PurgeComm(PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
        TRACE("BytesRead=%d,data[0]=0x%02x,data[1]=0x%02x\r\n", BytesRead, data[0], data[1]);
		return 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Uart_SendByte
*	功能说明: 发送一个字节数据
*	形    参：c  字符
*	返 回 值: 0
*********************************************************************************************************
*/
void CSerialPort::UART_SendByte(uint8_t c)
{
	DWORD BytesSent;
    OVERLAPPED m_ov;

    memset(&m_ov, 0, sizeof(OVERLAPPED));
	WriteFile(
		&c, 				/* Pointer to message buffer in calling function */
		1,       		/* Length of message to send */
		&BytesSent,      /* Where to store the number of bytes sent */
		&m_ov);       	/* Overlapped structure */
}

/*
*********************************************************************************************************
*	函 数 名: UART_SendPacket
*	功能说明: 发送一串数据
*	形    参: data  数据
*             length  数据长度
*	返 回 值: 无
*********************************************************************************************************
*/
void CSerialPort::UART_SendPacket(uint8_t* data, uint16_t length)
{
	DWORD BytesSent;
	OVERLAPPED m_ov;

    memset(&m_ov, 0, sizeof(OVERLAPPED));
	WriteFile(
		data, 		/* Pointer to message buffer in calling function */
		length,       /* Length of message to send */
		&BytesSent,   /* Where to store the number of bytes sent */
		&m_ov);       /* Overlapped structure */
}

/*
*********************************************************************************************************
*	函 数 名: UART_ReceiveByte
*	功能说明: 接收发送端发来的字符
*	形    参：c  字符
*             timeout  溢出时间
*	返 回 值: 0 接收成功， -1 接收失败
*********************************************************************************************************
*/
uint8_t CSerialPort::UART_ReceiveByte(uint8_t* c, uint32_t timeout)
{
    BOOL  bRead = TRUE;
    BOOL  bResult = TRUE;
    DWORD dwError = 0;
    DWORD BytesRead = 0;
    char RXBuff;
    int cnt = 0;
    OVERLAPPED m_ov;
    COMSTAT comstat;
	CDownloadToolDlg* pDlg = (CDownloadToolDlg*)theApp.GetMainWnd();
	timeout = timeout;

	while (1)
	{
        memset(&comstat, 0, sizeof(COMSTAT));
		bResult =ClearCommError(NULL, &comstat);

		if (comstat.cbInQue == 0)
		{
			Sleep(10);
			if ((timeout--) < 100)
			{
				TRACE(_T("Receive_Byte failed\r\n"));
				return -1;
			}
			continue;
		}

        memset(&m_ov, 0, sizeof(OVERLAPPED));
        m_ov.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);
		
        bResult = ReadFile(
            &RXBuff,          /* RX Buffer Pointer */
			1,          /* Read one byte */
			&BytesRead, /* Stores number of bytes read */
			&m_ov);     /* pointer to the m_ov structure */

		if (m_ov.hEvent)
			CloseHandle(m_ov.hEvent);

		if (!bResult)
		{
			if (ERROR_IO_PENDING == GetLastError())
			{
                WaitForSingleObject(m_ov.hEvent, 1000);
			}
		}
		PurgeComm(PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
       
        *c = RXBuff;
		TRACE(_T("ReceiveByte = 0x%02x\r\n"), *c);

		return 0;
	}
}
