#pragma once

#include <pch.hpp>

namespace DXRL
{

class GPUFence
{
public:
    GPUFence();
    GPUFence(ID3D12Device* device);
    GPUFence(GPUFence const&) = delete;
    GPUFence(GPUFence&& rhs);

    GPUFence& operator=(GPUFence const&) = delete;
    GPUFence& operator=(GPUFence&& rhs);

    ~GPUFence();

    ID3D12Fence* Get() const { return fence_.Get(); }

    UINT64 CompletedValue() const { return fence_->GetCompletedValue(); }

    void WaitForValue(UINT64 value);

private:
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
    HANDLE event_ = nullptr;
};

}