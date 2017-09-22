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

    GPUWorker& Worker() { return renderingWorker_; }
    GPUWorker& Worker(GPU_WORKER_TYPE type) 
    { switch (type) { case GPU_WORKER_TYPE_DIRECT: return renderingWorker_; case GPU_WORKER_TYPE_COPY: return copyWorker_; case GPU_WORKER_TYPE_COMPUTE: return computeWorker_; } }

    GPUWorker& RenderingWorker() { return renderingWorker_; }
    GPUWorker& CopyWorker() { return copyWorker_; }
    GPUWorker& ComputeWorker() { return computeWorker_; }

private:
    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};