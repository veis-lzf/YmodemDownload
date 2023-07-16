/**
  ******************************************************************************
  * @file    usart.c
  * @brief   This file provides code for the configuration
  *          of the USART instances.
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

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
#include "AppCfg.h"
#include "Ymodem.h"

#if defined(ENABLE_UART_RX_DMA)
static void USART1_DMA_Config(void);
static void Start_USART1_RXDMA(void);

#define USART1_DMA_NUMBER		DMA1
#define USART1_DMA_RXCHANNEL	LL_DMA_CHANNEL_5
#endif

__IO uint8_t rx_buffer[DBG_RX_BUFFER_SIZE] __attribute__((at(SRAM_BASE | 0xc00)));;
__IO uint32_t rx_buffer_size = 0;

/* USER CODE END 0 */


/* USER CODE BEGIN 1 */
void IAP_UartInit(uint32_t baund)
{
	LL_USART_InitTypeDef USART_InitStruct = { 0 };

	LL_GPIO_InitTypeDef GPIO_InitStruct = { 0 };

	/* Peripheral clock enable */
	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_USART1);

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
	/**USART1 GPIO Configuration
	PA9   ------> USART1_TX
	PA10   ------> USART1_RX
	*/
	GPIO_InitStruct.Pin = LL_GPIO_PIN_9;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
	GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
	GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = LL_GPIO_PIN_10;
	GPIO_InitStruct.Mode = LL_GPIO_MODE_FLOATING;
	LL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	USART_InitStruct.BaudRate = baund;
	USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
	USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
	USART_InitStruct.Parity = LL_USART_PARITY_NONE;
	USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
	USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
	USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
	LL_USART_Init(USART1, &USART_InitStruct);
	LL_USART_ConfigAsyncMode(USART1);
	LL_USART_Enable(USART1);
	
#if defined(ENABLE_USART1_IRQ)
// 使能空闲中断和接收中断
//	LL_USART_EnableIT_RXNE(USART1);
	LL_USART_EnableIT_IDLE(USART1);
	
	NVIC_SetPriority(USART1_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 1));
	NVIC_EnableIRQ(USART1_IRQn);
#endif

#if defined(ENABLE_UART_RX_DMA)
	USART1_DMA_Config();
	Start_USART1_RXDMA();
#endif
}

#if defined(ENABLE_UART_RX_DMA)
static void Start_USART1_RXDMA(void)
{
	// 配置DMA地址
	LL_DMA_ConfigAddresses(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, (uint32_t)(&(USART1->DR)), 
		(uint32_t)rx_buffer, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
	
	// 使能DMA接收
	LL_DMA_SetDataLength(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, DBG_RX_BUFFER_SIZE); // 目的缓冲区大小
	LL_DMA_EnableIT_TC(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL); // 传输完成中断
	LL_DMA_EnableChannel(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL);
	LL_USART_EnableDMAReq_RX(USART1);
}

static void USART1_DMA_Config(void)
{
	// 使能时钟
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);
	
	// 配置优先级和中断方式
	NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(),0, 1));
	NVIC_EnableIRQ(DMA1_Channel5_IRQn);

	LL_DMA_SetDataTransferDirection(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_DIRECTION_PERIPH_TO_MEMORY); // 配置DMA方向
	LL_DMA_SetChannelPriorityLevel(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_PRIORITY_MEDIUM); // 中优先级
	LL_DMA_SetMode(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_MODE_NORMAL); // 单次模式
	LL_DMA_SetMemoryIncMode(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_MEMORY_INCREMENT); // 目的地址递增
	LL_DMA_SetPeriphIncMode(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_PERIPH_NOINCREMENT); // 外设地址不递增
	// 配置传输数据量和传输的大小
	LL_DMA_SetMemorySize(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_MDATAALIGN_BYTE); // 传输单位为字节
	LL_DMA_SetPeriphSize(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, LL_DMA_MDATAALIGN_BYTE); // 字节对齐
}
#endif

uint8_t recv_status = 0;
void UART_RxCpltCallback(USART_TypeDef *huart)
{
	if(huart == USART1)
	{
	#if defined(ENABLE_UART_RX_DMA)
		LL_DMA_DisableChannel(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL);
		rx_buffer_size = DBG_RX_BUFFER_SIZE - LL_DMA_GetDataLength(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL);
		
		recv_status  = 1;
		
		LL_DMA_SetDataLength(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL, DBG_RX_BUFFER_SIZE); // 目的缓冲区大小
		LL_DMA_EnableChannel(USART1_DMA_NUMBER, USART1_DMA_RXCHANNEL);
		
	#endif
	}
}

void USART1_SendByte(uint8_t ch)
{
	LL_USART_TransmitData8(USART1, ch);
	while(!(USART1->SR & LL_USART_SR_TC));
	LL_USART_ClearFlag_TC(USART1);
}

/* USER CODE BEGIN 1 */
/**
  * 函数功能: 重定向c库函数printf到串口1
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fputc(int ch, FILE *f)
{
	USART1_SendByte((uint8_t)ch);
	return ch;
}

/**
  * 函数功能: 重定向c库函数getchar,scanf到串口1
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明：无
  */
int fgetc(FILE *f)
{
	uint8_t ch = 0;
	ch = LL_USART_ReceiveData8(USART1);
	return ch;
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
