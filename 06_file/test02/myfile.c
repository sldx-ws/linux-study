#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define FILE_NAME "log.txt"

int main() {
    //close(0);
    //close(2);
    //close(1);

    umask(0);
    //int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666);
    //if (fd < 0) {
    //    perror("open:");
    //    return 1;
    //}
    //dup2(fd, 1); // 输出重定向
    //printf("printf: open fd: %d\n", fd); // printf -> stdout
    //fprintf(stdout, "sprintf: open fd: %d\n", fd); // printf -> stdout

    //const char* msg = "hello world\n";
    //write(1, msg, strlen(msg));
    //fflush(stdout);


    int fd = open(FILE_NAME, O_RDONLY);
    if (fd < 0) {
        perror("open:");
        return 1;
    } 

    dup2(fd, 0); // 输入重定向

    char line[64];
    while (1) {
        if (fgets(line, sizeof(line), stdin) == NULL) // stdin -> 0
            break;
    
        printf("%s", line);
    } 

    close(fd);

    return 0;
}
