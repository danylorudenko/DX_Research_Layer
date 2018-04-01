#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine\GPUFence.hpp>

namespace DXRL
{

class GPUCommandAllocator
{
public:
    GPUCommandAllocator();
    GPUCommandAllocator(ID3D12Device* device, D3D12_COMMAND_LIST_TYPE type);
    GPUCommandAllocator(GPUCommandAllocator const&) = delete;
    GPUCommandAllocator(GPUCommandAllocator&& rhs);

    GPUCommandAllocator& operator=(GPUCommandAllocator const&) = delete;
    GPUCommandAllocator& operator=(GPUCommandAllocator&& rhs);

    ID3D12CommandAllocator* Get() const { return commandAllocator_.Get(); }

    void Reset() { commandAllocator_->Reset(); }

    UINT64 FenceTargetValue() const { return fenceTargetValue_; }
    void SetFenceTargetValue(UINT64 targetValue) { fenceTargetValue_ = targetValue; }

private:
    Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_ = nullptr;
    UINT64 fenceTargetValue_ = 0U;
};

}