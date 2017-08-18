#pragma once
#ifndef __THREAD_H__
#define __THREAD_H__
#include <pthread.h>
#include <sstream>
#include <iostream>
#include <functional>
using std::function;
namespace wd
{
inline std::string int2str(int number)
{
    std::ostringstream oss;
    oss<<number;
    return oss.str();
}
inline unsigned short str2unshort(const std::string & str)
{
    unsigned short port;
    std::istringstream iss(str);
    iss>>port;
    return port;
}
inline int str2int(const std::string & str)
{
    int number;
    std::istringstream iss(str);
    iss>>number;
    return number;
}
namespace current_thread
{
extern __thread const char * threadName;
}
class Thread
{
public:
    typedef function<void()> func_callback;
    Thread(func_callback cb, const std::string & name = std::string());
    ~Thread();
    void start();
    void join();
    pthread_t getthreadid()const
    {
        return pthid;
    }
private:
    static void * threadfunc(void *);
    func_callback _cb;
    bool is_running;
    pthread_t pthid;
    std::string _name;
};
}
#endif
