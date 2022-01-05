#include "base/TimeStamp.h"
#include <stdio.h>

using namespace BaseLib;

std::string TimeStamp::toString() const
{
    char buf[64] = {0};
    time_t seconds = static_cast<time_t>(microSecondSinceEpoch_ / kMicroSecondPerSecond_);
    struct tm tm_time;
    gmtime_r(&seconds, &tm_time);
    int microseconds = static_cast<int>(microSecondSinceEpoch_ % kMicroSecondPerSecond_);
    snprintf(buf, sizeof(buf), "%4d%02d%02d %02d:%02d:%02d.%06d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec,
             microseconds);
    return buf;
}

TimeStamp TimeStamp::now()
{
    timeval tv;
    gettimeofday(&tv, NULL);
    int64_t second = tv.tv_sec;
    int64_t microSecond = tv.tv_usec;
    return TimeStamp(second * kMicroSecondPerSecond_ + microSecond);
}