#include "ThreadPool.hpp"
#include <memory>
using namespace thread_pool;

#if 0   // test Thread.hpp
void* run(void* args)
{
    while (1)
    {
        std::cout << pthread_self() << std::endl;
        sleep(1);
    }

    pthread_exit(nullptr);
}

int main()
{
    Thread t1(run, nullptr);    
    Thread t2(run, nullptr);    

    t1.start();
    t2.start();

    t1.join();
    t2.join();

    return 0;
}
#endif

#if 0  // test ThreadPool.hpp
int main()
{
    std::unique_ptr<ThreadPool<int>> tp(new ThreadPool<int>());
    tp->run(); 

    while (1)
    {
        sleep(1);
    }

    return 0;
}
#endif

int main()
{

    return 0;
}