#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <pthread.h>

std::string changeId(const pthread_t &thread_id)
{
    char tid[128];
    snprintf(tid, sizeof(tid), "%#X", thread_id);
    return tid;
}

void *start_routine(void *args)
{
    std::string threadname = static_cast<const char *>(args);
    // pthread_detach(pthread_self());  // 设置自己为分离状态

    int cnt = 5;
    while (cnt--)
    {
        std::cout << threadname << " running ... : " << changeId(pthread_self()) << std::endl;
        sleep(1);
    }

    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, start_routine, (void *)"thread 1");
    pthread_detach(tid);

    std::cout << "main thread running ... new thread id: " << changeId(tid) << std::endl;

    // 一个线程默认是joinable的，如果设置了分离状态，不能够进行等待了
    int n = pthread_join(tid, nullptr);  // 失败返回错误码
    std::cout << "result: " << n << " : " << strerror(n) << std::endl;

    while (true)
    {
        std::cout << "main thread running ... new thread id: " << changeId(tid) << std::endl;
        sleep(1);
    }

    return 0;
}