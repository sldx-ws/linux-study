#ifndef __COMM_HPP__
#define __COMM_HPP__

#include <iostream>
#include <cerrno>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define PATHNAME "."
#define PROJ_ID 0x66
// 共享内存的大小，一般建议是 4kb 的整数倍
// 系统分配共享内存是 4kb 为单位的  --内存划分内存块的基本单位Page
#define MAX_SIZE 4097  // os内核给你的会向上取整 2 * 4096kb。  内核给你的，和你能用的，是两码事

key_t getKey() 
{
    key_t k = ftok(PATHNAME, PROJ_ID);
    if (k < 0)
    {
        // cin, cout, cerr -> stdin, stdout, stderr -> 0, 1, 2
        std::cerr << "ftok: " << errno << ": " << strerror(errno) << std::endl;
        exit(1);
    }

    return k;
}

int getShmHelper(key_t k, int flags)
{
    int shmid = shmget(k, MAX_SIZE, flags);
    if (shmid < 0) 
    {
        std::cerr << "shmget: " << errno << ": " << strerror(errno) << std::endl;
        exit(2);
    }

    return shmid;
}

int getShm(key_t k) 
{
    return getShmHelper(k, IPC_CREAT); // 0 也一样
}

int createShm(key_t k) 
{
    return getShmHelper(k, IPC_CREAT | IPC_EXCL | 0600);
}

void *attachShm(int shmid) 
{
    void *mem = shmat(shmid, nullptr, 0);  // 64位系统，8 byte
    if ((long long)mem == -1L) 
    {
        std::cerr << "shmat: " << errno << ": " << strerror(errno) << std::endl;
        exit(3);
    }

    return mem;
}

void detachShm(void *start)
{
    if (shmdt(start) == -1) 
        std::cerr << "shmdt: " << errno << ": " << strerror(errno) << std::endl;
}

void delShm(int shmid) 
{
    if (shmctl(shmid, IPC_RMID, nullptr) == -1) 
        std::cerr << "shmctl: " << errno << ": " << strerror(errno) << std::endl;
}

#endif