#ifndef TINYCOAPI_H
#define TINYCOAPI_H

#include <functional>
#include "Parameter.h"

namespace toyco
{

typedef std::function<void ()> CoFunc;

void go(CoFunc func, int stackSize = Parameter::COR_STACK_SIZE);
void scheJoin();

}; // namespace toyco

#endif

