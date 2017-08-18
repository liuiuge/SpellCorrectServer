#include "inetaddress.h"

#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>

namespace wd{
inetaddress::inetaddress(short port)
{
    memset(&_addr, 0, sizeof(sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = INADDR_ANY;
}

inetaddress::inetaddress(const char * socketip, short port)
{
    memset(&_addr, 0, sizeof(sockaddr_in));
    _addr.sin_family = AF_INET;
    _addr.sin_port = htons(port);
    _addr.sin_addr.s_addr = inet_addr(socketip);
}

inetaddress::inetaddress(const sockaddr_in & socketaddr)
    :_addr(socketaddr)
{}
const struct sockaddr_in *inetaddress::getsockaddr()const
{ return &_addr;  }

std::string inetaddress::ip() const
{ return std::string(inet_ntoa(_addr.sin_addr)); }

unsigned short inetaddress::port() const
{ return ntohs(_addr.sin_port); }

}

