#pragma once

#include <Rendering\Data\Resource\GPUUploadHeapBuffer.hpp>

GPUUploadHeapBuffer::GPUUploadHeapBuffer() = default;

GPUUploadHeapBuffer::GPUUploadHeapBuffer(
    ID3D12Device* device,
    void const* data,
    D3D12_RESOURCE_DESC* resourceDesc,
    std::size_t size)
{
    framesCount_ = framesCount;
    size_ = size;
    if (isConstBuffer) {
        size_ = (size_ + 255) & ~255;
    }

    for (int i = 0; i < framesCount; i++) {
        resources_.push_back(Microsoft::WRL::ComPtr<ID3D12Resource>{nullptr});
        HRESULT result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resources_[i]));
        ThrowIfFailed(result);

        gpuAddresses_.push_back(resources_[i]->GetGPUVirtualAddress());
        states_.push_back(D3D12_RESOURCE_STATE_GENERIC_READ);

        if (data != nullptr) {
            BYTE* mappedHeap = nullptr;
            Map(i, reinterpret_cast<void**>(&mappedHeap), nullptr);
            memcpy(mappedHeap, data, size);
            Unmap(i, nullptr);
        }
    }
}

GPUUploadHeapBuffer::GPUUploadHeapBuffer(GPUUploadHeapBuffer&& rhs) :
    GPUResourceFrameSet(std::move(rhs))
{
}

GPUUploadHeapBuffer& GPUUploadHeapBuffer::operator=(GPUUploadHeapBuffer&& rhs)
{
    GPUResourceFrameSet::operator=(std::move(rhs));
    return *this;
}
