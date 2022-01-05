#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <vector>
#include <queue>
#include <unordered_set>
#include "base/NoCopyAble.h"
#include "base/Mutex.h"
#include "base/CountDownLatch.h"
#include "base/Thread.h"
#include "Context.h"
#include "Epoll.h"
#include "Timer.h"


namespace toyco
{

extern __thread int processorId;
extern __thread bool isInCor;

class Coroutine;

class Processor : public BaseLib::NoCopyAble
{
public:
    enum
    {
        PRO_RUNNING = 0, PRO_STOPPING, PRO_STOPED
    };
    Processor(int proId);
    ~Processor();

    void loop();

    void start();
    void stop();
    void join();
    
    bool isStop() const
    { return state_ == PRO_STOPPING || state_ == PRO_STOPED; }

    void addCoroutine(Coroutine* co);
    void addCorToDeathQue();

    inline Context* getContext()
    { return &mainCtx_; }
    inline Coroutine* getCurCoroutine()
    { return curCoroutine_; }
    int getCorNum() const;
    inline void setcurCoroutine(Coroutine* curCoroutine)
    { curCoroutine_ = curCoroutine; }

    void wait(double seconds);
    void wakeUp();
    void waitEvents(int fd, int events);

private:
    typedef std::vector<Coroutine*> CorQue;
    int processorId_;
    Coroutine* curCoroutine_;
    std::unordered_set<Coroutine*> coSet_;
    Context mainCtx_;
    mutable BaseLib::MutexLock mutex_;
    BaseLib::CountDownLatch latch_;
    BaseLib::Thread *proThread_;
    Epoll epoll_;
    Timer timer_;
    CorQue readyQue_; // 新加入的协程，未进行调度
    CorQue eventQue_; // 事件队列
    CorQue timeOutQue_; // 超时队列
    CorQue deadedQue_; // 死亡队列
    int state_;
};

} // namespace toyco

#endif