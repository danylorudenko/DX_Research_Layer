#pragma once

#include <pch.hpp>

class GPUResource
{
public:
    GPUResource();
    GPUResource(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_STATES state = D3D12_RESOURCE_STATE_COMMON);
    GPUResource(const GPUResource&) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(const GPUResource&) = delete;
    GPUResource& operator=(GPUResource&& rhs);
    
    ID3D12Resource* Get() const { return resource_.Get(); }
    UINT64 Size() const { return size_; };

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const { return gpuAddress_; }

    void Map(void** dest, D3D12_RANGE* range) { ThrowIfFailed(resource_->Map(0, range, dest)); }
    void Unmap(D3D12_RANGE* range) { resource_->Unmap(0, range); }

    void Transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state);
    D3D12_RESOURCE_STATES State() const { return state_; }

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    UINT64 size_;
    D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = 0;
    D3D12_RESOURCE_STATES state_ = D3D12_RESOURCE_STATE_COMMON;
};