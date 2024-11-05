#pragma once
#include <cstdio>
#include <string>
#include <cassert>
#include <functional>
#include <pthread.h>

namespace thread_pool
{

class Thread
{
    using func_t = std::function<void*(void*)>;
public:
    const int num = 1024;

public:
    Thread()
    {
        char nameBuffer[num];
        snprintf(nameBuffer, sizeof nameBuffer, "thread-%d", threadNum++);
        _name = nameBuffer;
    }

    void start(func_t func, void *args = nullptr)
    {
        _func = func;
        _args = args;
        
        int n = pthread_create(&_tid, nullptr, start_routine, this);
        assert(0 == n);
        (void)n;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(0 == n);
        (void)n;
    }

    std::string threadName()
    {
        return _name;
    }

private:
    static void *start_routine(void *args)
    {
        Thread* _this = static_cast<Thread*>(args);
        return _this->callback();
    }

    void *callback() { return _func(_args); }

private:
    std::string _name;
    func_t _func;
    void* _args;
    pthread_t _tid;

    static int threadNum;
};

int Thread::threadNum = 1;

}  // end of namespace thread_pool