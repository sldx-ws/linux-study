#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <cerrno>
#include <cstring>
#include <strings.h>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace Server
{
    using namespace std;

    static const string defaultIp = "0.0.0.0"; // TODO
    static const int gnum  = 1024;

    enum
    {
        USAGE_ERR = 1,
        SOCKET_ERR,
        BIND_ERR,
        OPEN_ERR
    };

    typedef function<void (int, string, uint16_t, string)> func_t;

    class udpServer
    {
    public:
        udpServer(const func_t &cb, const uint16_t &port, string ip = defaultIp)
            : _callback(cb), _port(port), _ip(ip), _sockfd(-1)
        {
        }

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
            char buffer[gnum];
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
                    string clientip = inet_ntoa(peer.sin_addr);  // (1. 网络序列 2. int -> 点分十进制IP) -> inet_ntoa
                    uint16_t clientport = ntohs(peer.sin_port);
                    string message = buffer;

                    cout << clientip << "[" << clientport << "]# " << message << endl;
                    // 只把数据读上来就完了吗？对数据做处理
                    _callback(_sockfd, clientip, clientport, message);
                }
            }
        }

        ~udpServer()
        {
        }

    private:
        uint16_t _port;
        string _ip; // 实际上，一款网络服务器，不建议指明一个IP
        int _sockfd;
        func_t _callback;  // 回调
    };

} // end of namespace Server
