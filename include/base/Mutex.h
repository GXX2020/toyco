#ifndef MUTEX_H
#define MUTEX_H

#include <pthread.h>
#include <assert.h>
#include "base/NoCopyAble.h"
#include "base/CurrentThread.h"

#define CHECK(ret) ({ __typeof__ (ret) errnum = (ret);         \
                       assert(errnum == 0); (void) errnum;})

namespace BaseLib
{

class MutexLock : public NoCopyAble
{
friend class UnassignHold;
public:
    MutexLock()
     : holder_(0)
    {
        CHECK(pthread_mutex_init(&mutex_, NULL));
    }

    ~MutexLock()
    {
        CHECK(pthread_mutex_destroy(&mutex_));
    }

    void lock()
    {
        CHECK(pthread_mutex_lock(&mutex_));
        assignHolder();
    }

    void unlock()
    {
        CHECK(pthread_mutex_unlock(&mutex_));
        unAssignHolder();
    }
    
    pthread_mutex_t* getMutex() 
    {
        return &mutex_;
    }

private:
    void assignHolder()
    {
        holder_ = CurrentThread::getTid();
    }
    
    void unAssignHolder()
    {
        holder_ = 0;
    }

    pthread_mutex_t mutex_;
    pid_t holder_;
};

class UnassignHold : public NoCopyAble
{
public:
    explicit UnassignHold(MutexLock& lock)
     : lock_(lock)
    {
        lock_.unAssignHolder();
    }
    ~UnassignHold()
    {
        lock_.assignHolder();
    }
private:
    MutexLock &lock_;
};

class MutexLockGuard : public NoCopyAble
{
public:
    explicit MutexLockGuard(MutexLock& mutex)
     : mutex_(mutex)
    {
        mutex_.lock();
    }
    ~MutexLockGuard()
    {
        mutex_.unlock();
    }
private:
    MutexLock &mutex_;
};

} // namespace BaseLib

#endif