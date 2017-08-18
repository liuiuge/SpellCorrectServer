#pragma once
#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "condition.h"
#include <functional>
using std::function;
#include <queue>
using std::queue;
namespace wd{
class buffer{
public:
    typedef function<void()> Elem_type;
    buffer(size_t);
    bool empty();
    bool full();
    void push(const Elem_type& );
    Elem_type pop();
    void wakeup_empty();
private:
    Mutex_lock _mutex;
    Condition _not_empty;
    Condition _not_full;
    size_t _size;
    queue<Elem_type> _que;
    bool _flag;
};//end of buffer
}//end of namespace
#endif
