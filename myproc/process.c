#include "process.h"

void ProcessOn()
{
    int cnt = 0; 
    char bar[NUM];
    memset(bar, '\0', sizeof(bar));

    const char* lable = "|\\-/";
    // 循环101次
    while (cnt <= 100)
    {
        printf("[%-100s][%d%%][%c]\r", bar, cnt, lable[cnt % 4]);
        fflush(stdout);
        bar[cnt++] = STYLE;
        // sleep(1);
        usleep(50000);
    }

    printf("\n");
}
