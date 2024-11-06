#pragma once
#include "log.hpp"
#include <iostream>
#include <string>
#include <cstring>      // memset
#include <sys/types.h>  // socket bind listen accept
#include <sys/socket.h> // socket bind listen accept
#include <arpa/inet.h>  // struct sockaddr_in
#include <unistd.h>     // read

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
            logMessage(NORMAL, "accept a new link success");  // ?
            cout << "sock: " << sock << endl;

            // version1: 婴儿版
#ifdef TOY
            serviceIO(sock);
            close(sock);
#endif

        }
    }

#ifdef TOY
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
#endif

    ~tcpServer() = default;

private:
    int _listenSock;
    string _ip;
    uint16_t _port;
};

};  // end of namespace Server