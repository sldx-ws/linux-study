#include "comm.hpp"

int main()
{
    std::cout << "client begin" << std::endl;
    int wfd = open(NAMED_PIPE, O_WRONLY);
    std::cout << "client end" << std::endl;
    if (wfd < 0)
        exit(1);

    // write
    char buffer[1024];

    while (1)
    {
        std::cout << "Please Say# ";
        fgets(buffer, sizeof(buffer), stdin);
        if (strlen(buffer) > 0)
            buffer[strlen(buffer) - 1] = 0; // 去掉\n

        ssize_t n = write(wfd, buffer, strlen(buffer));
        assert(n == strlen(buffer));
        (void)n;
    }

    close(wfd);
    return 0;
}