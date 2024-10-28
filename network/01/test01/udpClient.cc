#include "udpClient.hpp"
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

    unique_ptr<udpClient> uclt(new udpClient(serverIp, serverPort));
    uclt->initClient();
    uclt->run();

    return 0;
}