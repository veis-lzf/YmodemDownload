#include "cqueue.h"
#include "usart.h"

#ifdef USART_QUEUE

#ifndef MEM_MALLOC
	#include "string.h"
	struct SQueue g_queue;
	static __IO uint8_t rx_buffer[DBG_RX_BUFFER_SIZE];
#endif

/**
 * [CreateQueue 创建一个队列]
 * @param  nCount [队列的长度]
 * @return        [队列指针]
 */
p_queue CreateQueue(uint32_t nCount)
{
#ifdef MEM_MALLOC
    p_queue p = malloc(sizeof(struct SQueue));
    p->m_pData = malloc(sizeof(DATA) * nCount); // 核心数据长度
    p->m_nCount = nCount; // 队列长度
#else
	p_queue p = &g_queue;
    p->m_pData = (DATA *)rx_buffer;
	
	// 队列长度
	if(nCount >= DBG_RX_BUFFER_SIZE)
		p->m_nCount = DBG_RX_BUFFER_SIZE;
	else
		p->m_nCount = nCount;
#endif
    p->m_nSize = 0; // 队列中元素个数
    p->m_nHead = p->m_nTail = 0; // 队头和队尾
    return p;
}
 
/**
 * [RemoveAll 释放队列中申请的内存]
 * @param p [队列指针]
 */
void RemoveAll(p_queue p)
{
    if(!p) return;
#ifdef MEM_MALLOC
	p->m_nCount = p->m_nSize = 0;
    p->m_nHead = p->m_nTail = 0;
    free(p->m_pData);
    free(p);
#else
	memset(p, 0, p->m_nCount);
	p->m_nCount = p->m_nSize = 0;
    p->m_nHead = p->m_nTail = 0;
	p = NULL;
#endif
}
 
/**
 * [GetSize 获取队列中的元素个数]
 * @param  p [队列指针]
 * @return   [队列中元素的个数]
 */
int GetSize(p_queue p)
{
    ASSERT_ERROR(p);
    return p->m_nSize;
}
 
/**
 * [IsFull 判断队列是否满了]
 * @param  p [队列指针]
 * @return   [满则返回1.否则返回0]
 */
int IsFull(p_queue p)
{
    ASSERT_ERROR(p);
    return (p->m_nTail + 1) % p->m_nCount == p->m_nHead;
}
 
/**
 * [IsEmpty 判断队列是否为空]
 * @param  p [队列指针]
 * @return   [空则返回1，否则返回0]
 */
int IsEmpty(p_queue p)
{
    ASSERT_ERROR(p);
    return p->m_nHead == p->m_nTail;
}
 
/**
 * [Push 入队一个数据]
 * @param  p     [队列指针]
 * @param  pdata [需要入队的数据的地址]
 * @return       [成功入队返回TRUE，否则返回FALSE]
 */
int Push(p_queue p, const DATA *pdata)
{
    ASSERT_ERROR(p);
    if(IsFull(p))
        return FALSE;
    ++(p->m_nSize);
    p->m_nTail = p->m_nTail % p->m_nCount;
    p->m_pData[(p->m_nTail)++] = *pdata;
    return TRUE;
}
 
/**
 * [PushData 入队一个数据]
 * @param  p [队列指针]
 * @param  d [需要入队的数据]
 * @return   [成功入队返回TRUE,否则返回FALSE]
 */
int PushData(p_queue p, const DATA d)
{
    ASSERT_ERROR(p);
    if(IsFull(p))
        return FALSE;
    ++(p->m_nSize);
    p->m_nTail = p->m_nTail % p->m_nCount;
    p->m_pData[(p->m_nTail)++] = d;
    return TRUE;
}
 
/**
 * [Pop 弹出一个元素]
 * @param  p     [队列指针]
 * @param  pdata [接收弹出的数据，不需要此传出参数可以输入NULL]
 * @return       [成功弹出返回TRUE,否则返回FALSE]
 */
int Pop(p_queue p, DATA *pdata)
{
    ASSERT_ERROR(p);
    if (IsEmpty(p))
        return FALSE;
    --(p->m_nSize);
    p->m_nHead = p->m_nHead % p->m_nCount;
    if(pdata)
        *pdata = p->m_pData[p->m_nHead];
    ++(p->m_nHead);
    return TRUE;
}
#endif /* USART_QUEUE */
