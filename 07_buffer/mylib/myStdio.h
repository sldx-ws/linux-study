#pragma once 

#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

#define SIZE 1024
#define SYNC_NOW  (1 << 0)
#define SYNC_LINE (1 << 1)
#define SYNC_FULL (1 << 2)

typedef struct _FILE {
    int flags;  // 刷新方式
    int fileno; // fd 
    int cap;    // buffer的总容量
    int size;   // buffer当前的使用量
    char buffer[SIZE];
} FILE_;


FILE_* fopen_(const char* path_name, const char* mode);
void fwrite_(const void* ptr, int num, FILE_* fp);
void fflush_(FILE_* fp); 
void fclose_(FILE_* fp);
