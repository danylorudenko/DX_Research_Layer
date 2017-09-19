#pragma once

#include <Data\UploadBuffer.hpp>

UploadBuffer::UploadBuffer(
    ID3D12Device& device,
    void* data,
    std::size_t dataSize,
    bool isConstBuffer,
    std::size_t dataCount) : dataSize_(dataSize), dataCount_(dataCount)
{
    if (isConstBuffer) {
        dataSize_ = (dataSize_ + 255) & ~255;
    }

    HRESULT result = device.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(dataSize_ * dataCount),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&uploadBuffer_));
    ThrowIfFailed(result);

    result = uploadBuffer_->Map(0, nullptr, reinterpret_cast<void**>(&mappedData_));
    ThrowIfFailed(result);
}

ComPtr<ID3D12Resource> UploadBuffer::LoadToDefaultBuffer(ID3D12Device& device)
{
    ComPtr<ID3D12Resource> defaultBuffer;

    HRESULT result = device.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(dataSize_ * dataCount_),
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&defaultBuffer));

    D3D12_SUBRESOURCE_DATA subResourceData = {};
    subResourceData.pData = data_;
    subResourceData.RowPitch = dataSize_;
    subResourceData.SlicePitch = subResourceData.RowPitch;

    //UpdateSubresources<1>()
    return defaultBuffer;
}