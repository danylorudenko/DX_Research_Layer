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
    UINT64 Capacity() const { return capacity_; }

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress() const { return gpuAddress_; }

    void CreateResource(ID3D12Device* device, std::size_t size, D3D12_RESOURCE_STATES initialState = D3D12_RESOURCE_STATE_COMMON);
    void UpdateData(ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUResource& src, std::size_t offsetInSrc, std::size_t numBytes);

    void Transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state);
    D3D12_RESOURCE_STATES State() const { return state_; }

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resource_;
    UINT64 size_ = 0;
    UINT64 capacity_ = 0;
    D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_ = 0;
    D3D12_RESOURCE_STATES state_ = D3D12_RESOURCE_STATE_COMMON;
};