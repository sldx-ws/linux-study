#include "tcpServer.hpp"
#include <memory>
using namespace Server;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    uint16_t serverPort = atoi(argv[1]);

    unique_ptr<tcpServer> tcs(new tcpServer(serverPort));
    tcs->initServer();
    tcs->start();

    return 0;
}