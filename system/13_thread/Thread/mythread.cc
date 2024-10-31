#include <memory>
#include <unistd.h>
#include "Thread.hpp"
#include "Mutex.hpp"

#if 0
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
#endif

int tickets = 10000;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
Mutex mutex(&lock);
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

#ifdef T1
void *get_ticket(void *args)
{
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        LockGuard lockGuard(&lock);
        // mutex.lock();
        if (tickets > 0)
        {
            usleep(1000);
            std::cout << name << ": " << tickets << std::endl;
            tickets--;
            // mutex.unlock();
        }
        else
        {
            // mutex.unlock();
            break;
        }
    }

    usleep(1000);
    pthread_exit(nullptr);
}
#else
void *start_routine(void *args)
{
    std::string name = static_cast<const char *>(args);
    while (true)
    {
        pthread_mutex_lock(&lock);
        pthread_cond_wait(&cond, &lock);
        tickets--;
        std::cout << name << ": " << tickets << std::endl;
        pthread_mutex_unlock(&lock);
    }
}
#endif  // ifdef T1

int main()
{
#ifdef T1
    Thread t1(get_ticket, (void*)"thread1");
    Thread t2(get_ticket, (void*)"thread2");
    Thread t3(get_ticket, (void*)"thread3");
    Thread t4(get_ticket, (void*)"thread4");
#else
    Thread t1(start_routine, (void*)"thread1");
    Thread t2(start_routine, (void*)"thread2");
    Thread t3(start_routine, (void*)"thread3");
    Thread t4(start_routine, (void*)"thread4");

    while (true)
    {
        sleep(1);
        // pthread_cond_signal(&cond);
        pthread_cond_broadcast(&cond);
        std::cout << "main thread wakeup one thread..." << std::endl;
    }
#endif  // ifdef T1

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}