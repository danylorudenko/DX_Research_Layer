#include <Rendering\Data\Resource\GPUResource.hpp>

GPUResource::GPUResource() = default;

GPUResource::GPUResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& resourcePtr, D3D12_RESOURCE_STATES initialState, wchar_t const* name) :
    resourcePtr_(resourcePtr), state_(initialState)
{
    AssignID3D12ResourceDebugName(name);
}

GPUResource::GPUResource(GPUResource&& rhs) = default;

GPUResource& GPUResource::operator=(GPUResource&& rhs) = default;

D3D12_RESOURCE_STATES GPUResource::State() const
{
    return state_;
}

Microsoft::WRL::ComPtr<ID3D12Resource> const& GPUResource::Get() const
{
    return resourcePtr_;
}

ID3D12Resource* GPUResource::GetPtr() const
{
    return resourcePtr_.Get();
}

void GPUResource::PrepareTransition(D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_BARRIER& transitionDesc)
{
    transitionDesc = CD3DX12_RESOURCE_BARRIER::Transition(GetPtr(), State(), targetState);
    state_ = targetState;
}