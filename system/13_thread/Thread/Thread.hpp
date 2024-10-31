#pragma once

#include <iostream>
#include <string>
#include <cstring>
#include <cassert>
#include <functional>
#include <pthread.h>

class Thread;

// 上下文
// 当成一个大号的结构体
class Context
{
public:
    Context()
        : _this(nullptr)
        , _args(nullptr)
    {}

public: 
    Thread *_this;
    void *_args;
};

class Thread
{
public:
    // typedef std::function<void *(void *)> _funct;
    using func_t = std::function<void* (void*)>;
    const int num = 1024;

public:
    Thread(func_t func, void *args = nullptr, int number = 0)
        : _func(func)
        , _args(args)
    {
        char buffer[num];
        snprintf(buffer, sizeof buffer, "thread-%d", number);
        _name = buffer;

        Context *ctx = new Context;
        ctx->_this = this;
        ctx->_args = _args;

        int n = pthread_create(&_tid, nullptr, start_routine, ctx);
        assert(0 == n);
        (void)n;
    }

    void join()
    {
        int n = pthread_join(_tid, nullptr);
        assert(0 == n);
        (void)n;
    }

private:
    static void *start_routine(void *args)  // 类内成员，有缺省参数
    {
        Context *ctx = static_cast<Context *>(args);
        void *ret = ctx->_this->run(ctx->_args);

        delete ctx;
        return ret;

        // 静态方法不能调用成员方法或者成员变量
        // return _func(_args);  // err
    }

    void *run(void *args) { return _func(args); }

private:
    std::string _name;
    func_t _func;
    void *_args;
    pthread_t _tid;
};
