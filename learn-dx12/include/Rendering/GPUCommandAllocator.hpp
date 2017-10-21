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

    ID3D12CommandAllocator* Get() const { return commandAllocator_.Get(); }

    void Reset() { commandAllocator_->Reset(); }

    UINT64 FenceTargetValue() const { return fenceTargetValue_; }
    void SetFenceTargetValue(UINT64 targetValue) { fenceTargetValue_ = targetValue; }

private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
    UINT64 fenceTargetValue_ = 0U;
};