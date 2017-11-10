#pragma once

#include <pch.hpp>

#include <Rendering\Data\GPUResource.hpp>

class GPUUploadHeap : public GPUResource
{
public:
    GPUUploadHeap();
    GPUUploadHeap(ID3D12Device* device, void const* data, std::size_t size, bool isConstBuffer = false);
    GPUUploadHeap(GPUUploadHeap const&) = delete;
    GPUUploadHeap(GPUUploadHeap&& rhs);

    GPUUploadHeap& operator=(GPUUploadHeap const&) = delete;
    GPUUploadHeap& operator=(GPUUploadHeap&& rhs);

    void Map(void** dest, D3D12_RANGE* range) { ThrowIfFailed(resource_->Map(0, range, dest)); }
    void Unmap(D3D12_RANGE* range) { resource_->Unmap(0, range); }
};