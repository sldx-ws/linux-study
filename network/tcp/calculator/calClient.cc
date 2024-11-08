#include "calClient.hpp"
#include <memory>

using namespace Client;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\n";
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    string serverIp = argv[1];
    uint16_t serverPort = atoi(argv[2]);

    unique_ptr<CalClient> tct(new CalClient(serverIp, serverPort));
    tct->initClient();
    tct->start();

    return 0;
}