#include "BlockQueue.hpp"
#include "Task.hpp"
#include <iostream>
#include <ctime>
#include <unistd.h>
using namespace std;

int Add(int x, int y)
{
    return x + y;
}

void* producer_start_routine(void* args)
{
    BlockQueue<Task>* bq = static_cast<BlockQueue<Task>*>(args);
    while (1)
    {
        int x = rand() % 20 + 1;
        int y = rand() % 20 + 1;
        Task in(x, y, Add);

        bq->push(in);
        cout << x << " + " << y << " = ";

        sleep(1);
    }

    pthread_exit(nullptr);
}

void* consumer_start_routine(void* args)
{
    BlockQueue<Task>* pbq = static_cast<BlockQueue<Task>*>(args);
    Task out;

    while (1)
    {
        pbq->pop(&out);

        cout << out() << endl;
    }

    pthread_exit(nullptr);
}

int main()
{
    srand((unsigned int)time(nullptr));
    BlockQueue<Task>* pbq = new BlockQueue<Task>();
    pthread_t c;
    pthread_t p;
    char buffer[1024];

    pthread_create(&p, nullptr, producer_start_routine, pbq);
    pthread_create(&c, nullptr, consumer_start_routine, pbq);

    pthread_join(c, nullptr);
    pthread_join(p, nullptr);

    delete pbq;

    return 0;
}