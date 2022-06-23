#pragma once
#include "KThread.h"
class KThreadSend :
    public KThread
{
    bool        Run() override;
public:
    KThreadSend();
    KThreadSend(LPVOID pValue);
    ~KThreadSend();
};

