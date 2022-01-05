#ifndef PROCESSORSELECTOR_H
#define PROCESSORSELECTOR_H

#include <vector>
#include "base/NoCopyAble.h"

namespace toyco
{

class Processor;

class ProcessorSelector : public BaseLib::NoCopyAble
{
public:
    ProcessorSelector(const std::vector<Processor*>& proSet);
    Processor* getBestProcessor();
private:
    const std::vector<Processor*>& proSet_;
};

} // namespace toyco

#endif