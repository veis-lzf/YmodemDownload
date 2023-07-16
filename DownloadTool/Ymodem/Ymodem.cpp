
#include "../pch.h"
#include "../Ymodem/Ymodem.h"
#include "../Serial/CSerialPort.h"

/*
*********************************************************************************************************
*	                                   Ymodem文件传输协议介绍
*********************************************************************************************************
*/
/*
第1阶段： 同步
	从机给数据发送同步字符 C

第2阶段：发送第1帧数据，包含文件名和文件大小
	主机发送：
	---------------------------------------------------------------------------------------
	| SOH  |  序号 - 0x00 |  序号取反 - 0xff | 128字节数据，含文件名和文件大小字符串|CRC0 CRC1|
	|-------------------------------------------------------------------------------------|
	从机接收：
	接收成功回复ACK和CRC16，接收失败（校验错误，序号有误）继续回复字符C，超过一定错误次数，回复两个CA，终止传输。

第3阶段：数据传输
	主机发送：
	---------------------------------------------------------------------------------------
	| SOH/STX  |  从0x01开始序号  |  序号取反 | 128字节或者1024字节                |CRC0 CRC1|
	|-------------------------------------------------------------------------------------|
	从机接收：
	接收成功回复ACK，接收失败（校验错误，序号有误）或者用户处理失败继续回复字符C，超过一定错误次数，回复两个CA，终止传输。

第4阶段：结束帧
	主机发送：发送EOT结束传输。
	从机接收：回复ACK。

第5阶段：空帧，结束通话
	主机发送：一帧空数据。
	从机接收：回复ACK。
*/


/*
*********************************************************************************************************
*	函 数 名: Int2Str
*	功能说明: 将整数转换成字符
*	形    参: str 字符  intnum 整数
*	返 回 值: 无
*********************************************************************************************************
*/
void Int2Str(uint8_t* str, int32_t intnum)
{
	uint32_t i, Div = 1000000000, j = 0, Status = 0;

	for (i = 0; i < 10; i++)
	{
		str[j++] = (intnum / Div) + 48;

		intnum = intnum % Div;
		Div /= 10;
		if ((str[j - 1] == '0') && (Status == 0))
		{
			j = 0;
		}
		else
		{
			Status++;
		}
	}
}

/*
*********************************************************************************************************
*	函 数 名: Ymodem_PrepareIntialPacket
*	功能说明: 准备第一包要发送的数据
*	形    参: data 数据
*             fileName 文件名
*             length   文件大小
*	返 回 值: 0
*********************************************************************************************************
*/
void Ymodem_PrepareIntialPacket(uint8_t* data, const uint8_t* fileName, uint32_t* length)
{
	uint16_t i, j;
	uint8_t file_ptr[10];

	/* 第一包数据的前三个字符  */
	data[0] = SOH; /* soh表示数据包是128字节 */
	data[1] = 0x00;
	data[2] = 0xff;

	/* 文件名 */
	for (i = 0; (fileName[i] != '\0') && (i < FILE_NAME_LENGTH); i++)
	{
		data[i + PACKET_HEADER] = fileName[i];
	}

	data[i + PACKET_HEADER] = 0x00;

	/* 文件大小转换成字符 */
	_itot_s(*length, (TCHAR *)file_ptr, sizeof(file_ptr), 10);

	for (j = 0, i = i + PACKET_HEADER + 1; file_ptr[j] != '\0'; )
	{
		data[i++] = file_ptr[j++];
	}
	data[i + PACKET_HEADER] = 0x00;

	/* 其余补0 */
	for (j = i; j < PACKET_SIZE + PACKET_OVERHEAD; j++)
	{
		data[j] = 0;
	}
}

