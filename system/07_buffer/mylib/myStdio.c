#include "myStdio.h"

FILE_ *fopen_(const char *path_name, const char *mode)
{
    int flags = 0;
    int defaultMode = 0666;

    if (strcmp(mode, "r") == 0)
        flags |= O_RDONLY;
    else if (strcmp(mode, "w") == 0)
        flags |= (O_WRONLY | O_CREAT | O_TRUNC);
    else if (strcmp(mode, "a") == 0)
        flags |= (O_WRONLY | O_CREAT | O_APPEND);
    else
        // to do
        ;

    int fd = 0;
    if (flags & O_RDONLY)
        fd = open(path_name, flags);
    else
        fd = open(path_name, flags, defaultMode);

    if (fd < 0)
    {
        const char *err = strerror(errno);
        write(2, err, strlen(err));
        return NULL; // 为什么打开文件失败返回NULL
    }

    FILE_ *fp = (FILE_ *)malloc(sizeof(FILE_));
    assert(fp != NULL);

    fp->flags = SYNC_LINE; // 默认设置为行刷新
    fp->fileno = fd;
    fp->cap = SIZE;
    fp->size = 0;
    memset(fp->buffer, 0, SIZE);

    return fp;
}

void fwrite_(const void *ptr, int num, FILE_ *fp)
{
    // 1、写入缓冲区
    memcpy(fp->buffer + fp->size, ptr, num); // 这里不考虑缓冲区溢出问题
    fp->size += num;

    // 2、判断是否刷新
    if (fp->flags & SYNC_NOW)
    {
        write(fp->fileno, fp->buffer, fp->size);
        fp->size = 0; // 清空缓冲区
    }
    else if (fp->flags & SYNC_FULL)
    {
        if (fp->size == fp->cap)
        {
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }
    }
    else if (fp->flags & SYNC_LINE)
    {
        if (fp->buffer[fp->size - 1] == '\n')
        { // 假设传进来的只有最后一个字符是\n
            write(fp->fileno, fp->buffer, fp->size);
            fp->size = 0;
        }
    }
    else
    {
        // to do
    }
}

void fflush_(FILE_ *fp)
{
    if (fp->size > 0)
        write(fp->fileno, fp->buffer, fp->size);

    fsync(fp->fileno); // 将数据，强制要求os进行外设刷新
    fp->size = 0;
}

void fclose_(FILE_ *fp)
{
    fflush_(fp);
    close(fp->fileno);
}
