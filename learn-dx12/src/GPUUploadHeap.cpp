#pragma once

#include <Data\GPUUploadHeap.hpp>

GPUUploadHeap::GPUUploadHeap(
    ID3D12Device* device,
    void const* data,
    std::size_t elementSize,
    std::size_t elementCount,
    bool isConstBuffer) :

    elementSize_(elementSize),
    elementCount_(elementCount)
{
    if (isConstBuffer) {
        elementSize_ = (elementSize_ + 255) & ~255;
    }

    HRESULT result = device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(elementSize_ * elementCount),
        D3D12_RESOURCE_STATE_GENERIC_READ,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer_));
    ThrowIfFailed(result);

    result = uploadBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));

    if (data != nullptr) {
        memcpy(mappedData_, data, elementSize_ * elementCount_);
    }
    
    ThrowIfFailed(result);
}

BYTE* GPUUploadHeap::MappedData()
{
    return mappedData_;
}

std::size_t GPUUploadHeap::BufferLength() const
{
    return elementSize_ * elementCount_;
}
