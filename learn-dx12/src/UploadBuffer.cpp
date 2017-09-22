#pragma once

#include <Data\UploadBuffer.hpp>

UploadBuffer::UploadBuffer(
    ID3D12Device& device,
    void* data,
    std::size_t elementSize,
    bool isConstBuffer,
    std::size_t elementCount) :

    elementSize_(elementSize),
    elementCount_(elementCount)
{
    if (isConstBuffer) {
        elementSize_ = (elementSize_ + 255) & ~255;
    }

    HRESULT result = device.CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(elementSize_ * elementCount),
        D3D12_RESOURCE_STATE_GENERIC_READ,
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

ComPtr<ID3D12Resource> UploadBuffer::GenerateCommonResource(ID3D12GraphicsCommandList* helperCommandList) const
{
    ID3D12Device* device = nullptr;
    HRESULT result = uploadBuffer_->GetDevice(IID_PPV_ARGS(&device)); ThrowIfFailed(result);

    ComPtr<ID3D12Resource> commonResource = nullptr;
    device->CreateCommittedResource(
        &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
        D3D12_HEAP_FLAG_NONE,
        &CD3DX12_RESOURCE_DESC::Buffer(BufferLength()),
        D3D12_RESOURCE_STATE_COPY_DEST,
        nullptr,
        IID_PPV_ARGS(&commonResource));

    device->Release();

    // ===============

    D3D12_SUBRESOURCE_DATA subData;

    helperCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(uploadBuffer_.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COPY_SOURCE));
    helperCommandList->CopyResource(commonResource.Get(), uploadBuffer_.Get());

    D3D12_RESOURCE_BARRIER barriers[] = {
        CD3DX12_RESOURCE_BARRIER::Transition(uploadBuffer_.Get(), D3D12_RESOURCE_STATE_COPY_SOURCE, D3D12_RESOURCE_STATE_COPY_DEST),
        CD3DX12_RESOURCE_BARRIER::Transition(commonResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON)
    };
    helperCommandList->ResourceBarrier(2, barriers);

    return commonResource;
}
