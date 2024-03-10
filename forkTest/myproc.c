#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main()
{
    pid_t id = fork();

    if (id == 0)
    {
        while (1)
        {
            printf("子进程，PID:%d, PPID:%d, id:%d\n", getpid(), getppid(), id);

            sleep(1);
        }
    }
    else if (id > 0)
    {
        while (1)
        {
            printf("父进程，PID:%d, PPID:%d, id:%d\n", getpid(), getppid(), id);

            sleep(2);
        }
    }
    else 
    {

    }

    return 0;
}
