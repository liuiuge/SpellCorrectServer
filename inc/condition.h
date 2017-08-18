#pragma once
#ifndef __CONDITION_H__
#define __CONDITION_H__

#include "mutexlock.h"
#include <iostream>
namespace wd{
class Condition
{
public:
    Condition(Mutex_lock & mutex1):
        _mutex(mutex1)
    {
        pthread_cond_init(&_cond, NULL);
    }
    void wait()
    {
        pthread_cond_wait(&_cond,_mutex.get_mutex_ptr());
    }
    void notify(){
        pthread_cond_signal(&_cond);
    }
    void notifyall(){
        pthread_cond_broadcast(&_cond);
    }
    ~Condition(){
        pthread_cond_destroy(&_cond);
    }
private:
    pthread_cond_t _cond;
    Mutex_lock & _mutex;
};
}
#endif
