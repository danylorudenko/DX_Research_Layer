#pragma once

#include <pch.hpp>

struct GPUResourceDesc
{
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceHandle = nullptr;
    D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON;
    std::size_t size = 0;
    std::size_t capcatity = 0;
};

class GPUResource
{
public:
    GPUResource();
    GPUResource(GPUResourceDesc const* resourceDesc);

    GPUResource(GPUResource const& rhs) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(GPUResource const& rhs) = delete;
    GPUResource& operator=(GPUResource&& rhs);
    
    ID3D12Resource* Get();
    Microsoft::WRL::ComPtr<ID3D12Resource>& GetHandle();

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const;

    D3D12_RESOURCE_STATES GetVirtualState() const;
    D3D12_RESOURCE_STATES SetVirtualState(D3D12_RESOURCE_STATES state);



private:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceHandle_;
    D3D12_RESOURCE_STATES virtualState_;
    std::size_t size_;
    std::size_t capacity_;
};