#include <Data\GPUResource.hpp>

GPUResource::GPUResource(ID3D12Device* device, UINT64 size) :
    size_(size)
{
    ThrowIfFailed(device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        targetState_,
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
    targetState_ = rhs.targetState_;

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUResource& GPUResource::operator=(GPUResource&& rhs)
{
    resource_ = std::move(rhs.resource_);
    size_ = rhs.size_;
    gpuAddress_ = rhs.gpuAddress_;
    targetState_ = rhs.targetState_;

    ZeroMemory(&rhs, sizeof(rhs));

    return *this;
}

