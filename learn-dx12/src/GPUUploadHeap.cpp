#pragma once

#include <Data\GPUUploadHeap.hpp>

GPUUploadHeap::GPUUploadHeap(
    ID3D12Device* device,
    void const* data,
    std::size_t size,
    bool isConstBuffer) :

    size_(size)
{
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

    ThrowIfFailed(resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_)));

    if (data != nullptr) {
        memcpy(mappedData_, data, size_);
    }
}

GPUUploadHeap::GPUUploadHeap(GPUUploadHeap&& rhs) :
    size_(rhs.size_)
{
    resource_ = std::move(rhs.resource_);
    mappedData_ = rhs.mappedData_;

    ZeroMemory(&rhs, sizeof(rhs));
}

GPUUploadHeap& GPUUploadHeap::operator=(GPUUploadHeap&& rhs)
{
    resource_ = std::move(rhs.resource_);
    mappedData_ = rhs.mappedData_;
    size_ = rhs.size_;

    ZeroMemory(&rhs, sizeof(rhs));
    return *this;
}

GPUUploadHeap::~GPUUploadHeap()
{
    resource_->Unmap(0, nullptr);
    mappedData_ = nullptr;
}

BYTE* GPUUploadHeap::MappedData()
{
    if (mappedData_ == nullptr) {
        resource_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
    }
    return mappedData_;
}

std::size_t GPUUploadHeap::Size() const
{
    return size_;
}
