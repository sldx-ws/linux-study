#include <stdio.h>
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
