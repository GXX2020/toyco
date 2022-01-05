#ifndef TIMER_H
#define TIMER_H

#include <set>
#include <vector>
#include "base/NoCopyAble.h"
#include "base/TimeStamp.h"

namespace toyco
{

const int TIMER_BUFFER_SIZE = 1024;

class Coroutine;
class Epoll;
class Timer : public BaseLib::NoCopyAble
{
public:
    Timer();
    ~Timer();
    
    void init(Epoll* epoll);
    void runAt(BaseLib::TimeStamp time, Coroutine* cor);
    void runAfter(double second, Coroutine* cor);
    void getExpiredCors(std::vector<Coroutine*>& coSet);
    
private:
    void resetTimerFd(BaseLib::TimeStamp time);
    typedef std::pair<BaseLib::TimeStamp, Coroutine*> Entry;
    typedef std::set<Entry> TimerQue;
    int timerFd_;
    char buf[TIMER_BUFFER_SIZE];
    TimerQue tQue_;
};

} // namespace toyco

#endif