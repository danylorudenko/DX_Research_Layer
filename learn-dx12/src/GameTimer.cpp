#include <pch.hpp>

#include <Utility\GameTimer.hpp>

GameTimer::GameTimer() :
    deltaTime_(-1.0),
    baseTime_(0),
    pausedTime_(0),
    stopTime_(0),
    prevTime_(0),
    currTime_(0)
{
    __int64 countsPerSecond;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSecond);
    secondsPerCount_ = 1.0 / static_cast<double>(countsPerSecond);
}

float GameTimer::GameTime() const
{
    return (currTime_ - baseTime_) * secondsPerCount_;
}

float GameTimer::TotalTime() const
{
    return ((currTime_ + pausedTime_) - baseTime_) * secondsPerCount_;
}

void GameTimer::Reset()
{
    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

    currTime_ = currentTime;
    prevTime_ = currentTime;
    stopTime_ = 0;
    stopped_ = false;
}

float GameTimer::DeltaTime() const
{
    return deltaTime_;
}

void GameTimer::Tick()
{
    if (stopped_) {
        deltaTime_ = 0.0f;
        return;
    }

    __int64 currentTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);
    currTime_ = currentTime;
    
    deltaTime_ = (currentTime - prevTime_) * secondsPerCount_;
    prevTime_ = currentTime;

    if (deltaTime_ < 0.0) {
        deltaTime_ = 0.0;
    }
}

void GameTimer::Stop()
{
    if (!stopped_) {
        __int64 currentTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&currentTime);

        stopTime_ = currentTime;
        stopped_ = true;
    }
}

void GameTimer::Start()
{
    if (stopped_) {
        __int64 startTime;
        QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

        pausedTime_ += startTime - stopTime_;
        prevTime_ = startTime;
        stopTime_ = 0;
        stopped_ = false;
    }
}