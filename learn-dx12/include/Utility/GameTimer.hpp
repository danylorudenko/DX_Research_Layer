#pragma once

class GameTimer
{
public:
    GameTimer();

    float GameTime() const;
    float TotalTime() const;
    float DeltaTime() const;

    void Reset();
    void Start();
    void Stop();
    void Tick();

private:
    double secondsPerCount_;
    double deltaTime_;

    __int64 baseTime_;
    __int64 pausedTime_;
    __int64 stopTime_;
    __int64 prevTime_;
    __int64 currTime_;

    bool stopped_;
};