#pragma once

// 协议头
#define HEARD_ID		0xAA	// 协议头

// 协议标识
#define SEND_ID			0x9F	// 上位机发送标识
#define DEBUG_ID		0xF1	// 调试命令主标识
#define CHANG_BAUND_ID	0x88	// 更改串口波特率协议，立即生效

// 应答标识
#define CMD_ACK_ID		0x80	// 应答命令标识
#define DATA_ACK_ID		0x01	// 下位机数据应答标识
#define	HAND_CODE		0x80	// 握手码
#define	OUTPUT_CMD		0x7F	// 设置波形输出参数
#define	COOLECT_CMD		0x7E	// 上位机采集波形
#define GET_OUTPUTWAVE	0x79	// 读取波形输出的一个周期数据	

#define SETBAUND_CMD	0x00	// 设置波特率
#define READBAUND_CMD	0x01	// 读取波特率

// 下载通讯接口方式
typedef enum
{
	UART_MODE = 0,
	I2C_MODE,
	SPI_MODE,
	GPIO_MODE,
} eDownLoadMode;

// 波特率列表
typedef enum
{
	BAUD_1200_RATE    = 1200,
	BAUD_2400_RATE    = 2400,
	BAUD_4800_RATE    = 4800,
	BAUD_9600_RATE    = 9600,
	BAUD_19200_RATE   = 19200,
	BAUD_38400_RATE   = 38400,
	BAUD_115200_RATE  = 115200,
	BAUD_230400_RATE  = 230400,
	BAUD_393939_RATE  = 393939,
	BAUD_460800_RATE  = 460800,
	BAUD_921600_RATE  = 921600,
	BAUD_2000000_RATE = 2000000,
	BAUD_3000000_RATE = 3000000,
} eBaudRateList;

typedef enum
{
	BAUD_1200_INDEX,
	BAUD_2400_INDEX,
	BAUD_4800_INDEX,
	BAUD_9600_INDEX,
	BAUD_19200_INDEX,
	BAUD_38400_INDEX,
	BAUD_115200_INDEX,
	BAUD_230400_INDEX,
	BAUD_393939_INDEX,
	BAUD_460800_INDEX,
	BAUD_921600_INDEX,
	BAUD_2000000_INDEX,
	BAUD_3000000_INDEX,
} eBaudRateIndex;

// 下载文件类型
#define MAX_FILENAME_SIZE	256
typedef enum
{
	UNKONW_FILE, // 未知
	BINARY_FILE, // bin格式文件
	HEX_FILE, // hex格式文件
	S19_FILE, // s19格式文件
} eFileType;

typedef struct
{
	uint8_t file_name[MAX_FILENAME_SIZE]; // 文件名
	eFileType file_type; // 文件类型
	uint32_t file_size; // 文件大小
	uint32_t tm_modify; // 修改时间秒
} sFileType;
