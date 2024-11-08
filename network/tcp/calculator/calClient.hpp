#pragma once
#include <iostream>
#include <string>       // getline
#include <cstring>      // memset
#include <sys/types.h>  // socket connect inet_addr
#include <sys/socket.h> // socket connect
#include <arpa/inet.h>  // struct sockaddr_in inet_addr
#include <netinet/in.h> // inet_addr
#include "protocol.hpp"

#define NUM 1024

namespace Client
{

using namespace std;

enum
{   
    USAGE_ERR = 1,
    SOCKET_ERR
};

class CalClient
{
public:
    CalClient(const string& serverIp, const uint16_t& serverPort)
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
            string line;
            string inBuffer;
            while (true)
            {
                cout << "myCal>>> ";
                getline(cin, line);

                Request req = ParseLine(line); // "11+111"
                string content;
                req.serialize(&content);
                string send_string = enLength(content);
                std::cout << "send string:\n" << send_string << std::endl;
                send(_sock, send_string.c_str(), send_string.size(), 0); // bug

                string package, text;
                // "content_len"\r\n"exitCode result"\r\n
                if (!recvPackage(_sock, inBuffer, &package)) continue;
                if (!deLength(package, &text)) continue;

                // "exitCode result"
                Response resp;
                resp.deserialize(text);
                cout << "exitCode: " << resp._exitCode << endl;
                cout << "result: " << resp._result << endl;
            }
        }
    }

    Request ParseLine(const std::string& line)
    {
        // "11+111"
        int status = 0; // 0:操作符之前，1:碰到了操作符 2:操作符之后
        int i = 0;
        int cnt = line.size();
        std::string left, right;
        char op;
        while (i < cnt)
        {
            switch (status)
            {
            case 0:
            {
                if(!isdigit(line[i]))
                {
                    op = line[i];
                    status = 1;
                }
                else left.push_back(line[i++]);
                break;
            }
            case 1:
                i++;
                status = 2;
                break;
            case 2:
                right.push_back(line[i++]);
                break;
            }
        }
        std::cout << std::stoi(left)<<" " << std::stoi(right) << " " << op << std::endl;
        return Request(std::stoi(left), std::stoi(right), op);
    }

    ~CalClient() = default;

private:
    int _sock;
    string _serverIp;
    uint16_t _serverPort;
};

}  // end of namespace Client