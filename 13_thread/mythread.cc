#include <iostream>
#include <string>
#include <unistd.h>
#include <pthread.h>

using namespace std;

void *start_routine(void *args)
{
    string name = static_cast<const char *>(args);
    while (1)
    {
        cout << "new thread create success, name: " << name << endl;
        sleep(1);
    }
}

int main() 
{
    pthread_t id;
    pthread_create(&id, nullptr, start_routine, (void *)"thread one");

    while (1)
    {
        cout << "new thread create success, name: main thread" << endl;
        sleep(1);
    }

    return 0;
}