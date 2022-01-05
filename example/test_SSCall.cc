#include "tinyCoApi.h"
#include <iostream>
#include <unistd.h>
#include "base/Thread.h"
#include "SSCall.h"
#include <fcntl.h>

using namespace toyco;

int pp[2];

void func1()
{
    char str[128];
    fprintf(stdout, "before read in func1\n");
    int n = SSCall::read(pp[0], str, 128);
    fprintf(stdout, "after read in func1\n");
    str[n] = 0;
    fprintf(stdout, str, n);
    fprintf(stdout, "\n");
}

void func2()
{
    char str[6]= "hello";
    fprintf(stdout, "before write in func2\n");
    int n = SSCall::write(pp[1], (void*)str, 5);
    fprintf(stdout, "after write in func2\n");
    fprintf(stdout, "send success\n");
}

int main()
{
    SSCall::makePipe(pp);
    toyco::go(func1, 32*1024);
    toyco::go(func2, 32*1024);
    toyco::scheJoin();
    return 0;
}