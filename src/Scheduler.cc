#include "Scheduler.h"
#include "Processor.h"
#include "Corontine.h"
#include <assert.h>
#include <iostream>
#include <unistd.h>

using namespace toyco;

Scheduler::Scheduler(int coreNum)
: coreNum_(coreNum),
  proSet_(coreNum),
  selector_(proSet_)
{
    assert(coreNum_ > 0);
    // 初始化processor
    for (int i = 0; i < coreNum_; i++)
    {
        Processor *pro = new Processor(i);
        proSet_[i] = pro;
        pro->start();
    }
}

Scheduler::~Scheduler()
{
    for (auto pro : proSet_)
    {
        pro->stop();
        pro->join();
    }
    {
        BaseLib::MutexLockGuard guard(mutex_);
        for (auto& pro : proSet_)
        {
            delete pro;
        }
        proSet_.clear();
    }
}

void Scheduler::makeNewCo(CoFunc cFunc, int stackSize)
{
    {
        BaseLib::MutexLockGuard guard(mutex_);
        Processor* myProcess = selector_.getBestProcessor();
        if (!myProcess->isStop())
        {
            // 创建新的协程
            Coroutine *newCo = new Coroutine(cFunc, myProcess, stackSize);
            // 加入processor中的等待队列中
            myProcess->addCoroutine(newCo);
        }
    }
}

void Scheduler::join()
{
    for (auto pro : proSet_)
    {
        pro->join();
    }
}

Processor* Scheduler::getProcesser()
{
    assert(processorId >= 0 && processorId < proSet_.size());
    return proSet_[processorId];
}

Processor* Scheduler::getProcesser(int proId)
{
    assert(proId >= 0 && proId < proSet_.size());
    return proSet_[proId];
}