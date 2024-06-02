#include "myStdio.h"
#include <stdio.h>

int main()
{
    FILE_ *fp = fopen_("./log.txt", "w");
    if (fp == NULL)
        return 1;

    const char *msg = "hello world\n";
    // const char* msg = "hello world";
    int cnt = 10;
    while (1)
    {
        --cnt;
        fwrite_(msg, strlen(msg), fp);
        sleep(1);
        printf("count: %d\n", cnt);
        if (cnt == 0)
            break;
    }

    fclose_(fp);

    return 0;
}
