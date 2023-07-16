#include "flash.h"
#include "usart.h"


#define FLASH_TYPEPROGRAM_BYTE        0x00000000U  /*!< Program byte (8-bit) at a specified address           */
#define FLASH_TYPEPROGRAM_HALFWORD    0x00000001U  /*!< Program a half-word (16-bit) at a specified address   */
#define FLASH_TYPEPROGRAM_WORD        0x00000002U  /*!< Program a word (32-bit) at a specified address        */


// 解锁flash
FlashStatus FLASH_Unlock(void)
{
	FlashStatus status = FLASH_COMPLETE;

	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
	{
		WRITE_REG(FLASH->KEYR, FLASH_KEY1);
		WRITE_REG(FLASH->KEYR, FLASH_KEY2);

		if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
		{
			status = FLASH_ERROR;
		}
	}
	return status;
}

// 上锁flash
FlashStatus FLASH_Lock(void)
{
	FlashStatus status = FLASH_COMPLETE;

	SET_BIT(FLASH->CR, FLASH_CR_LOCK);
	if(READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 1U)
	{
		status = FLASH_ERROR;
	}
	return status;
}

// 解锁选项字节flash区域
FlashStatus HAL_FLASH_OB_Unlock(void)
{
	FlashStatus status = FLASH_UNLOCK;

	if(READ_BIT(FLASH->CR, FLASH_CR_OPTWRE) != 0U)
	{
		WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY1);
		WRITE_REG(FLASH->OPTKEYR, FLASH_OPTKEY2);
		
		if(READ_BIT(FLASH->CR, FLASH_CR_OPTWRE) == 0U)
		{
			status = FLASH_LOCK;
		}
	}
	return status;
}

// 上锁选项字节flash区域
FlashStatus HAL_FLASH_OB_Lock(void)
{
	FlashStatus status = FLASH_LOCK;

	SET_BIT(FLASH->CR, FLASH_CR_OPTWRE);

	if (READ_BIT(FLASH->CR, FLASH_CR_OPTWRE) == 0U)
	{
		status = FLASH_UNLOCK;
	}

	return status;
}

// 从指定地址开始擦除指定长度，必须是1024的整数倍
FlashStatus FLASH_Erase(uint32_t addr, uint32_t size)
{
	#define PAGE_ERASE_SUCCESS	0xFFFFFFFF // 判断是否擦除成功
	#define TIMEOUT_COUNT		0xFFFFFFFF // 超时等待
	__IO uint32_t nIndex = 0, timeout = 0;
	FlashStatus status = FLASH_COMPLETE;
	
	// step0：校验地址是否为页的整数倍，校验地址是否为小于基址，是否越界
	if(((addr + size) % STM32_PAGE_SIZE != 0) || (addr < STM32_FLASH_BASE) 
		|| (addr >= FLASH_BASE + (MAX_PAGE_COUNT * STM32_PAGE_SIZE)))
		return FLASH_ERROR;
	
	FLASH_Unlock();
	
	// step1：等待flash可操作
	timeout = 0;
	while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
	{
		if(timeout++ >= TIMEOUT_COUNT) 
			break;
	}
	
	// step2：擦除flash
	for(nIndex = addr; nIndex < addr + size; nIndex += STM32_PAGE_SIZE)
	{
		SET_BIT(FLASH->CR, FLASH_CR_PER); // 按页擦除
		WRITE_REG(FLASH->AR, FLASH_AR_FAR & nIndex); // 待擦除页首地址
		SET_BIT(FLASH->CR, FLASH_CR_STRT); // 开始擦除
		
		// 等待擦除
		timeout = 0;
		while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
		{
			if(timeout++ >= TIMEOUT_COUNT) 
			{
				FLASH_Lock();
				return FLASH_ERASE_ERROR;
			}
		}
		
		// 读取验证是否擦除成功
		if(*(uint32_t *)nIndex != PAGE_ERASE_SUCCESS)
		{
			status = FLASH_ERASE_ERROR;
			//p_err("FLASH_Erase error at 0x%x", nIndex);
			break;
		}
	}
	// 清除标记
	SET_BIT(FLASH->SR, FLASH_SR_EOP);
	CLEAR_BIT(FLASH->CR, FLASH_CR_PER);
	
	FLASH_Lock();
	
	return status;
}

