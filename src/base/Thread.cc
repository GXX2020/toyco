#include "base/Thread.h"
#include <iostream>
#include <assert.h>

using namespace BaseLib;

struct ThreadData
{
    ThreadData(pid_t *tid, Thread::ThreadFunc func, CountDownLatch *latch, std::string threadName)
     : tid_(tid),
       func_(func),
       latch_(latch),
       threadName_(threadName)
    {}

    void runInThread()
    {
        *tid_ = CurrentThread::getTid();
        tid_ = nullptr;
        CurrentThread::t_threadName = threadName_.empty() ? "defaultThreadName" : threadName_.c_str();
        latch_->countDown();
        latch_ = nullptr;
        try
        {
            func_();
            CurrentThread::t_threadName = "finished";
        }
        catch(const std::exception& e)
        {
            std::cerr << "error in runInThread: ";
            std::cerr << e.what() << '\n';
        }
    }

    pid_t *tid_;
    Thread::ThreadFunc func_;
    CountDownLatch *latch_;
    std::string threadName_;

};

void* runThread(void* data)
{
    if (data == nullptr) {
        fprintf(stderr, "In RunThread: data is null\n");
    }
    ThreadData *obj = static_cast<ThreadData*>(data);
    obj->runInThread();
    delete obj;
    return NULL;
}

AtomicInt32 Thread::numCreated_;

Thread::Thread(ThreadFunc func, const std::string& threadName)
 : threadId_(0),
   tid_(0),
   func_(func),
   latch_(1),
   started_(false),
   joined_(false),
   threadName_(threadName)
{
    numCreated_.increment();
    if (threadName_.empty()) 
    {
        char buf[32] = {0};
        snprintf(buf, sizeof(buf), "Thread%d", numCreated_.get());
        threadName_ = buf;
    }
}

Thread::~Thread()
{
    if (started_ && !joined_)
    {
        pthread_detach(threadId_);
    }
}

void Thread::start()
{
    assert(!started_);
    started_ = true;
    ThreadData* data = new ThreadData(&tid_, func_, &latch_, threadName_);
    if (pthread_create(&threadId_, NULL, runThread, data)) // 线程创建失败
    {
        started_ = false;
        delete data;
        fprintf(stderr, "In Thread::start(): pthread_create error\n");
    }
    else
    {
        latch_.wait();
        assert(tid_ > 0);
    }
}

int Thread::join()
{
    assert(started_);
    assert(!joined_);
    joined_ = true;
    return pthread_join(threadId_, nullptr);
}
