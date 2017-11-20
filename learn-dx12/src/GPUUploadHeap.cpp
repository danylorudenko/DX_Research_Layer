#pragma once

#include <Rendering\Data\GPUUploadHeap.hpp>

GPUUploadHeap::GPUUploadHeap() = default;

GPUUploadHeap::GPUUploadHeap(
    int framesCount,
    ID3D12Device* device,
    void const* data,
    std::size_t size,
    D3D12_RESOURCE_DESC* resourceDesc,
    bool isConstBuffer)
{
    framesCount_ = framesCount;
    size_ = size;
    if (isConstBuffer) {
        size_ = (size_ + 255) & ~255;
    }

    for (int i = 0; i < framesCount; i++) {
        HRESULT result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&resources_[i]));
        ThrowIfFailed(result);

        gpuAddresses_[i] = resources_[i]->GetGPUVirtualAddress();
        states_[i] = D3D12_RESOURCE_STATE_GENERIC_READ;

        if (data != nullptr) {
            BYTE* mappedHeap = nullptr;
            Map(i, reinterpret_cast<void**>(&mappedHeap), nullptr);
            memcpy(mappedHeap, data, size);
            Unmap(i, nullptr);
        }
    }
}

GPUUploadHeap::GPUUploadHeap(GPUUploadHeap&& rhs) :
    GPUFrameResource(std::move(rhs))
{
}

GPUUploadHeap& GPUUploadHeap::operator=(GPUUploadHeap&& rhs)
{
    GPUFrameResource::operator=(std::move(rhs));
    return *this;
}
