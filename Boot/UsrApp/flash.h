#ifndef __FLASH_H__
#define __FLASH_H__

#include "main.h"
#include "AppCfg.h"
#include "iap.h"

#if (PLATFORM_VERSION == STM32_F103)
	// FLASH起始地址
	#define STM32_FLASH_BASE	FLASH_BASE 	// STM32 FLASH的起始地址
	// FLASH 页大小
	#define STM32_PAGE_SIZE		0x400 // 1KB
	// 起始页
	#define STM32_PAGE_START	((FLASH_APP1_ADDR - FLASH_BASE)/1024)
	// 页总数64,flash总大小64*1KB
	#define MAX_PAGE_COUNT		64
	#define STM32_APP_PAGE_COUNT	(MAX_PAGE_COUNT - STM32_PAGE_START)
#elif(PLATFORM_VERSION == STM32_F407)
	// FLASH起始地址
	#define STM32_FLASH_BASE FLASH_BASE 	// STM32 FLASH的起始地址
	// FLASH 扇区的起始地址
	#define STM32_SECTOR0	0x08000000
#endif

// public:
typedef enum
{
	FLASH_ERROR = 0,
	FLASH_ERASE_ERROR,
	FLASH_WRITE_ERROR,
	FLASH_READ_ERROR,
	FLASH_COMPLETE,
	FLASH_LOCK,
	FLASH_UNLOCK,
} FlashStatus;

// public:
FlashStatus FLASH_Erase(uint32_t addr, uint32_t size); // 从指定地址开始擦除指定长度
uint32_t FLASH_ReadWord(uint32_t addr);	// 读出字
FlashStatus FLASH_Write(uint32_t WriteAddr,uint32_t *pBuffer,uint32_t NumToWrite); // 从指定地址开始写入指定长度的数据
void FLASH_Read(uint32_t ReadAddr,uint32_t *pBuffer,uint32_t NumToRead); // 从指定地址开始读出指定长度的数据

#endif /* __FLASH_H__ */
