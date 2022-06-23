#pragma once
#include "KThread.h"
class KCommandServer : public KThread
{
private:
    enum
    {
        COMMAND_NORMAL = 0,
        COMMAND_HELP = 1,
        COMMAND_EXIT = 2,
        COMMAND_KICK = 3,
    };
    std::string                 m_Input;
    int                         m_CommandMode = 0;
    std::map<std::string, int>  m_CommandMap = { {"/help",1} ,{"/exit",2},{"/kick",3}};
    std::map<std::string,int>
        ::iterator              m_iter_map;
private:
    void        HelpMode();
    void        ExitMode();
    void        KickMode();
public:
    bool        Run() override;
public:
    KCommandServer();
    KCommandServer(LPVOID pValue);
    ~KCommandServer();
};

