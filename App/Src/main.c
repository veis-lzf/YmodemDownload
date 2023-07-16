/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "oled.h"
#include "dht11.h"
#include "esp8266.h"
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

// esp8266相关宏定义，根据自己的AP和Server来修改如下宏定义
#define User_ESP8266_ApSsid "veis"     // 要连接的热点的名称
#define User_ESP8266_ApPwd "123456789" // 要连接的热点的密钥

#define User_ESP8266_TcpServer_IP "192.168.137.22" // 要连接的TCP服务器的IP
#define User_ESP8266_TcpServer_Port "8080"         // 要连接的TCP服务器的端口

#define TCP_Server_Port "8088"
#define TCP_Server_Timeout "1800"
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern __IO uint8_t ucTcpClosedFlag; // 标志TCP服务器连接状态
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Systick_Config(void);
void WIFI_Config(void);
void Esp8266_ConnectAp(const char *ssid, const char *password);
// 实现的字符串处理函数
const char *MyStrstr(const char *src, const char *sub); // 字符串查找函数
const char *MyStrcat(char *aim, const char *insert);    // 字符串拼接函数
int MyStrlen(char *aim);

uint8_t ch = 0xaa;
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
DHT11_Data_TypeDef DHT11_Data; // 存储DHT11返回的数据

