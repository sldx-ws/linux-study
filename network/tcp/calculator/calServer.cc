#include "calServer.hpp"
#include <memory>
using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_port\n\n";
}

// req: 一定是处理好的一个完整的请求
// resp: 根据req，进行业务处理，填充resp。不用管任何IO、序列化和反序列化等任何细节
bool calculator(const Request& req,  Response& resp)
{
    resp._exitCode = OK;
    resp._result = OK;

    switch(req._op)
    {
    case '+':
        resp._result = req._x + req._y;
        break;
    case '-':
        resp._result = req._x - req._y;
        break;
    case '*':
        resp._result = req._x * req._y;
        break;
    case '/':
    {
        if (req._y == 0) resp._exitCode = DIV_ZERO;
        else resp._result = req._x / req._y;
        break;
    }        
    case '%':
    {
        if (req._y == 0) resp._exitCode = MOD_ZERO;
        else resp._result = req._x % req._y;
        break;
    }
    default:
        resp._exitCode = OP_ERROR;
        break;
    }

    return true;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t serverPort = atoi(argv[1]);

    unique_ptr<CalServer> tcs(new CalServer(serverPort));
    tcs->initServer();
    tcs->start(calculator);

    return 0;
}