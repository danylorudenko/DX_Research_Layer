#pragma once

#include <Data\GPUUploadHeap.hpp>

GPUUploadHeap::GPUUploadHeap() = default;

GPUUploadHeap::GPUUploadHeap(
    ID3D12Device* device,
    void const* data,
    std::size_t size,
    bool isConstBuffer)
{
    size_ = size;
    if (isConstBuffer) {
        size_ = (size_ + 255) & ~255;
    }

    HRESULT result = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(size_),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&resource_));
    ThrowIfFailed(result);

    gpuAddress_ = resource_->GetGPUVirtualAddress();
    state_ = D3D12_RESOURCE_STATE_GENERIC_READ;

    if (data != nullptr) {
        BYTE* mappedHeap = nullptr;
        Map(reinterpret_cast<void**>(&mappedHeap), nullptr);
        memcpy(mappedHeap, data, size);
        Unmap(nullptr);
    }
}

GPUUploadHeap::GPUUploadHeap(GPUUploadHeap&& rhs)
{
    GPUResource::operator=(std::move(rhs));

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUUploadHeap& GPUUploadHeap::operator=(GPUUploadHeap&& rhs)
{
    GPUResource::operator=(std::move(rhs));

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}
