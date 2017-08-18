#include "timer.h"
#include <poll.h>
#include <sys/timerfd.h>
#include <sys/types.h>
#include <unistd.h>
#include <iostream>
namespace wd
{
Timer::Timer(const Timer_callback & cb, int initialtime, int intervaltime)
    :_timerfd(create_timer_fd())
    ,_intervaltime(intervaltime)
    ,_initialtime(initialtime)
    ,_isstarted(false)
    ,_cb(cb)
    {
        set_timer_fd(_initialtime,_intervaltime);
    }
void Timer::start()
{
   _isstarted = 1;
   struct pollfd pfd;
   pfd.fd = _timerfd;
   pfd.events = POLLIN;
   while(_isstarted)
   {
       int ret = poll(&pfd, 1, -1);
       if(-1 == ret)
       {
           if(errno == EINTR)
               continue;
           perror("poll error");
           exit(EXIT_FAILURE);
       }
       else if( 0 == ret )
       {
           printf("poll time out");
       }
       else
       {
           if(pfd.events == POLLIN)
           {
               handle_read();
               if(_cb)
                   _cb();
           }
       }
   }
}
int Timer::create_timer_fd()
{
    int ret = timerfd_create(CLOCK_REALTIME, 0);
    if(-1 == ret)
        perror("timerfd create");
    return ret;
}
void Timer::set_timer_fd(int init_sec, int inte_sec)
{
    struct itimerspec it;
    it.it_interval.tv_sec=inte_sec;
    it.it_interval.tv_nsec = 0;
    it.it_value.tv_sec = init_sec;
    it.it_value.tv_nsec = 0;
    int ret = timerfd_settime(_timerfd, 0, &it, NULL);
    if(-1 == ret)
    {
        perror("timerfd_settime err");
        exit(EXIT_FAILURE);
    }
}
void Timer::handle_read()
{
    uint64_t howmany;
    ssize_t ret = read(_timerfd , &howmany,sizeof(howmany));
    if(ret != sizeof(howmany))
        perror("handle read");
}
void Timer::stop()
{
    if(_isstarted)
    {
        set_timer_fd(0,0);
        _isstarted = 0;
    }
}
Timer::~Timer()
{
    if(_isstarted)
        stop();
}
}
#ifdef HELLO
class Timer
{
    int _timerfd, _intervaltime, _initialtime;
    bool _isstarted;
    typedef std::function<void()> Timer_callback;
    Timer_callback _cb;
    int create_timer_fd();
    void handle_read();
    void set_timer_fd(int initialtime, int intervaltime);
public:
    Timer(const Timer_callback & cb, int initialtime,int intervaltime);
    void start();
    void stop();
};
}
#endif
