#include <memory>
#include <unistd.h>
#include "Thread.hpp"

void *thread_run(void *args)
{
    std::string work_type = static_cast<const char *>(args);
    while (true)
    {
        std::cout << "我是一个新线程，我正在做: " << work_type << std::endl;
        sleep(1);
    }
}

int main()
{
    std::unique_ptr<Thread> thread1(new Thread(thread_run, (void *)"hello thread", 1));
    std::unique_ptr<Thread> thread2(new Thread(thread_run, (void *)"count thread", 2));
    std::unique_ptr<Thread> thread3(new Thread(thread_run, (void *)"log thread", 3));

    thread1->join();
    thread2->join();
    thread3->join();


    return 0;
}
