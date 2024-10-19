#include "comm.hpp"

int main() 
{
    key_t k = getKey();
    printf("key: %#x\n", k);  // key
    int shmid = createShm(k);
    printf("shmid: %d\n", shmid);  // shmid

    // sleep(5);

    char *start = (char*)attachShm(shmid);
    printf("attach success, address start: %p\n", start);

    // 使用
    while (1)
    {
        // char buffer[]; read(pipefd, buffer, ...)
        printf("client say: %s\n", start);
        struct shmid_ds ds;
        shmctl(shmid, IPC_STAT, &ds);
        printf("获取属性: size: %d, cpid: %d, myselfPid: %d, key: %#x\n", \
                ds.shm_segsz, ds.shm_cpid, getpid(), ds.shm_perm.__key);
        sleep(1);
    }

    // 去关联
    detachShm(start);

    sleep(5);

    // 删除共享内存
    delShm(shmid);

    return 0;
}