#include "ThreadPool.hpp"
#include "Task.hpp"
#include <memory>
using namespace thread_pool;

int main()
{
    // std::unique_ptr<ThreadPool<Task>> tp(new ThreadPool<Task>()); 
    // tp->run();

    ThreadPool<Task>::getInstance()->run();

    int x, y;
    char op;
    while (1)
    {
        std::cout << "Enter data x: ";
        std::cin >> x;
        std::cout << "Enter data y: ";
        std::cin >> y;
        std::cout << "Enter operator: ";
        std::cin >> op;

        Task t(x, y, op, myMath);
        ThreadPool<Task>::getInstance()->push(t);
        usleep(1000);
    }

    return 0;
}