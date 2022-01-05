#ifndef CURRENTTHREAD_H
#define CURRENTTHREAD_H

#include <sys/types.h>

namespace BaseLib
{
namespace CurrentThread
{

extern __thread pid_t t_cacheTid;
extern __thread const char* t_threadName;

void initThreadInfo();

inline int getTid()
{
    if (t_cacheTid == 0)
    {
        initThreadInfo();
    }
    return t_cacheTid;
}

inline const char* getThreadName()
{ return t_threadName; }

} // namespace CurrentThread
} // namespace BaseLib

#endif