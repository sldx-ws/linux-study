#include "RingQueue.hpp"
#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
using namespace std;

void* producer_start_routine(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    while (1)
    {
        int random = rand() % 20 + 1;
        rq->push(random);
        cout << "product: " << random << endl;
    }  

    pthread_exit(nullptr);
}

void* consumer_start_routine(void* args)
{
    RingQueue<int>* rq = static_cast<RingQueue<int>*>(args);
    while (1)
    {
        int out;
        rq->pop(&out);
        cout << "consume: " << out << endl;
        sleep(1);
    }

    pthread_exit(nullptr);
}

int main()
{
    srand((unsigned int)time(nullptr) ^ getpid());
    RingQueue<int>* rq = new RingQueue<int>();    
    pthread_t p, c;
    
    pthread_create(&p, nullptr, producer_start_routine, rq);
    pthread_create(&c, nullptr, consumer_start_routine, rq);


    pthread_join(p, nullptr);
    pthread_join(c, nullptr);

    delete rq;
    return 0;
}