#include "HttpServer.hpp"
#include <memory>
using namespace std;
using namespace Server;

void Usage(std::string proc)
{
    cerr << "\r\nUsage:\r\n\t" << proc << " port\r\n\r\n";
}

std::string suffixToDesc(const std::string suffix)
{
    std::string ct = "Content-Type: ";
    if (suffix == ".html") ct += "text/html";
    else if (suffix == ".jpg") ct += "application/x-jpg;image/jpeg";

    ct += "\r\n";
    return ct;
}

// 1. 服务器和网页分离
// 2. url -> / : web根目录
// 3. 要正确的给客户端返回资源类型，我们首先要自己知道！所有的资源都有后缀
bool Get(const HttpRequest& req, HttpResponse& resp)
{
    cout << "----------------http start -----------------" << endl;
    cout << req._inBuffer << std::endl;
    std::cout << "method: " << req._method << std::endl;
    std::cout << "url: " << req._url << std::endl;
    std::cout << "httpVersion: " << req._httpVersion << std::endl;
    std::cout << "path: " << req._path << std::endl;
    std::cout << "suffix: " << req._suffix << std::endl;
    std::cout << "size: " << req._size << "bytes" << std::endl;
    cout << "----------------http end -------------------" << endl;

    std::string respLine = "HTTP/1.1 200 OK\r\n";  // 状态行
    std::string respHeader = suffixToDesc(req._suffix); // 响应报头
    if (req._size > 0)
    {
        respHeader += "Context-Length: ";
        respHeader += std::to_string(req._size);
        respHeader += "\r\n";
    }
    std::string respBlank = "\r\n";  // 空行
    // 响应正文
    std::string body;
    if (!Util::readFile(req._path, &body))
    {
        Util::readFile(html_404, &body); // 一定成功
    }

    resp._outBuffer += respLine;
    resp._outBuffer += respHeader;
    resp._outBuffer += respBlank;
    cout << "----------------http response start -----------------" << endl;
    std::cout << resp._outBuffer << std::endl;
    cout << "----------------http response end -------------------" << endl;
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