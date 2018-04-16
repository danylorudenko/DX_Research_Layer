#include <pch.hpp>

#include <Foundation\PerformanceTimer.hpp>

namespace DXRL
{

PerformanceTimer::PerformanceTimer() :
    deltaTime_(-1.0),
    baseTime_(0),
    prevTime_(0),
    currTime_(0)
{
    __int64 countsPerSecond;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
    secondsPerCount_ = 1.0 / static_cast<double>(countsPerSecond);
}
float PerformanceTimer::TotalTime() const
{
    return static_cast<float>((currTime_ - baseTime_) * secondsPerCount_);
}

void PerformanceTimer::Reset()
{
    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

	baseTime_ = currentTime;
    currTime_ = currentTime;
    prevTime_ = currentTime;
}

float PerformanceTimer::DeltaTime() const
{
    return static_cast<float>(deltaTime_);
}

void PerformanceTimer::Tick()
{
    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    currTime_ = currentTime;
    
    deltaTime_ = (currentTime - prevTime_) * secondsPerCount_;
    prevTime_ = currentTime;

    if (deltaTime_ < 0.0) {
        deltaTime_ = 0.0;
    }
}

}