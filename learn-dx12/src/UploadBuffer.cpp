#pragma once

#include <Data\UploadBuffer.hpp>

UploadBuffer::UploadBuffer(
    ID3D12Device& device,
    void* data,
    std::size_t elementSize,
    bool isConstBuffer,
    std::size_t elementCount)
{
    if (isConstBuffer) {
        elementSize_ = (elementSize_ + 255) & ~255;
    }

    HRESULT result = device.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(elementSize_ * elementCount),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer_));
    ThrowIfFailed(result);

    result = uploadBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
    ThrowIfFailed(result);
}

BYTE* UploadBuffer::MappedData()
{
    return mappedData_;
}

std::size_t UploadBuffer::BufferLength() const
{
    return elementSize_ * elementCount_;
}
