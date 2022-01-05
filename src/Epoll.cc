#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include "Epoll.h"
#include "SSCall.h"
#include "Parameter.h"

using namespace toyco;

Epoll::Epoll()
 : epollFd_(-1),
   pipeFd_{0},
   pipeBuf_{0},
   activateEvents_{Parameter::EPOLL_ARRAY_FIRST_SIZE}
{
}

Epoll::~Epoll()
{
    if (epollFd_ != -1)
    {
        ::close(epollFd_);
    }
}

int Epoll::init()
{
    epollFd_ = epoll_create1(EPOLL_CLOEXEC);
    if (epollFd_ < 0)
    {
        fprintf(stderr, "in Epoll::init() error.\n");
        return -1;
    }
    // 将激活管道加入epoll中
    SSCall::makePipe(pipeFd_);
    addEvents(nullptr, pipeFd_[0], EPOLLIN | EPOLLRDHUP);
    return 0;
}

int Epoll::modifyEvents(Coroutine* cor, int fd, int events)
{
    return update(EPOLL_CTL_ADD, cor, fd, events);
}

int Epoll::addEvents(Coroutine* cor, int fd, int events)
{
    return update(EPOLL_CTL_ADD, cor, fd, events);
}

int Epoll::deleteEvents(Coroutine* cor, int fd, int events)
{
    return update(EPOLL_CTL_DEL, cor, fd, events);
}

void Epoll::getActEvents(int timesOut, std::vector<Coroutine*> &actEvs)
{
    assert(epollFd_ >= 0);
    int n = static_cast<int>(activateEvents_.size());
    int actNum = ::epoll_wait(epollFd_, &*activateEvents_.begin(), n, timesOut);
    if (actNum < 0 || actNum > n)
    {
        fprintf(stderr, "in Epoll::getActEvents error.\n");
        return;
    }
    for (int i = 0; i < actNum; i++)
    {
        Coroutine *cor = static_cast<Coroutine*>(activateEvents_[i].data.ptr);
        if (cor != nullptr)
        {
            actEvs.push_back(cor);
        }
        else // 处理激活管道事件
        {
            handleWakeUp();
        }
    }
    if (actNum == n)
    {
        activateEvents_.reserve(n * 2);
    }
}

void Epoll::wakeUp()
{
    const char msg[1] = {'a'};
    int n = ::write(pipeFd_[1], static_cast<const void *>(msg), sizeof(msg));
    assert(n == 1);
}

int Epoll::update(int op, Coroutine* cor, int fd, int events)
{
    assert(epollFd_ >= 0);
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.data.ptr = cor;
    ev.events = events;
    assert(op == EPOLL_CTL_MOD || op == EPOLL_CTL_DEL || op == EPOLL_CTL_ADD);
    if (epoll_ctl(epollFd_, op, fd, &ev) < 0)
    {
        return -1;
    }
    return 0;
}

void Epoll::handleWakeUp()
{
    int pipeBufLen = 128;
    while (pipeBufLen >= 128)
    {
        pipeBufLen = ::read(pipeFd_[0], static_cast<void *>(pipeBuf_), sizeof(pipeBuf_));
    }
}