// 从指定地址开始写入
FlashStatus FLASH_Program(uint8_t program_type, uint32_t addr, uint32_t val)
{
	#define TIMEOUT_COUNT		0xFFFFFFFF // 超时等待
	__IO uint32_t timeout = 0;
	FlashStatus status = FLASH_COMPLETE;

	// step0：解锁
	FLASH_Unlock();
	
	// step1：等待flash可操作
	timeout = 0;
	while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
	{
		if(timeout++ >= TIMEOUT_COUNT) 
			break;
	}
	if(RESET == READ_BIT(FLASH->CR, FLASH_CR_PG))
		SET_BIT(FLASH->CR, FLASH_CR_PG); // 配置编程
	
	switch(program_type)
	{
	#if 0
	case FLASH_TYPEPROGRAM_BYTE: // 写入字节（8bit）
		*(__IO uint8_t *)addr = (uint8_t)(val);
		break;
	#endif
	case FLASH_TYPEPROGRAM_HALFWORD: // 写入半字（16bit）
		*(__IO uint16_t *)addr = (uint16_t)(val);
		break;
		
	case FLASH_TYPEPROGRAM_WORD: // 写入字（32bit）
		*(__IO uint16_t *)addr = (uint16_t)(val);
		__ISB();
		*(__IO uint16_t *)(addr+2) = (uint16_t)(val>>16);
		break;
	
	default:
		status = FLASH_WRITE_ERROR;
		break;
	}
	
	if(status != FLASH_WRITE_ERROR)
	{
		// 等待写入
		timeout = 0;
		while(READ_BIT(FLASH->SR, FLASH_SR_BSY))
		{
			if(timeout++ >= TIMEOUT_COUNT) 
			{
				FLASH_Lock();
				return FLASH_ERROR;
			}
		}
		// 读取验证是否写入成功
		if(*(uint32_t *)addr != val)
		{
			status = FLASH_WRITE_ERROR;
			//p_err("FLASH_Write error at 0x%x", addr);
		}
	}
	// 清除标记
	if(READ_BIT(FLASH->SR, FLASH_SR_EOP))
		SET_BIT(FLASH->SR, FLASH_SR_EOP);
	
	if(READ_BIT(FLASH->CR, FLASH_CR_PG))
		CLEAR_BIT(FLASH->CR, FLASH_CR_PG); 
	
	FLASH_Lock(); // 上锁
	
	return status;
}

// 读出字
uint32_t FLASH_ReadWord(uint32_t addr)
{
	return *(__IO uint32_t*)addr;
}


// 从指定地址开始写入指定长度的数据
FlashStatus FLASH_Write(uint32_t WriteAddr, uint32_t *pBuffer, uint32_t NumToWrite)
{
	uint32_t i = 0;
	FlashStatus status = FLASH_COMPLETE;
	
	/* Step0：校验地址是否为小于基址，是否越界 */
	if((WriteAddr < STM32_FLASH_BASE) || (WriteAddr >= FLASH_BASE + (MAX_PAGE_COUNT * STM32_PAGE_SIZE)))
		return FLASH_ERROR;
	
	/* Step1：写入前先进行擦除操作 */
	if(NumToWrite < STM32_PAGE_SIZE)
		FLASH_Erase(WriteAddr, STM32_PAGE_SIZE); // 小于1页，直接按1页擦除
	else if(NumToWrite % STM32_PAGE_SIZE == 0)
		FLASH_Erase(WriteAddr, NumToWrite); // 整数倍，直接按页倍数擦除
	else
		FLASH_Erase(WriteAddr, (NumToWrite/STM32_PAGE_SIZE)+1); // 非整数倍，多擦除1页
	
	/* Step2：解锁 */
	status = FLASH_Unlock();
	if(status != FLASH_COMPLETE)
		return FLASH_ERROR;
	
	/* Step3：写入flash */
	while(i < NumToWrite)
	{
		status = FLASH_Program(FLASH_TYPEPROGRAM_WORD, WriteAddr, *(pBuffer + i));
		if(status != FLASH_COMPLETE)
		{
			FLASH_Lock();
			return FLASH_WRITE_ERROR;
		}
		WriteAddr += 4;
		i++; // 数据偏移1个
	}
	
	/* Step4：重新上锁锁 */
	status = FLASH_Lock();
	if(status != FLASH_COMPLETE)
		return FLASH_ERROR;
	
	return status;
}

// 从指定地址开始读出指定长度的数据
void FLASH_Read(uint32_t ReadAddr, uint32_t *pBuffer, uint32_t NumToRead)
{
	uint32_t dataIndex;
	
	// 校验地址是否为小于基址，是否越界
	if((ReadAddr < STM32_FLASH_BASE) || (ReadAddr >= FLASH_BASE + (MAX_PAGE_COUNT * STM32_PAGE_SIZE)))
		return;
	
    for (dataIndex = 0; dataIndex < NumToRead; dataIndex++)
    {
        pBuffer[dataIndex] = FLASH_ReadWord(ReadAddr + dataIndex * 4);
    }
}
