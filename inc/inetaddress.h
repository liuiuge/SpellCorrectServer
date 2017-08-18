#pragma once
#ifndef __INETADDRESS_H__
#define __INETADDRESS_H__

#include <netinet/in.h>
#include <string>

namespace wd{
class inetaddress{
public:
    inetaddress(short port);
    inetaddress(const char * socketip, short port);
    inetaddress(const struct sockaddr_in & socketaddr);
    const struct sockaddr_in * getsockaddr() const;
    std::string ip() const;
    unsigned short port() const;
private:
    sockaddr_in _addr;
};
}
#endif
