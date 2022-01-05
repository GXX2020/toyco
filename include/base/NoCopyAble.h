#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

namespace BaseLib
{

class NoCopyAble
{
public:
    NoCopyAble(const NoCopyAble&) = delete;
    void operator = (const NoCopyAble&) = delete;
protected:
    NoCopyAble() = default;
    ~NoCopyAble() = default;
};

}

#endif