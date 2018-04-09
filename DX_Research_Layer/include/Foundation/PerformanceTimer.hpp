#pragma once

namespace DXRL
{

class PerformanceTimer
{
public:
    PerformanceTimer();

    float TotalTime() const;
    float DeltaTime() const;

    void Reset();
    void Tick();

private:
    double secondsPerCount_;
    double deltaTime_;

    __int64 baseTime_;
    __int64 prevTime_;
    __int64 currTime_;

};

}