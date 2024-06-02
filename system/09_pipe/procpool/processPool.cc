#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <cassert>
#include <ctime>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MakeSeed() srand((unsigned int)time(nullptr) ^ getpid() ^ 0x1314 ^ rand() % 1234)

#define PROCSS_NUM 5

///////////// 模拟子进程要完成的某种任务 /////////////
typedef void (*func_t)();

void downloadTask()
{
    std::cout << getpid() << ": 下载任务\n"
              << std::endl;
    sleep(1);
}

void ioTask()
{
    std::cout << getpid() << ": IO任务\n"
              << std::endl;
    sleep(1);
}

void flushTask()
{
    std::cout << getpid() << ": 刷新任务\n"
              << std::endl;
    sleep(1);
}

void LoadTaskFunc(std::vector<func_t> &out)
{
    out.push_back(downloadTask);
    out.push_back(ioTask);
    out.push_back(flushTask);
}

///////////// 下面的代码是一个多进程程序 /////////////
class subEp
{ // Endpoint另一端
public:
    subEp(pid_t subId, int writeFd)
        : _subId(subId), _writeFd(writeFd)
    {
        char nameBuffer[1024];
        snprintf(nameBuffer, sizeof nameBuffer, "process-%d[pid(%d)-fd(%d)]", num++, _subId, _writeFd);
        _name = nameBuffer;
    }

public:
    static int num;
    std::string _name;
    pid_t _subId;
    int _writeFd;
};
int subEp::num = 0;

int recvTask(int readFd)
{
    int code = 0;
    ssize_t s = read(readFd, &code, sizeof code);
    if (s == 4)
        return code;
    else if (s <= 0)
        return -1;
    else
        return 0;
}

void sendTask(const subEp &process, int taskNum)
{
    std::cout << "send task num: " << taskNum << "send to " << process._name << std::endl;
    int n = write(process._writeFd, &taskNum, sizeof(taskNum));
    assert(n == sizeof(int));
    (void)n;
}

void createSubProcess(std::vector<subEp> &subs, std::vector<func_t> &funcMap)
{
    std::vector<int> deleteFd;
    for (int i = 0; i < PROCSS_NUM; ++i)
    {
        int fds[2];
        int n = pipe(fds);
        assert(n == 0);
        (void)n;

        pid_t id = fork();
        if (id == 0)
        {
            for (int i = 0; i < deleteFd.size(); ++i)
                close(deleteFd[i]);

            // 子进程进行处理任务
            close(fds[1]);
            while (1)
            {
                // 1、获取命令码，如果没有发送，我们子进程应该阻塞
                int commandCode = recvTask(fds[0]);

                // 2、完成任务
                if (commandCode >= 0 && commandCode < funcMap.size())
                    funcMap[commandCode]();
                else if (commandCode == -1)
                    break;
            }
            exit(0);
        }

        close(fds[0]);
        subEp sub(id, fds[1]);
        subs.push_back(sub);
        deleteFd.push_back(fds[1]);
    }
}

void loadBlanceContrl(const std::vector<subEp> &subs, const std::vector<func_t> &funcMap, int count)
{
    int processNum = subs.size();
    int taskNum = funcMap.size();
    bool forever = (count == 0 ? true : false);
    while (1)
    {
        // 1、选择一个子进程 --> std::vector<subEp> --> index --随机数
        int subIdx = rand() % processNum;
        // 2、选择一个任务 --> std::vector<func_t> --> index
        int taskIdx = rand() % taskNum;
        // 3、任务发送给选择的进程
        sendTask(subs[subIdx], taskIdx);

        sleep(1);

        if (!forever)
        {
            --count;
            if (count == 0)
                break;
        }
    }

    // write quie -> read 0
    for (int i = 0; i < processNum; ++i)
        close(subs[i]._writeFd);
}

void waitProcess(std::vector<subEp> processes)
{
    int processNum = processes.size();
    for (int i = 0; i < processNum; ++i)
    {
        waitpid(processes[i]._subId, nullptr, 0);
        std::cout << "wait sub process success ...: " << processes[i]._subId << std::endl;
    }
}

int main()
{
    MakeSeed();
    // 1、建立子进程并建立和子进程通信的信道
    // 1.1 加载方法表
    std::vector<func_t> funcMap;
    LoadTaskFunc(funcMap);
    // 1.2 创建子进程，并且维护好父子通信信道
    std::vector<subEp> subs;
    createSubProcess(subs, funcMap);

    // 2、走到这里就是父进程，控制子进程，负载均衡的向自己进程发送命令码
    int taskCnt = 3; // 0：永远进行
    loadBlanceContrl(subs, funcMap, taskCnt);

    // 3、回收子进程信息
    waitProcess(subs);

    return 0;
}