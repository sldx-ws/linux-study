#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <cstring>      // memset
#include <cstdlib>      // exit
#include <unistd.h>     // close
#include <sys/wait.h>   // wait
#include <sys/types.h>  // socket bind listen accept send wait
#include <sys/socket.h> // socket bind listen accept send
#include <arpa/inet.h>  // struct sockaddr_in
#include "protocol.hpp"

namespace Server
{

enum
{
    USAGE_ERR = 1,
    SOCKET_ERR,
    BIND_ERR,
    LISTEN_ERR
};

static const int G_BACKLOG = 5;

using func_t = std::function<bool (const HttpRequest &, HttpResponse &)>;

class HttpServer
{
public:
    HttpServer(func_t func, uint16_t port)
        : _listenSock(-1)
        , _port(port)
        , _func(func)
    {}

    void initServer()
    {
        _listenSock = socket(AF_INET, SOCK_STREAM, 0);
        if (_listenSock < 0)
        {
            exit(SOCKET_ERR);
        }

        struct sockaddr_in local;
        memset(&local, 0, sizeof local);
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);
        local.sin_addr.s_addr = INADDR_ANY;
        if (bind(_listenSock, (struct sockaddr*)&local, sizeof local) < 0)
        {
            exit(BIND_ERR);
        }

        if (listen(_listenSock, G_BACKLOG) < 0)
        {
            exit(LISTEN_ERR);
        }
    }

    void start()
    {
        for (;;)
        {
            struct sockaddr_in peer;
            memset(&peer, 0, sizeof(peer));
            socklen_t len = sizeof peer;
            int sock = accept(_listenSock, (struct sockaddr*)&peer, &len);
            if (sock < 0)
            {
                continue;
            }

            pid_t id = fork();
            if (0 == id)
            {
                close(_listenSock);
                if (fork() > 0) exit(0);
                
                HandlerHttp(sock);

                close(sock);
                exit(0);
            }

            close(sock);  // 父进程必须关
            waitpid(id, nullptr, 0);
        }
    }

    void HandlerHttp(int sock)
    {
        // 1. 读到完整的http请求

        // 2. 反序列化

        // 3. httpRequest, httpResponse => _func(httpRequest, httpResponse)

        // 4. 序列化

        // 5.send


        char buffer[4096];
        HttpRequest req;
        HttpResponse resp;
        size_t n = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (n > 0)
        {
            buffer[n] = 0;
            req._inBuffer = buffer;
            req.parse();
            _func(req, resp);
            send(sock, resp._outBuffer.c_str(), resp._outBuffer.size(), 0);
        }
    }

private:
    int _listenSock;
    uint16_t _port;
    func_t _func;
};

};  // end of namespace Server