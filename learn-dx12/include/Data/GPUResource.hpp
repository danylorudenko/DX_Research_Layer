#pragma once

#include <pch.hpp>

class GPUResource
{
public:
    GPUResource(ID3D12Device* device, UINT64 size);
    GPUResource(const GPUResource&) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(const GPUResource&) = delete;
    GPUResource& operator=(GPUResource&& rhs);
    
    ID3D12Resource* Get() const { return resource_.Get(); }

private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    UINT64 size_;
    D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = 0;
    D3D12_RESOURCE_STATES currentState_ = D3D12_RESOURCE_STATE_COMMON;
    D3D12_RESOURCE_STATES targetState_ = D3D12_RESOURCE_STATE_COMMON;
};