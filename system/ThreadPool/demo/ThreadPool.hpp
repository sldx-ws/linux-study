#pragma once
#include "Thread.hpp"
#include "LockGuard.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>

namespace thread_pool
{

const int G_NUM = 5;

template <class T>
class ThreadPool
{
public:
    ThreadPool(const int& num = G_NUM)
        : _num(G_NUM)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_cond, nullptr);

        for (int i = 0; i < G_NUM; ++i)
        {
            _threads.push_back(new Thread(handler_task, this));
        }
    }

    void run()
    {
        for (auto& t : _threads)
        {
            t->start();
            std::cout << t->threadName() << " run..." << std::endl;
        }
    }

    void push(T& t)
    {
        pthread_mutex_lock(&_mutex);
        _task_queue.push(t);
        pthread_cond_signal(&_cond);
        pthread_mutex_unlock(&_mutex);
    }

    ~ThreadPool()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_cond);

        for (auto& e : _threads)
        {
            delete e;
        }
    }
private:
    static void* handler_task(void* args)
    {
        ThreadPool<T>* _this = static_cast<ThreadPool<T>*>(args);
        while (true)
        {
            _this->queueLock();
            while (_this->queueIsEmpty())
            {
                _this->threadWait();
            }

            T task = _this->queuePop(); // 将task从taskQueue中拿到当前thread独立的栈中
            _this->queueUnlock();
            task(); // 先解锁，再执行task
        }

        pthread_exit(nullptr);
    }

public:
    void queueLock() { pthread_mutex_lock(&_mutex); }
    void queueUnlock() { pthread_mutex_unlock(&_mutex); }
    bool queueIsEmpty() { return _task_queue.empty(); }
    void threadWait() { pthread_cond_wait(&_cond, &_mutex); }
    T queuePop()
    {
        T task = _task_queue.front();
        _task_queue.pop();
        return task;
    }
private:
    int _num;
    std::vector<Thread*> _threads;
    std::queue<T> _task_queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;
};

}  // end of namespace thread_pool