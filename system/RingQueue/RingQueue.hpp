#pragma once
#include <vector>
#include <semaphore.h>
#include <pthread.h>
#include <cassert>

template <class T>
class RingQueue
{
public:
    RingQueue(int cap = 5) 
        : _rq(cap)
        , _cap(cap)
        , _producer_step(0)
        , _consumer_step(0)
    {
        int n = sem_init(&_space_sem, 0, _cap);
        assert(0 == n);
        sem_init(&_data_sem, 0, 0);
        assert(0 == n);

        pthread_mutex_init(&_producer_mutex, nullptr);
        pthread_mutex_init(&_consumer_mutex, nullptr);
    }

    void push(T& in)
    {
        P(&_space_sem);
        pthread_mutex_lock(&_producer_mutex);

        _rq[_producer_step++] = in;
        _producer_step %= _cap;

        pthread_mutex_unlock(&_producer_mutex);
        V(&_data_sem);
    }

    void pop(T* out)
    {
        P(&_data_sem);
        pthread_mutex_lock(&_consumer_mutex);

        *out = _rq[_consumer_step++];
        _consumer_step %= _cap;

        pthread_mutex_unlock(&_consumer_mutex);
        V(&_space_sem);
    }

    ~RingQueue()
    {
        sem_destroy(&_space_sem);
        sem_destroy(&_data_sem);
        pthread_mutex_destroy(&_producer_mutex);
        pthread_mutex_destroy(&_consumer_mutex);
    }
private:
    void P(sem_t* sem)
    {
        int n = sem_wait(sem);
        assert(0 == n);
        (void)n;
    }

    void V(sem_t* sem)
    {
        int n = sem_post(sem);
        assert(0 == n);
        (void)n;
    }
private:
    std::vector<T> _rq;
    int _producer_step;
    int _consumer_step;
    int _cap;
    sem_t _space_sem;
    sem_t _data_sem;
    pthread_mutex_t _producer_mutex;
    pthread_mutex_t _consumer_mutex;
};