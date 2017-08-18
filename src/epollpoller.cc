#include "epollpoller.h"
#include "socketutil.h"
#include "acceptor.h"
#include <assert.h>
namespace wd
{
epollpoller::epollpoller(acceptor & accpt):
    _accpt(accpt),
    _epoll_fd(create_epoll_fd()),
    _listenfd(accpt.fd()),
	_wakeupfd(create_event_fd()),
    _isloopng(0),
    _eventlist(1024)
{
    add_epoll_read_fd(_epoll_fd, _listenfd);
	add_epoll_read_fd(_epoll_fd,_wakeupfd);
}

epollpoller::~epollpoller()
{
    ::close(_epoll_fd);
}
void epollpoller::loop()
{
    _isloopng = 1;
    while(_isloopng)
    {
        wait_epoll_fd();
    }
}
void epollpoller::unloop()
{
    if(_isloopng)
        _isloopng = 0;
}
void epollpoller::runinloop(const functor & cb)
{
    {
        MutexLockGuard mlg(_mutex);
        _pendingfuntors.push_back(cb);
    }
    wakeup();
}
void epollpoller::do_pending_functors()
{
    vector<functor> functorx;
    {
        MutexLockGuard mlg(_mutex);
        functorx.swap(_pendingfuntors);
    }
    for(size_t i = 0; i < functorx.size(); ++i)
        functorx[i]();
}
void epollpoller::wakeup()
{
    uint64_t one = 1;
    ssize_t n = write(_wakeupfd, &one, sizeof(one));
	printf("n = %ld\n",n);
    if( n != sizeof(one) )
    {
        perror("epollpoller wakeup ");
    }
}
void epollpoller::handleread()
{
    uint64_t one = 1;
    ssize_t n = read(_wakeupfd, &one, sizeof(one));
    if(n != sizeof(one))
        perror("epollpoller handleread");
}
void epollpoller::set_connection_callback(epoll_callback cb)
{
    _on_connection_cb = cb;
}
void epollpoller::set_message_callback(epoll_callback cb)
{
    _on_message_cb = cb;
}
void epollpoller::set_close_callback(epoll_callback cb)
{
    _on_close_cb = cb;
}
void epollpoller::wait_epoll_fd()
{
	int nready;
	do
	{
		nready = ::epoll_wait(_epoll_fd, 
			&(*_eventlist.begin()),
			_eventlist.size(),
			5000);
	}while(nready == -1 && errno == EINTR);
	if(nready == -1)
	{
		perror("epoll_wait error");
		exit(EXIT_FAILURE);
	}
	else if(nready == 0)
	{
		printf("epoll_wait timeout\n");	
	}
	else
	{
		//做一个扩容的操作
		if(nready == static_cast<int>(_eventlist.size()))
		{
			_eventlist.resize(_eventlist.size() * 2);
		}
		
		//遍历每一个激活的文件描述符
		for(int idx = 0; idx != nready; ++idx)
		{
			if(_eventlist[idx].data.fd == _listenfd)
			{
				if(_eventlist[idx].events & EPOLLIN)
				{
					handle_connection();
				}
			}
			else if(_eventlist[idx].data.fd == _wakeupfd)
			{
				printf("wakeupfd light\n");
				if(_eventlist[idx].events & EPOLLIN)
				{
					handleread();
					do_pending_functors();
				}
			}
			else
			{
				if(_eventlist[idx].events & EPOLLIN)
				{
					handle_message(_eventlist[idx].data.fd);
				}
			}
		}//end for
	}//end else
}void epollpoller::handle_connection()
{
    int peerfd = _accpt.accept();
    add_epoll_read_fd(_epoll_fd, peerfd);
    tcpconnectionptr conn(new tcpconnection(peerfd,this));
    conn->set_connection_callback(_on_connection_cb);
    conn->set_message_callback(_on_message_cb);
    conn->set_close_callback(_on_close_cb);

    std::pair<connectionmap::iterator,bool> ret;
    ret = _connmap.insert(std::make_pair(peerfd, conn));
    assert(ret.second == true);
    (void)ret;
	conn->handle_connection_callback();
}
void epollpoller::handle_message(int peerfd)
{   
    bool isclosed = is_connection_closed(peerfd);
    connectionmap::iterator it = _connmap.find(peerfd);
    assert(it != _connmap.end());
    if(isclosed)
    {
        it->second->handle_close_callback();
        del_epoll_read_fd(_epoll_fd,peerfd);
        _connmap.erase(it);
    }else
    {
        it->second->handle_message_callback();
    }
}
}

#ifdef HELLO
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

#endif
