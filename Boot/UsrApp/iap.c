/*******************************************************************************
* 文件名          : iap.c
* 作者            : veis
* 时间		      : 07/06/2023
* 文件描述        : IAP升级
********************************************************************************/
#include "iap.h"
#include "flash.h"
#include "usart.h"

iapfun jump2app; 

#define FIFO_BASE			0x400
#define IAP_BUFFER_SIZE		256
uint32_t iapbuf[IAP_BUFFER_SIZE] __attribute__((at(SRAM_BASE | FIFO_BASE))); // 1K字节缓存
//uint32_t iapbuf[512]; // 2K字节缓存

// 软复位
void SoftReset(void)
{
	NVIC_SystemReset();
}

//THUMB指令不支持汇编内联
//采用如下方法实现执行汇编指令WFI  
__asm void WFI_SET(void)
{
	WFI;		  
}
//关闭所有中断(但是不包括fault和NMI中断)
__asm void INTX_DISABLE(void)
{
	CPSID   I
	BX      LR	  
}
//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
//设置栈顶地址
//addr:栈顶地址
__asm void MSR_MSP(uint32_t addr) 
{
	MSR MSP, r0 			//set Main Stack value
	BX r14
}


// 跳转到应用程序段
// appxaddr:用户代码起始地址.
void iap_load_app(uint32_t appxaddr)
{
	INTX_DISABLE();
	if(((*(__IO uint32_t*)appxaddr)& 0x2FFE0000) == SRAM_BASE)	// 检查栈顶地址是否合法
	{ 
		jump2app = (iapfun)*(__IO uint32_t*)(appxaddr + 4);		// 用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(__IO uint32_t*)appxaddr);					    // 初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();									            // 跳转到APP
	}
	else
	{
		p_err("jump to app failed!");
		p_err("wait to software reset...");
		SoftReset();
	}
}

// appxaddr:应用程序的起始地址
// appbuf:应用程序CODE.
// appsize:应用程序大小(字节).
void iap_write_appbin(uint32_t appxaddr, uint8_t *appbuf, uint32_t appsize)
{
	uint32_t t;
	uint16_t count = 0;
	uint32_t temp;
	uint8_t *dfu = appbuf;
	
	for(t = 0; t < appsize; t += 4)
	{						   
		temp =(uint32_t)(dfu[3] << 24);   
		temp |=(uint32_t)(dfu[2] << 16);    
		temp |=(uint32_t)(dfu[1] << 8);
		temp |=(uint32_t)(dfu[0]);	  
		dfu  += 4;//偏移4个字节
		iapbuf[count++] = temp;	    
		if(count == IAP_BUFFER_SIZE)
		{
			count = 0; 
			FLASH_Write(appxaddr, iapbuf, 512);
			appxaddr += IAP_BUFFER_SIZE*4; // 偏移2048  512*4=2048
		}
	} 
	if(count)
	{
		FLASH_Write(appxaddr, iapbuf, count); // 将最后的一些内容字节写进去. 
	}		
}
