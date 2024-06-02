#include <iostream>
#include <vector>
#include <signal.h>
#include <unistd.h>

using namespace std;

// #define BLOCK_SIGNAL 2
#define MAX_SIGNUM 31 // 31个普通信号

static vector<int> sigarr = {2};

static void show_pending(const sigset_t &pending)
{
    for (int signo = MAX_SIGNUM; signo >= 1; signo--)
    {
        if (sigismember(&pending, signo))
            cout << "1";
        else
            cout << "0";
    }

    cout << endl;
}

static void handler(int signo)
{
    cout << signo << "号信号已经被递达!!!" << endl;
}

int main()
{
    for (const auto &sig : sigarr)
        signal(sig, handler);

    // 1、先尝试屏蔽指定的信号
    sigset_t block, oblock, pending;
    // 1.1 初始化
    sigemptyset(&block);
    sigemptyset(&oblock);
    // 1.2 添加要屏蔽的信号
    // sigaddset(&block, BLOCK_SIGNAL);
    for (const auto &sig : sigarr)
        sigaddset(&block, sig);
    // 1.3 开始屏蔽（设置进内核（进程））
    sigprocmask(SIG_SETMASK, &block, &oblock);

    // 2、遍历打印pending信号集
    int cnt = 10;
    while (1)
    {
        // 2.1 初始化
        sigemptyset(&pending);
        // 2.2 获取它
        sigpending(&pending);
        // 2.3 打印pending信号集
        show_pending(pending);
        sleep(1);

        if (cnt-- == 0) 
        {
            sigprocmask(SIG_SETMASK, &oblock, &block); // 一旦对特定信号进行解除屏蔽，一般os至少立马抵达一个信号

            // 执行ctrl^c信号的默认动作，终止进程，不会返回用户态，这行代码不会被执行
            // 可以对2号信号自定义捕捉
            cout << "恢复对信号的屏蔽，不屏蔽任何信号" << endl; 
        }
    }

    return 0;
}