/*
*********************************************************************************************************
*	函 数 名: Ymodem_PreparePacket
*	功能说明: 准备发送数据包
*	形    参: SourceBuf 要发送的原数据
*             data      最终要发送的数据包，已经包含的头文件和原数据
*             pktNo     数据包序号
*             sizeBlk   要发送数据数
*	返 回 值: 无
*********************************************************************************************************
*/
int sendsize = 0;
void Ymodem_PreparePacket(uint8_t* SourceBuf, uint8_t* data, uint8_t pktNo, uint32_t sizeBlk)
{
	uint16_t i, size, packetSize;
	uint8_t* file_ptr;

	/* 设置好要发送数据包的前三个字符data[0]，data[1]，data[2] */
	/* 根据sizeBlk的大小设置数据区数据个数是取1024字节还是取128字节*/
	packetSize = sizeBlk >= PACKET_1K_SIZE ? PACKET_1K_SIZE : PACKET_SIZE;

	/* 数据大小进一步确定 */
	size = sizeBlk < packetSize ? sizeBlk : packetSize;

	/* 首字节：确定是1024字节还是用128字节 */
	if (packetSize == PACKET_1K_SIZE)
	{
		data[0] = STX;
	}
	else
	{
		data[0] = SOH;
	}

	/* 第2个字节：数据序号 */
	data[1] = pktNo;
	/* 第3个字节：数据序号取反 */
	data[2] = (~pktNo);
	file_ptr = SourceBuf;

	/* 填充要发送的原始数据 */
	for (i = PACKET_HEADER; i < size + PACKET_HEADER; i++)
	{
		data[i] = *file_ptr++;
	}

	/* 不足的补 EOF (0x1A) 或 0x00 */
	if (size <= packetSize)
	{
		for (i = size + PACKET_HEADER; i < packetSize + PACKET_HEADER; i++)
		{
			data[i] = 0x1A; /* EOF (0x1A) or 0x00 */
		}
	}

	sendsize += size;

	TRACE(_T("SendSize = %d\r\n"), sendsize);
	::PostMessage((HWND)theApp.GetMainWnd()->m_hWnd, WM_WRITE_PACK_FINISH_MSG, (WPARAM)WM_WRITE_PACK_FINISH_MSG, (LPARAM)&sendsize);
}

/*
*********************************************************************************************************
*	函 数 名: UpdateCRC16
*	功能说明: 上次计算的CRC结果 crcIn 再加上一个字节数据计算CRC
*	形    参: crcIn 上一次CRC计算结果
*             byte  新添加字节
*	返 回 值: 无
*********************************************************************************************************
*/
uint16_t UpdateCRC16(uint16_t crcIn, uint8_t byte)
{
	uint32_t crc = crcIn;
	uint32_t in = byte | 0x100;

	do
	{
		crc <<= 1;
		in <<= 1;
		if (in & 0x100)
			++crc;
		if (crc & 0x10000)
			crc ^= 0x1021;
	} while (!(in & 0x10000));

	return crc & 0xffffu;
}

/*
*********************************************************************************************************
*	函 数 名: Cal_CRC16
*	功能说明: 计算一串数据的CRC
*	形    参: data  数据
*             size  数据长度
*	返 回 值: CRC计算结果
*********************************************************************************************************
*/
uint16_t Cal_CRC16(const uint8_t* data, uint32_t size)
{
	uint32_t crc = 0;
	const uint8_t* dataEnd = data + size;

	while (data < dataEnd)
		crc = UpdateCRC16(crc, *data++);

	crc = UpdateCRC16(crc, 0);
	crc = UpdateCRC16(crc, 0);

	return crc & 0xffffu;
}

/*
*********************************************************************************************************
*	函 数 名: CalChecksum
*	功能说明: 计算一串数据总和
*	形    参: data  数据
*             size  数据长度
*	返 回 值: 计算结果的后8位
*********************************************************************************************************
*/
uint8_t CalChecksum(const uint8_t* data, uint32_t size)
{
	uint32_t sum = 0;
	const uint8_t* dataEnd = data + size;

	while (data < dataEnd)
		sum += *data++;

	return (sum & 0xffu);
}

