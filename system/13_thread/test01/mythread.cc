#include <iostream>
#include <assert.h>
#include <string>
#include <unistd.h>
#include <pthread.h>
using namespace std;

void *start_routine(void *args)
{
    string name = static_cast<const char *>(args)
    while (true)
    {
        cout << "new thread create success, name: " << name << endl;
        sleep(1);
    }
}

int main() 
{
    pthread_t id;
    int n = pthread_create(&id, nullptr, start_routine, (void *)"thread new");
    assert(0 == n);
    (void)n;
    
    // 主线程
    while (true)
    {
        cout << "new thread create success, name: main thread" << endl;
        sleep(1);
    }

    return 0;
}