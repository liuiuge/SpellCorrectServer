#include "Socket.h"
#include "socketutil.h"
#include "inetaddress.h"
namespace wd{
Socket::Socket(int sockfd)
    :_sockfd(sockfd)
{}
Socket::Socket()
    :_sockfd(create_socket_fd())
{}
Socket::~Socket()
{
    close(_sockfd);
}

void Socket::nonblock()
{
    set_non_block(_sockfd);
}
void Socket::shutdownwrite()
{
    if(::shutdown(_sockfd, SHUT_WR) == -1)
    {
        perror("shutdown write error\n");
    }
}
inetaddress Socket::getlocaladdress(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(::getsockname(sockfd, (struct sockaddr *)& addr, &len) == -1)
        perror("getsockname error\n");
    return inetaddress(addr);
}
inetaddress Socket::getpeeraddress(int sockfd)
{
    struct sockaddr_in addr;
    socklen_t len = sizeof(sockaddr_in);
    if(::getpeername(sockfd, (struct sockaddr *)&addr, &len) == -1)
        perror("getpeername error\n");
    return inetaddress(addr);
}

}
