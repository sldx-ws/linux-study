#include <iostream>
#include <unistd.h>
#include <signal.h>

void handler(int signo)
{
    std::cout << "我捕捉到了一个信号，该信号编号是：" << signo << std::endl;
}

int main()
{
    // signal(2, handler);
    // signal(SIGALRM, handler);
    // alarm(3);

    // while (1)
    // {
    //     std::cout << "running..." << std::endl;
    //     sleep(1);
    // }      

    signal(SIGFPE, handler);
    
    int a = 10; 
    a /= 0;   // 收到信号 8) SIGFPE
    while (1)
    {
        std::cout << "running..." << std::endl;
        sleep(1);
    }

    return 0;
}
