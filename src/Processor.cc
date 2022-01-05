#include <iostream>
#include "base/Mutex.h"
#include "Processor.h"
#include "Corontine.h"
#include "Parameter.h"

namespace toyco
{
__thread int processorId = -1;
__thread bool isInCor = false;
}

using namespace toyco;

Processor::Processor(int proId)
 : processorId_(proId),
   curCoroutine_(nullptr),
   latch_(1),
   proThread_(nullptr),
   eventQue_(10),
   state_(PRO_STOPED)
{
    state_ = PRO_RUNNING;
    mainCtx_.makeCurContext();
}

Processor::~Processor()
{
    if (state_ == PRO_RUNNING)
    {
        stop();
    }
    if (state_ == PRO_STOPPING)
    {
        join();   
    }
    if (proThread_ != nullptr)
    {
        delete proThread_;
    }
    for (auto cor : coSet_)
    {
        delete cor;
    }
}

void Processor::loop()
{
    // 初始化
    processorId = processorId_;
    readyQue_.clear();
    eventQue_.clear();
    timeOutQue_.clear();
    deadedQue_.clear();
    epoll_.init();
    timer_.init(&epoll_);
    latch_.countDown();

    // 进入循环
    while (state_ == PRO_RUNNING)
    {
        // 首先处理等待队列
        {
            BaseLib::MutexLockGuard gu(mutex_);
            for (Coroutine* coItem : readyQue_)
            {
                coSet_.insert(coItem);
                coItem->resume();
            }
            readyQue_.clear();
        }

        // 处理超时队列
        timer_.getExpiredCors(timeOutQue_);
        for (Coroutine* cor : timeOutQue_)
        {
            cor->resume();
        }
        timeOutQue_.clear();

        // 处理事件队列
        epoll_.getActEvents(Parameter::EPOLL_TIMEOUT_MS, eventQue_);
        for (Coroutine* cor : eventQue_)
        {
            cor->resume();
        }
        eventQue_.clear();

        // 删除死亡的协程
        for (Coroutine* deadCor : deadedQue_)
        {
            coSet_.erase(deadCor);
            delete deadCor;
        }
        deadedQue_.clear();
    }
    state_ = PRO_STOPED;
}

void Processor::start()
{
    proThread_ = new BaseLib::Thread(std::bind(&Processor::loop, this));
    proThread_->start();
}

void Processor::stop()
{
    state_ = PRO_STOPPING;
}

void Processor::join()
{
    assert(proThread_ != nullptr);
    proThread_->join();
}

void Processor::addCoroutine(Coroutine* co)
{
    latch_.wait();
    {
        BaseLib::MutexLockGuard guard(mutex_);
        readyQue_.push_back(co);
    }
    // 激活loop循环
    wakeUp();
}

void Processor::addCorToDeathQue()
{
    assert(curCoroutine_ != nullptr);
    deadedQue_.push_back(curCoroutine_);
}

int Processor::getCorNum() const
{
    BaseLib::MutexLockGuard guard(mutex_);
    return coSet_.size();
}

void Processor::wait(double seconds)
{
    timer_.runAfter(seconds, curCoroutine_);
    curCoroutine_->yield();
}

void Processor::wakeUp()
{
    epoll_.wakeUp();
}

void Processor::waitEvents(int fd, int events)
{
    epoll_.addEvents(curCoroutine_, fd, events);
    curCoroutine_->yield();
    epoll_.deleteEvents(curCoroutine_, fd, events);
}
