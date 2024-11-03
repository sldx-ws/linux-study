#pragma once
#include <iostream>
#include <string>
#include <cstdio>
#include <functional>

class Task
{
    using func_t = std::function<int(int,int,char)>;
public:
    Task() = default;

    Task(int x, int y, char op, func_t func)
        : _x(x)
        , _y(y)
        , _op(op)
        , _callback(func)
    {}

    std::string operator()()
    {
        int result = _callback(_x, _y, _op);
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = %d", _x, _op, _y, result);
        return buffer;
    }

    std::string toTaskString()
    {
        char buffer[1024];
        snprintf(buffer, sizeof buffer, "%d %c %d = ?", _x, _op, _y);
        return buffer;
    }

private:
    int _x;
    int _y;
    char _op;
    func_t _callback;
};

const std::string oper = "+-*/%";

int myMath(int x, int y, char op)
{
    int result = 0;
    
    switch (op)
    {
    case '+':
        result = x + y;
        break;
    case '-':
        result = x - y;
        break;
    case '*':
        result = x * y;
        break;
    case '/':
    {
        if (0 == y)
        {
            std::cerr << "div zero error!" << std::endl;
            result = -1;
        }
        else
        {
            result = x / y;
        }
    }
        break;
    case '%':
    {
        if (y == 0)
        {
            std::cerr << "mod zero error!" << std::endl;
            result = -1;
        }
        else
        {
            result = x % y;
        }
    }
        break;
    default:
        break;
    }

    return result;
}