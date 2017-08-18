#pragma once
//启动监听服务器，只负责数据收发
#ifndef __ACCEPTOR_H__
#define __ACCEPTOR_H__
#include "Socket.h"
#include "inetaddress.h"
namespace wd
{
class acceptor{
    Socket _listensock;
    inetaddress _addr;
    void set_reuse_addr(bool on);
    void set_reuse_port(bool on);
    void bind();
    void listen();
public:
    acceptor(int listenfd, const inetaddress & addr);
    void ready();
    int accept();
    int fd() const;

};
}
#endif
