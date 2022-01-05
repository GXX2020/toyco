#ifndef COUNTDOWNLATCH_H
#define COUNTDOWNLATCH_H

#include "base/NoCopyAble.h"
#include "base/Condition.h"

namespace BaseLib
{

class CountDownLatch : public NoCopyAble
{
public:
    explicit CountDownLatch(int count)
     : count_(count),
       mutex_(),
       cond_(mutex_)
    {}

    void wait();
    void countDown();
    
    int getCount() const;

private:
    int count_;
    mutable MutexLock mutex_;
    Condition cond_;
};

} // namespace BaseLib

#endif