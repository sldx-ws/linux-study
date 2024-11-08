#pragma once
#include <iostream>
#include <string>
#include <functional>
#include <cstring>      // memset
#include <cstdlib>      // exit
#include <sys/wait.h>   // wait
#include <sys/types.h>  // socket bind listen accept send wait
#include <sys/socket.h> // socket bind listen accept send
#include <arpa/inet.h>  // struct sockaddr_in
#include "log.hpp"
#include "protocol.hpp"

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

// const Request& req: 输入型
// Response& resp: 输出型
using func_t = std::function<bool (const Request& req, Response& resp)>;

// 解耦
void handlerEnter(int sock, func_t func)
{
    static std::string inBuffer;
    while (true)
    {
        // 1.读取: "content_len"\r\n"x op y"\r\n
            // 保证读到的是 “一个” 完整的请求
        std::string req_text, req_str;
        if (!recvPackage(sock, inBuffer, &req_text)) return;
        std::cout << "带报头的请求: \n" << req_text << std::endl;
        if (!deLength(req_text, &req_str)) return;
        std::cout << "去掉报头的正文: \n" << req_text << std::endl;

        // 2.对请求Request，反序列化
            // 得到一个结构化的请求对象
        Request req;
        if (!req.deserialize(req_str)) return;

        // 3.计算处理 req.x req.op req.y  --- 业务逻辑
            // 得到一个结构化的响应
        Response resp;
        func(req, resp);  // req的处理结果，放入resp

        // 4.对响应Response，序列化
            // 得到一个“字符串”
        std::string resp_str;
        resp.serialize(&resp_str);

        std::cout << "计算完成，序列化响应: " << resp_str << std::endl; 

        // 5.最后发送响应
            // 构建成为一个完整的报文
        std::string send_string = enLength(resp_str);
        std::cout << "构建完成完整的响应\n" << send_string << std::endl; 

        send(sock, send_string.c_str(), send_string.size(), 0);
    }
}

class CalServer
{
public:
    CalServer(uint16_t port, string ip = defaultIp)
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

    void start(func_t func)
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

            pid_t id = fork();
            if (0 == id)
            {
                close(_listenSock);  // 子进程关闭不影响父进程
                
                handlerEnter(sock, func);
                close(sock);

                exit(0);
            }

            close(sock);  // 父进程必须关
            pid_t ret = waitpid(id, nullptr, 0);
            if (ret > 0)
            {
                logMessage(NORMAL, "wait child success");
            }
        }
    }

private:
    int _listenSock;
    string _ip;
    uint16_t _port;
};

};  // end of namespace Server