//开启所有中断
__asm void INTX_ENABLE(void)
{
	CPSIE   I
	BX      LR  
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  SCB->VTOR = FLASH_BASE | 0x4000;
  INTX_ENABLE();
  HAL_Init();
  SystemClock_Config();
  Systick_Config();
	
  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
//	HAL_UART_Receive_IT(&huart1, &ch, 1);
//  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
//  OLED_Init(); // 初始化OLED
  DHT11_Init(); // 初始化DHT11
//  ESP8266_Init(); // 初始化ESP8266
//  WIFI_Config();
  
  // 第一次数据丢弃
  DHT11_Read_TempAndHumidity(&DHT11_Data);
  HAL_Delay(1200);
  // OLED上显示标题 OLED Show
//  OLED_Clear();
//  OLED_ShowString(30, 0, (unsigned char *)"OLED Show", 16);
  /* USER CODE END 2 */

  while (1)
  {
    /* USER CODE BEGIN 3 */
    // 读取DHT11温湿度数据并更新到OLED
    if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
    {
      char temp[20];
      char humid[20];
      printf("Humid: %0.f %%RH\r\n", DHT11_Data.humidity);
      printf("Temp:  %.0f  \r\n", DHT11_Data.temperature);
     // OLED_ShowString(0, 2, (unsigned char *)temp, 16);
     // OLED_ShowString(0, 4, (unsigned char *)humid, 16);
    }
    // 更新到APP
    if (strEsp8266_Fram_Record.InfBit.FramFinishFlag)
    {
      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
      printf("接收到的字符为：%c\r\n", esp8266_rxdata);

      if (esp8266_rxdata == 'a') // 打开LED0
      {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
        ESP8266_SendString(DISABLE, "LED ON", MyStrlen("LED ON"), Multiple_ID_0);
      }
      if (esp8266_rxdata == 'b') // 关闭LED0
      {
        HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
        ESP8266_SendString(DISABLE, "LED OFF", MyStrlen("LED OFF"), Multiple_ID_0);
      }
      if (esp8266_rxdata == 'c') // 打印温度数据
      {
        char str1[50] = {0};
        sprintf(str1, "temperature:%.0f C", DHT11_Data.temperature);
        ESP8266_SendString(DISABLE, str1, MyStrlen(str1), Multiple_ID_0);
      }
      if (esp8266_rxdata == 'd') // 打印湿度数据
      {
        char str2[50] = {0};
        sprintf(str2, "humidity:%.0f %%RH", DHT11_Data.humidity);
        ESP8266_SendString(DISABLE, str2, MyStrlen(str2), Multiple_ID_0);
      }
      ESP8266_Clear_Buffer();
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

void WIFI_Config()
{
  char macip[20] = {0};
  char macip_s[30] = {0};
  char port[20] = {0};
  
  Esp8266_ConnectAp(User_ESP8266_ApSsid, User_ESP8266_ApPwd); // 连接路由器，实现入网
  printf("\r\n建立热点\r\n");
  ESP8266_BuildAP("veis_esp8266","123456789",WPA_WPA2_PSK);
  ESP8266_ExitUnvarnishSend();
  ESP8266_Enable_MultipleId(ENABLE); 
  ESP8266_StartOrShutServer(ENABLE,TCP_Server_Port,TCP_Server_Timeout);
  ESP8266_Inquire_ApIp(macip, 20);

  printf("配置 ESP8266 完毕\n");
  __HAL_UART_ENABLE_IT(&husartx_esp8266, UART_IT_IDLE); // 使能帧中断
  
  sprintf(macip_s, "IP:%s",macip);
  sprintf(port, "Port:%s",TCP_Server_Port);
  // 在OLED上显示配置信息
  OLED_ShowString(0, 0, (unsigned char *)"ssid:veis_esp8266", 16);
  OLED_ShowString(0, 2, (unsigned char *)"key:123456789", 16);
  OLED_ShowString(0, 4, (unsigned char *)macip_s, 16);
  OLED_ShowString(0, 6, (unsigned char *)port, 16);

  while(1)
  {
      ESP8266_ReceiveString(DISABLE);
      if (strEsp8266_Fram_Record.InfBit.FramFinishFlag)
      {      
          if(esp8266_rxdata == 'e') // 连接APP后退出OLED显示
          {
            break;
          }
      }
  }
  printf("\r\n连接APP完成！\r\n");

}

// 连接AP函数
void Esp8266_ConnectAp(const char *ssid, const char *password)
{
  uint8_t count = 0;
  printf("开始wifi配置\r\n");
  ESP8266_Net_Mode_Choose(STA_AP); // 设置为STA模式
  ESP8266_Cmd("AT+RST", "OK", "ready", 2500);
  while (!ESP8266_JoinAP((char *)ssid, (char *)password)) // 连接wifi
  {
    ++count;
    if (3 == count)
    {
      printf("wifi连接失败！\r\n");
      return;
    }
  }
  printf("连接WIFI成功！\r\nssid:%s\tpassword:%s\r\n", ssid, password);
}

// 字符串拼接函数
const char *MyStrcat(char *a, const char *insert)
{
  char *address = a;
  while (*a != '\0')
    a++;
  while (*insert != '\0')
    *a++ = *insert++;
  *a = '\0';
  return address;
}

// 字符串查找函数
// 查找成功返回目的字符串首地址；
// 查找失败返回0
const char *MyStrstr(const char *src, const char *sub)
{
  const char *bp, *sp;

  //	 判断是否为空字符串
  if (!src || !sub)
  {
    return src;
  }
  //	 遍历字符串
  while (*src)
  {
    bp = src;
    sp = sub;
    do
    {
      if (!*sp) // 到了sub的结束位置，返回src位置
        return src;
    } while (*bp++ == *sp++);
    src++;
  }
  return NULL;
}

// 字符串长度求取函数
int MyStrlen(char *aim)
{
  int count = 0;
  while (*aim++)
  {
    count++;
  }
  return count;
}

// 配置Systick定时器相关参数
void Systick_Config(void)
{
  // HAL_RCC_GetHCLKFreq()/1000    1ms中断一次
  // HAL_RCC_GetHCLKFreq()/100000	 10us中断一次
  // HAL_RCC_GetHCLKFreq()/1000000 1us中断一次
  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq() / 1000); // 配置并启动系统滴答定时器
  // 系统滴答定时器时钟源
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  // 系统滴答定时器中断优先级配置
  HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
}

// 串口接收完成回调函数
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *UartHandle)
{
	
	if(UartHandle == &huart1)
	{
		putchar(ch);
		//HAL_UART_Receive_IT(UartHandle, &ch, 1);
	}
	HAL_UART_Receive_IT(UartHandle, &ch, 1);
//	else
//	{
//		
//	  if (strEsp8266_Fram_Record.InfBit.FramLength < (RX_BUF_MAX_LEN - 1)) // 预留1个字节写结束符
//		strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++] = esp8266_rxdata;
//	  HAL_UART_Receive_IT(&husartx_esp8266, &esp8266_rxdata, 1);
//	}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
