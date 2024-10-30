#pragma once
#include <iostream>
#include <string>
#include <cerrno>       // errno
#include <cstring>      // strerror
#include <cstdio>       // perror
#include <cstdlib>      // exit atoi
#include <strings.h>     // bzero
#include <sys/types.h>  // socket bind inet_addr inet_ntoa recvfrom 
#include <sys/socket.h> // socket bind inet_addr inet_ntoa recvfrom
#include <netinet/in.h> // inet_addr
#include <arpa/inet.h>  // _ to _ _  struct sockaddr_in inet_addr inet_ntoa
 
namespace Server
{

using namespace std;

static const string defaultIp = "0.0.0.0"; // TODO
static const int G_NUM  = 1024;

enum
{
    USAGE_ERR = 1,
    SOCKET_ERR,
    BIND_ERR,
};

class udpServer
{
public:
    udpServer(const uint16_t &port, string ip = defaultIp)
        :  _port(port)
        , _ip(ip)
        , _sockfd(-1)
    {}

    void initServer()
    {
        // 1. 创建套接字
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == _sockfd)
        {
            cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
            exit(SOCKET_ERR);
        }
        cout << "socket success: " << " : " << _sockfd << endl;

        // 2. 绑定prot, ip(TODO)
        // 未来的服务器要明确的port，不能随意改变
        struct sockaddr_in local;
        bzero(&local, sizeof(local));
        local.sin_family = AF_INET;
        local.sin_port = htons(_port);  // 给别人发消息，port和ip需要发给对方
        // local.sin_addr.s_addr = inet_addr(_ip.c_str());  // (1. string -> uint32_t  2. htoil();) -> inet_addr()
        local.sin_addr.s_addr = htonl(INADDR_ANY);  // 任意地址bind，服务器的真实写法

        int n = bind(_sockfd, (struct sockaddr *)&local, sizeof(local));
        if (-1 == n)
        {
            cerr << "bind error: " << errno << " : " << strerror(errno) << endl;
            exit(BIND_ERR);
        }
        // UDP Server 的预备工作完成
    }

    void start()
    {
        // 服务器的本质其实就是一个死循环
        char buffer[G_NUM];
        for (;;)
        {
            struct sockaddr_in peer;
            socklen_t len = sizeof(peer);  // 必填
            ssize_t s= recvfrom(_sockfd, buffer, sizeof(buffer) - 1, 0, (struct sockaddr *)&peer, &len);
            
            // 1. 数据是什么  2. 数据是谁发的
            if (s > 0)
            {
                // 读取数据
                buffer[s] = 0;
                string clientIp = inet_ntoa(peer.sin_addr);  // (1. 网络序列 2. int -> 点分十进制IP) -> inet_ntoa
                uint16_t clientPort = ntohs(peer.sin_port);
                string message = buffer;

                cout << clientIp << "[" << clientPort << "]# " << message << endl;
            }
        }
    }

    ~udpServer() = default;

private:
    int _sockfd;
    uint16_t _port;
    string _ip; // 实际上，一款网络服务器，不建议指明一个IP
};

} // end of namespace Server
