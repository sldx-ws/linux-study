#pragma once
#include "log.hpp"
#include <iostream>
#include <string>
#include <cstring>      // memset
#include <cstdlib>      // exit
#include <sys/wait.h>   // wait
#include <sys/types.h>  // socket bind listen accept wait
#include <sys/socket.h> // socket bind listen accept
#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // read
#include <signal.h>
#include <pthread.h>

#ifdef THREAD_POOL
#include "log.hpp"
#include "Task.hpp"
#include "ThreadPool.hpp"
using namespace thread_pool;
#endif

namespace Server
{

using namespace std;

enum
{
    USAGE_ERR = 1,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR
};

static const string defaultIp = "0.0.0.0";
static const int G_BACKLOG = 5;

class tcpServer
{
public:
    tcpServer(uint16_t port, string ip = defaultIp)
        : _listenSock(-1)
        , _port(port)
        , _ip(ip)
    {}

    void initServer()
    {
#ifdef THREAD_POOL
        ThreadPool<Task>::getInstance()->run();
        logMessage(NORMAL, "Thread init success");
#endif
        _listenSock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenSock < 0)
        {
            logMessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success: %d", _listenSock);

        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_listenSock, (struct sockaddr*)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "bind socket error");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "bind socket success");

        if (listen(_listenSock, G_BACKLOG) < 0)
        {
            logMessage(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }

    void start()
    {
        char buffer[1024];
        for (;;)
        {
            struct sockaddr_in peer;
            memset(&peer, 0, sizeof(peer));
            socklen_t len = sizeof peer;
            int sock = accept(_listenSock, (struct sockaddr*)&peer, &len);
            if (sock < 0)
            {
                logMessage(ERROR, "accept error, next");
                continue;
            }
            logMessage(NORMAL, "accept a new link success, get new sock: %d", sock);
            cout << "sock: " << sock << endl;

            // version4: 线程池版
#ifdef THREAD_POOL
            ThreadPool<Task>::getInstance()->push(Task(sock, serviceIO));
#endif
        }
    }

private:
    int _listenSock;
    string _ip;
    uint16_t _port;
};

};  // end of namespace Server