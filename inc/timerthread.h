#pragma once
#ifndef __TIMERTHREAD_H__
#define __TIMERTHREAD_H__
#include "timer.h"
#include "thread.h"
namespace wd
{
class timerthread
{
    typedef std::function<void()> Timer_callback;
    Timer _timer;
    Thread _thread;
    bool _isstarted;
public:
    timerthread(const Timer_callback & cb, int initialtime, int intervaltime);
    void start();
    void stop();
};
}
#endif
