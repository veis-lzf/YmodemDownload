/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H__
#define __USART_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#include "AppCfg.h"
#include "clock.h"
/* USER CODE END Includes */

/* USER CODE BEGIN Private defines */
#ifdef TRACE_DBG

#define os_time_get()	GetTick()//HAL_GetTick()
#define p_track_enter()	do{printf("[%d.%03d] ",  os_time_get()/1000, os_time_get()%1000); \
							printf("enter file:%s,function:%s,line:%d", __FILE__, __func__, __LINE__); \
							printf("\r\n");}while(0)

#define p_track_exit()	do{printf("[%d.%03d] ",  os_time_get()/1000, os_time_get()%1000); \
							printf("exit file:%s,function:%s,line:%d", __FILE__, __func__, __LINE__); \
							printf("\r\n");}while(0)
							
#define p_info(...) do{printf("[I: %d.%03d] ",  os_time_get()/1000, os_time_get()%1000); printf(__VA_ARGS__); printf("\r\n");}while(0)
#define p_err(...)	do{printf("[E: %d.%03d] ",  os_time_get()/1000, os_time_get()%1000); printf(__VA_ARGS__); printf("\r\n");}while(0)
#else
#define p_err(...)
#define p_info(...)
#endif


#define DBG_RX_BUFFER_SIZE	(2*1024)
extern __IO uint8_t rx_buffer[DBG_RX_BUFFER_SIZE];
extern __IO uint32_t rx_buffer_size;
/* USER CODE END Private defines */


/* USER CODE BEGIN Prototypes */
void IAP_UartInit(uint32_t baund);
void USART1_SendByte(uint8_t ch);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
