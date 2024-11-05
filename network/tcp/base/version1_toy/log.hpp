#pragma once

#include <iostream>
#include <string>

#define DEBUG   0
#define NORMAL  1
#define WARNING 2
#define ERROR   3
#define FATAL   4

void logMessage(int level, const std::string& message)
{
    // [日志等级][时间戳/时间][pid][message]
    // [WARNING][2024-10-29 16:25:59][][创建socket失败]

    //  暂定
    std::cout << message << std::endl;
}