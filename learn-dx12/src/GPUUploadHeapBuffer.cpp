#pragma once

#include <Rendering\Data\Resource\GPUUploadHeapBuffer.hpp>

namespace
{
    template < std::size_t ALIGNMENT >
    inline std::size_t constexpr alignTo(std::size_t size)
    {
        auto constexpr alignment = ALIGNMENT - 1;
        return (size + alignment) & ~alignment;
    }
}

GPUUploadHeapBuffer::GPUUploadHeapBuffer() = default;

GPUUploadHeapBuffer::GPUUploadHeapBuffer(ID3D12Device* device, void const* data, D3D12_RESOURCE_DESC const* resourceDesc, std::size_t size, bool isConstBuffer) :
    GPUBuffer{}, isConstBuffer_(isConstBuffer)
{
    if (isConstBuffer_) {
        size = alignTo<256>(size);
    }
    bufferSize_ = size;

    {
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resourceHandle_));
        ThrowIfFailed(result);
    }

    void* gpuBufferPtr;

    D3D12_RANGE range{};
    range.Begin = 0;
    range.End = bufferSize_ - 1;
    Map(&gpuBufferPtr, &range);

    std::memcpy(gpuBufferPtr, data, size);

    Unmap(&range);
}

GPUUploadHeapBuffer::GPUUploadHeapBuffer(GPUUploadHeapBuffer&& rhs) :
    GPUBuffer{ std::move(rhs) }, isConstBuffer_(rhs.isConstBuffer_)
{

}

GPUUploadHeapBuffer& GPUUploadHeapBuffer::operator=(GPUUploadHeapBuffer&& rhs)
{
    GPUBuffer::operator=(std::move(rhs));
    isConstBuffer_ = rhs.isConstBuffer_;
    return *this;
}
