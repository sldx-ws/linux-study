#pragma once
#include <pthread.h>

class Mutex
{
public:
    Mutex(pthread_mutex_t* lock_p)
        : _lock_p(lock_p)
    {}

    void lock()
    {
        if (_lock_p != nullptr)
            pthread_mutex_lock(_lock_p);
    }

    void unlock()
    {
        if (_lock_p != nullptr)
            pthread_mutex_unlock(_lock_p);
    }
private:
    pthread_mutex_t* _lock_p;
};



class LockGuard
{
public:
    LockGuard(pthread_mutex_t* mutex)
        : _mutex(mutex)
    {
        _mutex.lock();
    }

    ~LockGuard()
    {
        _mutex.unlock();
    }
private:
    Mutex _mutex;
};
