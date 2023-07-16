#include "ymodem.h"
#include "crc.h"
#include "usart.h"
#include <stdlib.h>


// 数据包处理回调函数指针
pfvCallBack this_cb = NULL;
// 发送数据处理回调函数指针
pfvSendCallBack pfvSendByte = NULL;

#if defined(USE_USER_STRCONVERT)

#define IS_AF(c)  ((c >= 'A') && (c <= 'F'))
#define IS_af(c)  ((c >= 'a') && (c <= 'f'))
#define IS_09(c)  ((c >= '0') && (c <= '9'))
#define ISVALIDHEX(c)  IS_AF(c) || IS_af(c) || IS_09(c)
#define ISVALIDDEC(c)  IS_09(c)
#define CONVERTDEC(c)  (c - '0')

#define CONVERTHEX_alpha(c)  (IS_AF(c) ? (c - 'A'+10) : (c - 'a'+10))
#define CONVERTHEX(c)   (IS_09(c) ? (c - '0') : CONVERTHEX_alpha(c))

/**
  * @brief  Convert a string to an integer
  * @param  inputstr: The string to be converted
  * @param  intnum: The integer value
  * @retval 1: Correct
  *         0: Error
  */
static uint32_t Str2Int(uint8_t *inputstr, uint32_t *intnum)
{
  uint32_t i = 0, res = 0;
  uint32_t val = 0;

  if (inputstr[0] == '0' && (inputstr[1] == 'x' || inputstr[1] == 'X'))
  {
    if (inputstr[2] == '\0')
    {
      return 0;
    }
    for (i = 2; i < 11; i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1; */
        res = 1;
        break;
      }
      if (ISVALIDHEX(inputstr[i]))
      {
        val = (val << 4) + CONVERTHEX(inputstr[i]);
      }
      else
      {
        /* Return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 8 digit hex --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }
  else /* max 10-digit decimal input */
  {
    for (i = 0;i < 11;i++)
    {
      if (inputstr[i] == '\0')
      {
        *intnum = val;
        /* return 1 */
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'k' || inputstr[i] == 'K') && (i > 0))
      {
        val = val << 10;
        *intnum = val;
        res = 1;
        break;
      }
      else if ((inputstr[i] == 'm' || inputstr[i] == 'M') && (i > 0))
      {
        val = val << 20;
        *intnum = val;
        res = 1;
        break;
      }
      else if (ISVALIDDEC(inputstr[i]))
      {
        val = val * 10 + CONVERTDEC(inputstr[i]);
      }
      else
      {
        /* return 0, Invalid input */
        res = 0;
        break;
      }
    }
    /* Over 10 digit decimal --invalid */
    if (i >= 11)
    {
      res = 0;
    }
  }

  return res;
}
#endif
/**
 * @brief  Abort Ymode session
 * @retval None
 */
void Ymodem_Abort(void)
{
	if(pfvSendByte == NULL)
		return;
	pfvSendByte(CA);
	pfvSendByte(CA);
}

/**
 * @brief  Ack received packet
 * @retval None
 */
void Ymodem_SendAck(void)
{
	if(pfvSendByte == NULL)
		return;
	pfvSendByte(ACK);
}

/**
 * @brief  NAk received packet
 * @retval None
 */
void Ymodem_SendNAck(void)
{
	if(pfvSendByte == NULL)
		return;
	pfvSendByte(NAK);
}

/**
 * @brief  start received packet
 * @retval None
 */
void Ymodem_SendCRC16(void)
{
	if(pfvSendByte == NULL)
		return;
	pfvSendByte(CRC16);
}

/**
  * @brief  注册数据处理和数据发送的回调函数
  * @param  [I]func: 回调函数地址
  * @retval 无
  */
void Ymodem_Init(pfvCallBack func, pfvSendCallBack send_func)
{
	this_cb = func;
	pfvSendByte = send_func;
}

/**
  * @brief  处理收到的数据包
  * @param  [I]buf: 数据包数组首地址
  * @param  [I]size: 数据长度
  * @retval 无
  */
static void Handle_DataPackage(uint8_t *buf, uint32_t size)
{
	if(this_cb != NULL)
		this_cb(buf, size);
}

/**
  * @brief  处理Ymodem协议第0包数据
  * @param  [I]buf: 协议第0包数据缓存首地址
  * @param  [I]size: 协议包大小
  * @param  [O]file_info: 文件信息块地址，见@sFileType
  * @retval 无
  */
static void Handle_FirstPackage(uint8_t *buf, uint32_t size, sFileType *file_info)
{
	uint16_t i = 0, index = 0;
	char size_buf[FILE_SIZE_LENGTH] = {0};
	// char modify_time_buf[FILE_MODIFYSEC_LENGTH] = {0}; // 文件修改时间
	// 得到文件名字
	while(buf[i] != 0x00 && i < size)
	{
		file_info->file_name[index++] = buf[i];
		i++;
	}
	// 得到文件类型
	char *p_type = strstr((const char *)(file_info->file_name), ".");
	if(NULL != p_type)
	{
		if(strcmp(".bin", p_type) == 0)
			file_info->file_type = BINARY_FILE;
		else if(strcmp(".hex", p_type) == 0)
			file_info->file_type = HEX_FILE;
		else if(strcmp(".s19", p_type) == 0)
			file_info->file_type = S19_FILE;
		else
			file_info->file_type = UNKONW_FILE;
	}
	// 得到文件大小，单位：字节
	i++;
	index = 0;
	while((buf[i] != 0x20 && buf[i] != 0x00) && i < size)
	{
		size_buf[index++] = buf[i];
		i++;
	}	
#if defined(USE_USER_STRCONVERT)
	Str2Int((uint8_t *)size_buf, &(file_info->file_size));
#else
	file_info->file_size = atol(size_buf);
#endif
}

/**
  * @brief  处理Ymodem协议数据应答
  * @param  [I]buf: 协议数据包数据缓存首地址
  * @param  [I]size: 协议包大小
  * @param  [O]file_info: 文件信息块地址，见@sFileType
  * @retval 无
  */
static void Handle_PackageData(uint8_t *buf, uint32_t size, uint32_t index, sFileType *file_info)
{
	if(file_info == NULL)
		return;

	if(index == PACKET_FILEINFO_INDEX) // 第0包
	{
		Handle_FirstPackage(buf, size, file_info);
		Ymodem_SendAck();
		Ymodem_SendCRC16();
	}
	else
	{
		Handle_DataPackage(buf, size);
		Ymodem_SendAck();
	}
}

/**
  * @brief  结束码应答处理
  * @param  [O]isEndFlag: 处理完2个0x04应答后，置为1
  * @retval 无
  */
static void EndCodeAck_Handler(uint8_t *isEndFlag)
{
	static uint8_t eot_count = 0; // 结束码计数

	(*isEndFlag)++;
	if(eot_count > 0) // 应答第2个0x04
	{
		Ymodem_SendAck();
		Ymodem_SendCRC16();
		return;
	}
	// 应答第1个0x04
	Ymodem_SendNAck();
	eot_count++;
}

/**
  * @brief  Ymodem接收处理
  * @param  [I]buf: 协议数据包数据缓存首地址
  * @param  [I]size: 协议包大小
  * @param  [O]file_info: 文件信息块地址，见@sFileType
  * @param  [O]recv_size: 有效数据包大小统计（不包含第0包和结束应答包）
  * @retval 包处理状态
  */
eYmodemStatus Ymodem_Receive(uint8_t *buf, uint32_t buf_size, sFileType *file_info, uint32_t * recv_size)
{
	static uint32_t package_count = 0; // 包计数
	static uint32_t error_count = 0; // 错误计数
	static uint8_t isEndFlag = 0; // 结束帧标记
	uint32_t package_size; // 包大小
	uint16_t crc_16 = 0; // CRC16结果
	uint8_t package_type = buf[PACKET_FILEINFO_INDEX]; // 包类型
	eYmodemStatus status = YMODEM_NO_FILE; // 处理状态
	
	if(buf == NULL)
	{
		status = YMODEM_ABORTED;
		goto RETURN;
	}
	
	if(package_count != 0 && (buf_size == 1) && (buf[0] == EOT)) // 判断是否为结束码
	{
		EndCodeAck_Handler(&isEndFlag);
		if(isEndFlag)
		{
			status = YMODEM_CONTINUE;
			goto RETURN;
		}
	}
	
	// 判断文件大小是否有效
	if(buf_size > (PACKET_1K_SIZE + PACKET_OVERHEAD) || 
		buf_size < (PACKET_SIZE + PACKET_OVERHEAD))
	{
		Ymodem_Abort();
		status = YMODEM_ABORTED;
		goto RETURN;
	}
	// 判断正反码
	if((buf[PACKET_SEQNO_INDEX] + buf[PACKET_SEQNO_COMP_INDEX]) != 0xFF)
	{
		Ymodem_Abort();
		status = YMODEM_ABORTED;
		goto RETURN;
	}
	
	package_size = (package_type == SOH) ? PACKET_SIZE : PACKET_1K_SIZE;
	crc_16 = crc16(buf+PACKET_HEADER, package_size);
	if( crc_16 != ((buf[buf_size - 2] << 8) | buf[buf_size - 1]))
	{
		status = YMODEM_CONTINUE;
		if(error_count++ > MAX_ERRORS)
		{
			status = YMODEM_TOO_MANY_ERRORS;
			Ymodem_Abort();
			goto RETURN;
		}
		Ymodem_SendNAck(); // 重发
	}
	
	if(isEndFlag) // 结束帧
	{
		package_count = 0;
		isEndFlag = 0;
		error_count = 0;
		if(buf[PACKET_HEADER] == 0x00)
		{
			Ymodem_SendAck();
		#ifdef SUPPORT_CONTINUE_DOWNLOAD_MODE // 连续传输
			Ymodem_SendCRC16();
			status = YMODEM_CONTINUE;
		#else
			status = YMODEM_DONE;
		#endif
			goto RETURN;
		}
	}
	
	Handle_PackageData(buf + PACKET_HEADER, package_size, package_count, file_info);

	if(package_count == 0)
	{
		status = YMODEM_FILE_INFO_RECV;
	}
	else
	{
		(*recv_size) += package_size; // 接收大小计数
		status = YMODEM_CONTINUE;
	}
	
	package_count++;

RETURN:
	return status;
}
