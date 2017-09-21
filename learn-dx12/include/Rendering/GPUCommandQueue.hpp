#pragma once

#include <pch.hpp>

#include <Rendering\GPUFence.hpp>

class GPUCommandQueue
{
public:
    GPUCommandQueue(ID3D12Device* device);
    GPUCommandQueue(const GPUCommandQueue&) = delete;
    GPUCommandQueue(GPUCommandQueue&& rhs);

    GPUCommandQueue& operator=(const GPUCommandQueue&) = delete;
    GPUCommandQueue& operator=(GPUCommandQueue&& rhs);


    ID3D12CommandQueue* Get() { return commandQueue_.Get(); }

    void ExecuteCommandLists(ID3D12CommandList* commandLists, std::size_t count = 1);
    void WaitForFence();

    UINT64 FenceCompletedValue() const;
    UINT64 FenceTargetValue() const;

private:
    Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_ = nullptr;

    GPUFence fence_;
};