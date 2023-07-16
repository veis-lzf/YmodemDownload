#include "../pch.h"
#include "../framework.h"
#include "CThreadComm.h"
#include "../DownloadToolDlg.h"
#include "../DownloadTool.h"
#include "CSerialPort.h"
#include "../Ymodem/ymodem.h"


CThreadComm::CThreadComm(void)
{
}

CThreadComm::~CThreadComm(void)
{
}

void CThreadComm::runTask(void)
{
#define RX_TASK_SLEEP_TIME	500
	CDownloadToolDlg *pComDlg = (CDownloadToolDlg*)theApp.GetMainWnd(); // 获取主对话框指针
	if (pComDlg == NULL)
		return;

	CSerialPort *pSerialPort = (CSerialPort *)GetThreadData(); // 获取串口类指针
	if (pSerialPort == NULL)
		return;

	DWORD dwError = 0;
	COMSTAT comStat;
	OVERLAPPED m_ov;
	BOOL bRet = TRUE;
	uint8_t BytesRead;
	DWORD dwRead = 0;
	//uint8_t err_count = 0;

	while (TRUE)
	{
		memset(&comStat, 0, sizeof(COMSTAT));
		pSerialPort->ClearCommError(&dwError, &comStat);
		if (comStat.cbInQue == 0)
		{
			//err_count++;
			//TRACE(_T("error:%d\r\n"), err_count);
			//if (err_count >= 50)
			//{
			//	TRACE(_T("wait timeout\r\n"));
			//	break;
			//}
			if (m_hThread == NULL)
			{
				break;
			}
			TRACE(_T("wait timeout\r\n"));
			goto SLEEP;
		}
		else
		{
			memset(&m_ov, 0, sizeof(OVERLAPPED));
			m_ov.hEvent = ::CreateEvent(NULL, TRUE, FALSE, NULL);

			bRet = pSerialPort->ReadFile(
				&BytesRead, /* RX Buffer Pointer */
				1,		    /* Read one byte */
				&dwRead,    /* Stores number of bytes read */
				&m_ov);     /* pointer to the m_ov structure */

			if (m_ov.hEvent)
				CloseHandle(m_ov.hEvent);

			if (!bRet)
			{
				if (ERROR_IO_PENDING == GetLastError())
				{
					WaitForSingleObject(m_ov.hEvent, 1000);
				}
			}
			pSerialPort->PurgeComm(PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

			TRACE(_T("read count:%d, 0x%02x\r\n"), dwRead, BytesRead);
			if (BytesRead == CRC16)
			{
				PostMessage(pComDlg->GetSafeHwnd(), WM_HAND_OK_MSG, (WPARAM)WM_HAND_OK_MSG, NULL);
				break;
			}
		}
SLEEP:
		Sleep(RX_TASK_SLEEP_TIME);
	}
	this->StopThread();
}
