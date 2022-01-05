#ifndef SINGLETON_H
#define SINGLETON_H

#include <pthread.h>
#include <assert.h>
#include "base/NoCopyAble.h"
#include <stdlib.h>

namespace BaseLib
{

template<typename T>
class Singleton : public NoCopyAble
{
public:
    static T& instance()
    {
        pthread_once(&ponce_, &Singleton::init);
        assert(value_ != nullptr);
        return *value_;
    }
private:
    static void init()
    {
        value_ = new T();
        ::atexit(destory);
    }
    static void destory()
    {
        delete value_;
        value_ = nullptr;
    }
    static pthread_once_t ponce_;
    static T* value_;
};

template<typename T>
pthread_once_t Singleton<T>::ponce_ = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::value_ = nullptr;

} // namespace BaseLib

#endif