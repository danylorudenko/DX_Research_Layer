#pragma once

#include <pch.hpp>

class GPUFence
{
public:
    GPUFence(ID3D12Device* device);
    GPUFence(const GPUFence&) = delete;
    GPUFence(GPUFence&& rhs);

    GPUFence& operator=(const GPUFence&) = delete;
    GPUFence& operator=(GPUFence&& rhs);

    ID3D12Fence* Get() const { return fence_.Get(); }

    void SetTargetValue(UINT64 targetValue);
    UINT64 TargetValue() const;

    void WaitForQueue(ID3D12CommandQueue* queue);

private:
    Microsoft::WRL::ComPtr<ID3D12Fence> fence_ = nullptr;
    HANDLE event_ = nullptr;
    UINT64 targetValue_ = 0U;
};