#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FILE_NAME "log.txt"

int main() {
    //FILE* fp = fopen(FILE_NAME, "w");
    //if (NULL == fp) {
    //    perror("fopen:");
    //    return 1;
    //}

    //int cnt = 5;
    //while (cnt)
    //    fprintf(fp, "%s: %d\n", "hello world", cnt--);


    umask(0);
    //int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC, 0666); // write
    //int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_APPEND, 0666); // append
    int fd = open(FILE_NAME, O_RDONLY); // read 
    if (fd < 0) {
        perror("open:");
        return 1;
    }

    //char outBuffer[64];
    //int cnt = 5;

    //while (cnt) {
    //    sprintf(outBuffer, "hello: %d\n", cnt--);
    //    write(fd, outBuffer, strlen(outBuffer));
    //}
    
    char buffer[1024];
    ssize_t num = read(fd, buffer, sizeof(buffer) - 1);
    if (num > 0) 
        buffer[num] = 0;

    printf("%s", buffer);

    close(fd);
    return 0;
}
