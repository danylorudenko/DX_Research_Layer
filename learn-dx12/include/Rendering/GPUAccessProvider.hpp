#pragma once

#include <pch.hpp>

#include <Rendering\GPUWorker.hpp>

class GPUAccessProvider
{
public:
    GPUAccessProvider(ID3D12Device* device);
    GPUAccessProvider(const ID3D12Device&) = delete;
    GPUAccessProvider(GPUAccessProvider&& rhs);

    GPUAccessProvider& operator=(const GPUAccessProvider&) = delete;
    GPUAccessProvider& operator=(GPUAccessProvider&&);



private:
    template<GPU_WORKER_TYPE TYPE> GPUWorker& Worker();
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_DIRECT>() { return renderingWorker_; }
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_COPY>() { return copyWorker_; }
    template<> GPUWorker& Worker<GPU_WORKER_TYPE_COMPUTE>() { return computeWorker_; }

private:
    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};