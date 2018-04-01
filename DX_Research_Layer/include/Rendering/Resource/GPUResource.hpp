#pragma once

#include <pch.hpp>

#include <Rendering\GPUEngine\GPUEngine.hpp>

namespace DXRL
{

class GPUResource
{
public:

    DXRL_DEFINE_UNCOPYABLE_MOVABLE_DEFAULT(GPUResource)
    ~GPUResource() = default;
    
    GPUResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& resourcePtr, D3D12_RESOURCE_STATES state, wchar_t const* name = L"GPUResource");
    
    D3D12_RESOURCE_STATES State() const;
    ID3D12Resource* GetPtr() const;
    Microsoft::WRL::ComPtr<ID3D12Resource> const& Get() const;

    void Transition(GPUEngine& executionEngine, D3D12_RESOURCE_STATES targetState);
    void PrepareTransition(D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_BARRIER& transitionDesc);

    void UpdateData(GPUEngine& executionEngine, GPUResource const& source, std::size_t offset, std::size_t size);

protected:
    Microsoft::WRL::ComPtr<ID3D12Resource> resourcePtr_;
    D3D12_RESOURCE_STATES state_;
};

}