#pragma once
#include <functional>
class Task
{
public:
    using func_t = std::function<int(int, int)>;
    Task() = default;
    Task(int x, int y, func_t func)
        : _x(x)
        , _y(y)
        , _callback(func)
    {}

    int operator()()
    {
        return _callback(_x, _y);
    }
private:
    int _x;
    int _y;
    func_t _callback;
};