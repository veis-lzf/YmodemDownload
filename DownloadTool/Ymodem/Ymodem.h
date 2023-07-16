#pragma once

#include "../pch.h"
#include "../Serial/CSerialPort.h"
#include "../DownloadToolDlg.h"
#include "../DownloadTool.h"
#include "../protocol.h"

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)

#define NAK_TIMEOUT             (0x100000)
#define MAX_ERRORS              (5)



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
void Ymodem_PrepareIntialPacket(uint8_t* data, const uint8_t* fileName, uint32_t* length);

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
void Ymodem_PreparePacket(uint8_t* SourceBuf, uint8_t* data, uint8_t pktNo, uint32_t sizeBlk);


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
uint8_t Ymodem_Transmit(uint8_t* buf, const uint8_t* sendFileName, uint32_t sizeFile);

/*
*********************************************************************************************************
*	函 数 名: xymodem_send
*	功能说明: 发送文件
*	形    参: filename  文件名
*	返 回 值: 0  文件发送成功
*********************************************************************************************************
*/
int xymodem_send(sFileType* file, uint8_t* buf);
