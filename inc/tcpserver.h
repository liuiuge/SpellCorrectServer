#pragma once
#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include "acceptor.h"
#include "epollpoller.h"

using std::string;
namespace wd
{
class tcpserver
{
public:
    typedef tcpconnection::tcp_connection_callback tcp_server_callback;
    tcpserver(const string & ip, unsigned short port);
    tcpserver(unsigned short port);

    void start();
    void stop();

    void set_connection_callback(tcp_server_callback cb);
    void set_message_callback(tcp_server_callback cb);
    void set_close_callback(tcp_server_callback cb);
private:
    acceptor _acce;
    epollpoller _poller;
    tcp_server_callback _connection_callback;
    tcp_server_callback _message_callback;
    tcp_server_callback _close_callback;
};
}


#endif
