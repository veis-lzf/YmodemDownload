/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "flash.h"
#include "AppCfg.h"
#include "ymodem.h"
#include "iap.h"
#include "clock.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
p_queue g_pDataQueue = NULL;
extern uint8_t recv_status;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#ifdef FLASH_WR_TEST
#define UserFlashAddress	(uint32_t)(FLASH_APP1_ADDR)
void write_to_flash(void)
{
	uint32_t buff[64];
	uint32_t count_len = sizeof(buff) / sizeof(buff[0]);
	printf("\r\nWriteData successful!\r\n");
	for(uint32_t i = 0; i < count_len; ++i)
	{
		if(i != 0 && i % 16 == 0)
			printf("\r\n");
		buff[i] = rand() % 0xfffedfff + 1;
		printf("0x%08x,", buff[i]);
	}
    FLASH_Write(UserFlashAddress, buff, count_len);
	printf("\r\n");
}

void read_from_flash(void)
{
	uint32_t buff[64];
	uint32_t count_len = sizeof(buff) / sizeof(buff[0]);
	FLASH_Read(UserFlashAddress, buff,count_len);
	printf("\r\nReadData successful!\r\n");
	for(uint32_t i = 0; i < count_len; ++i)
	{
		if(i != 0 && i % 16 == 0)
			printf("\r\n");
		printf("0x%08x,", buff[i]);
	}
	printf("\r\n");

}
#endif

#ifdef YMODEM_TEST

sFileType file_info = {0};
uint32_t cur_recv_size = 0;
uint8_t SRdkFrm_f10HzFlag = 0;
uint8_t SRdkFrm_InUpdateModeFlag = 0;
__IO uint32_t Sys_f10HzCnt = 0;

// 处理接收到的数据
void Handle_RecvData(uint8_t *buf, uint32_t size)
{
	static uint32_t offset_addr = 0;
	
	iap_write_appbin(FLASH_APP1_ADDR + offset_addr, buf, size);
	offset_addr += size;
}
#endif

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
	/* USER CODE BEGIN 1 */
	eYmodemStatus ymodem_status = YMODEM_NO_FILE;
	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */

	LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_AFIO);
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);

	NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_2);

	/* System interrupt init*/

	/** NOJTAG: JTAG-DP Disabled and SW-DP Enabled
	*/
	LL_GPIO_AF_Remap_SWJ_NOJTAG();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	/* USER CODE BEGIN 2 */
	clock_init(); // 初始化tick
	IAP_UartInit(115200);
	#ifdef USART_QUEUE_TEST
	g_pDataQueue = CreateQueue(1*1024);
	#endif

	#ifdef FLASH_WR_TEST 
	FLASH_Erase(FLASH_APP1_ADDR, 40*1024);
	write_to_flash();
	read_from_flash();
	#endif
	
	#ifdef YMODEM_TEST
	Ymodem_Init(Handle_RecvData, USART1_SendByte);
	Ymodem_SendCRC16();
	Ymodem_SendCRC16();
//	iap_load_app(FLASH_APP1_ADDR);
	#endif
	
  /* USER CODE END 2 */
	
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
	while (1)
	{
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
		#ifdef YMODEM_TEST
		if(recv_status) // 接收完成
		{
			Sys_f10HzCnt = 0;
			ymodem_status = Ymodem_Receive((uint8_t *)rx_buffer, rx_buffer_size, &file_info, &cur_recv_size);
			
			if(ymodem_status == YMODEM_FILE_INFO_RECV)
			{
				SRdkFrm_InUpdateModeFlag = 1;
				SRdkFrm_f10HzFlag = 0;
				// p_info("recv: file_name:%s, size:%d Byte", file_info.file_name, file_info.file_size);
			}
			else if(ymodem_status == YMODEM_NO_FILE)
			{
				// p_info("Invalid update file!");
			//	Ymodem_SendCRC16();
			}
			else if(ymodem_status == YMODEM_CONTINUE)
			{
				// p_info("Invalid update file!");
			}
			else if(ymodem_status == YMODEM_DONE)
			{
				#define TIMEOUT_WAIT	0XFFFF
				uint32_t i = 0;
				p_info("wait...");
				while(i++ < TIMEOUT_WAIT);
				p_info("Update successful, file_name:%s, size:%d Byte", file_info.file_name, file_info.file_size);
				p_info("jump to app...");
				iap_load_app(FLASH_APP1_ADDR);
			}
			recv_status = 0;
			memset((void *)rx_buffer, 0, DBG_RX_BUFFER_SIZE);
		}
		else
		{
			#define TIMEOUT_COUNT_WAIT	20 // 等20*100ms
			if(!SRdkFrm_InUpdateModeFlag && SRdkFrm_f10HzFlag)
			{
				SRdkFrm_f10HzFlag = 0;
			//	Ymodem_SendCRC16();
				Sys_f10HzCnt++;
			}
			if(Sys_f10HzCnt >= TIMEOUT_COUNT_WAIT)
			{
				Sys_f10HzCnt = 0;
				SRdkFrm_f10HzFlag = 0;
				p_info("timeout! wait... jump to app...");
				iap_load_app(FLASH_APP1_ADDR);
			}
		}
		#endif
		
		#ifdef USART_QUEUE_TEST
		if(IsFull(g_pDataQueue))
		{
			uint8_t ch;
			while(!IsEmpty(g_pDataQueue))
			{
			  Pop(g_pDataQueue, &ch);
			  printf("%c", ch);
			}
		}
		#endif
	}
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);
  while(LL_FLASH_GetLatency()!= LL_FLASH_LATENCY_2)
  {
  }
  LL_RCC_HSE_Enable();

   /* Wait till HSE is ready */
  while(LL_RCC_HSE_IsReady() != 1)
  {

  }
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);
  LL_RCC_PLL_Enable();

   /* Wait till PLL is ready */
  while(LL_RCC_PLL_IsReady() != 1)
  {

  }
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);

   /* Wait till System clock is ready */
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {

  }
  LL_Init1msTick(72000000);
  LL_SetSystemCoreClock(72000000);
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
