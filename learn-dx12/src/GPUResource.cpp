#include <Rendering\Data\GPUResource.hpp>

GPUResource::GPUResource() = default;

GPUResource::GPUResource(ID3D12Device* device, UINT64 size, D3D12_RESOURCE_STATES state) :
    state_(state),
    size_(size),
    capacity_(size)
{
    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
            D3D12_HEAP_FLAG_NONE,
            &CD3DX12_RESOURCE_DESC::Buffer(capacity_),
            state_,
            nullptr,
            IID_PPV_ARGS(&resource_)
        );
        ThrowIfFailed(result);
    }

    gpuAddress_ = resource_->GetGPUVirtualAddress();
}

GPUResource::GPUResource(GPUResource&& rhs) :
    size_(rhs.size_), capacity_(rhs.capacity_)
{
    resource_ = std::move(rhs.resource_);
    gpuAddress_ = rhs.gpuAddress_;
    state_ = rhs.state_;
}

GPUResource& GPUResource::operator=(GPUResource&& rhs)
{
    resource_ = std::move(rhs.resource_);
    size_ = rhs.size_;
    gpuAddress_ = rhs.gpuAddress_;
    state_ = rhs.state_;

    return *this;
}

void GPUResource::CreateResource(ID3D12Device* device, std::size_t size, D3D12_RESOURCE_STATES initialState)
{
    resource_ = nullptr;
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size),
        initialState,
        nullptr,
        IID_PPV_ARGS(&resource_));
    state_ = initialState;

    size_ = 0;
    capacity_ = size;

    gpuAddress_ = resource_->GetGPUVirtualAddress();
}

void GPUResource::UpdateData(ID3D12GraphicsCommandList* commandList, std::size_t offsetInDest, GPUResource& src, std::size_t offsetInSrc, std::size_t numBytes)
{
    commandList->CopyBufferRegion(resource_.Get(), offsetInDest, src.Get(), offsetInSrc, numBytes);
}

void GPUResource::Transition(ID3D12GraphicsCommandList* commandList, D3D12_RESOURCE_STATES state)
{
    commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(resource_.Get(), state_, state));
    state_ = state;
}
