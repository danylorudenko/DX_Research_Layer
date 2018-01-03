#pragma once

#include <pch.hpp>

class GPUResource
{
public:
    GPUResource();
    GPUResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& resourcePtr, D3D12_RESOURCE_STATES state, wchar_t const* name);

    GPUResource(GPUResource const&) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(GPUResource const&) = delete;
    GPUResource& operator=(GPUResource&& rhs);

    D3D12_RESOURCE_STATES State() const;
    ID3D12Resource* GetPtr() const;
    Microsoft::WRL::ComPtr<ID3D12Resource> const& Get() const;

    void PrepareTransition(D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_BARRIER& transitionDesc);
    
protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourcePtr_;
    D3D12_RESOURCE_STATES state_;
};