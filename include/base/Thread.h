#ifndef THREAD_H
#define THREAD_H

#include <string>
#include <pthread.h>
#include <functional>
#include "base/NoCopyAble.h"
#include "base/CountDownLatch.h"
#include "base/Atomic.h"

namespace BaseLib
{

class Thread : NoCopyAble
{
public:
    typedef std::function<void ()> ThreadFunc;
    Thread(ThreadFunc func, const std::string& threadName = std::string());
    ~Thread();

    void start();
    int join();

    inline bool started() const
    { return started_; }
    inline bool joined() const
    { return joined_; }

    inline pthread_t getThreadId() const
    { return threadId_; }
    inline pid_t getTid() const 
    { return tid_; }
    inline std::string getThreadName() const
    { return threadName_; } 

private:
    pthread_t threadId_;
    pid_t tid_;
    ThreadFunc func_;
    CountDownLatch latch_;
    bool started_;
    bool joined_;
    std::string threadName_;
    static AtomicInt32 numCreated_;
};

} // namespace BaseLib

#endif