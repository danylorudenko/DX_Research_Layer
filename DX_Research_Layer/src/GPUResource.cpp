#include <Rendering\Resource\GPUResource.hpp>

namespace DXRL
{

GPUResource::GPUResource(Microsoft::WRL::ComPtr<ID3D12Resource>&& resourcePtr, D3D12_RESOURCE_STATES initialState, wchar_t const* name) :
    resourcePtr_(resourcePtr), state_(initialState)
{
    resourcePtr_->SetName(name);
}

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

void GPUResource::Transition(GPUEngine& executionEngine, D3D12_RESOURCE_STATES targetState)
{
    executionEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(GetPtr(), state_, targetState));
    state_ = targetState;
}

void GPUResource::PrepareTransition(D3D12_RESOURCE_STATES targetState, D3D12_RESOURCE_BARRIER& transitionDesc)
{
    transitionDesc = CD3DX12_RESOURCE_BARRIER::Transition(GetPtr(), State(), targetState);
    state_ = targetState;
}

void GPUResource::UpdateData(GPUEngine& executionEngine, GPUResource const& source, std::size_t offset, std::size_t size)
{
    executionEngine.Commit().CopyBufferRegion(resourcePtr_.Get(), 0, source.GetPtr(), static_cast<UINT64>(offset), static_cast<UINT64>(size));
}

}