#include <iostream>
#include <cstdio>
#include <cstring>
#include <cassert>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

using namespace std;

// 父进程进行读取，子进程进行写入
// 父进程读的慢
int main() {
    // 第一步：创建管道文件，打开读写端
    int fds[2];
    int n = pipe(fds);
    assert(n == 0);

    // 第二步：fork
    pid_t id = fork();
    assert(id >= 0);

    if (id == 0) {
        // 子进程的通信代码
        close(fds[0]);
        const char *s = "我是子进程，我正在给你发消息";
        int cnt = 0;
        while (1) {
            ++cnt;
            char buffer[1024];
            snprintf(buffer, sizeof buffer, "child -> parent say: %s[%d][%d]", s, cnt, getpid());
            // 写端写满的时候，再写会阻塞，等待对方读取 
            write(fds[1], buffer, strlen(buffer));
            // sleep(5); // 每隔 5s 写一次
            cout << "count: " << cnt << endl;
        }

        close(fds[1]);
        exit(0);
    } 

    // 父进程的通信代码
    close(fds[1]);
    while (1) {
        sleep(100);
        char buffer[1024];
        ssize_t s = read(fds[0], buffer, sizeof(buffer) - 1);
        if (s > 0) 
            buffer[s] = 0;

        cout << "Get Message# " << buffer << "| my pid: " << getpid() << endl;
    }


    n = waitpid(id, nullptr, 0);
    assert(n == id);
    close(fds[0]);

    // 0,1,2 -> 3,4
    // [0]: read
    // [1]: write
    std::cout << "fds[0]" << fds[0] << endl;
    std::cout << "fds[1]" << fds[1] << endl;
    return 0;
}