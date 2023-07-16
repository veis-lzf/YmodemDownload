#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "main.h"

#include <stdlib.h>
#include <stdio.h>
 
#ifndef uint32_t
#define uint32_t unsigned int
#endif
 
#define DEBUG_ASSERT
 
#ifndef NULL
#define NULL (void *)0
#endif
 
#ifndef FALSE
#define FALSE 0
#endif
 
#ifndef TRUE
#define TRUE 1
#endif
 
// 定义校验宏
#ifdef DEBUG_ASSERT
#define ASSERT_ERROR(x) do{if(!x){printf("[ERROR]:file[%s] line[%d] function[%s]\r\n", __FILE__, __LINE__, __func__);while(1);}}while(0)
#else
#define ASSERT_ERROR(x)
#endif
 
typedef uint8_t DATA;
 
 
// 环形队列数据结构
typedef struct SQueue
{
    DATA *m_pData;
    uint32_t m_nHead, m_nTail;
    uint32_t m_nCount, m_nSize;
} queue, *p_queue;
 
p_queue CreateQueue(uint32_t nCount);
void RemoveAll(p_queue p);
int GetSize(p_queue p);
int IsFull(p_queue p);
int IsEmpty(p_queue p);
int Push(p_queue p, const DATA *pdata);
int PushData(p_queue p, const DATA d);
int Pop(p_queue p, DATA *pdata);
 
#endif
