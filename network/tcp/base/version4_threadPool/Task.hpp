#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <functional>

#ifdef THREAD_POOL
void serviceIO(int sock)
{
    char buffer[1024];
    while (true)
    {
        ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
        if (n > 0)
        {
            buffer[n] = 0;
            std::cout << "recv message: " << buffer << std::endl;

            std::string outBuffer = buffer;
            outBuffer += " server[echo]";

            write(sock, outBuffer.c_str(), outBuffer.size());  // 多路转接
        }
        else if ( 0 == n )
        {
            // 代表client退出
            logMessage(NORMAL, "client quit, me too!");
            break;
        }
    }

    close(sock);
}
#endif

class Task
{
    using func_t = std::function<void(int)>;
public:
    Task() = default;

    Task(int sock, func_t func)
        : _sock(sock)
        , _callback(func)
    {}

    void operator()()
    {
        _callback(_sock);
    }

private:
    int _sock;
    func_t _callback;
};
