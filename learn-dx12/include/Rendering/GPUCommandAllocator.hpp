#pragma once

#include <pch.hpp>

#include <Rendering\GPUFence.hpp>

class GPUCommandAllocator
{
public:
    GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
    GPUCommandAllocator(const GPUCommandAllocator&) = delete;
    GPUCommandAllocator(GPUCommandAllocator&& rhs);

    GPUCommandAllocator& operator=(const GPUCommandAllocator&) = delete;
    GPUCommandAllocator& operator=(GPUCommandAllocator&& rhs);

    ~GPUCommandAllocator();

    void Reset() { commandAllocator_->Reset(); }

    GPUFence& Fence() { return fence_; }

    UINT64 FenceTargetValue() const { return fenceTargetValue_; }
    UINT64 FenceCompletedValue() const { return fence_.CompletedValue(); }
    void SetFenceTargetValue(UINT64 targetValue) { fenceTargetValue_ = targetValue; }

private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
    GPUFence fence_;
    UINT64 fenceTargetValue_ = 0U;
};