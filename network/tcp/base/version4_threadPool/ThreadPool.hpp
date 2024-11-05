#pragma once
#include "Thread.hpp"
#include "LockGuard.hpp"
#include <iostream>
#include <vector>
#include <queue>
#include <unistd.h>
#include <mutex>

namespace thread_pool
{

const int G_NUM = 10;

template <class TaskType>
class ThreadPool;

template <class TaskType>
class ThreadData
{
public:
    ThreadData(ThreadPool<TaskType>* tp, const std::string& name)
        : _tp(tp)
        , _name(name)
    {}

public:
    ThreadPool<TaskType>* _tp;
    std::string _name;
};

template <class TaskType>
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
            _threads.push_back(new Thread());
        }
    }

    void run()
    {
        for (auto& t : _threads)
        {
            ThreadData<TaskType>* td = new ThreadData<TaskType>(this, t->threadName());

            t->start(handler_task, td);
            std::cout << t->threadName() << " run..." << std::endl;
        }
    }

    void push(TaskType t)
    {
        LockGuard lg(&_mutex);
        _task_queue.push(t);
        pthread_cond_signal(&_cond);
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

    static ThreadPool<TaskType>* getInstance()
    {
        if (nullptr == tp)
        {
            _singletonLock.lock();
            if (nullptr == tp)
            {
                tp = new ThreadPool<TaskType>();
            }
            _singletonLock.unlock();
        }

        return tp;
    }

private:
    static void* handler_task(void* args)
    {
        ThreadData<TaskType>* td = static_cast<ThreadData<TaskType>*>(args);
        while (true)
        {
            TaskType task;
            {
                LockGuard lg(td->_tp->mutex());
                while (td->_tp->queueIsEmpty())
                {
                    td->_tp->threadWait();
                }

                task = td->_tp->queuePop(); // 将task从taskQueue中拿到当前thread独立的栈中
            }

            task();
        }

        delete td;
        pthread_exit(nullptr);
    }

public:
    bool queueIsEmpty() { return _task_queue.empty(); }
    void threadWait() { pthread_cond_wait(&_cond, &_mutex); }

    TaskType queuePop()
    {
        TaskType task = _task_queue.front();
        _task_queue.pop();
        return task;
    }

    pthread_mutex_t* mutex()
    {
        return &_mutex;
    }

private:
    int _num;
    std::vector<Thread*> _threads;
    std::queue<TaskType> _task_queue;
    pthread_mutex_t _mutex;
    pthread_cond_t _cond;

    static ThreadPool<TaskType>* tp;  // 懒汉模式
    static std::mutex _singletonLock;
};

template <class TaskType>
ThreadPool<TaskType>* ThreadPool<TaskType>::tp = nullptr;

template <class TaskType>
std::mutex ThreadPool<TaskType>::_singletonLock;

}  // end of namespace thread_pool