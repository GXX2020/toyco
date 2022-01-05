#include "Corontine.h"
#include "Processor.h"
#include <iostream>

using namespace toyco;

void runInCor(Processor* pro)
{
    pro->getCurCoroutine()->runCFunc();
    // 运行结束 加入deaded队列
    pro->addCorToDeathQue();
}

Coroutine::Coroutine(CoFunc cFunc, Processor* myProcess, int stackSize)
 : ctx_(stackSize),
   cFunc_(cFunc),
   myProcess_(myProcess),
   status_(CO_READY)
{}

Coroutine::~Coroutine()
{
}

void Coroutine::resume()
{
    assert(status_ != CO_RUNNING);
    isInCor = true;
    myProcess_->setcurCoroutine(this);
    Context* mainCtx =  myProcess_->getContext();
    if (status_ == CO_DEADED)
    {
        return;
    }
    switch (status_)
    {
    case CO_READY:
        status_ = CO_RUNNING;         
        ctx_.makeContext((void(*)(void))runInCor, myProcess_, mainCtx);
        ctx_.swapToMe(mainCtx);
        break;
    case CO_WAITING:
        status_ = CO_RUNNING;
        ctx_.swapToMe(mainCtx);
        break;
    default:
        fprintf(stderr, "Coroutine status error\n");
    }
}

void Coroutine::yield()
{
    assert(status_ == CO_RUNNING);
    isInCor = false;
    status_ = CO_WAITING;
    myProcess_->getContext()->swapToMe(&ctx_);
}

void Coroutine::runCFunc()
{
    cFunc_();
    status_ = CO_DEADED;
}