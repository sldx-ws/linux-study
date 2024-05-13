#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void Count(int cnt)
{
    while (cnt--)
    {
        printf("cnt: %2d\r", cnt);
        fflush(stdout);
        sleep(1);
    }

    printf("\n");
}

void handler(int signo)
{
    // printf("pid: %d, %d 号信号，正在被捕捉\n", getpid(), signo);

    // 1、有非常多的子进程，同一时刻退出
    // waitpid(-1) -> while(1) waitpid(-1)

    // 2、有非常多的子进程，同一时刻 部分 退出
    while (1)
    {
        // pid_t ret = waitpid(-1, NULL, 0); // 假设退出5个，第6个没退或者全退了，后续会阻塞式等待

        pid_t ret = waitpid(-1, NULL, WNOHANG);
        if (ret == 0) break;
    }

}

int main()
{
    // signal(SIGCHLD, handler); // 子进程退出时给父进程发送SIGCHILD信号

    signal(SIGCHLD, SIG_IGN);
    // signal(SIGCHLD, SIG_DFL);

    printf("我是父进程，pid: %d, ppid: %d\n", getpid(), getppid());

    pid_t id = fork();
    if (id == 0)
    {
        printf("我是子进程，pid: %d, ppid: %d, 我要退出了\n", getpid(), getppid());
        Count(5);
        exit(1);
    }

    while (1)
    {
        sleep(1);
    }

    return 0;
}