#pragma once
#ifndef __SPELLCORRECTSERVER_H__
#define __SPELLCORRECTSERVER_H__

#include "timerthread.h"
#include "configuration.h"
#include "tcpserver.h"
#include "threadpool.h"
using std::map;
namespace wd{
class spellcorrectserver
{
    configuration _conf;
    map<string,string> _ctemp;
    tcpserver _tcpserver;
    Thread_pool _threadpool;
    timerthread _timer;
public:
    spellcorrectserver(const string & conf_file_name);
    void start();
    void onConnection(const tcpconnectionptr & conn);
    void onMessage(const tcpconnectionptr & conn);
    void onClose(const tcpconnectionptr & conn);
};
}
#endif
