#include <Data\GPUResource.hpp>

GPUResource::GPUResource(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_STATES state) :
    state_(state),
    size_(size)
{
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size_),
        state_,
        nullptr,
        IID_PPV_ARGS(&resource_)
    ));

    gpuAddress_ = resource_->GetGPUVirtualAddress();
}

GPUResource::GPUResource(GPUResource&& rhs) :
    size_(rhs.size_)
{
    resource_ = std::move(rhs.resource_);
    gpuAddress_ = rhs.gpuAddress_;
    state_ = rhs.state_;

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUResource& GPUResource::operator=(GPUResource&& rhs)
{
    resource_ = std::move(rhs.resource_);
    size_ = rhs.size_;
    gpuAddress_ = rhs.gpuAddress_;
    state_ = rhs.state_;

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

void GPUResource::Transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state)
{
    commandList->ResourceBarrier(
        1, 
        &CD3DX12_RESOURCE_BARRIER::Transition(
            resource_.Get(),
            state_,
            state));
}
