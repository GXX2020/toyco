#include "ProcessorSelector.h"
#include "Processor.h"
#include <assert.h>
#include <iostream>

using namespace toyco;

ProcessorSelector::ProcessorSelector(const std::vector<Processor*>& proSet)
 : proSet_(proSet)
{}

Processor* ProcessorSelector::getBestProcessor()
{
    int n = proSet_.size();
    assert(n != 0);
    int bestProId = -1;
    for (int i = 0; i < n; i++)
    {
        if (bestProId == -1 || proSet_[bestProId]->getCorNum() > proSet_[i]->getCorNum())
        {
            bestProId = i;
        } 
    }
    assert(bestProId != -1);
    return proSet_[bestProId];
}
