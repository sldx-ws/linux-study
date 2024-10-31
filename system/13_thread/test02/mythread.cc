#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>
using namespace std;

// 当成结构体使用
class ThreadData
{
public:
    pthread_t tid;
    char nameBuffer[64];
};

void *start_routine(void *args)
{
    sleep(1);
    ThreadData *td = static_cast<ThreadData *>(args);  
    int cnt = 10;
    while (cnt)
    {
        cout << "new thread create success, name: " << td->nameBuffer << " cnt: " << cnt-- << endl;
        sleep(1);
    }
    delete td;
    return nullptr;
}

int main() 
{
    vector<ThreadData *> threads;
#define NUM 10
    for (int i = 0; i < NUM; ++i)
    {
        ThreadData *td = new ThreadData();
        snprintf(td->nameBuffer, sizeof(td->nameBuffer), "%s:%d", "thread", i + 1);
        pthread_create(&td->tid, nullptr, start_routine, td);
        threads.push_back(td);
    }

    for (auto &iter : threads)
    {
        cout << "create thread: " << iter->nameBuffer << " : " << iter->tid << " success" << endl;
    }

    // 主线程
    while (true)
    {
        cout << "new thread create success, name: main thread" << endl;
        sleep(1);
    }

    return 0;
}