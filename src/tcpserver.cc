#include "tcpserver.h"
#include "inetaddress.h"
#include "socketutil.h"

using std::cout;
using std::endl;
namespace wd
{
tcpserver::tcpserver(const string & ip, unsigned short port):
    _acce(create_socket_fd(),inetaddress(ip.c_str(),port)),
    _poller(_acce)
    {}
tcpserver::tcpserver(unsigned short port):
    _acce(create_socket_fd(),inetaddress(port)),
    _poller(_acce)
    {}

void tcpserver::start()
{
    _acce.ready();
    _poller.set_connection_callback(_connection_callback);
    _poller.set_message_callback(_message_callback);
    _poller.set_close_callback(_close_callback);
    _poller.loop();
}
void tcpserver::stop()
{
    _poller.unloop();
}

void tcpserver::set_connection_callback(tcp_server_callback cb)
{
    _connection_callback = cb;
}
void tcpserver::set_message_callback(tcp_server_callback cb)
{
    _message_callback = cb;
}
void tcpserver::set_close_callback(tcp_server_callback cb)
{
    _close_callback = cb;
}
}
#ifdef HELLO
    acceptor _acce;
    epollpoller _poller;
    tcpservercallback _connection_callback;
    tcpservercallback _message_callback;
    tcpservercallback _close_callback;
#endif
