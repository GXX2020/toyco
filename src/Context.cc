#include <assert.h>
#include <stdlib.h>
#include "Context.h"
#include <iostream>

using namespace toyco;

Context::~Context()
{
    if (stack_)
    {
        free(stack_);
    }
}

void Context::makeCurContext()
{
    ::getcontext(&ctx_);
}

void Context::makeContext(void (*func)(), Processor* p, Context* nextLink)
{
    if (stack_ == nullptr) 
    {
        stack_ = malloc(stackSize_);
        assert(stack_ != nullptr);
    }
    ::getcontext(&ctx_);
    ctx_.uc_stack.ss_sp = stack_;
    ctx_.uc_stack.ss_size = stackSize_;
    ctx_.uc_link = nextLink->getContextPtr();
    ::makecontext(&ctx_, func, 1, p);
}

void Context::swapToMe(Context* oldCtx)
{
    if (oldCtx == nullptr)
    {
        ::setcontext(&ctx_);
    }
    ::swapcontext(oldCtx->getContextPtr(), &ctx_);
}