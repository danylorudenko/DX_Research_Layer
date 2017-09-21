#pragma once

#include <pch.hpp>

#include <Rendering\GPUWorker.hpp>

class GPUAccessProvider
{
public:


private:
    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};