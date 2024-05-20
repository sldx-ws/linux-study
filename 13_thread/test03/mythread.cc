#include <iostream>
#include <cassert>
#include <string>
#include <vector>
#include <unistd.h>
#include <pthread.h>

using namespace std;

// 当成结构体使用
class ThreadData
{
public:
    int number;
    pthread_t tid;
    char namebuffer[64];
};

class ThreadReturn
{
public:
    int exit_code;
    int exit_result;
};

// 新线程
void *start_routine(void *args)
{
    ThreadData *td = static_cast<ThreadData *>(args);  // 安全的进行强转
    int cnt = 10;
    while (cnt)
    {
        cout << "new thread create success, name: " << td->namebuffer << " cnt: " << cnt-- << endl;
        sleep(1);
    }
    // return nullptr;
    // pthread_exit(nullptr);

    // void *ret = (void *)td->number
    // return (void *)td->number; // warning: int 4B, void* 8B
    pthread_exit((void *)520);

    ThreadReturn *tr = new ThreadReturn();
    tr->exit_code = 5;
    tr->exit_result = 20;

    // return (void *)tr;  // 右值
}

int main() 
{
    // 1. 创建一批线程
    vector<ThreadData *> threads;
#define NUM 10
    for (int i = 0; i < NUM; ++i)
    {
        ThreadData *td = new ThreadData();
        td->number = i + 1;
        snprintf(td->namebuffer, sizeof(td->namebuffer), "%s:%d", "thread", i + 1);
        pthread_create(&td->tid, nullptr, start_routine, td);
        threads.push_back(td);
    }

    for (auto &iter : threads)
    {
        cout << "create thread: " << iter->namebuffer << " : " << iter->tid << " success" << endl;
    }

    // 线程可以被 cancel ，注意：线程要被取消，前提是这个线程已经跑起来了
    // 线程如果是被取消的，退出码：-1 (PTHREAD_CANCELED)
    sleep(5);
    for (int i = 0; i < threads.size() / 2; ++i)
    {
        pthread_cancel(threads[i]->tid);
        cout << "pthread_cancel : " << threads[i]->namebuffer << "success" << endl;
    }

    for (auto &iter : threads)
    {
        void *ret = nullptr; // 注意：void*
        int n = pthread_join(iter->tid, &ret); // void **, *retp = return (void*)td->number
        assert(0 == n);
        cout << "join : " << iter->namebuffer << "success, exit_code: " << (long long)ret << endl; // void* 8B -> long long 8B

        // ThreadReturn *ret = nullptr;
        // // pthread_join: 默认认为函数会调用成功！不考虑异常问题，异常问题是进程该考虑的问题
        // int n = pthread_join(iter->tid, (void **)&ret); // void **, *retp = return (void*)td->number
        // assert(0 == n);
        // cout << "join : " << iter->namebuffer << "success, exit_code: " << ret->exit_code << ", exit_result:  " << ret->exit_result << endl; // void* 8B -> long long 8B

        delete iter;
    }

    cout << "main thread quit" << endl;

    return 0;
}