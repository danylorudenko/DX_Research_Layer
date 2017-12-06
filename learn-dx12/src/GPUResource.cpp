#include <Rendering\Data\GPUResource.hpp>

#include <Rendering\GPUEngine.hpp>

GPUResource::GPUResource() :
    resourceHandle_{ nullptr }, virtualState_{ (D3D12_RESOURCE_STATES)-1 }, gpuAddress_{ (D3D12_GPU_VIRTUAL_ADDRESS)0 }
{ }

GPUResource::GPUResource(ID3D12Device* device, D3D12_RESOURCE_DESC const* resourceDesc, D3D12_RESOURCE_STATES initialState)
{
    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            initialState,
            nullptr,
            IID_PPV_ARGS(&resourceHandle_));
        ThrowIfFailed(result);
    }
    virtualState_ = initialState;
}

GPUResource::GPUResource(GPUResource&& rhs) = default;

GPUResource& GPUResource::operator=(GPUResource&& rhs) = default;

D3D12_RESOURCE_STATES GPUResource::VirtualState() const
{
    return virtualState_;
}

D3D12_GPU_VIRTUAL_ADDRESS GPUResource::GPUVirtualAddress()
{
    return gpuAddress_ != 0
        ? gpuAddress_
        : gpuAddress_ = resourceHandle_->GetGPUVirtualAddress();
}

D3D12_RESOURCE_STATES GPUResource::Transition(GPUEngine& executionEngine, D3D12_RESOURCE_STATES targetState)
{
    auto const prevState = virtualState_;
    executionEngine.Commit().ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resourceHandle_.Get(), prevState, targetState));
    virtualState_ = targetState;

    return prevState;
}

ID3D12Resource* GPUResource::Get()
{
    return resourceHandle_.Get();
}

ID3D12Resource* GPUResource::operator->()
{
    return resourceHandle_.Get();
}