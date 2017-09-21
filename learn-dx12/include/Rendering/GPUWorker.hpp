#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandList.hpp>
#include <Rendering\GPUCommandQueue.hpp>

enum GPU_WORKER_TYPE
{
    GPU_WORKER_TYPE_DIRECT,
    GPU_WORKER_TYPE_COPY,
    GPU_WORKER_TYPE_COMPUTE
};

class GPUWorker
{
public:
    GPUWorker(ID3D12Device* device);
    GPUWorker(const GPUWorker&) = delete;
    GPUWorker(GPUWorker&& rhs);

    GPUWorker& operator=(const GPUWorker&) = delete;
    GPUWorker& operator=(GPUWorker&& rhs);

    void Begin();
    void End();
    ID3D12GraphicsCommandList& Commit();

    void Finalize();

private:
    GPUCommandList commandList_;
    GPUCommandQueue commandQueue_;
};