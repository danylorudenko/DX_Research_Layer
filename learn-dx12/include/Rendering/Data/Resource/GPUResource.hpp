#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>

class GPUEngine;

class GPUResource
{
public:
    GPUResource();
    GPUResource(ID3D12Device* device, D3D12_RESOURCE_DESC const* resourceDesc, D3D12_RESOURCE_STATES initialState);
    
    GPUResource(GPUResource const&) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(GPUResource const&) = delete;
    GPUResource& operator=(GPUResource&& rhs);

    ID3D12Resource* Get();
    ID3D12Resource* operator->();

    D3D12_RESOURCE_STATES VirtualState() const;

    D3D12_GPU_VIRTUAL_ADDRESS GPUVirtualAddress();

    D3D12_RESOURCE_STATES Transition(GPUEngine& executionEngine, D3D12_RESOURCE_STATES targetState);

    void UpdateData(GPUEngine& executionEngine, std::size_t offsetInDestination, GPUResource& src, std::size_t numBytes, std::size_t offsetInSrc = 0);
    
protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourceHandle_;
    D3D12_RESOURCE_STATES virtualState_;
    D3D12_GPU_VIRTUAL_ADDRESS gpuAddress_;
    
};