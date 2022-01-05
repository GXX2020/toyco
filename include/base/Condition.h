#ifndef CONDITION_H
#define CONDITION_H

#include <pthread.h>
#include "base/NoCopyAble.h"
#include "base/Mutex.h"

namespace BaseLib
{

class Condition : public NoCopyAble
{
public:
    explicit Condition(MutexLock &mutex)
     : mutex_(mutex)
    {
        CHECK(pthread_cond_init(&cond_, NULL));
    }

    ~Condition()
    {
        CHECK(pthread_cond_destroy(&cond_));
    }

    void wait()
    {
        UnassignHold unassign(mutex_);
        CHECK(pthread_cond_wait(&cond_, mutex_.getMutex()));
    }

    void notify()
    {
        CHECK(pthread_cond_signal(&cond_));
    }

    void notifyAll()
    {
        CHECK(pthread_cond_broadcast(&cond_));
    }

private:
    pthread_cond_t cond_;
    MutexLock &mutex_;
};

} // namespace BaseLib

#endif