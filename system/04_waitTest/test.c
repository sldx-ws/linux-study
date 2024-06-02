#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>

int main() {
    pid_t id = fork();

    if (id == 0) {
       int cnt = 5; 
        while (cnt--) {
            printf("子进程正在运行, cnt: %d\n", cnt);
            sleep(1);
        }
        exit(1);
    } else if (id < 0) {
        // to do
    }


    while (1) {
        int status = 0;
        pid_t ret = waitpid(id, &status, WNOHANG); 

        if (ret == 0) {
            printf("wait...\n");
            sleep(1);
        } else if (ret > 0) {
            printf("wait success\n");
            printf("exit_code: %d, signal_code: %d\n", (status >> 8) & 0xFF, status & 0x7F);
            break;
        } else {
            printf("wait fail\n");
            break;
        }
    }

    return 0;
}
