#include <iostream>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <pthread.h>

// 添加__thread，可以将一个内置类型设置为线程局部存储
__thread int g_val = 100;

std::string changeId(const pthread_t &thread_id)
{
    char tid[128];
    snprintf(tid, sizeof(tid), "%#X", thread_id);
    return tid;
}

void *start_routine(void *args)
{
    std::string threadname = static_cast<const char *>(args);

    while (true)
    {
        std::cout << threadname << " running ... : " << changeId(pthread_self()) << \
            " g_val: " << g_val << " &g_val: " << &g_val <<  std::endl;
        sleep(1);
        g_val++;
    }

    return nullptr;
}

int main()
{
    pthread_t tid;
    pthread_create(&tid, nullptr, start_routine, (void *)"thread 1");
    std::string main_id = changeId(pthread_self());
    pthread_detach(tid);

    while (true)
    {
        std::cout << "main thread running ... new thread id: " << \
            changeId(tid) << " main thread id: " << main_id << " g_val: " << g_val << " &g_val: " << &g_val << std::endl;
        sleep(1);
    }

    return 0;
}