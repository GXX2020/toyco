#include "base/CurrentThread.h"
#include <unistd.h>
#include <sys/syscall.h>


namespace BaseLib
{
namespace CurrentThread
{

__thread pid_t t_cacheTid = 0;
__thread const char* t_threadName = "UNKNOWN";

void initThreadInfo()
{
    t_cacheTid = static_cast<pid_t>(::syscall(SYS_gettid));
}

} // namespace CurrentThread
} // namespace BaseLib