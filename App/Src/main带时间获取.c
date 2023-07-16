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

// 事件定义
#define IDEL_EVEN (1L << 0) // 空闲事件
#define KEY0_EVEN (1L << 1) // 按键0事件
#define WKUP_EVEN (1L << 2) // 按键wakeup事件
#define IR_EVEN (1L << 3) // 红外传感器触发事件

#define MSGSIZE 48 // GET请求返回的消息长度
#define UrlLength 106 // 连接的url长度

// esp8266相关宏定义
#define User_ESP8266_ApSsid "veis"     //要连接的热点的名称
#define User_ESP8266_ApPwd "123456789" //要连接的热点的密钥

#define User_ESP8266_TcpServer_IP "192.168.137.22" //要连接的TCP服务器的IP
#define User_ESP8266_TcpServer_Port "8080"          //要连接的TCP服务器的端口

#define TimeServerIP "api.k780.com"
#define HttpPort "80"
// 相关订阅参数参数，详情请了解API网址 https://www.nowapi.com/api
#define AppKey "46110"
#define Sign "93ee0233e57e3afa6507ff2cbb9909a3"

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t even_flag = 0;              // 事件标识符，最多表示32个事件
extern __IO uint8_t ucTcpClosedFlag; // 标志TCP服务器连接状态
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void Systick_Config(void);
void Esp8266_ConnectServer(ENUM_NetPro_TypeDef netpro, const char *ip_addr, const char *port);
void Esp8266_ConnectAp(const char *ssid, const char *password);
const char *CreateGetUrl(void); // 连接GET请求字符串

// 自己实现的字符串处理函数，效率相比于标准库要高
const char *MyStrstr(const char *src, const char *sub); // 字符串查找函数
const char *MyStrcat(char *aim, const char *insert);    // 字符串拼接函数
int MyStrlen(char *aim);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

char linkstr[UrlLength] = {0}; // 用于存储Get请求字符串
DHT11_Data_TypeDef DHT11_Data; // 存储DHT11返回的数据

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  uint8_t ucStatus;
  uint8_t count = 0;
  char time_message[MSGSIZE] = {0};

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  Systick_Config();
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  DHT11_Init();

  ESP8266_Init();
//  char ssid[20];
//  char password[20];
//  printf("请输入WIFI名称：");
//  scanf("%s",ssid);
//  setbuf(stdin,NULL);
//  printf("%s\r\n",ssid);
//  printf("请输入WIFI密码：");
//  scanf("%s",password);
//  setbuf(stdin,NULL);
//  printf("%s\r\n",password);
//  Esp8266_ConnectAp(ssid, password);                // 连接路由器，实现入网
  Esp8266_ConnectAp(User_ESP8266_ApSsid, User_ESP8266_ApPwd);                // 连接路由器，实现入网
//  Esp8266_ConnectServer(enumTCP, TimeServerIP, HttpPort);     // 连接授时服务器
  __HAL_UART_ENABLE_IT(&husartx_esp8266, UART_IT_IDLE);       // 使能帧中断
//  // 获取时间的GET请求
//  char *url = (char *)CreateGetUrl();
//  ESP8266_Usart("%s", url);
//  //  ESP8266_Usart("GET http://api.k780.com/?app=life.time&appkey=10003&sign=b59bc3ef6191eb9f747dd4e83c99f2a4&format=json\r\n");

//  while (1)
//  {
//    if (strEsp8266_Fram_Record.InfBit.FramFinishFlag)
//    {
//      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
//      //      printf ( "\r\n%s\r\n", strEsp8266_Fram_Record .Data_RX_BUF );
//      sprintf(time_message, "%s", strEsp8266_Fram_Record.Data_RX_BUF); // 把时间日期数据拷贝到时间消息数字中
//      ESP8266_Clear_Buffer();
//      ESP8266_ExitUnvarnishSend();
//      count++;
//      if (count > 1)
//      {
//        ESP8266_Usart("AT+CIPCLOSE\r\n");
//        HAL_Delay(1000);
//        //          printf("%s\r\n",time_message);
//        HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); // 指示灯点亮
//        break;
//      }
//    }
//  }

