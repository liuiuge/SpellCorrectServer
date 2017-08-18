
#include "timerthread.h"
#include "spellcorrectserver.h"
#include "mytask.h"
#include "dictproducer.h"
#include <stdio.h>
#include <stdlib.h>
using std::cout;
namespace wd{
spellcorrectserver::spellcorrectserver(const string & conf_file_name)
    :_conf(conf_file_name)
    ,_ctemp(_conf.getconfigmap())
    ,_tcpserver(_ctemp["ip"],static_cast<unsigned short>(atoi(_ctemp["port"].c_str())))
    ,_threadpool(4,10)
    ,_timer(std::bind(&cachemanager::updateCache),
            wd::str2int(_ctemp["init_time"]),
            wd::str2int(_ctemp["intv_time"]))
{}
void spellcorrectserver::start()
{
    _threadpool.start();
    _timer.start();
	wd::dict_producer dict1(_ctemp["srcen"]);
    dict1.build_dict();
    dict1.store_dict(_ctemp["dicten"].c_str());
    wd::dict_producer dict2(_ctemp["srccn"],1);
    dict2.build_cn_dict();
    dict2.store_dict(_ctemp["dictcn"].c_str());
	wd::mydict *md1=wd::mydict::createInstance();
	md1->init_dict(_ctemp["dicten"].c_str(), _ctemp["dictcn"].c_str());
	printf("%s,%s\n", _ctemp["ip"].c_str(), _ctemp["port"].c_str());
	_tcpserver.set_connection_callback(bind(&wd::spellcorrectserver::onConnection,this, std::placeholders::_1));
	_tcpserver.set_message_callback(bind(&spellcorrectserver::onMessage,this,std::placeholders::_1));
	_tcpserver.set_close_callback(bind(&spellcorrectserver::onClose,this,std::placeholders::_1));
    _tcpserver.start();
}
void spellcorrectserver::onConnection(const tcpconnectionptr & conn)
{
	printf("\n> %s has connected!\n", conn->to_string().c_str());
	conn->send("hello there, welcome");
}
void spellcorrectserver::onMessage(const tcpconnectionptr & conn)
{
    string s = conn->receive();
    
    _threadpool.addtask(std::bind(&wd::mytask::execute,mytask(s,conn)));
}
void spellcorrectserver::onClose(const tcpconnectionptr & conn)
{
    printf("close %s", conn->to_string().c_str());
}
}
#ifdef HELLO
class spellcorrectserver
    configuration _conf;
    tcpserver _tcpserver;
    Thread_pool _threadpool;
    Timer_thread _timer;
public:
    spellcorrectserver(const string & conf_file_name);
    void start();
    void onConnection(tcpconnectionptr & conn);
    void onMessage(tcpconnectionptr & conn);
    void onclose(tcpconnectionptr & conn);
};
}
#endif
