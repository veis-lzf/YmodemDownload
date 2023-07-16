/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __YMODEM_H__
#define __YMODEM_H__

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
/* Exported types ------------------------------------------------------------*/
#include "AppCfg.h"
#include "Updfile.h"
/* Exported constants --------------------------------------------------------*/
#if defined(ENABLE_USB) || defined(ENABLE_UART_RX_DMA)
 #define RX_BUFFER_SIZE  (2048+8)
#else
 #define RX_BUFFER_SIZE  (128+8)
#endif
/* Exported macro ------------------------------------------------------------*/
#define PACKET_FILEINFO_INDEX   (0)
#define PACKET_SEQNO_INDEX      (1)
#define PACKET_SEQNO_COMP_INDEX (2)

#define PACKET_HEADER           (3)
#define PACKET_TRAILER          (2)
#define PACKET_OVERHEAD         (PACKET_HEADER + PACKET_TRAILER)
#define PACKET_SIZE             (128)
#define PACKET_1K_SIZE          (1024)

#define FILE_NAME_LENGTH        (256)
#define FILE_SIZE_LENGTH        (16)
#define FILE_MODIFYSEC_LENGTH   (16)
#define FILE_SUFFIX_LENGTH      (10)

#define SOH                     (0x01)  /* start of 128-byte data packet */
#define STX                     (0x02)  /* start of 1024-byte data packet */
#define EOT                     (0x04)  /* end of transmission */
#define ACK                     (0x06)  /* acknowledge */
#define NAK                     (0x15)  /* negative acknowledge */
#define CA                      (0x18)  /* two of these in succession aborts transfer */
#define CRC16                   (0x43)  /* 'C' == 0x43, request 16-bit CRC */

#define ABORT1                  (0x41)  /* 'A' == 0x41, abort by user */
#define ABORT2                  (0x61)  /* 'a' == 0x61, abort by user */

#define NAK_TIMEOUT             500 // ms (0x100000)
#define MAX_ERRORS              (5)

typedef enum
{
	YMODEM_ABORTED = 0x01, // 结束传输
	YMODEM_CONTINUE, // 继续接收
	YMODEM_TOO_MANY_ERRORS, // 错误累计超过MAX_ERRORS结束传输
	YMODEM_DONE, // 传输完成
	YMODEM_NO_FILE, // 空文件
	YMODEM_INVALID_FILE_SIZE, // 文件大小无效
	YMODEM_FILE_INFO_RECV, // 文件信息
} eYmodemStatus;

/* Exported functions ------------------------------------------------------- */
typedef void (*pfvCallBack)(uint8_t *buf, uint32_t size); // 回调函数指针
typedef void (*pfvSendCallBack)(uint8_t dat);

void Ymodem_SendAck(void);
void Ymodem_SendNAck(void);
void Ymodem_Abort(void);
void Ymodem_SendCRC16(void);
void Ymodem_Init(pfvCallBack func, pfvSendCallBack send_func);
eYmodemStatus Ymodem_Receive(uint8_t *buf, uint32_t buf_size, 
	sFileType *file_info, uint32_t * recv_size);

#endif  /* __YMODEM_H_ */

