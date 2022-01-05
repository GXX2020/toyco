#include "base/CountDownLatch.h"

using namespace BaseLib;

void CountDownLatch::wait()
{
    MutexLockGuard guard(mutex_);
    while (count_ > 0)
    {
        cond_.wait();
    }
}

void CountDownLatch::countDown()
{
    MutexLockGuard guard(mutex_);
    count_--;
    if (count_ == 0)
    {
        cond_.notifyAll();
    }
}

int CountDownLatch::getCount() const
{
    MutexLockGuard guard(mutex_);
    return count_;
}