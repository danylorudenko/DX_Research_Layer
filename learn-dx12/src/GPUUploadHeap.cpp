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
        Microsoft::WRL::ComPtr<ID3D12Resource> tempRef{ nullptr };
        auto const result = device->CreateCommittedResource(
            &CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
            D3D12_HEAP_FLAG_NONE,
            resourceDesc,
            D3D12_RESOURCE_STATE_GENERIC_READ,
            nullptr,
            IID_PPV_ARGS(&tempRef));
        ThrowIfFailed(result);

        resources_.push_back(tempRef);
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

GPUUploadHeap::GPUUploadHeap(GPUUploadHeap&& rhs) :
    GPUFrameResource(std::move(rhs))
{
}

GPUUploadHeap& GPUUploadHeap::operator=(GPUUploadHeap&& rhs)
{
    GPUFrameResource::operator=(std::move(rhs));
    return *this;
}

void GPUUploadHeap::Map(int frameIndex, void** dest, D3D12_RANGE* range)
{
    auto const result = resources_[frameIndex]->Map(0, range, dest);
    ThrowIfFailed(result);
}

void GPUUploadHeap::Unmap(int frameIndex, D3D12_RANGE* range)
{
    resources_[frameIndex]->Unmap(0, range);
}
