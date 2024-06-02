#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <signal.h>

using namespace std;

void Count(int cnt)
{
    while (cnt)
    {
        printf("cnt: %2d\r", cnt--);
        fflush(stdout);
        sleep(1);
    }
    cout << endl;
}

void handler(int signo)
{
    cout << "get a signo: " << signo << "正在处理中……" << endl;
    Count(10);
}

int main() 
{
    struct sigaction act, oldact;
    act.sa_handler = handler;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    sigaddset(&act.sa_mask, 3);
    
    sigaction(SIGINT, &act, &oldact);
    
    while (true)
    {
        sleep(1);
    }
    
    return 0;
}
