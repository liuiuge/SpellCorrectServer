

//Author:liuiuge(1220595883@qq.com

#include "threadpool.h"
#include <unistd.h>

using std::bind;
namespace wd{
Thread_pool::Thread_pool(size_t threadnum, size_t queSize):
       _threadnum(threadnum),
       _quesize(queSize),
       _taskbuffer(_quesize),
       _isexit(false)
    {
        _vecthread.reserve(_threadnum);
    }
Thread_pool::~Thread_pool()
   {
       if(!_isexit)
           stop();
   }
void Thread_pool::start()
    {
	for(size_t idx = 0;idx != _threadnum; ++idx){
           Thread * pthread = new Thread(std::bind(&Thread_pool::threadpoolFunc,this),int2str(idx));
           _vecthread.push_back(pthread);
       }
    for(auto & thread: _vecthread){
		thread->start();
	}
    }
void Thread_pool::stop(){
    if(!_isexit){
        while(!_taskbuffer.empty())
           sleep(1);
//	_taskbuffer.wakeup();
        _isexit = true;
        for(auto & thread: _vecthread){
            thread->join();
            delete thread;
        }
   }
}
void Thread_pool::addtask(const Task & task)
   {
       _taskbuffer.push(task);
   }
Task Thread_pool::gettask()
   {
       return _taskbuffer.pop();
   }
void Thread_pool::threadpoolFunc()
   {
       while(!_isexit)
       {
           Task  task= gettask();
           if(task)
               task();
       }
   }
}
