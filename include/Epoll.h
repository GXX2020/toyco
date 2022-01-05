#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>
#include <vector>
#include "base/NoCopyAble.h"

namespace toyco
{

class Coroutine;

class Epoll : public BaseLib::NoCopyAble
{
public:
    Epoll();
    ~Epoll();

    int init();

    int modifyEvents(Coroutine* cor, int fd, int events);
    int addEvents(Coroutine* cor, int fd, int events);
    int deleteEvents(Coroutine* cor, int fd, int events);
    
    void getActEvents(int timesOut, std::vector<Coroutine*> &actEvs);

    void wakeUp();
    
private:
    int update(int op, Coroutine* cor, int fd, int events);
    void handleWakeUp();
    int epollFd_;
    int pipeFd_[2];
    char pipeBuf_[128];
    std::vector<struct epoll_event> activateEvents_;
};

} // namespace toyco

#endif