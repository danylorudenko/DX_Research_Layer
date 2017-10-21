#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>

class GPUFutureOperation
{
public:
    GPUFutureOperation();

    void Reset();
    void SendToExecution();
    void Wait();

private:
    GPUEngine* executionEngine_;
};