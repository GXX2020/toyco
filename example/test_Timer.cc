#include "tinyCoApi.h"
#include <iostream>
#include <unistd.h>
#include "Context.h"
#include "base/Thread.h"
#include "SSCall.h"

using namespace toyco;

void func1()
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "i = " << i << " from func1" << std::endl;
        SSCall::sleep(3);
    }
}

void func2()
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "i = " << i << " from func2" << std::endl;
        SSCall::sleep(3);
    }
}

void func3()
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "i = " << i << " from func3" << std::endl;
        SSCall::sleep(3);
    }
}

void func4()
{
    for (int i = 1; i <= 5; i++)
    {
        std::cout << "i = " << i << " from func4" << std::endl;
        SSCall::sleep(3);
    }
}

int main()
{
    toyco::go(func1, 32*1024);
    toyco::go(func2, 32*1024);
    toyco::go(func3, 32*1024);
    toyco::go(func4, 32*1024);
    toyco::scheJoin();
    return 0;
}