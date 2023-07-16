#ifndef __IAP_H__
#define __IAP_H__

#include "main.h"

// 定义一个函数类型的参数. 
typedef  void (*iapfun)(void);

// app地址，bootloader大小16kB
#define FLASH_APP1_ADDR    (uint32_t)(FLASH_BASE | 0x00004000)

void iap_load_app(uint32_t appxaddr); // 跳转到APP程序执行
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t applen);	// 在指定地址开始,写入bin
void WFI_SET(void);		     // 执行WFI指令
void INTX_DISABLE(void);     // 关闭所有中断
void INTX_ENABLE(void);	     // 开启所有中断
void MSR_MSP(uint32_t addr); // 设置堆栈地址
void SoftReset(void);        // 软复位

#endif /* __IAP_H__ */
