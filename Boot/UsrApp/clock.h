#ifndef __CLOCK_H__
#define __CLOCK_H__

#include "stm32f1xx.h"
#include <stdint.h>

//extern __IO uint32_t tick;
// public:
void clock_init(void);
uint32_t GetTick(void);
uint32_t GetOverFlowTick(void);
void Tick_UpdateHandler(void);

#ifdef USE_NOOS_DELAY
void delay_us(uint32_t nus);
void delay_xms(uint16_t nms);
void delay_ms(uint16_t nms);
#endif

#endif /* __CLOCK_H__ */
