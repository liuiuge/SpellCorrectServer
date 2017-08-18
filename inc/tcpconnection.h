#pragma once
#ifndef __TCPCONNECTION_H__
#define __TCPCONNECTION_H__
#include "inetaddress.h"
#include "Socket.h"
#include "socketio.h"
#include <string>
#include <memory>
using std::shared_ptr;
#include <functional>
using std::function;
namespace wd
{
class epollpoller;
class tcpconnection;
typedef shared_ptr<tcpconnection> tcpconnectionptr;
class tcpconnection:
public std::enable_shared_from_this<tcpconnection>
{
public:
    typedef function<void(const tcpconnectionptr &)> tcp_connection_callback;
    tcpconnection(int sockfd, epollpoller *loop);
    ~tcpconnection();
    std::string receive();
    void send(const std::string & msg);
    void sendandclose(const std::string &msg);
    void sendinLoop(const std::string & msg);
    void shutdown();
    std::string to_string();
    void set_connection_callback(tcp_connection_callback cb);
    void set_message_callback(tcp_connection_callback cb);
    void set_close_callback(tcp_connection_callback cb);
    void handle_connection_callback();
    void handle_message_callback();
    void handle_close_callback();
private:
    Socket _sockfd;
    socketio _sockio;
    const inetaddress _localaddr;
    const inetaddress _peeraddr;
    bool is_shutdown_write;
    epollpoller * _loop;
    tcp_connection_callback _on_connection_cb;
    tcp_connection_callback _on_message_cb;
    tcp_connection_callback _on_close_cb;
};
}
#endif
