#ifndef SSCALL_H
#define SSCALL_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "Parameter.h"

namespace toyco
{

namespace SSCall
{

int read(int fd, void *buf, int len);
int write(int fd, void *buf, int len);
void sleep(double seconds);
void makePipe(int fd[2]);
void setNoBlock(int fd);
void setBlock(int fd);

int socket(int domain, int type, int protocol); // 创建套接字
int bind(int sockFd, const sockaddr* addr, socklen_t len); // 命名套接字
int listen(int sockFd, int n = Parameter::BACK_LOG);                // 监听
int accept(int sockFd, sockaddr* addr, socklen_t* len);


} // namespace SSCall

} // namespace toyco

#endif