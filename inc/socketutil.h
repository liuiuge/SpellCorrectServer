#pragma once
#ifndef __SOCKETUTIL_H__
#define __SOCKETUTIL_H__
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/eventfd.h>
#include <sys/epoll.h>

namespace wd{

inline int create_socket_fd()
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if(fd == -1){
        perror("socket create error\n");
    }
    return fd;
}
inline void set_non_block(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}
inline int create_epoll_fd()
{
    int efd= epoll_create1(0);
    if(-1 == efd){
        perror("epoll error");
        exit(EXIT_FAILURE);
    }
    return efd;
}
inline int create_event_fd()
{
	int evefd = ::eventfd(0, EFD_NONBLOCK|EFD_CLOEXEC);
	if(-1 == evefd)
	{
		perror("eventfd create error");
	}
	return evefd;
}
	
inline void add_epoll_read_fd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    ev.events = EPOLLIN;
    int ret = epoll_ctl(efd, EPOLL_CTL_ADD,fd, &ev);
    if(ret == -1)
    {
        perror("epoll add fail");
        exit(EXIT_FAILURE);
    }
}
inline void del_epoll_read_fd(int efd, int fd)
{
    struct epoll_event ev;
    ev.data.fd = fd;
    int ret = epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
    if(ret == -1){
        perror("epoll_ctl del error");
        exit(EXIT_FAILURE);
    }
}
inline size_t recv_peek(int sockfd, void * buf, int len)
{
    int nread;
    do{
        nread = recv(sockfd, buf, len, MSG_PEEK);
    }while(nread == -1 && errno == EINTR);
    return nread;
}
inline bool is_connection_closed(int sockfd)
{
    char buf[1024];
    int nread= recv_peek(sockfd,buf, sizeof(buf));
    if(-1 == nread)
    {
        perror("recvPeek---");
        return true;
    }
    return (0 == nread);
}
}
#endif
