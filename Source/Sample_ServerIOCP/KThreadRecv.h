#pragma once
#include "KThread.h"
class KThreadRecv :
    public KThread
{
public:
    bool        Run() override;
public:
    KThreadRecv();
    KThreadRecv(LPVOID pValue);
    ~KThreadRecv();
};

