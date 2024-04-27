#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main() {
    // c
    printf("printf output\n");
    fprintf(stdout, "sprintf output\n");
    fputs("fputs output\n", stdout);

    // system call 
    const char* msg = "write output\n";
    write(1, msg, strlen(msg));

    fork();

    return 0;
}
