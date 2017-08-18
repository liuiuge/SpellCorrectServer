#pragma once
#ifndef __TIMER_H__
#define __TIMER_H__
#include <functional>
namespace wd
{
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
    ~Timer();
};
}
#endif
