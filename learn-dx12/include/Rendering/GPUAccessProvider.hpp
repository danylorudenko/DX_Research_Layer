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

    GPUWorker& RenderingWorker() { return renderingWorker_; }
    GPUWorker& CopyWorker() { return copyWorker_; }
    GPUWorker& ComputeWorker() { return computeWorker_; }

private:
    GPUWorker renderingWorker_;
    GPUWorker copyWorker_;
    GPUWorker computeWorker_;
};