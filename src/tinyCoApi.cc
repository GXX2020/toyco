#include "tinyCoApi.h"
#include "base/Singleton.h"
#include "Scheduler.h"

using namespace toyco;

void toyco::go(CoFunc func, int stackSize)
{
    BaseLib::Singleton<Scheduler>::instance().makeNewCo(func, stackSize);
}

void toyco::scheJoin()
{
    BaseLib::Singleton<Scheduler>::instance().join();
}
