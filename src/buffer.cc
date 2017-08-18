
#include "buffer.h"
namespace wd{
buffer::buffer(size_t size):
    _mutex(),
    _not_empty(_mutex),
    _not_full(_mutex),
    _size(size),
    _flag(1)
{}
bool buffer::empty()
{
    return _que.empty();
}
bool buffer::full()
{
    return _que.size() == _size;
}
void buffer::push(const Elem_type & value)
{
    MutexLockGuard mlg(_mutex);
    while(full())
        _not_full.wait();
    _que.push(value);
    _not_empty.notify();
}
buffer::Elem_type buffer::pop()
{
    MutexLockGuard mlg(_mutex);
    while(empty() && _flag)
        _not_empty.wait();
    if(_flag){
        Elem_type value = _que.front();
        _que.pop();
        _not_full.notify();
        return value;
    }
    return NULL;
}
void buffer::wakeup_empty()
{
    _flag = 0;
    _not_empty.notifyall();
}
#ifdef HOLA
class buffer{
public:
    typedef function<void()> Elem_type;
    buffer(int );
    bool empty();
    bool full();
    void push(Elem_type );
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
#endif
}//end of namespace
