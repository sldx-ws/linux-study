#include "HttpServer.hpp"
#include <memory>
using namespace std;
using namespace Server;

void Usage(std::string proc)
{
    cerr << "\r\nUsage:\r\n\t" << proc << " port\r\n\r\n";
}

bool Get(const HttpRequest& req, HttpResponse& resp)
{
    cout << "----------------http start -----------------" << endl;
    cout << req._inBuffer << std::endl;
    std::cout << "method: " << req._method << std::endl;
    std::cout << "url: " << req._url << std::endl;
    std::cout << "httpVersion: " << req._httpVersion << std::endl;
    std::cout << "path: " << req._path << std::endl;
    cout << "----------------http end -------------------" << endl;

    std::string respLine = "HTTp/1.1 200 OK\r\n";  // 状态行
    std::string respBlank = "\r\n";  // 空行
    // 响应正文
    std::string body = "<html lang=\"zh-CN\"><head><meta charset=\"UTF-8\"><title>for test</title></head><body><header><h1>欢迎来到我的网页！</h1></header></body></html>";

    resp._outBuffer += respLine;
    resp._outBuffer += respBlank;
    resp._outBuffer += body;

    return true;
}

// ./HttpServer 8080
int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(0);
    }

    uint16_t port = atoi(argv[1]);
    unique_ptr<HttpServer> httpSvr(new HttpServer(Get, port));
    httpSvr->initServer();
    httpSvr->start();

    return 0;
}