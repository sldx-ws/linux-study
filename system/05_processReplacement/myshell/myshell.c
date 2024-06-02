#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <assert.h>
#include <string.h>

#define NUM 1024
#define OPT_NUM 64  // 最允许64个选项

char lineCommand[NUM];
char* myargv[OPT_NUM];
int lastCode = 0;
int lastSig = 0;

int main() {
    while (1) {
        // 输出提示符
        printf("[%s@%s %s]$ ", getenv("USER"), getenv("HOSTNAME"), getenv("PWD"));
        fflush(stdout);
    
        // 获取用户输入
        char* s = fgets(lineCommand, sizeof(lineCommand) - 1, stdin);
        assert(s != NULL);
        (void)s;
        lineCommand[strlen(lineCommand) - 1] = 0; // 清除fgets读取的最后一个\n
         
        myargv[0] = strtok(lineCommand, " ");

        int i = 1;
        if (myargv[0] != NULL && strcmp("ls", myargv[0]) == 0) {
            myargv[i++] = (char*)"--color=auto";
        }
        while (myargv[i++] = strtok(NULL, " "));

        // 如果是cd命令， 不需要创建子进程，让shell自己执行对应的命令，本质就是执行系统调用
        // 像这种不需要让子进程来执行，而是让shell自己执行的命令 ---内建/内置命令
        if (myargv[0] != NULL && strcmp(myargv[0], "cd") == 0) {
            if (myargv[1] != NULL) {
                chdir(myargv[1]);
                continue;
            }
        }
    
        if (myargv[0] != NULL && myargv[1] != NULL) {
            if (strcmp(myargv[0], "echo") == 0 && strcmp(myargv[1], "$?") == 0) {
                printf("lastCode: %d, lastSig: %d\n", lastCode, lastSig);
                continue;
            } 
        }
    #ifdef DEBUG
        for (int i = 0; myargv[i]; ++i) 
            printf("myargv[%d]: %s\n", i, myargv[i]);
    #endif

        // 执行命令
        pid_t id = fork();
        assert(id != -1);

        if (id == 0) {
            execvp(myargv[0], myargv);
            exit(1);
        }

        int status = 0;
        pid_t ret = waitpid(id, &status, 0);
        assert(ret > 0);
        (void)ret;

        lastCode = (status >> 8) & 0xff;
        lastSig = status & 0x7f;
    }

    return 0;
}
