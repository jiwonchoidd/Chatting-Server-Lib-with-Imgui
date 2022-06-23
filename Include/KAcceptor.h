#pragma once
#include "KThread.h"
class KAcceptor: public KThread
{
public:
    bool        Run() override;
public:
    KAcceptor();
    KAcceptor(LPVOID pValue);
    ~KAcceptor();
};