//  // 查找时间和日期的字符串
//  printf("\r\n");
//  char *time1 = (char *)MyStrstr(time_message, "\"datetime_1\":\""); // 类似于正则匹配
//  time1 = time1 + MyStrlen("\"datetime_1\":\""); // 计算地址偏移
//  while (*time1 != '"')
//    printf("%c", *time1++);
//  printf("\r\n");
 
  ESP8266_Link_Server(enumTCP, User_ESP8266_TcpServer_IP, User_ESP8266_TcpServer_Port, Single_ID_0);
  // 第一次数据丢弃
  DHT11_Read_TempAndHumidity(&DHT11_Data);
  HAL_Delay(1200);
  // OLED上显示问候语HELLO OLED
  OLED_ShowString(30, 0, (unsigned char *)"HELLO OLED", 16);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    
    /* USER CODE BEGIN 3 */
    
    if (DHT11_Read_TempAndHumidity(&DHT11_Data) == SUCCESS)
    {
  //    printf("读取DHT11成功!-->湿度为%.0f％RH ，温度为 %.0f℃ \r\n", DHT11_Data.humidity, DHT11_Data.temperature);
  //    ESP8266_Usart("读取DHT11成功!-->湿度为%.0f％RH ，温度为 %.0f℃ \r\n", DHT11_Data.humidity, DHT11_Data.temperature);
      // 显示湿度
      OLED_ShowNum(0, 2, DHT11_Data.humidity, 2, 16);
      OLED_ShowString(17, 2, (unsigned char *)" %RH", 16);
      // 显示温度
      OLED_ShowNum(0, 4, DHT11_Data.temp_high8bit, 2, 16);
      OLED_ShowString(18, 4, (unsigned char *)" 'C", 16);
    }
    else
    {
      printf("读取DHT11信息失败\n");
    }

    if (strEsp8266_Fram_Record.InfBit.FramFinishFlag)
    {
      strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength] = '\0';
      if(esp8266_rxdata == '1')
      {
          HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_SET);
      }
      if(esp8266_rxdata == '0')
      {
          HAL_GPIO_WritePin(LED0_GPIO_Port,LED0_Pin,GPIO_PIN_RESET);
      }
      if(esp8266_rxdata == '2')
      {
          char str1[50] = {0};
          sprintf(str1,"temperature = %.0f C\n",DHT11_Data.temperature);
          ESP8266_SendString(DISABLE,str1,MyStrlen(str1),Single_ID_0);
      }
      if(esp8266_rxdata == '3')
      {
          char str2[50] = {0};
          sprintf(str2,"humidity = %.0f %%RH\n",DHT11_Data.humidity);
          ESP8266_SendString(DISABLE,str2,MyStrlen(str2),Single_ID_0);
      }
  //    printf("\r\n%s\r\n", strEsp8266_Fram_Record.Data_RX_BUF);
  //    ESP8266_Usart("读取DHT11成功!-->湿度为%.0f％RH ，温度为 %.0f℃ \r\n", DHT11_Data.humidity, DHT11_Data.temperature);
      ESP8266_Clear_Buffer();
    }
    if (ucTcpClosedFlag) // 检测是否失去连接
    {
      ESP8266_ExitUnvarnishSend(); // 退出透传模式
      do
        ucStatus = ESP8266_Get_LinkStatus(); // 获取连接状态
      while (!ucStatus);
      if (ucStatus == 4) // 确认失去连接后重连
      {
        printf("正在重连热点和服务器 ......\n");
        while (!ESP8266_JoinAP(User_ESP8266_ApSsid, User_ESP8266_ApPwd));
        while (!ESP8266_Link_Server(enumTCP, User_ESP8266_TcpServer_IP, User_ESP8266_TcpServer_Port, Single_ID_0));
        printf("重连热点和服务器成功!!!\n");
      }
      while (!ESP8266_UnvarnishSend());
    }
    HAL_Delay(1200);
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
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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

// 连接服务器函数
void Esp8266_ConnectServer(ENUM_NetPro_TypeDef netpro, const char *ip_addr, const char *port)
{
  uint8_t count = 0;
  ESP8266_Enable_MultipleId(DISABLE);                                              // 关闭多连接
  while (!ESP8266_Link_Server(netpro, (char *)ip_addr, (char *)port, Single_ID_0)) // 连接服务器
  {
    ++count;
    if (5 == count)
    {
      printf("服务器连接失败！\r\n");
      return;
    }
  }
  while (!ESP8266_UnvarnishSend()); // 启动透传模式
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

// 链接GET请求的字符串
const char *CreateGetUrl(void)
{
  const char *str1 = "GET http://api.k780.com";
  const char *str2 = "/?app=life.time&appkey=";
  const char *str3 = "&sign=";
  const char *str4 = "&format=json\r\n";
  MyStrcat(linkstr, str1);
  MyStrcat(linkstr, str2);
  MyStrcat(linkstr, AppKey);
  MyStrcat(linkstr, str3);
  MyStrcat(linkstr, Sign);
  MyStrcat(linkstr, str4);
  linkstr[UrlLength - 1] = '\0';
  return linkstr;
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
  if (strEsp8266_Fram_Record.InfBit.FramLength < (RX_BUF_MAX_LEN - 1)) // 预留1个字节写结束符
    strEsp8266_Fram_Record.Data_RX_BUF[strEsp8266_Fram_Record.InfBit.FramLength++] = esp8266_rxdata;
  HAL_UART_Receive_IT(&husartx_esp8266, &esp8266_rxdata, 1);
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
