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
        _listenSock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenSock < 0)
        {
            logMessage(FATAL, "create socket error");
            exit(SOCKET_ERR);
        }
        logMessage(NORMAL, "create socket success");  // ?

        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_listenSock, (struct sockaddr*)&local, sizeof local) < 0)
        {
            logMessage(FATAL, "create socket error");
            exit(BIND_ERR);
        }
        logMessage(NORMAL, "create socket success");

        if (listen(_listenSock, G_BACKLOG) < 0)
        {
            logMessage(FATAL, "listen socket error");
            exit(LISTEN_ERR);
        }
        logMessage(NORMAL, "listen socket success");
    }

    void start()
    {

#ifdef PROCESS2
        signal(SIGCHLD, SIG_IGN);
#endif

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
            logMessage(ERROR, "accept a new link success");  // ?
            cout << "sock: " << sock << endl;

            // version2: 多进程版
#ifdef PROCESS1
            
            pid_t id = fork();
            if (0 == id)
            {
                close(_listenSock);  // 子进程关闭不影响父进程

                if (fork() > 0)
                    exit(0);
                
                serviceIO(sock);
                close(sock);

                exit(0);
            }

            close(sock);  // 父进程必须关
            pid_t ret = waitpid(id, nullptr, 0);
            if (ret > 0)
            {
                cout << "wait success: " << ret << endl;
            }
#elif PROCESS2  
            pid_t id = fork();
            if (0 == id)
            {
                close(_listenSock);

                serviceIO(sock);
                close(sock);

                exit(0);
            }

            close(sock);  // 父进程必须关
#endif  // #ifdef PROCESS1
        }
    }

    void serviceIO(int sock)
    {
        char buffer[1024];
        while (true)
        {
            ssize_t n = read(sock, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = 0;
                cout << "recv message: " << buffer << endl;

                string outBuffer = buffer;
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
    }

private:
    int _listenSock;
    string _ip;
    uint16_t _port;
};

};  // end of namespace Server