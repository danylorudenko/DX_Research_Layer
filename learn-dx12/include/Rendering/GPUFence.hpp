#pragma once

#include <pch.hpp>

#include <Rendering\GPUCommandQueue.hpp>
#include <Rendering\GPUCommandAllocator.hpp>

class GPUFence
{
public:
    GPUFence(ID3D12Device* device);
    GPUFence(const GPUFence&) = delete;
    GPUFence(GPUFence&& rhs);

    GPUFence& operator=(const GPUFence&) = delete;
    GPUFence& operator=(GPUFence&& rhs);

    ~GPUFence();

    ID3D12Fence* Get() const { return fence_.Get(); }

    UINT64 CompletedValue() const { return fence_->GetCompletedValue(); };

    void WaitForQueue(GPUCommandQueue& queue, GPUCommandAllocator& allocatorInUse);

private:
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
    HANDLE event_ = nullptr;
};