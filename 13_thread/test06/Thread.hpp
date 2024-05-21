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
    Thread *this_;
    void *args_;
public:
    Context()
        : this_(nullptr)
        , args_(nullptr)
    {}

    ~Context() {}
};

class Thread
{
public:
    // using func_t = std::function<void* (void*)>;
    typedef std::function<void *(void *)> func_t;
    const int num = 1024;

public:
    Thread(func_t func, void *args = nullptr, int number = 0)
        : func_(func)
        , args_(args)
    {
        // name_ = "thread-";
        // name_ += std::to_string(number);

        char buffer[num];
        snprintf(buffer, sizeof buffer, "thread-%d", number);
        name_ = buffer;

        Context *ctx = new Context;
        ctx->this_ = this;
        ctx->args_ = args_;

        int n = pthread_create(&tid_, nullptr, start_routine, ctx);
        assert(0 == n);
        (void)n;
    }

    static void *start_routine(void *args)  // 类内成员，有缺省参数
    {
        Context *ctx = static_cast<Context *>(args);
        void *ret = ctx->this_->run(ctx->args_);

        delete ctx;
        return ret;

        // 静态方法不能调用成员方法或者成员变量
        // return func_(args_);

    }

    void join()
    {
        int n = pthread_join(tid_, nullptr);
        assert(0 == n);
        (void)n;
    }

    void *run(void *args)
    {
        return func_(args);
    }

    ~Thread()
    {
        // do nothing
    }

private:
    std::string name_;
    func_t func_;
    void *args_;
    pthread_t tid_;
};