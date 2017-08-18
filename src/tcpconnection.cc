#include "tcpconnection.h"
#include "epollpoller.h"
#include <stdio.h>
#include <string.h>
namespace wd
{
tcpconnection::tcpconnection(int sockfd,epollpoller * loop):
    _sockfd(sockfd),
    _sockio(sockfd),
    _localaddr(wd::Socket::getlocaladdress(sockfd)),
    _peeraddr(wd::Socket::getpeeraddress(sockfd)),
    is_shutdown_write(false),
    _loop(loop)
{
    _sockfd.nonblock();
}
        
tcpconnection::~tcpconnection()
{
    if(!is_shutdown_write)
    {
        is_shutdown_write = true;
        shutdown();
    }
    printf("~tcpconnection()\n");
}
std::string tcpconnection::receive()
{
    char buf[65535];
    memset(buf, 0, sizeof(buf));
    size_t ret = _sockio.readline(buf, sizeof(buf));
    if(ret == 0){
        return std::string();
    }else
        return std::string(buf);
}
void tcpconnection::send(const std::string & msg)
{
    _sockio.writen(msg.c_str(), msg.size());
}
void tcpconnection::sendandclose(const std::string & msg)
{
    send(msg);
    shutdown();
}
void tcpconnection::sendinLoop(const std::string & msg)
{
    _loop->runinloop(std::bind(&tcpconnection::sendandclose, this, msg));
}
void tcpconnection::shutdown()
{
    if(!is_shutdown_write)
        _sockfd.shutdownwrite();
    is_shutdown_write = 1;
}
std::string tcpconnection::to_string()
{
    char str[128];
    ::snprintf(str, sizeof(str),"%s:%d -> %s:%d",
        _localaddr.ip().c_str(),
        _localaddr.port(),
        _peeraddr.ip().c_str(),
        _peeraddr.port());
    return std::string(str);
}
void tcpconnection::set_connection_callback(tcp_connection_callback cb)
{
    _on_connection_cb = cb;
}
void tcpconnection::set_message_callback(tcp_connection_callback cb)
{
    _on_message_cb = cb;
}
void tcpconnection::set_close_callback(tcp_connection_callback cb)
{
    _on_close_cb = cb;
}
void tcpconnection::handle_connection_callback()
{
    if(_on_connection_cb)
        _on_connection_cb(shared_from_this());
}
void tcpconnection::handle_message_callback()
{
    if(_on_message_cb)
        _on_message_cb(shared_from_this());
}
void tcpconnection::handle_close_callback()
{
    if(_on_close_cb)
        _on_close_cb(shared_from_this());
}

#ifdef HELLO
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
    Socket _sockfd;
    socketio _sockio;
    const inetaddress _localaddr;
    const inetaddress _peeraddr;
    bool is_shutdown_write;
    epollpoller * _loop;
    tcp_connection_callback _on_connection_cb;
    tcp_connection_callback _on_message_cb;
    tcp_connection_callback _on_close_cb;
#endif
}
