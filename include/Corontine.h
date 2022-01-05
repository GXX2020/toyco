#ifndef COROUTINE_H
#define COROUTINE_H

#include <functional>
#include "Context.h"

namespace toyco 
{

class Processor;

class Coroutine
{
public:
    enum 
    {
        CO_READY = 0, CO_RUNNING, CO_WAITING, CO_DEADED
    };
    typedef std::function<void ()> CoFunc;
    
    Coroutine(CoFunc cFunc, Processor* myProcess, int stackSize);
    ~Coroutine();

    void resume();
    void yield();

    void runCFunc();
    Processor* getProcessor()
    { return myProcess_; }

private:
    Context ctx_;
    CoFunc cFunc_;
    Processor* myProcess_;
    int status_;
};

} // namespace toyco

#endif