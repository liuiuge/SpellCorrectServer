#include "acceptor.h"
#include "socketutil.h"

#include <iostream>

using std::cout;

namespace wd
{
acceptor::acceptor(int listenfd, const inetaddress & addr):
    _listensock(listenfd),
    _addr(addr)
{}
int acceptor::fd() const
{
    return _listensock.fd();
}
void acceptor::ready()
{
    set_reuse_addr(true);
    set_reuse_port(true);
    bind();
    listen();
}
void acceptor::set_reuse_addr(bool flag)
{
    int on = (flag ? 1:0);
    if(::setsockopt(_listensock.fd(),
        SOL_SOCKET,
        SO_REUSEADDR,
        &on,
        static_cast<socklen_t>(sizeof(on))) == -1)
    {
        perror("setsockopt reuseaddr error");
        ::close(_listensock.fd());
        exit(EXIT_FAILURE);
    }
}
void acceptor::set_reuse_port(bool flag)
{
#ifdef SO_REUSEPORT
    int on = (flag ? 1 : 0);
    if(::setsockopt(_listensock.fd(),
        SOL_SOCKET,
        SO_REUSEPORT,
        &on,
        static_cast<socklen_t>(sizeof(on))) == -1)
    {
        perror("setsockopt reuse port err\n");
        ::close(_listensock.fd());
        exit(EXIT_FAILURE);
    }
#else
    if( flag )
        fprintf(stderr, "SO_REUSEPORT is not support");
#endif
}
void acceptor::bind()
{
    if(-1 == ::bind(_listensock.fd(), 
        (const struct sockaddr *)_addr.getsockaddr(),
        sizeof(inetaddress)))
	
    {
        perror("bind error");
        ::close(_listensock.fd());
        exit(EXIT_FAILURE);
    }
	cout<<"bind\n";
}
void acceptor::listen()
{
    if(-1 == ::listen(_listensock.fd(),10))
    {
        perror("listen error");
        ::close(_listensock.fd());
        exit(EXIT_FAILURE);
    }
	cout<<"listen\n";
}
int acceptor::accept(){
    int peerfd =::accept(_listensock.fd(), NULL,NULL);
    if(-1 == peerfd)
    {
        perror("accept error");
    }
    return peerfd;

}
}


