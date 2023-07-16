#pragma once
#include "CThread.h"
class CThreadComm :
    public CThread
{
public:
    CThreadComm(void);
    virtual ~CThreadComm(void);
    virtual void runTask(void);
};

