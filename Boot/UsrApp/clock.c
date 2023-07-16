#include "clock.h"

extern uint8_t SRdkFrm_f10HzFlag;
/* 
 SystemFrequency / 1000 1ms 中断一次 * 
 SystemFrequency / 100000 10us 中断一次 * 
 SystemFrequency / 1000000 1us 中断一次 */
#define TICK_SLICE	1000
__IO uint32_t tick = 0; // tick计数
__IO uint32_t overflow_flag = 0; // 计数溢出，标记，统计溢出时间

#ifdef USE_NOOS_DELAY
static uint8_t  fac_us = 0; // us延时倍乘数			   
static uint16_t fac_ms = 0; // ms延时倍乘数,在os下,代表每个节拍的ms数
#endif

// 初始话tick时基，使用systick产生中断来计数
void clock_init(void)
{
	/* set reload register */	
	SysTick->LOAD = ((SystemCoreClock / TICK_SLICE) & SysTick_LOAD_RELOAD_Msk) - 1; 
	
	/* set Priority for Cortex-M3 System Interrupts */
	NVIC_SetPriority(SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
	
	/* Load the SysTick Counter Value */
	SysTick->VAL = 0;
	
	/* Enable SysTick IRQ and SysTick Timer */
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | 
				   SysTick_CTRL_TICKINT_Msk   | 
				   SysTick_CTRL_ENABLE_Msk;
}

// 获取当前系统运行的tick，单位ms
uint32_t GetTick(void)
{
	return tick;
}

// 获取计数溢出的次数
uint32_t GetOverFlowTick(void)
{
	return overflow_flag;
}

#ifdef USE_NOOS_DELAY
// 延时nus
// nus为要延时的us数.	
// 注意:nus的值,不要大于798915us(最大值即2^24/fac_us@fac_us=21)
void delay_us(uint32_t nus)
{		
	uint32_t temp;	    	 
	SysTick->LOAD=nus*fac_us; 				//时间加载	  		 
	SysTick->VAL=0x00;        				//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ; //开始倒数 	 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk; //关闭计数器
	SysTick->VAL =0X00;       				//清空计数器 
}

// 延时nms
// 注意nms的范围
// SysTick->LOAD为24位寄存器,所以,最大延时为:
// nms<=0xffffff*8*1000/SYSCLK
// SYSCLK单位为Hz,nms单位为ms
// 对168M条件下,nms<=798ms 
void delay_xms(uint16_t nms)
{	 		  	  
	uint32_t temp;		   
	SysTick->LOAD=(uint32_t)nms*fac_ms;			//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           			//清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数 
	do
	{
		temp=SysTick->CTRL;
	}while((temp&0x01)&&!(temp&(1<<16)));	//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;     		  		//清空计数器	  	    
}

// 延时nms 
// nms:0~65535
void delay_ms(uint16_t nms)
{	 	 
	uint8_t repeat=nms/540;						//这里用540,是考虑到某些客户可能超频使用,
											//比如超频到248M的时候,delay_xms最大只能延时541ms左右了
	uint16_t remain=nms%540;
	while(repeat)
	{
		delay_xms(540);
		repeat--;
	}
	if(remain)delay_xms(remain);
} 
#endif

// systick中断回调处理函数
void Tick_UpdateHandler(void)
{
	tick++;
	
	if(tick % 100 == 0)
		SRdkFrm_f10HzFlag = 1;
	
	if((tick & 0XFFFFFFFF) == 0)
	{
		overflow_flag++;
		tick = 0;
	}
}


