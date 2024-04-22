#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("myproc running...\n");

    // 系统环境变量  
    printf("PATH: %s\n", getenv("PATH"));
    printf("PWD: %s\n", getenv("PWD"));

    // 自定义环境变量
    printf("MYENV: %s\n", getenv("MYENV"));

    return 0;
}
