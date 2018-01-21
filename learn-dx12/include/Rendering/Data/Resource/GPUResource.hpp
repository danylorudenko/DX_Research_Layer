#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine.hpp>

class GPUResource
{
public:
    GPUResource();
    GPUResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& resourcePtr, D3D12_RESOURCE_STATES state, wchar_t const* name = L"GPUResource");

    GPUResource(GPUResource const&) = delete;
    GPUResource(GPUResource&& rhs);

    GPUResource& operator=(GPUResource const&) = delete;
    GPUResource& operator=(GPUResource&& rhs);

    D3D12_RESOURCE_STATES State() const;
    ID3D12Resource* GetPtr() const;
    Microsoft::WRL::ComPtr<ID3D12Resource> const& Get() const;

    void Transition(GPUEngine& executionEngine, D3D12_RESOURCE_STATES targetState);
    void PrepareTransition(D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_BARRIER& transitionDesc);

    void UpdateData(GPUEngine& executionEngine, GPUResource const& source, std::size_t offset, std::size_t size);

    virtual ~GPUResource();
    
protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourcePtr_;
    D3D12_RESOURCE_STATES state_;
};