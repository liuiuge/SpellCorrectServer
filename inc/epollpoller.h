#pragma once
#ifndef __EPOLLPOLLER_H__
#define __EPOLLPOLLER_H__
#include "tcpconnection.h"
#include "mutexlock.h"
#include <sys/epoll.h>
#include <vector>
using std::vector;
#include <map>
#include <functional>
namespace wd
{
class acceptor;
class epollpoller
{
public:
    typedef tcpconnection::tcp_connection_callback epoll_callback;
    typedef std::function<void()> functor;
    epollpoller(acceptor & accpt);
    ~epollpoller();
    void loop();
    void unloop();
    void runinloop(const functor & cb);
    void do_pending_functors();
    void wakeup();
    void handleread();
    void set_connection_callback(epoll_callback cb);
    void set_message_callback(epoll_callback cb);
    void set_close_callback(epoll_callback cb);
private:
    void wait_epoll_fd();
    void handle_connection();
    void handle_message(int peerfd);

    acceptor & _accpt;
    int _epoll_fd;
    int _listenfd;
    int _wakeupfd;
    bool _isloopng;

    Mutex_lock _mutex;
    vector<functor> _pendingfuntors;
    
    typedef std::vector<struct epoll_event> eventlist;
    eventlist _eventlist;
    typedef std::map<int, tcpconnectionptr> connectionmap;
    connectionmap _connmap;
    epoll_callback _on_connection_cb;
    epoll_callback _on_message_cb;
    epoll_callback _on_close_cb;
};
}
#endif
