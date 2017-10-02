#pragma once

#include <pch.hpp>

#include <memory>

#include <Rendering\GPUCommandList.hpp>
#include <Rendering\GPUCommandQueue.hpp>

enum GPU_ENGINE_TYPE
{
    GPU_ENGINE_TYPE_DIRECT = D3D12_COMMAND_LIST_TYPE_DIRECT,
    GPU_ENGINE_TYPE_COPY = D3D12_COMMAND_LIST_TYPE_COPY,
    GPU_ENGINE_TYPE_COMPUTE = D3D12_COMMAND_LIST_TYPE_COMPUTE
};

class GPUEngine
{
public:
    GPUEngine(ID3D12Device* device, GPU_ENGINE_TYPE type);
    GPUEngine(const GPUEngine&) = delete;
    GPUEngine(GPUEngine&& rhs);

    GPUEngine& operator=(const GPUEngine&) = delete;
    GPUEngine& operator=(GPUEngine&& rhs);

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