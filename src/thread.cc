#include "thread.h"
#include "cachemanager.h"
#include <functional>
using std::function;
namespace wd
{
namespace current_thread
{
__thread const char * threadName = "wd";
}
struct ThreadData
{
	typedef Thread::func_callback ThreadCallback;
	ThreadCallback _cb;
	string _name;

	ThreadData(const ThreadCallback & cb, const string & name)
	: _cb(cb)
	, _name(name)
	{}


	void runInThread()
	{
		current_thread::threadName = _name.empty()?"wd":_name.c_str();

		if(_cb)
			_cb();
	}

};
Thread::Thread(func_callback cb, const string & name)
    :_cb(cb)
    ,is_running(0)
    ,pthid(0)
    ,_name(name)
{}
Thread::~Thread(){
    if(is_running){
        is_running= 0;
        pthread_detach(pthid);
    }
}
void Thread::start(){
    ThreadData * pdata = new ThreadData(_cb, _name);
    pthread_create(&pthid, NULL, &Thread::threadfunc, pdata);
    is_running = 1;
}
void Thread::join(){
    pthread_join(pthid, NULL);
}
void * Thread::threadfunc(void *argu)
{
    ThreadData *p = static_cast<ThreadData *>(argu);
    if(p)
        p->runInThread();
    delete p;
    return NULL;
}
}
#ifdef HELLO
private:
    static void * threadfunc(void *);
    func_callback _cb;
    bool is_running;
    pthread_t pthid;
};
void * Thread::threadfunc(void *argu)
{
    Thread *p = static_cast<Thread *>(argu);
    if(p)
        p->_cb();
    return NULL;
}
}
#endif
