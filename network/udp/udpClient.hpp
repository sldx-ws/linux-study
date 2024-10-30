#pragma once
#include <iostream>
#include <string>
#include <cstdio>       // perror
#include <cstdlib>      // exit
#include <cstring>      // memset
#include <sys/types.h>  // socket inet_addr inet_ntoa bind sendto
#include <sys/socket.h> // socket inet_addr inet_ntoa bind sendto
#include <arpa/inet.h>  // _ to _ _  struct sockaddr_in  inet_addr inet_ntoa

namespace Client
{

using namespace std;

enum
{
    USAGE_ERR = 1,
    SOCKET_ERR,
};

class udpClient
{
public:
    udpClient(const string &serverIp, const uint16_t &serverPort)
        : _serverIp(serverIp), _serverPort(serverPort), _sockfd(-1), _quit(false)
    {}

    void initClient()
    {
        // 创建socket
        _sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (-1 == _sockfd)
        {
            cerr << "socket error: " << errno << " : " << strerror(errno) << endl;
            exit(SOCKET_ERR);
        }
        cout << "socket success: " << " : " << _sockfd << endl;

        // client要不要bind(必须要)，client不需要显示的bind！！！
        // 由os自动形成端口进行bind
    }

    void run()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_addr.s_addr = inet_addr(_serverIp.c_str());
        server.sin_port = htons(_serverPort);

        string message;
        while (!_quit)
        {
            cout << "Please Enter# ";
            cin >> message;

            sendto(_sockfd, message.c_str(), message.size(), 0, (struct sockaddr *)&server, sizeof(server));
        }
    }

    ~udpClient() = default;

private:
    int _sockfd;
    string _serverIp;
    uint16_t _serverPort;
    bool _quit;
};

} // end of namespace Client