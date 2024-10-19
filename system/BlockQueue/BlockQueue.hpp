#pragma once 
#include <queue>
#include <pthread.h>

template <class T>
class BlockQueue
{
public:
    BlockQueue(const int& max = 5)
        : _maxCap(max)
    {
        pthread_mutex_init(&_mutex, nullptr);
        pthread_cond_init(&_producer_cond, nullptr);
        pthread_cond_init(&_consumer_cond, nullptr);
    }

    ~BlockQueue()
    {
        pthread_mutex_destroy(&_mutex);
        pthread_cond_destroy(&_consumer_cond);
        pthread_cond_destroy(&_producer_cond);
    }
    
    void push(T& in)
    {
        pthread_mutex_lock(&_mutex);
        while (isFull())
        {
            pthread_cond_wait(&_producer_cond, &_mutex);
        }

        pthread_cond_signal(&_consumer_cond);
        _q.push(in);

        pthread_cond_signal(&_consumer_cond);
        pthread_mutex_unlock(&_mutex);
    }

    void pop(T* out)
    {
        pthread_mutex_lock(&_mutex);
        while (isEmpty())
        {
            pthread_cond_wait(&_consumer_cond, &_mutex);
        }

        *out = _q.front();
        _q.pop();
        pthread_cond_signal(&_producer_cond);
        pthread_mutex_unlock(&_mutex);
    }
private:
    bool isEmpty() { return _q.empty(); }
    bool isFull() { return _maxCap == _q.size(); }
private:
    std::queue<T> _q;
    int _maxCap;
    pthread_cond_t _producer_cond;
    pthread_cond_t _consumer_cond;
    pthread_mutex_t _mutex;
};