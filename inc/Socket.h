#pragma once
//只负责fd的创建关闭
#ifndef __SOCKET_H__
#define __SOCKET_H__
namespace wd{
class inetaddress;
class Socket{
public:
    Socket(int sockfd);
    Socket();
    ~Socket();
    void shutdownwrite();
    int fd()const {	return _sockfd;	}
    void nonblock();
    static inetaddress getlocaladdress(int sockfd);
    static inetaddress getpeeraddress(int sockfd);
private:
    int _sockfd;
};
}
#endif
