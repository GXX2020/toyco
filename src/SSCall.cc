#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include "base/Singleton.h"
#include "SSCall.h"
#include <assert.h>
#include "Scheduler.h"
#include "Processor.h"

using namespace toyco;

int readInCor(int fd, void *buf, int len)
{
    // 设置fd为非阻塞
    int oldFlags = ::fcntl(fd, F_GETFL, 0);
    int newFlags = oldFlags | O_NONBLOCK;
    ::fcntl(fd, F_SETFL, newFlags);

    int rn = ::read(fd, buf, len);
    if (rn >= 0)
    {
        return rn;
    }
    if (rn == -1 && errno == EINTR)
    {
        return readInCor(fd, buf, len);
    }
    BaseLib::Singleton<Scheduler>::instance().getProcesser()->waitEvents(fd, EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLHUP);
    rn = ::read(fd, buf, len);
    // 恢复原先fd的flags
    ::fcntl(fd, F_SETFL, oldFlags);
    assert(rn >= 0);
    return rn;
}

int SSCall::read(int fd, void *buf, int len)
{
    if (isInCor)
    {
        return readInCor(fd, buf, len);
    }
    return ::read(fd, buf, len);
}

int writeInCor(int fd, void *buf, int len)
{
    int oldFlags = ::fcntl(fd, F_GETFL, 0);
    int newFlags = oldFlags | O_NONBLOCK;
    ::fcntl(fd, F_SETFL, newFlags);
    int wn = ::write(fd, buf, len);
    if (wn >= 0)
    {
        return wn;
    }
    if (wn == -1 && errno == EINTR)
    {
        return writeInCor(fd, buf, len);
    }
    BaseLib::Singleton<Scheduler>::instance().getProcesser()->waitEvents(fd, EPOLLOUT);
    wn = ::write(fd, buf, len);
    ::fcntl(fd, F_SETFL, oldFlags);
    assert(wn >= 0);
    return wn;
}

int SSCall::write(int fd, void *buf, int len)
{
    if (isInCor)
    {
        return writeInCor(fd, buf, len);
    }
    return ::write(fd, buf, len);
}

void sleepInCor(double seconds)
{
    BaseLib::Singleton<Scheduler>::instance().getProcesser()->wait(seconds);
}

void SSCall::sleep(double seconds)
{
    if (isInCor)
    {
        sleepInCor(seconds);
    }
    else
    {
        ::sleep(static_cast<unsigned int>(seconds));
    }
}

void SSCall::makePipe(int fd[2])
{
    int ret = ::socketpair(AF_UNIX, SOCK_STREAM, 0, fd);
    setNoBlock(fd[0]);
    setNoBlock(fd[1]);
    if (ret < 0)
    {
        fprintf(stderr, "socketpair error\n");
    }
}

void SSCall::setNoBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void SSCall::setBlock(int fd)
{
    int flags = fcntl(fd, F_GETFL, 0);
    fcntl(fd, F_SETFL, flags & ~O_NONBLOCK);
}

int SSCall::socket(int domain, int type, int protocol) 
{
    int sockFd = ::socket(domain, type, protocol);
    int on = 1;
    setsockopt(sockFd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
    if (sockFd < 0)
    {
        fprintf(stderr, "SSCall::socket() error\n");
    }
    return sockFd;
}

int SSCall::bind(int sockFd, const sockaddr* addr, socklen_t len) 
{
    int ret = ::bind(sockFd, addr, len);
    if (ret < 0)
    {
        fprintf(stderr, "SSCall::bind() error\n");
    }
    return ret;
}

int SSCall::listen(int sockFd, int n)              
{
    int ret = ::listen(sockFd, n);
    if (ret < 0)
    {
        fprintf(stderr, "SSCall::listen() error\n");
    }
    return ret;
}

int acceptInCor(int sockFd, sockaddr* addr, socklen_t* len) 
{
    int oldFlags = ::fcntl(sockFd, F_GETFL, 0);
    int newFlags = oldFlags | O_NONBLOCK;
    ::fcntl(sockFd, F_SETFL, newFlags);
    int connfd = ::accept(sockFd, addr, len);
    if (connfd >= 0)
    {
        SSCall::setNoBlock(connfd);
        return connfd;
    }
    if (connfd < 0 && errno == EINTR)
    {
        return acceptInCor(sockFd, addr, len);
    }
    BaseLib::Singleton<Scheduler>::instance().getProcesser()->waitEvents(sockFd, EPOLLIN | EPOLLPRI | EPOLLRDHUP | EPOLLHUP);
    connfd = ::accept(sockFd, addr, len);
    SSCall::setNoBlock(connfd);
    ::fcntl(sockFd, F_SETFL, oldFlags);
    return connfd;
}

int SSCall::accept(int sockFd, sockaddr* addr, socklen_t* len)
{
    if (isInCor)
    {
        return acceptInCor(sockFd, addr, len);
    }
    int connfd = ::accept(sockFd, addr, len);
    if (connfd >= 0)
    {
        setNoBlock(connfd);
        return connfd;
    }
    return -1;
}



