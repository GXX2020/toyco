#include <signal.h>
#include <sys/timerfd.h>
#include <unistd.h>
#include <assert.h>
#include <iostream>
#include <string.h>
#include "Timer.h"
#include "Epoll.h"

struct timespec curTimeFromNow(BaseLib::TimeStamp curTime)
{
    int64_t diffMicroTime = curTime.microSecondSinceEpoch() - BaseLib::TimeStamp::now().microSecondSinceEpoch();
    struct timespec t;
    if (diffMicroTime < 1)
    {
        t.tv_sec = 0;
        t.tv_nsec = 1000;
    }
    else
    {
        t.tv_sec = static_cast<time_t>(diffMicroTime / BaseLib::TimeStamp::kMicroSecondPerSecond_);
        t.tv_nsec = static_cast<long>((diffMicroTime % BaseLib::TimeStamp::kMicroSecondPerSecond_) * 1000);
    }
    
    return t;
}

using namespace toyco;

Timer::Timer()
 : timerFd_(-1)
{
}

Timer::~Timer()
{
}

void Timer::init(Epoll* epoll)
{
    timerFd_ = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
    assert(timerFd_ > 0);
    // 将timerfd加入epoll中
    epoll->addEvents(nullptr, timerFd_, EPOLLIN | EPOLLPRI | EPOLLRDHUP);
}

void Timer::runAt(BaseLib::TimeStamp time, Coroutine* cor)
{
    tQue_.insert(std::pair<BaseLib::TimeStamp, Coroutine*>(time, cor));
    if (tQue_.begin()->first == time)
    {
        resetTimerFd(time);
    }
}

void Timer::runAfter(double second, Coroutine* cor)
{
    BaseLib::TimeStamp finTime(BaseLib::addTimeBySecond(BaseLib::TimeStamp::now(), second));
    runAt(finTime, cor);
}

void Timer::getExpiredCors(std::vector<Coroutine*>& coSet)
{
    Entry sentry(BaseLib::TimeStamp::now(), reinterpret_cast<Coroutine*>(UINTPTR_MAX));
    auto end = tQue_.lower_bound(sentry);
    assert(end == tQue_.end() || sentry.first < end->first);
    auto it = tQue_.begin();
    while (it != end)
    {
        if (it->second != nullptr)
        {
            coSet.push_back(it->second);
        }
        it++;
    }

    tQue_.erase(tQue_.begin(), end);
    ssize_t rn = TIMER_BUFFER_SIZE;
    while (rn >= TIMER_BUFFER_SIZE)
    {
        rn = ::read(timerFd_, buf, TIMER_BUFFER_SIZE);
    }
    if (!tQue_.empty())
    {
        resetTimerFd(tQue_.begin()->first);
    }
}

void Timer::resetTimerFd(BaseLib::TimeStamp time)
{
    struct itimerspec newTime;
    struct itimerspec oldTime;
    memset(&newTime, 0, sizeof(newTime));
    memset(&oldTime, 0, sizeof(oldTime));
    newTime.it_value = curTimeFromNow(time);
    int ret = ::timerfd_settime(timerFd_, 0, &newTime, &oldTime);
    if (ret != 0)
    {
        fprintf(stderr, "in Timer::resetTimerFd() error.\n");
    }
}