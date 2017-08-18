#include "socketio.h"
#include "socketutil.h"

namespace wd
{
socketio::socketio(int sockfd)
    :_sockfd(sockfd)
{}
size_t socketio::readn(char *buf, size_t count)
{
    size_t nleft = count;
    char * pbuf = buf;
    while(nleft > 0)
    {
        int nread = ::read(_sockfd, pbuf, nleft);
        if(nread == -1){
            if(errno == EINTR)
                continue;
            return EXIT_FAILURE;
        }else if(0 == nread)
        {
            break;
        }
        pbuf += nread;
        nleft -= nread;
    }
    return (count - nleft);
}
size_t socketio::writen(const char *buf, size_t count)
{
    size_t nleft = count;
    const char * pbuf = buf;
    while(nleft>0)
    {
        int nwrite = ::write(_sockfd, pbuf, nleft);
        if(nwrite == -1){
            if(errno == EINTR)
                continue;
            return EXIT_FAILURE;
        }
        nleft -=nwrite;
        pbuf += nwrite;
    }
    return (count - nleft);
}
size_t socketio::recv_peek(char *buf, size_t count)
{
    int nread;
    do
    {
        nread = ::recv(_sockfd, buf, count, MSG_PEEK);
    }while(nread == -1 && errno == EINTR);
    return nread;
}
size_t socketio::readline(char * buf, size_t max_len)
{
    size_t nleft = max_len - 1;
    char *pbuf = buf;
    size_t total = 0;
    while( nleft > 0 )
    {
        int nread = recv_peek( pbuf, nleft );
        if( nread <= 0 )
            return nread;
        for(int idx = 0; idx != nread; ++idx)
        {
            if(pbuf[idx] == '\n')
            {
                size_t nsize = idx +1;
                if(readn(pbuf, nsize) != nsize)
                    return EXIT_FAILURE;
                pbuf += nsize;
                total += nsize;
                *pbuf = 0;
                return total;
            }
            if(readn(pbuf, nread) != (size_t)nread)
                return EXIT_FAILURE;
            pbuf += nread;
            nleft -= nread;
            total += nread;
        }
    }
    *pbuf = 0;
    return max_len -1;
}
}
