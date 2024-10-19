#include "comm.hpp"

int main() 
{
    int k = getKey();
    printf("key: %#x\n", k);
    int shmid = getShm(k);
    printf("shmid: %d\n", shmid);  // shmid
    
    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    const char *message = "hello server, 我是另一个进程，正在和你通信";
    pid_t id = getpid();
    int cnt = 1;
    // char buffer[1024];
    while (1)
    {
        sleep(1);
        // pid, count, message
        // snprintf(buffer, sizeof(buffer), "%s[pid: %d][消息编号: %d]", message, id, count);
        // memcpy(start, buffer, strlen(buffer) + 1);

        snprintf(start, MAX_SIZE, "%s[pid: %d][消息编号: %d]", message, id, cnt++);
    }

    detachShm(start);

    // done
    
    return 0;
}