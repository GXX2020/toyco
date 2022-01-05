#ifndef SHEDULER_H
#define SHEDULER_H

#include <vector>
#include <functional>
#include "base/NoCopyAble.h"
#include "base/Singleton.h"
#include "base/Mutex.h"
#include "ProcessorSelector.h"
#include "Parameter.h"

namespace toyco
{

class Processor;

class Scheduler : public BaseLib::NoCopyAble
{
public:
    friend class ProcessorSelector;
    typedef std::function<void ()> CoFunc;
    Scheduler(int coreNum = 2);
    ~Scheduler();

    void makeNewCo(CoFunc cFunc, int stackSize);                   // 创建新协程
    void join();                                                   // 等待调度器结束

    Processor* getProcesser();                                     // 获取processor;
    Processor* getProcesser(int proId);
    
private:
    int coreNum_;
    std::vector<Processor*> proSet_;
    ProcessorSelector selector_;
    BaseLib::MutexLock mutex_;
};

} // namespace toyco

#endif