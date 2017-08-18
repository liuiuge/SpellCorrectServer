#pragma once
#ifndef __MUTEXLOCK_H__
#define __MUTEXLOCK_H__

#include <pthread.h>
namespace wd{
class Mutex_lock
{
public:
    Mutex_lock():
        _islocked(false)
    {
        pthread_mutex_init(&_mutex, NULL);
    }
    ~Mutex_lock()
    {
        pthread_mutex_destroy(&_mutex);
    }
    void lock()
    {
        _islocked = true;
        pthread_mutex_lock(&_mutex);
    }
    void unlock()
    {
        pthread_mutex_unlock(&_mutex);
        _islocked = false;
    }
    pthread_mutex_t * get_mutex_ptr()
    {
        return & _mutex;
    }
private:
    pthread_mutex_t _mutex;
    bool _islocked;
};
class MutexLockGuard
{
public:
	MutexLockGuard(Mutex_lock & mutex)
	: _mutex(mutex)
	{
		_mutex.lock();
	}

	~MutexLockGuard()
	{
		_mutex.unlock();
	}
private:
	Mutex_lock & _mutex;
};
}
#endif
