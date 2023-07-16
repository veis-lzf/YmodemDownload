#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

/* 传输协议类型 */
#define YMODEM	(0x01)
#define XMODEM	(0x02)
#define ZMODEM	(0x03)

#define PROTOCOL_TYPE	YMODEM


#define BOOT_CMD_HAND_CODE 				0xA5	 // 握手码
#define BOOT_CMD_READ_STARTINFO         0X10     // 读开机信息
#define BOOT_CMD_READ_BOOTSTATE         0X17     // 读boot状态
#define BOOT_CMD_READ_IAP_RDDATA        0X18     // 读APP区数据
#define BOOT_CMD_READ_SYSTICK           0X19     // 读系统TICK

#define BOOT_CMD_WRITE_IAP_CTRL         0X87     // 升级控制
#define BOOT_CMD_WRITE_IAP_WRDATA       0X88     // 写APP区数据
#define BOOT_CMD_WRITE_IAP_RES          0X89     // 升级结果

#define SOF


#endif /* __PROTOCOL_H__ */
