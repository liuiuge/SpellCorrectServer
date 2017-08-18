#pragma once
#ifndef __THREADPOOL_H__
#define __THREADPOOL_H__
#include "thread.h"
#include "buffer.h"
#include <vector>
#include <memory>
using std::shared_ptr;
using std::vector;
typedef std::function<void()> Task;
namespace wd{
class Thread_pool
{
public:
    Thread_pool(size_t threadnum, size_t quesize);
    ~Thread_pool();
    void start();
    void stop();
    void addtask(const Task & task);
    void initCache();
private:
    Task gettask();
    size_t _threadnum;
    size_t _quesize;
    buffer _taskbuffer;
    void threadpoolFunc();
    vector<Thread *> _vecthread;
    bool _isexit;
};
}//end of namespace

#endif
