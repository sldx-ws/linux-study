#pragma once

#include <iostream>
#include <string>
#include <cstdarg>  // vsnprintf
#include <ctime>
#include <unistd.h> // getpid

#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

#define NUM 1024
#define LOG_NORMAL "log.txt"
#define LOG_ERR "log.error"


const char* to_levelStr(int level)
{
    switch(level)
    {
    case DEBUG:   return "DEBUG";
    case NORMAL:  return "NORMAL";
    case WARNING: return "WARNING";
    case ERROR:   return "ERROR";
    case FATAL:   return "FATAL";
    default:      return nullptr;
    }
}


// logMessage(DEBUG, "hello %f, %d, %c", 3.14, 10, 'c');
void logMessage(int level, const char* format, ...)
{
    // [日志等级][时间戳/时间][pid][message]
    // [WARNING][2024-10-29 16:25:59][][创建socket失败]
    char logPrefix[NUM];
    snprintf(logPrefix, sizeof(logPrefix), "[%s][%ld][pid: %d]", \
        to_levelStr(level), (long int)time(nullptr), getpid());

    char logContent[NUM];
    va_list arg;
    va_start(arg, format);
    vsnprintf(logContent, sizeof(logContent), format, arg);

    // std::cout << logPrefix << logContent << std::endl;
    FILE *log =  fopen(LOG_NORMAL, "a");
    FILE *err = fopen(LOG_ERR, "a");
    if(log != nullptr && err != nullptr)
    {
        FILE *curr = nullptr;
        if(level == DEBUG || level == NORMAL || level == WARNING) curr = log;
        if(level == ERROR || level == FATAL) curr = err;
        if(curr) fprintf(curr, "%s%s\n", logPrefix, logContent);
    }
    fclose(log);
    fclose(err);
}