/*
*********************************************************************************************************
*	函 数 名: Ymodem_Transmit
*	功能说明: 发送文件
*	形    参: buf  文件数据
*             sendFileName  文件名
*             sizeFile    文件大小
*	返 回 值: 0  文件发送成功
*********************************************************************************************************
*/
uint8_t Ymodem_Transmit(uint8_t* buf, const uint8_t* sendFileName, uint32_t sizeFile)
{
	uint8_t packet_data[PACKET_1K_SIZE + PACKET_OVERHEAD] = { 0 };
	uint8_t filename[FILE_NAME_LENGTH];
	uint8_t* buf_ptr, tempCheckSum;
	uint16_t tempCRC;
	uint16_t blkNumber;
	uint8_t receivedC[64] = {0}, CRC16_F = 0, i;
	uint32_t errors, ackReceived, size = 0, pktSize;

	CDownloadToolDlg* pDlg = (CDownloadToolDlg*)theApp.GetMainWnd();
	CSerialPort *pCom = (CSerialPort *)pDlg->OnGetCommPortHandle();
	errors = 0;
	ackReceived = 0;
	for (i = 0; i < (FILE_NAME_LENGTH - 1); i++)
	{
		filename[i] = sendFileName[i];
	}

	CRC16_F = 1;

	/* 初始化要发送的第一个数据包 */
	Ymodem_PrepareIntialPacket(&packet_data[0], filename, &sizeFile);

#if 0
	/* 等待第一个数据包可发送 */
	do
	{
		if (pCom->UART_ReceivePacket(&receivedC_start, 1, 10000) == 0)
		{
			if ((receivedC_start == CRC16))
			{
				/* 接收到应答 */
				ackReceived = 1;
			}
		}
		/* 没有等到 */
		else
		{
			errors++;
		}
	} while (!ackReceived && (errors < 0x0A));
#else
	ackReceived = 1;
#endif
	if (ackReceived)
	{
		ackReceived = 0;
		do
		{
			/* 根据CRC16_F发送CRC或者求和进行校验 */
			if (CRC16_F)
			{
				tempCRC = Cal_CRC16(&packet_data[PACKET_HEADER], PACKET_SIZE);
				packet_data[PACKET_HEADER + PACKET_SIZE] = (tempCRC >> 8);
				packet_data[PACKET_HEADER + PACKET_SIZE+1] = (tempCRC & 0xFF);
				pCom->UART_SendPacket(packet_data, PACKET_SIZE + PACKET_OVERHEAD);
			}
			else
			{
				tempCheckSum = CalChecksum(&packet_data[3], PACKET_SIZE);
				pCom->UART_SendByte(tempCheckSum);
			}

			/* 等待 Ack 和字符 'C' */
			if (pCom->UART_ReceivePacket(&receivedC[0], 2, 1000) == 0)
			{
				if ((receivedC[0] == ACK) && (receivedC[1] == CRC16))
				{
					/* 接收到应答 */
					ackReceived = 1;
				}
			}
			/* 没有等到 */
			else
			{
				errors++;
			}
			/* 发送数据包后接收到应答或者没有等到就推出 */
		} while (!ackReceived && (errors < 0x0A));
	}

	/* 超过最大错误次数就退出 */
	if (errors >= 0x0A)
	{
		return errors;
	}

	buf_ptr = buf;
	size = sizeFile;
	blkNumber = 0x01;

	/* 下面使用的是发送1024字节数据包 */
	/* Resend packet if NAK  for a count of 10 else end of communication */
	while (size)
	{
		/* 准备下一包数据 */
		Ymodem_PreparePacket(buf_ptr, &packet_data[0], (uint8_t)blkNumber, size);
		ackReceived = 0;
		receivedC[0] = 0;
		errors = 0;
		do
		{
			/* 发送下一包数据 */
			if (size >= PACKET_1K_SIZE)
			{
				pktSize = PACKET_1K_SIZE;
			}
			else
			{
				pktSize = PACKET_SIZE;
			}

			/* 根据CRC16_F发送CRC校验或者求和的结果 */
			if (CRC16_F)
			{
				tempCRC = Cal_CRC16(&packet_data[PACKET_HEADER], pktSize);
				packet_data[PACKET_HEADER + pktSize] = (tempCRC >> 8);
				packet_data[PACKET_HEADER + pktSize + 1] = (tempCRC & 0xFF);
				pCom->UART_SendPacket(packet_data, pktSize + PACKET_OVERHEAD);
			}
			else
			{
				tempCheckSum = CalChecksum(&packet_data[3], pktSize);
				pCom->UART_SendByte(tempCheckSum);
			}

			/* 等到Ack信号 */
			if ((pCom->UART_ReceiveByte(&receivedC[0], 1000) == 0) && (receivedC[0] == ACK))
			{
				ackReceived = 1;
				/* 修改buf_ptr位置以及size大小，准备发送下一包数据 */
				if (size > pktSize)
				{
					buf_ptr += pktSize;
					size -= pktSize;
					if (blkNumber == ((2 * 1024 * 1024) / 128))
					{
						return 0xFF; /* 错误 */
					}
					else
					{
						blkNumber++;
					}
				}
				else
				{
					buf_ptr += pktSize;
					size = 0;
				}
			}
			else
			{
				errors++;
			}

		} while (!ackReceived && (errors < 0x0A));

		/* 超过10次没有收到应答就退出 */
		if (errors >= 0x0A)
		{
			return errors;
		}
	}


	ackReceived = 0;
	receivedC[0] = 0x00;
	errors = 0;
	do
	{
		/* 发送EOT信号 */
		pCom->UART_SendByte(EOT);

		/* 等待Ack应答 */
		if (!ackReceived)
		{
			if ((pCom->UART_ReceiveByte(&receivedC[0], 1000) == 0))
			{
				if (receivedC[0] == NAK)
				{
					ackReceived++;
				}
			}
			else
			{
				errors++;
			}
		}
		else
		{
			if ((pCom->UART_ReceivePacket(receivedC, 2, 1000) == 0))
			{
				if (receivedC[0] == ACK && receivedC[1] == CRC16)
				{
					ackReceived++;
				}
			}
			else
			{
				errors++;
			}
		}

	} while ((ackReceived<2) && (errors < 0x0A));


	if (errors >= 0x0A)
	{
		return errors;
	}

	TRACE(_T("发送结束信号\r\n"));

#if 1
	/* 初始化最后一包要发送的数据 */
	ackReceived = 0;
	receivedC[0] = 0x00;
	errors = 0;

	packet_data[0] = SOH;
	packet_data[1] = 0;
	packet_data[2] = 0xFF;

	/* 数据包的数据部分全部初始化为0 */
	for (i = PACKET_HEADER; i < (PACKET_SIZE + PACKET_HEADER); i++)
	{
		packet_data[i] = 0x00;
	}

	do
	{
		/* 发送数据包 */
		/* 根据CRC16_F发送CRC校验或者求和的结果 */
		tempCRC = Cal_CRC16(&packet_data[PACKET_HEADER], PACKET_SIZE);
		packet_data[PACKET_HEADER + PACKET_SIZE] = (tempCRC >> 8);
		packet_data[PACKET_HEADER + PACKET_SIZE + 1] = (tempCRC & 0xFF);
		pCom->UART_SendPacket(packet_data, PACKET_SIZE + PACKET_OVERHEAD);

		/* 等待 Ack */
		if (pCom->UART_ReceiveByte(&receivedC[0], 1000) == 0)
		{
			if (receivedC[0] == ACK)
			{
				/* 数据包发送成功 */
				ackReceived = 1;
			}
		}
		else
		{
			errors++;
		}
	} while (!ackReceived && (errors < 0x0A));

	TRACE(_T("处理完毕\r\n"));

	/* 超过10次没有收到应答就退出 */
	if (errors >= 0x0A)
	{
		return errors;
	}
#endif

	return 0; /* 文件发送成功 */
}

/*
*********************************************************************************************************
*	函 数 名: xymodem_send
*	功能说明: 发送文件
*	形    参: filename  文件名
*	返 回 值: 0  文件发送成功
*********************************************************************************************************
*/
int xymodem_send(sFileType *file, uint8_t *buf)
{
	sendsize = 0;
	Ymodem_Transmit(buf, file->file_name, file->file_size);
	return 0;
}
