#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandAllocator.hpp>

class GPUCommandQueue
{
public:
    GPUCommandQueue();
    GPUCommandQueue(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type, std::size_t allocatorCount);
    GPUCommandQueue(const GPUCommandQueue&) = delete;
    GPUCommandQueue(GPUCommandQueue&& rhs);

    GPUCommandQueue& operator=(const GPUCommandQueue&) = delete;
    GPUCommandQueue& operator=(GPUCommandQueue&& rhs);

    ID3D12CommandQueue* Get() { return commandQueue_.Get(); }
    
    void ExecuteCommandLists(ID3D12CommandList* commandLists, std::size_t count = 1);
    GPUCommandAllocator& CurrentAlloc();
    GPUCommandAllocator& ProvideNextAlloc();

private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

    std::vector<GPUCommandAllocator> commandAllocators_;
    std::size_t currentAllocator_ = 0ULL;
};