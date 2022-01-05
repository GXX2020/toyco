#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include <stdint.h>
#include <sys/time.h>
#include <string>
#include "base/CopyAble.h"

namespace BaseLib
{

class TimeStamp : public CopyAble
{
public:
    TimeStamp()
     : microSecondSinceEpoch_(0) 
    {}

    explicit TimeStamp(int64_t microSecondSinceEpoch)
     : microSecondSinceEpoch_(microSecondSinceEpoch)
    {}
    
    inline time_t secondSinceEpoch() const
    { return static_cast<time_t>(microSecondSinceEpoch_ / kMicroSecondPerSecond_); }
    inline int64_t microSecondSinceEpoch() const
    { return microSecondSinceEpoch_; }

    std::string toString() const;

    bool vaild() const
    { return microSecondSinceEpoch_ > 0; }

    static TimeStamp now();

    static const int kMicroSecondPerSecond_ = 1000 * 1000;  // 每秒为10^6毫秒

private:
    int64_t microSecondSinceEpoch_;                             // 1970年到目前的毫秒数
};

inline bool operator == (const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSecondSinceEpoch() == rhs.microSecondSinceEpoch();
}

inline bool operator <= (const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSecondSinceEpoch() <= rhs.microSecondSinceEpoch();
}

inline bool operator < (const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSecondSinceEpoch() < rhs.microSecondSinceEpoch();
}

inline bool operator >= (const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSecondSinceEpoch() >= rhs.microSecondSinceEpoch();
}

inline bool operator > (const TimeStamp& lhs, const TimeStamp& rhs)
{
    return lhs.microSecondSinceEpoch() > rhs.microSecondSinceEpoch();
}

inline TimeStamp addTimeBySecond(const TimeStamp& timeStamp, double second)
{
    int64_t delta = static_cast<int64_t>(second * TimeStamp::kMicroSecondPerSecond_);
    return TimeStamp(timeStamp.microSecondSinceEpoch() + delta);
}


} // namespace BaseLib

#endif