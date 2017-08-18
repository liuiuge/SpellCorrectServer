#include "timerthread.h"
namespace wd
{
timerthread::timerthread(const Timer_callback & cb, int initialtime, int intervaltime)
    :_timer(cb, initialtime, intervaltime)
    ,_thread(std::bind(&Timer::start,&_timer))
    ,_isstarted(0)
{}
void timerthread::start()
{
    _isstarted = 1;
    _thread.start();
}
void timerthread::stop()
{
    if(_isstarted)
    {
        _timer.stop();
        _thread.join();
        _isstarted = 0;
    }
}
}
#ifdef HELLO
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
