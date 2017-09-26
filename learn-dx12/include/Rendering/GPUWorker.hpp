#pragma once

#include <pch.hpp>

#include <memory>

#include <Rendering\GPUCommandList.hpp>
#include <Rendering\GPUCommandQueue.hpp>

enum GPU_WORKER_TYPE
{
    GPU_WORKER_TYPE_DIRECT = D3D12_COMMAND_LIST_TYPE_DIRECT,
    GPU_WORKER_TYPE_COPY = D3D12_COMMAND_LIST_TYPE_COPY,
    GPU_WORKER_TYPE_COMPUTE = D3D12_COMMAND_LIST_TYPE_COMPUTE
};

class GPUWorker
{
public:
    GPUWorker(ID3D12Device* device, GPU_WORKER_TYPE type);
    GPUWorker(const GPUWorker&) = delete;
    GPUWorker(GPUWorker&& rhs);

    GPUWorker& operator=(const GPUWorker&) = delete;
    GPUWorker& operator=(GPUWorker&& rhs);

    void Reset();
    void Finalize();

    bool Finalized() const { return finalized_; }

    ID3D12GraphicsCommandList& Commit() { return *commandList_->Get(); }
    ID3D12CommandQueue* CommandQueue() const { return commandQueue_->Get(); }


private:
    std::unique_ptr<GPUCommandList> commandList_ = nullptr;
    std::unique_ptr<GPUCommandQueue> commandQueue_ = nullptr;

    bool finalized_ = true;
};