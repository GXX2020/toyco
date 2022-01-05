#include <iostream>
#include "base/Mutex.h"
#include "base/Condition.h"
#include "base/Thread.h"
#include <unistd.h>

using namespace BaseLib;


// 测试互斥锁
int globalArg = 0;
MutexLock mutex;

void thFun1()
{
    MutexLockGuard gu(mutex);
    std::cout << "in th1" << std::endl;
    std::cout << "pre add: globalArg = " << globalArg << std::endl;
    sleep(1);
    globalArg += 100;
    std::cout << "after add: globalArg = " << globalArg << std::endl;
}

void thFun2()
{
    MutexLockGuard gu(mutex);
    std::cout << "in th2" << std::endl;
    std::cout << "pre add: globalArg = " << globalArg << std::endl;
    sleep(1);
    globalArg += 100;
    std::cout << "after add: globalArg = " << globalArg << std::endl;
}


//条件变量测试
int count = 0;
MutexLock mutex1;
Condition cond(mutex1);

void productor()
{
    int time = 100;
    while(time--)
    {
        MutexLockGuard gu(mutex1);
        count++;
        std::cout << "produce 1, now cout = " << count << std::endl;
        cond.notify();
    }
}

void consumer1()
{
    int time = 50;
    while (time--)
    {
        MutexLockGuard gu(mutex1);
        while (count <= 0)
        {
        cond.wait();
        }
        count--;
        std::cout << "consume 1, now cout = " << count << std::endl;
    }
}

void consumer2()
{
    int time = 50;
    while (time--)
    {
        MutexLockGuard gu(mutex1);
        while (count <= 0)
        {
        cond.wait();
        }
        count--;
        std::cout << "consume 2, now cout = " << count << std::endl;
    }
}

int main()
{
    // 测试互斥锁
    // Thread th1(thFun1);
    // Thread th2(thFun2);
    // th1.start();
    // th2.start();
    // th1.join();
    // th2.join();

    // 条件变量测试
    Thread proTh(productor);
    Thread conTh1(consumer1);
    Thread conTh2(consumer2);
    proTh.start();
    conTh1.start();
    conTh2.start();
    proTh.join();
    conTh1.join();
    conTh2.join();
    return 0;
}