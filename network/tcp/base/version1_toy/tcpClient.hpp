#pragma once
#include <iostream>
#include <string>       // getline
#include <cstring>      // memset
#include <sys/types.h>  // socket connect inet_addr
#include <sys/socket.h> // socket connect
#include <arpa/inet.h>  // struct sockaddr_in inet_addr
#include <netinet/in.h> // inet_addr
#include <unistd.h>     // write

#define NUM 1024

namespace Client
{

using namespace std;

enum
{   
    USAGE_ERR = 1,
    SOCKET_ERR
};

class tcpClient
{
public:
    tcpClient(const string& serverIp, const uint16_t& serverPort)
        : _sock(-1)
        , _serverIp(serverIp)
        , _serverPort(serverPort)
    {}

    void initClient()
    {
        _sock = socket(AF_INET, SOCK_STREAM, 0);
        if (_sock < 0)
        {
            cerr << "socket create error" << endl;
            exit(SOCKET_ERR);
        }
    }

    void start()
    {
        struct sockaddr_in server;
        memset(&server, 0, sizeof server);
        server.sin_family = AF_INET;
        server.sin_port = htons(_serverPort);
        server.sin_addr.s_addr = inet_addr(_serverIp.c_str());

        if (connect(_sock, (struct sockaddr*)&server, sizeof server) != 0)
        {
            cerr << "socket connect error" << endl;
        }
        else
        {
            string msg;
            while (true)
            {
                cout << "Enter# ";
                getline(cin, msg);
                write(_sock, msg.c_str(), msg.size());

                char buffer[NUM];
                int n = read(_sock, buffer, sizeof(buffer) - 1);
                if (n > 0)
                {
                    buffer[n] = 0;
                    cout << "Server回显# " << buffer << endl;
                }
                else
                {
                    break;
                }
            }
        }
    }

    ~tcpClient() = default;

private:
    int _sock;
    string _serverIp;
    uint16_t _serverPort;
};

}  // end of namespace Client