#ifndef CONTEXT_H
#define CONTEXT_H

#include <ucontext.h>
#include "base/CopyAble.h"

namespace toyco
{

class Processor;
class Context : public BaseLib::CopyAble
{
public:
    Context(size_t stackSize = 0)
     : stack_(nullptr),
       stackSize_(stackSize)
    {}
    ~Context();

    void makeCurContext();
    void makeContext(void (*func)(), Processor* p, Context* nextLink);
    void swapToMe(Context* oldCtx);
    inline ucontext_t* getContextPtr()
    { return &ctx_; }

private:
    ucontext_t ctx_;
    void* stack_;
    size_t stackSize_;
};

} // namespace toyco

#endif